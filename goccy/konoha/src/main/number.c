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

#ifndef K_INCLUDE_BUILTINAPI

#include"commons.h"

#include<time.h>
#ifdef K_USING_POSIX_
#include<unistd.h>
#endif

/* ************************************************************************ */

#ifdef __cplusplus
extern "C" {
#endif

/* ------------------------------------------------------------------------ */
/* [random] */

void init_genrand(unsigned long s);
void init_genrand64(unsigned long long seed);
unsigned long genrand_int31(void);
unsigned long long genrand64_int63(void);
#ifndef K_USING_NOFLOAT
double genrand_real1(void);
double genrand64_real1(void);
#endif

void knh_srand(knh_uint_t seed)
{
	if(seed == 0) {
		/* You may choose a more secure way of generating the seed */
#ifdef K_USING_POSIX_
		seed = (knh_uint_t)time(NULL) + getpid();
#else
		seed = (knh_uint_t)time(NULL);
#endif
	}
#ifdef K_USING_INT32
	init_genrand((unsigned long)seed);
#else
	init_genrand64((unsigned long long int)seed);
#endif
}

knh_uint_t knh_rand(void)
{
#if defined(K_USING_INT32)
	return (knh_uint_t)genrand_int31();
#else
	return (knh_uint_t)genrand64_int63();
#endif
}

knh_float_t knh_float_rand(void)
{
#if defined(K_USING_NOFLOAT)
	return (knh_float_t)knh_rand();
#elif defined(K_USING_INT32)
	return (knh_float_t)genrand_real1();
#else
	return (knh_float_t)genrand64_real1();
#endif
}

/* ------------------------------------------------------------------------ */

KNHAPI2(knh_Int_t*) new_Int(CTX ctx, knh_class_t cid, knh_int_t value)
{
	knh_Int_t *b = (knh_Int_t*)new_hObject_(ctx, ClassTBL(cid));
	b->n.ivalue = value;
	return b;
}

KNHAPI2(knh_Float_t*) new_Float(CTX ctx, knh_class_t cid, knh_float_t value)
{
	knh_Float_t *b = (knh_Float_t*)new_hObject_(ctx, ClassTBL(cid));
	b->n.fvalue = value;
	return b;
}

/* ------------------------------------------------------------------------ */

#ifdef __cplusplus
}
#endif

#else /*K_INCLUDE_BUILTINAPI*/

/* ------------------------------------------------------------------------ */
//## @Static method void System.setRandomSeed(Int seed);

static METHOD System_setRandomSeed(CTX ctx, knh_sfp_t *sfp _RIX)
{
	knh_uint_t seed = Int_to(knh_uint_t, sfp[1]);
	knh_srand(seed);
	RETURNvoid_();
}

/* ------------------------------------------------------------------------ */
//## @Static method Int Int.random(Int n);

static METHOD Int_random(CTX ctx, knh_sfp_t *sfp _RIX)
{
	knh_uint_t n = knh_rand();
	knh_uint_t max = Int_to(knh_uint_t, sfp[1]);
	if(max > 0) {
		n = n % max;
	}
	RETURNi_(n);
}

/* ------------------------------------------------------------------------ */
//## @Static method Float! Float.random();

static METHOD Float_random(CTX ctx, knh_sfp_t *sfp _RIX)
{
	RETURNf_(knh_float_rand());
}

/* ------------------------------------------------------------------------ */
//## method void Int.%c();

static METHOD Int__c(CTX ctx, knh_sfp_t *sfp _RIX)
{
	char buf[16];
	knh_uint_t c = (knh_uint_t) sfp[1].ivalue;
	knh_format_utf8(buf, sizeof(buf), c);
	knh_write(ctx, sfp[0].w, B(buf));
}

/* ------------------------------------------------------------------------ */
//## method void Int.%u();

static METHOD Int__u(CTX ctx, knh_sfp_t *sfp _RIX)
{
	knh_write_ifmt(ctx, sfp[0].w, K_UINT_FMT, sfp[1].ivalue);
}

/* ------------------------------------------------------------------------ */
//## method void Int.%f();

static METHOD Int__f(CTX ctx, knh_sfp_t *sfp _RIX)
{
	knh_write_ffmt(ctx, sfp[0].w, K_FLOAT_FMT, (knh_float_t)sfp[1].ivalue);
}

/* ------------------------------------------------------------------------ */
//## method void Int.%x();

