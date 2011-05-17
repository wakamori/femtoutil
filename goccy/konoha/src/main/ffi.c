/****************************************************************************
 * KONOHA COPYRIGHT, LICENSE NOTICE, AND DISCRIMER
 *
 * Copyright (c) 2006-2011, Kimio Kuramitsu <kimio at ynu.ac.jp>
 *           (c) 2008-      Konoha Team konohaken@googlegroups.com
 * All rights reserved.
 *
 * You may choose one of the following two licenses when you use konoha.
 * If you want to use the latter license, please contact us.
 *
 * (1) GNU General Public License 3.0 (with K_UNDER_GPL)
 * (2) Konoha Non-Disclosure License 1.0
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER
 * OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
 * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 ****************************************************************************/

/* ************************************************************************ */

#include"commons.h"

/* ************************************************************************ */

#ifdef __cplusplus
extern "C" {
#endif

#ifdef K_USING_FFIDSL
#include <unistd.h>
#ifndef K_USING_MINGW
#include <sys/mman.h>
#endif

	
/* 
 * Contributors
 *  Shinpei Nakata
 */


static void bough_dumpBinary(unsigned char* ptr, size_t size);

/* ------------------------------------------------------------------------ */
// Memory allocation
// xmalloc freelist. each size is 
#define XBLOCK_SIZE (128)
#define XBLOCK_PAGESIZE (sysconf(_SC_PAGESIZE))
#define XBLOCK_NUMBER (XBLOCK_PAGESIZE / XBLOCK_SIZE)

static void *knh_xmalloc(CTX ctx, size_t page_num)
{
	size_t page_size = XBLOCK_PAGESIZE;
	void *block = KNH_VALLOC(ctx, page_size);
	if (unlikely(block == NULL)) {
		KNH_SYSLOG(ctx, NULL, LOG_CRIT, "OutOfMemory",
				   "*requested=%dbytes, used=%dbytes", page_num * page_size, ctx->stat->usedMemorySize);
	}
	int mret = mprotect(block, page_size, PROT_READ | PROT_WRITE | PROT_EXEC);
	if (mret != -1) {
		return block;
	}
	return NULL;
}

static inline void knh_xfree(CTX ctx, void* block, size_t size)
{
	knh_vfree(ctx, block, size);
}

typedef struct knh_xblock_t {
	unsigned char *block;
	struct knh_xblock_t *next;
} knh_xblock_t;

static knh_xblock_t* xfreelist = NULL;

static void* get_unused_xblock(CTX ctx)
{
	if (unlikely(xfreelist == NULL)) {
		unsigned char *xmem = (unsigned char*)knh_xmalloc(ctx, 1);
		assert(xmem != NULL);
		knh_xblock_t *p = (knh_xblock_t*)knh_malloc(ctx, XBLOCK_NUMBER * sizeof(knh_xblock_t));
		size_t idx = 0;
		for (idx = 0; idx < XBLOCK_NUMBER - 1; idx++) {
			p[idx].block = &(xmem[idx * XBLOCK_SIZE]);
			p[idx].next = &(p[idx + 1]);
			//			fprintf(stderr, "idx:%d, p:%p, block:%p, next:%p\n", idx, &(p[idx]), p[idx].block, p[idx].next);
		}
		p[idx].block = &(xmem[idx * XBLOCK_SIZE]);
		p[idx].next = NULL;
		xfreelist = p;
	}
	if (xfreelist->next == NULL) {
		unsigned char *xmem = (unsigned char*)knh_xmalloc(ctx, 1);
		knh_xblock_t *p = (knh_xblock_t*)knh_malloc(ctx, XBLOCK_NUMBER * sizeof(knh_xblock_t));
		size_t idx = 0;
		for (idx = 0; idx < XBLOCK_NUMBER - 1; idx++) {
			p[idx].block = &(xmem[idx * XBLOCK_SIZE]);
			p[idx].next = &(p[idx + 1]);
		}
		p[idx].block = &(xmem[idx * XBLOCK_SIZE]);
		p[idx].next = NULL;
		xfreelist->next = p;
	}
	assert(xfreelist->next != NULL);
	knh_xblock_t *ret = xfreelist;
	xfreelist = ret->next;
	return ret;
}


/* ------------------------------------------------------------------------ */
// generator
//  knh_Fmethod : void 

// (eax)
#define MOD_EADDR (0)
// (eax + disp8)
#define MOD_PLUS8 (1)
// (eax + disp32)
#define MOD_PLUS32 (2)
// $eax
#define MOD_IMD (3)

// Reg/opcode
#define _EAX (0)
#define _ECX (1)
#define _EDX (2)
#define _EBX (3)
#define _ESP (4)
#define _EBP (5)
#define _ESI (6)
#define _EDI (7)


#define FUNCTION function
#define FIDX fidx

#define WRITE_ASM(MOD, REG, RM) FUNCTION[FIDX++] = (MOD << 6) | (REG << 3) | RM
#define WRITE_HEX(hex) { FUNCTION[FIDX++] = hex; }

typedef struct knh_xcode_t {
	void *code;
	size_t codesize;
} knh_xcode_t;

/*
  void bough_shrinkBinary(CTX ctx, knh_xcode_t *xcode, size_t shrink_from, size_t shrink_size) {
	
}
*/

/*
  static void bough_putSfpToCStack(CTX ctx, knh_xcode_t *xcode, knh_ffiparam_t *param, char reg_from, char reg_to)
{
	// put param to register reg
	size_t fidx = xcode->codesize;
	unsigned char *function = (xcode->code + fidx);

	switch (param->type) {
	case CLASS_Int:
		WRITE_HEX(0x83);
		WRITE_ASM(MOD_IMD, reg_from, reg_to);
		WRITE_HEX((unsigned char)(param->sfpidx * 16));
		break;
	case CLASS_Float:

		break;
	default:

		break;
	}
	
}
*/

static knh_xblock_t* knh_generateWrapper(CTX ctx, void* callee, int argc, knh_ffiparam_t *argv)
{
	//unsigned char *FUNCTION = (unsigned char*)knh_xmalloc(ctx, 1);
	knh_xblock_t *blk = get_unused_xblock(ctx);
	unsigned char *function = blk->block;
	
	size_t fidx = 0;

	// magick word
	WRITE_HEX(0x55); // push ebp 
	WRITE_HEX(0x89); // mov esp->ebp
	WRITE_ASM(MOD_IMD, _ESP, _EBP);

	// incase we use ebp, store it.
	WRITE_HEX(0x53); // push ebx

	// we need stack argc * 8bytes at most
	size_t stacksize = argc * 8;
	// allocate stack (sub esp 0x38)
	WRITE_HEX(0x83);
	WRITE_HEX(0xec);
	WRITE_HEX((unsigned char)(stacksize + 0x8));

	// before going, we need edx to be store;
	// mov edx --> -0x4(ebp)
	WRITE_HEX(0x89); // mov r+disp r
	WRITE_ASM(MOD_PLUS8, _EDX, _EBP);
    WRITE_HEX(0xfc);

	//now, process first argument;
	int i;
	knh_ffiparam_t *t;

	for (i = 0; i < argc; i++) {
		t = &(argv[i]);
		if (t->sfpidx != -1) {
			// if sfpidx == -1, its ret value;
			// prepare ebx (put sfp from edx);
			WRITE_HEX(0x89);
			WRITE_ASM(MOD_IMD, _EDX, _EBX);

			switch(t->type) {
			case CLASS_Tvoid:
				// do noting.
				break;
			case CLASS_Int:
				// its 64bit int
				//TODO: we need to call translater, but now, we ignore
				//TODO: assume we cast Int --> int;
				// add ebx[sfp] + sfpidx * 16;
				WRITE_HEX(0x83); //add
				WRITE_ASM(MOD_IMD,  _EAX , _EBX);
				WRITE_HEX((unsigned char)(t->sfpidx * 16));

				// move ivalue(offset is 8);
				WRITE_HEX(0x8b); // mov r+disp, r
				WRITE_ASM(MOD_PLUS8, _EAX, _EBX);
				WRITE_HEX(0x8);

				// move eax to local value
				//WRITE_HEX(0x89);
				//WRITE_ASM(MOD_IMD, _EBP, _EAX);
				//				WRITE_HEX(0xf4);
				break;
			case CLASS_Float:
				WRITE_HEX(0x83);
				WRITE_ASM(MOD_IMD, _EAX, _EBX);
				WRITE_HEX((unsigned char)(t->sfpidx * 16));

				// load fvalue;

				WRITE_HEX(0xdd);// fld 64bit
				WRITE_HEX(0x43);// eax
				WRITE_HEX(0x8); // offset

				// push it to eax;
				WRITE_HEX(0xdd); // fstp : store & pop 64
				WRITE_HEX(0x1c);
				WRITE_HEX(0x24);

				break;
			default:
				break;
			}
		} else continue; // if its ret value
	}

	// now, call foreign function
	// NOT SURE??? since konoha is using FASTCALL,
	// call convension is always Fastcall.

	unsigned char default_disp = 0x8;
	unsigned char disp = 0x0;

	// argc contains ret value. remeber...
	// TODO :only for a single argument...
	for (i = argc - 1; i > 0; i--) {
		// push aruguments on the stack;
		// mov argument to eax!
		// TODO : now, we only consider 32 bit values

		if (argv[i].type == CLASS_Int) {
			function[fidx++] = 0x89; // mov r+disp r
			function[fidx++] = 0x45; // 0xXX(ebp)
			disp = default_disp + i * 4;
			disp = 0x100 - disp;
			function[fidx++] = disp; // -0x8
			//move to esp
			function[fidx++] = 0x89;
			function[fidx++] = 0x04;
			function[fidx++] = 0x24;
		}

	}

	//now call.
	// call foreign function
	//	intptr_t ucallee = (intptr_t)callee;
	//	intptr_t next_addr = (intptr_t)function + (intptr_t)fidx + 5 /*for call instruction */;
	//intptr_t rel = (ucallee > next_addr) ? ucallee - next_addr : next_addr - ucallee;
	//rel = -rel;
	//unsigned char *src = (unsigned char*)&rel;

	// absolute call
	unsigned char *src = (unsigned char*)&callee;
	// mov this to eax;
	WRITE_HEX(0xb8); // mov to eax
	WRITE_HEX(src[0]);
	WRITE_HEX(src[1]);
	WRITE_HEX(src[2]);
	WRITE_HEX(src[3]);

	// now call
	WRITE_HEX(0xff);
	WRITE_HEX(0xd0);

	//  function[fidx++] = 0xcc;
	// after calling, restore edx;
	// restore edx;
	WRITE_HEX(0x8b);
	WRITE_ASM(MOD_PLUS8, _EDX, _EBP);
	WRITE_HEX(0xfc);

	if (argv[0].sfpidx == -1) {
		switch(argv[0].type) {
		case CLASS_Tvoid:
			goto STEP_OUT;
			break;
		case CLASS_Int:
			// get return value, and give it to Konoha
			// ret value is on eax;
			// push eax
			function[fidx++] = 0x50; // push eax
			break;
		case CLASS_Float:
			// it is on FPU. we need no concern.
			break;
		}
	}

	// get rix (at 0x8(ebp)) --> eax
	WRITE_HEX(0x8b);  // mov
	WRITE_ASM(MOD_PLUS8, _EAX, _EBP); // 0xXX(ebp) eax
	WRITE_HEX(0x8); // 8

	// get edx --> ebx
	WRITE_HEX(0x89);
	WRITE_ASM(MOD_IMD, _EDX, _EBX);

	// ebx[sfp] + 16 * rix
	// first, 16 * rix = 2^4 * rix
	WRITE_HEX(0xc1); // shl
	WRITE_ASM(MOD_IMD, _ESP, _EAX);
	WRITE_HEX(0x4);

	// second, add eax to ebx;
	WRITE_HEX(0x01); //add
	WRITE_ASM(MOD_IMD, _EAX, _EBX); // add eax -> ebx;

	// now at ebx is pointing to sfp[rix];
	// copy retvalue to sfp[rix].ivalue (offset is 0x8)
	// pop eax;


	// offset is different for each types
	if (argv[0].sfpidx == -1) {
		switch (argv[0].type) {
		case CLASS_Int:
			function[fidx++] = 0x58; // pop eax
			// mov eax --> 0x8(%ebx)
			function[fidx++] = 0x89; // mov r+disp r
			function[fidx++] = 0x43; // eax: ebx
			function[fidx++] = 0x8; // 0x8
			break;
		case CLASS_Float:
			//fstpl 0x8(ebx)
			WRITE_HEX(0xdd);
			WRITE_ASM(MOD_PLUS8, _EBX, _EBX);
			WRITE_HEX(0x8);
			break;
		}
	}

	// from here, closing this function
	// close stack; add 0xXX esp
 STEP_OUT:
	WRITE_HEX(0x83);
	WRITE_HEX(0xc4);
	WRITE_HEX((unsigned char)(stacksize + 0x8));

	// restore ebx
	WRITE_HEX(0x5b); // pop ebx
	WRITE_HEX(0x5d);	// pop ebp;

	// ret $0x4
	WRITE_HEX(0xc2); // ret
	WRITE_HEX(0x4);
	WRITE_HEX(0x0);

	//	return function;
	return blk;
}

//typedef struct {
//	knh_type_t type;
//	knh_short_t sfpidx;
//	void *conv_func;
//} knh_ffiparam_t;  // for ffi
// sfp: -1 --> return
// sfp: 1+ --> arguments


static void bough_dumpBinary(unsigned char *ptr, size_t size)
{
	int i = 0;
	unsigned char byte;
	for (i = 0; i < size; i++) {
		byte = ptr[i];
		fprintf(stderr, "%02x ", byte);
		if (i % 16 == 15) fprintf(stderr, "\n");
	}
}


#endif/*K_USING_FFIDSL*/

knh_Fmethod knh_makeFmethod(CTX ctx, void *func, int argc, knh_ffiparam_t *argv)
{
#ifdef K_USING_FFIDSL
//	void *f = knh_generateWrapper(ctx, (void*)func, argc, argv);
	knh_xblock_t* blk = knh_generateWrapper(ctx, (void*)func, argc, argv);
	void *f = blk->block;
	if (f != NULL) {
		//		bough_dumpBinary(f, 128);
		return (void*)f;
	}
#endif
	return NULL; /* if FAILED */
}

/* ------------------------------------------------------------------------ */

#ifdef __cplusplus
}
#endif