static METHOD Int__x(CTX ctx, knh_sfp_t *sfp _RIX)
{
	knh_write_ifmt(ctx, sfp[0].w, K_INT_XFMT, sfp[1].ivalue);
}

/* ------------------------------------------------------------------------ */

static void knh_write_bits(CTX ctx, knh_OutputStream_t *w, knh_uint64_t n, size_t bits)
{
	size_t i;
	knh_uint64_t flag = 1ULL << (bits - 1);
	for(i = 0; i < bits; i++) {
		if(i > 0 && i % 8 == 0) {
			knh_putc(ctx, w, ' ');
		}
		if((flag & n) == flag) {
			knh_putc(ctx, w, '1');
		}else{
			knh_putc(ctx, w, '0');
		}
		flag = flag >> 1;
	}
}

/* ------------------------------------------------------------------------ */
//## method void Int.%bits();

static METHOD Int__bits(CTX ctx, knh_sfp_t *sfp _RIX)
{
	knh_write_bits(ctx, sfp[0].w, sfp[1].ivalue, sizeof(knh_int_t) * 8);
}

/* ------------------------------------------------------------------------ */
//## method void Float.%d();

static METHOD Float__d(CTX ctx, knh_sfp_t *sfp _RIX)
{
	knh_write_ifmt(ctx, sfp[0].w, K_INT_FMT, (knh_int_t)sfp[1].fvalue);
}

/* ------------------------------------------------------------------------ */
//## method void Float.%bits();

static METHOD Float__bits(CTX ctx, knh_sfp_t *sfp _RIX)
{
	knh_write_bits(ctx, sfp[0].w, sfp[1].ndata, sizeof(knh_float_t) * 8);
}


/* ------------------------------------------------------------------------ */
//## @Const mapper Boolean Object;
//## @Const mapper Boolean Tdynamic;

static TYPEMAP Boolean_Object(CTX ctx, knh_sfp_t *sfp _RIX)
{
	knh_Object_t *o = (Boolean_to(int, (sfp[0]))) ? KNH_TRUE : KNH_FALSE;
	RETURN_(o);
}

/* ------------------------------------------------------------------------ */
//## @Const mapper Int Object;
//## @Const mapper Int Number;
//## @Const mapper Int Tdynamic;

static TYPEMAP Int_Object(CTX ctx, knh_sfp_t *sfp _RIX)
{
	RETURN_(new_Int(ctx, CLASS_Int, sfp[0].ivalue));
}

/* ------------------------------------------------------------------------ */
//## @Const mapper Float Object;
//## @Const mapper Float Number;
//## @Const mapper Float Tdynamic;

static TYPEMAP Float_Object(CTX ctx, knh_sfp_t *sfp _RIX)
{
	RETURN_(new_Float(ctx, CLASS_Float, sfp[0].fvalue));
}

/* ------------------------------------------------------------------------ */
//## @Const mapper Boolean String;

static TYPEMAP Boolean_String(CTX ctx, knh_sfp_t *sfp _RIX)
{
	knh_String_t *s = (Boolean_to(int, (sfp[0]))) ? TS_true : TS_false;
	RETURN_(s);
}

/* ------------------------------------------------------------------------ */
//## @Const mapper Int String;

static TYPEMAP Int_String(CTX ctx, knh_sfp_t *sfp _RIX)
{
	char buf[256];
	knh_snprintf(buf, sizeof(buf), K_INT_FMT, sfp[0].ivalue);
	RETURN_(new_S(ctx, B(buf)));
}

/* ------------------------------------------------------------------------ */
//## @Const mapper Float String;

static TYPEMAP Float_String(CTX ctx, knh_sfp_t *sfp _RIX)
{
	char buf[256];
	knh_snprintf(buf, sizeof(buf), K_FLOAT_FMT, sfp[0].fvalue);
	RETURN_(new_S(ctx, B(buf)));
}

/* ------------------------------------------------------------------------ */
//## @Const @Semantic mapper Float Int;

static TYPEMAP Float_Int(CTX ctx, knh_sfp_t *sfp _RIX)
{
	knh_int_t v = (knh_int_t)sfp[0].fvalue;
	RETURNi_(v);
}

/* ------------------------------------------------------------------------ */
//## @Const @Semantic mapper Int Float;

static TYPEMAP Int_Float(CTX ctx, knh_sfp_t *sfp _RIX)
{
	knh_float_t v = (knh_float_t)sfp[0].ivalue;
	RETURNf_(v);
}

/* ------------------------------------------------------------------------ */

#endif /*K_INCLUDE_BUILTINAPI*/
