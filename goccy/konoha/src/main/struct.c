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

#define USE_bytes         1
#define USE_cwb           1

#define USE_array_index   1

#define K_PERROR_LIBNAME   "stdc"
#define USE_fopen   1
#define USE_fclose  1
#define USE_fread   1
#define USE_fwrite  1
#define USE_fflush  1
#define USE_fclose  1

#define USE_hash    1

#define USE_TIME_H
#define USE_getTimeMilliSecond 1

#include"commons.h"
#include"../../include/konoha1/konoha_code_.h"

#ifndef LIBNAME
#define LIBNAME "konoha"
#endif

#ifndef TYPE_BytesIm
#define CLASS_BytesIm CLASS_Bytes
#define TYPE_BytesIm  TYPE_Bytes
#endif

#ifndef TYPE_ArrayIm
#define CLASS_ArrayIm CLASS_Array
#define TYPE_ArrayIm  TYPE_Array
#endif

#ifndef TYPE_MapIm
#define CLASS_MapIm CLASS_Map
#define TYPE_MapIm  TYPE_Map
#endif

/* ************************************************************************ */

#ifdef __cplusplus
extern "C" {
#endif

/* --------------- */
/* Data */
/* --------------- */

#define K_INCLUDE_BUILTINAPI
#include "dspi.c"

//static TYPEMAP Array_Iterator(CTX ctx, knh_sfp_t *sfp _RIX);
//static TYPEMAP Iterator_Array(CTX ctx, knh_sfp_t *sfp _RIX);

#define knh_bodymalloc(ctx, C)   (knh_##C##EX_t*)KNH_MALLOC(ctx, sizeof(knh_##C##EX_t))
#define knh_bodyfree(ctx, p, C)  KNH_FREE(ctx, p, sizeof(knh_##C##EX_t))

/* ------------------------------------------------------------------------ */
/* DEFAULT */

static void DEFAULT_init(CTX ctx, Object *o)
{
	DBG_ASSERT((sizeof(knh_Object_t) - sizeof(knh_hObject_t)) == sizeof(knh_intptr_t) * 4);
	knh_intptr_t *p = (knh_intptr_t*)&(o->ref);
	p[0] = K_INT0; p[1] = K_INT0; p[2] = K_INT0; p[3] = K_INT0;
}

static void DEFAULT_initcopy(CTX ctx, Object *dst, const Object *src)
{
	KNH_TODO("copy operation");
}

knh_bool_t knh_class_canObjectCopy(CTX ctx, knh_class_t cid)
{
	const knh_ClassTBL_t *ct = ClassTBL(cid);
	return (ct->ospi->initcopy != DEFAULT_initcopy);
}

static void DEFAULT_reftrace(CTX ctx, Object *o FTRARG)
{
}

static void DEFAULT_free(CTX ctx, Object *o)
{
}

static void DEFAULT_checkin(CTX ctx, knh_sfp_t *sfp, Object *o)
{
}

static void DEFAULT_checkout(CTX ctx, Object *o, int isFailed)
{
}

static int DEFAULT_compareTo(const Object *o1, const Object *o2)
{
	return (int)((knh_intptr_t)o1 - (knh_intptr_t)o2);
}

static void DEFAULT_p(CTX ctx, knh_OutputStream_t *w, Object *o, int level)
{
	KNH_WARN(ctx, "TODO: must be defined %s_write", O__(o));
	knh_write__p(ctx, w, (void*)o->ref);
}

static void knh_write_TObject(CTX ctx, knh_OutputStream_t *w, knh_type_t type, Object **v, size_t i, int level)
{
	switch(type) {
		case CLASS_Boolean: {
			knh_boolean_t *d = (knh_boolean_t*)(v+i);
			knh_write_bool(ctx, w, d[0]);
			break;
		}
		case CLASS_Int: {
			knh_int_t *d = (knh_int_t*)(v+i);
			knh_write_ifmt(ctx, w, K_INT_FMT, d[0]);
			break;
		}
		case CLASS_Float: {
			knh_float_t *d = (knh_float_t*)(v+i);
			knh_write_ffmt(ctx, w, K_FLOAT_FMT, d[0]);
			break;
		}
		default:{
			knh_write_InObject(ctx, w, v[i], level);
		}
	}
}

static knh_String_t* DEFAULT_getkey(CTX ctx, knh_sfp_t *sfp)
{
	knh_cwb_t cwbbuf, *cwb = knh_cwb_open(ctx, &cwbbuf);
	knh_write_type(ctx, cwb->w, O_cid(sfp[0].o));
	knh_putc(ctx, cwb->w, ':');
	knh_write__p(ctx, cwb->w, sfp[0].o);
	return knh_cwb_newString(ctx, cwb);
}

static knh_hashcode_t DEFAULT_hashCode(CTX ctx, knh_sfp_t *sfp)
{
	return ((knh_hashcode_t)sfp[0].o) / sizeof(knh_Object_t);
}

static knh_int_t DEFAULT_toint(CTX ctx, knh_sfp_t *sfp)
{
	return 0L;
}

static knh_float_t DEFAULT_tofloat(CTX ctx, knh_sfp_t *sfp)
{
	return K_FLOAT_ZERO;
}

knh_TypeMap_t* DEFAULT_findTypeMapNULL(CTX ctx, knh_class_t scid, knh_class_t tcid, int mode)
{
	DBG_P("trying to generate scid=%s tcid=%s, isWrapper=%d", CLASS__(scid), CLASS__(tcid), mode);
	return NULL;
}

#define DEFAULT_1 NULL
#define DEFAULT_2 NULL
#define DEFAULT_3 NULL
#define DEFAULT_4 NULL
#define DEFAULT_5 NULL
#define DEFAULT_6 NULL

static knh_ClassDef_t TdynamicDef = {
	DEFAULT_init, DEFAULT_initcopy, DEFAULT_reftrace, DEFAULT_free,
	DEFAULT_checkin, DEFAULT_checkout, DEFAULT_compareTo, DEFAULT_p,
	DEFAULT_getkey, DEFAULT_hashCode, DEFAULT_toint, DEFAULT_tofloat,
	DEFAULT_findTypeMapNULL, DEFAULT_1, DEFAULT_2, DEFAULT_3,
	"dynamic", 0, 0, NULL,
	NULL, DEFAULT_4, DEFAULT_5, DEFAULT_6,
};

KNHAPI2(knh_ClassDef_t*) knh_getDefaultClassDef(void)
{
	return &TdynamicDef;
}

static knh_ClassDef_t TvoidDef = {
	DEFAULT_init, DEFAULT_initcopy, DEFAULT_reftrace, DEFAULT_free,
	DEFAULT_checkin, DEFAULT_checkout, DEFAULT_compareTo, DEFAULT_p,
	DEFAULT_getkey, DEFAULT_hashCode, DEFAULT_toint, DEFAULT_tofloat,
	DEFAULT_findTypeMapNULL, DEFAULT_1, DEFAULT_2, DEFAULT_3,
	"void", CFLAG_Tvoid, 0, NULL,
	NULL, DEFAULT_4, DEFAULT_5, DEFAULT_6,
};

static knh_ClassDef_t TvarDef = {
	DEFAULT_init, DEFAULT_initcopy, DEFAULT_reftrace, DEFAULT_free,
	DEFAULT_checkin, DEFAULT_checkout, DEFAULT_compareTo, DEFAULT_p,
	DEFAULT_getkey, DEFAULT_hashCode, DEFAULT_toint, DEFAULT_tofloat,
	DEFAULT_findTypeMapNULL, DEFAULT_1, DEFAULT_2, DEFAULT_3,
	"var", CFLAG_Tvar, 0, NULL,
	NULL, DEFAULT_4, DEFAULT_5, DEFAULT_6,
};

/* --------------- */
/* Object */

static void ObjectField_init(CTX ctx, Object *o)
{
	knh_ObjectField_t *of = (knh_ObjectField_t*)o;
	const knh_ClassTBL_t *t = O_cTBL(o);
	if(t->fsize > 0) {
		Object **v = &(of->smallobject);
		if(t->fsize > K_SMALLOBJECT_FIELDSIZE) {
			v = (Object**)KNH_MALLOC(ctx, t->fsize * sizeof(knh_Object_t*));
		}
		of->fields = v;
		knh_memcpy(v, t->protoNULL->fields, t->fsize * sizeof(knh_Object_t*));
#ifdef K_USING_RCGC
		size_t i;
		for(i = 0; i < t->fsize; i++) {
			if(!t->fields[i].israw) {
				knh_Object_RCinc(v[i]);
			}
		}
#endif
	}
	else {
		of->fields = NULL;
	}
}

static void ObjectField_initcopy(CTX ctx, Object *o, const Object *src)
{
	knh_ObjectField_t *of = (knh_ObjectField_t*)o;
	const knh_ClassTBL_t *t = O_cTBL(o);
	if(t->fsize > 0) {
		Object **v = &(of->smallobject);
		if(t->fsize > K_SMALLOBJECT_FIELDSIZE) {
			v = (Object**)KNH_MALLOC(ctx, t->fsize * sizeof(knh_Object_t*));
		}
		of->fields = v;
		knh_memcpy(v, src->ref, t->fsize * sizeof(knh_Object_t*));
#ifdef K_USING_RCGC
		size_t i;
		for(i = 0; i < t->fsize; i++) {
			if(!t->fields[i].israw) {
				knh_Object_RCinc(v[i]);
			}
		}
#endif
	}
	else {
		of->fields = NULL;
	}
}

static void ObjectField_reftrace(CTX ctx, Object *o FTRARG)
{
	knh_ObjectField_t *of = (knh_ObjectField_t*)o;
	const knh_ClassTBL_t *ct = O_cTBL(o);
	size_t i;
	for(i = 0; i < ct->fsize; i++) {
		if(ct->fields[i].israw == 0) {
			KNH_ADDREF(ctx, of->fields[i]);
		}
	}
	KNH_SIZEREF(ctx);
}

static void ObjectField_free(CTX ctx, Object *o)
{
	knh_ObjectField_t *of = (knh_ObjectField_t*)o;
	const knh_ClassTBL_t *t = O_cTBL(o);
	if(t->fsize > K_SMALLOBJECT_FIELDSIZE) {
		KNH_FREE(ctx, of->fields, t->fsize * sizeof(knh_Object_t*));
	}
	DBG_(of->fields = NULL);
}

static int ObjectField_compareTo(const Object *o, const Object *o2)
{
	return o - o2;
}

static void ObjectField_p(CTX ctx, knh_OutputStream_t *w, Object *o, int level)
{
	knh_putc(ctx, w, '{');
	{
		knh_fields_t *tf = O_cTBL(o)->fields;
		size_t i, fsize = O_cTBL(o)->fsize;
		Object **v = (Object**)o->ref;
		if(fsize > 0) {
			knh_write_fn(ctx, w, tf[0].fn);
			knh_write(ctx, w, STEXT(": "));
			knh_write_TObject(ctx, w, tf[0].type, v, 0, level);
		}
		for(i = 1; i < fsize; i++) {
			if(tf[i].type == TYPE_void) continue;
			knh_write_delim(ctx, w);
			knh_write_fn(ctx, w, tf[i].fn);
			knh_write(ctx, w, STEXT(": "));
			knh_write_TObject(ctx, w, tf[i].type, v, i, level);
		}
	}
	knh_putc(ctx, w, '}');
}

static knh_String_t* ObjectField_getkey(CTX ctx, knh_sfp_t *sfp)
{
	int keyidx = O_cTBL(sfp[0].o)->keyidx;
	if(keyidx != -1) {
		knh_ObjectField_t *of = (knh_ObjectField_t*)sfp[0].o;
		DBG_ASSERT(IS_bString(of->fields[keyidx]));
		return (knh_String_t*)of->fields[keyidx];
	}
	return DEFAULT_getkey(ctx, sfp);
}

static void Array_wdata(CTX ctx, void *pkr, Object *o, const knh_PackSPI_t *packspi);
#define is32BIT() sizeof(void *) / 4 == 1
static void Object_wdata(CTX ctx, void *pkr, Object *o, const knh_PackSPI_t *packspi)
{
	const knh_ClassTBL_t *tbl = O_cTBL(o);
	int i = 0;
	Object **v = (Object **)o->ref;
	size_t map_size = 0;
	for (i = 0; i < tbl->fsize; i++) {
		knh_fields_t *field = tbl->fields + i;
		knh_type_t type = field->type;
		if (is32BIT() &&
			(type == CLASS_Boolean || type == CLASS_Int || type == CLASS_Float)) {
			i++;
		}
		map_size++;
	}
	packspi->pack_beginmap(ctx, pkr, map_size + 1);
	packspi->pack_string(ctx, pkr, "ks:class", sizeof("ks:class"));
	packspi->pack_string(ctx, pkr, tbl->sname->str.text, tbl->sname->str.len);
	for (i = 0; i < tbl->fsize; i++) {
		knh_fields_t *field = tbl->fields + i;
		knh_type_t type = field->type;
		knh_String_t *key = knh_getFieldName(ctx, field->fn);
		packspi->pack_string(ctx, pkr, key->str.text, key->str.len);
		switch (type) {
		case CLASS_Boolean: {
			knh_boolean_t *value = (knh_boolean_t *)(v + i);
			packspi->pack_bool(ctx, pkr, value[0]);
			if (is32BIT()) i++;
			break;
		}
		case CLASS_Int: {
			knh_int_t *value = (knh_int_t *)(v + i);
			packspi->pack_int(ctx, pkr, value[0]);
			if (is32BIT()) i++;
			break;
		}
		case CLASS_Float: {
			knh_float_t *value = (knh_float_t *)(v + i);
			packspi->pack_float(ctx, pkr, value[0]);
			if (is32BIT()) i++;
			break;
		}
		case CLASS_String: {
			knh_String_t *value = (knh_String_t *)v[i];
			packspi->pack_string(ctx, pkr, value->str.text, value->str.len);
			break;
		}
		default:
			//Array or Object
			if (O_cTBL(v[i])->bcid == CLASS_Array) {
				Array_wdata(ctx, pkr, v[i], packspi);
			} else {
				Object_wdata(ctx, pkr, v[i], packspi);
			}
			break;
		}
	}
	fprintf(stderr, " }");
}

static knh_ClassDef_t ObjectDef = {
	ObjectField_init, ObjectField_initcopy, ObjectField_reftrace, ObjectField_free,
	DEFAULT_checkin, DEFAULT_checkout, ObjectField_compareTo, ObjectField_p,
	ObjectField_getkey, DEFAULT_hashCode, DEFAULT_toint, DEFAULT_tofloat,
	DEFAULT_findTypeMapNULL, Object_wdata, DEFAULT_2, DEFAULT_3,
	"Object", CFLAG_Object, 0, NULL,
	NULL, DEFAULT_4, DEFAULT_5, DEFAULT_6,
};

static void ObjectFieldN_init(CTX ctx, Object *o)
{
	knh_ObjectField_t *of = (knh_ObjectField_t*)o;
	const knh_ClassTBL_t *t = O_cTBL(o);
	if(t->fsize > 0) {
		Object **v = &(of->smallobject);
		if(t->fsize > K_SMALLOBJECT_FIELDSIZE) {
			v = (Object**)KNH_MALLOC(ctx, t->fsize * sizeof(knh_Object_t*));
		}
		of->fields = v;
		knh_memcpy(v, t->protoNULL->fields, t->fsize * sizeof(knh_Object_t*));
	}
	else {
		of->fields = NULL;
	}
}

static void ObjectFieldN_initcopy(CTX ctx, Object *o, const Object *src)
{
	knh_ObjectField_t *of = (knh_ObjectField_t*)o;
	const knh_ClassTBL_t *ct = O_cTBL(o);
	if(ct->fsize > 0) {
		Object **v = &(of->smallobject);
		if(ct->fsize > K_SMALLOBJECT_FIELDSIZE) {
			v = (Object**)KNH_MALLOC(ctx, ct->fsize * sizeof(knh_Object_t*));
		}
		of->fields = v;
		knh_memcpy(v, src->ref, ct->fsize * sizeof(knh_Object_t*));
	}
	else {
		of->fields = NULL;
	}
}

static void ObjectField1_reftrace(CTX ctx, Object *o FTRARG)
{
	knh_ObjectField_t *of = (knh_ObjectField_t*)o;
#ifdef K_USING_FASTREFS_
	KNH_SETREF(ctx, of->fields, 1);
#else
	KNH_ADDREF(ctx, of->fields[0]);
	KNH_SIZEREF(ctx);
#endif
}
static void ObjectField2_reftrace(CTX ctx, Object *o FTRARG)
{
	knh_ObjectField_t *of = (knh_ObjectField_t*)o;
#ifdef K_USING_FASTREFS_
	KNH_SETREF(ctx, of->fields, 2);
#else
	KNH_ADDREF(ctx, of->fields[0]);
	KNH_ADDREF(ctx, of->fields[1]);
	KNH_SIZEREF(ctx);
#endif
}
static void ObjectField3_reftrace(CTX ctx, Object *o FTRARG)
{
	knh_ObjectField_t *of = (knh_ObjectField_t*)o;
#ifdef K_USING_FASTREFS_
	KNH_SETREF(ctx, of->fields, 3);
#else
	KNH_ADDREF(ctx, of->fields[0]);
	KNH_ADDREF(ctx, of->fields[1]);
	KNH_ADDREF(ctx, of->fields[2]);
	KNH_SIZEREF(ctx);
#endif
}
static void ObjectField4_reftrace(CTX ctx, Object *o FTRARG)
{
	knh_ObjectField_t *of = (knh_ObjectField_t*)o;
#ifdef K_USING_FASTREFS_
	KNH_SETREF(ctx, of->fields, 4);
#else
	KNH_ADDREF(ctx, of->fields[0]);
	KNH_ADDREF(ctx, of->fields[1]);
	KNH_ADDREF(ctx, of->fields[2]);
	KNH_ADDREF(ctx, of->fields[3]);
	KNH_SIZEREF(ctx);
#endif
}

static knh_ClassDef_t ObjectNDef[] = {
	{
		ObjectFieldN_init, ObjectFieldN_initcopy, DEFAULT_reftrace, ObjectField_free,
		DEFAULT_checkin, DEFAULT_checkout, ObjectField_compareTo, ObjectField_p,
		ObjectField_getkey, DEFAULT_hashCode, DEFAULT_toint, DEFAULT_tofloat,
		DEFAULT_findTypeMapNULL, Object_wdata, DEFAULT_2, DEFAULT_3,
		"Object", CFLAG_Object, 0, NULL,
		NULL, DEFAULT_4, DEFAULT_5, DEFAULT_6,
	},
	{
		ObjectField_init, ObjectField_initcopy, ObjectField1_reftrace, ObjectField_free,
		DEFAULT_checkin, DEFAULT_checkout, ObjectField_compareTo, ObjectField_p,
		ObjectField_getkey, DEFAULT_hashCode, DEFAULT_toint, DEFAULT_tofloat,
		DEFAULT_findTypeMapNULL, Object_wdata, DEFAULT_2, DEFAULT_3,
		"Object", CFLAG_Object, 0, NULL,
		NULL, DEFAULT_4, DEFAULT_5, DEFAULT_6,
	},
	{
		ObjectField_init, ObjectField_initcopy, ObjectField2_reftrace, ObjectField_free,
		DEFAULT_checkin, DEFAULT_checkout, ObjectField_compareTo, ObjectField_p,
		ObjectField_getkey, DEFAULT_hashCode, DEFAULT_toint, DEFAULT_tofloat,
		DEFAULT_findTypeMapNULL, Object_wdata, DEFAULT_2, DEFAULT_3,
		"Object", CFLAG_Object, 0, NULL,
		NULL, DEFAULT_4, DEFAULT_5, DEFAULT_6,
	},
	{
		ObjectField_init, ObjectField_initcopy, ObjectField3_reftrace, ObjectField_free,
		DEFAULT_checkin, DEFAULT_checkout, ObjectField_compareTo, ObjectField_p,
		ObjectField_getkey, DEFAULT_hashCode, DEFAULT_toint, DEFAULT_tofloat,
		DEFAULT_findTypeMapNULL, Object_wdata, DEFAULT_2, DEFAULT_3,
		"Object", CFLAG_Object, 0, NULL,
		NULL, DEFAULT_4, DEFAULT_5, DEFAULT_6,
	},
	{
		ObjectField_init, ObjectField_initcopy, ObjectField4_reftrace, ObjectField_free,
		DEFAULT_checkin, DEFAULT_checkout, ObjectField_compareTo, ObjectField_p,
		ObjectField_getkey, DEFAULT_hashCode, DEFAULT_toint, DEFAULT_tofloat,
		DEFAULT_findTypeMapNULL, Object_wdata, DEFAULT_2, DEFAULT_3,
		"Object", CFLAG_Object, 0, NULL,
		NULL, DEFAULT_4, DEFAULT_5, DEFAULT_6,
	}
};

void knh_ClassTBL_setObjectCSPI(knh_ClassTBL_t *ct)
{
	size_t c, i;
	for(c = 0; c < ct->fsize; c++) {
		if(ct->fields[c].israw == 1) break;
	}
	for(i = c + 1; i < ct->fsize; i++) {
		if(ct->fields[i].israw == 0) {
			knh_setClassDef(ct, &ObjectDef);
			return;
		}
	}
	if(c <= 4) {
		DBG_P("NO OBJECT FIELD: %s c=%d", S_tochar(ct->lname), c);
		knh_setClassDef(ct, ObjectNDef + c);
	}
	else {
		knh_setClassDef(ct, &ObjectDef);
	}
}

/* --------------- */
/* Boolean */

static void NDATA_init(CTX ctx, Object *o)
{
	knh_Number_t *no = (knh_Number_t*)o;
	no->n.ivalue = 0;
}

static void NDATA_initcopy(CTX ctx, Object *o, const Object *src)
{
	knh_Number_t *no = (knh_Number_t*)o;
	knh_Number_t *so = (knh_Number_t*)src;
	no->n.data = so->n.data;
}

static void NDATA_free(CTX ctx, Object *o)
{
}

static knh_int_t Int_toint(CTX ctx, knh_sfp_t *sfp)
{
	return sfp[0].ivalue;
}

static knh_float_t Int_tofloat(CTX ctx, knh_sfp_t *sfp)
{
	return (knh_float_t)sfp[0].ivalue;
}

static knh_int_t Float_toint(CTX ctx, knh_sfp_t *sfp)
{
	return (knh_int_t)sfp[0].fvalue;
}

static knh_float_t Float_tofloat(CTX ctx, knh_sfp_t *sfp)
{
	return sfp[0].fvalue;
}

static knh_hashcode_t NDATA_hashCode(CTX ctx, knh_sfp_t *sfp)
{
	return (knh_hashcode_t)sfp[0].ndata;
}

static int Int_compareTo(const Object *o, const Object *o2)
{
	knh_int_t f = ((knh_Number_t*)o)->n.ivalue;
	knh_int_t f2 = ((knh_Number_t*)o2)->n.ivalue;
	return (f < f2) ? -1 : ((f == f2) ? 0 : 1);
}

static int Float_compareTo(const Object *o, const Object *o2)
{
	knh_float_t f = ((knh_Number_t*)o)->n.fvalue;
	knh_float_t f2 = ((knh_Number_t*)o2)->n.fvalue;
	return (f < f2) ? -1 : ((f == f2) ? 0 : 1);
}

static void Boolean_p(CTX ctx, knh_OutputStream_t *w, Object *o, int level)
{
	knh_write_bool(ctx, w, ((knh_Number_t*)o)->n.bvalue);
}

static void Int_p(CTX ctx, knh_OutputStream_t *w, Object *o, int level)
{
#if defined(K_USING_SEMANTICS)
	knh_Semantics_t *u = knh_getSemantics(ctx, O_cid(sfp[1].o));
	knh_write_intx(ctx, sfp[0].w, u, sfp[1].ivalue);
#else
	knh_write_ifmt(ctx, w, K_INT_FMT, ((knh_Number_t*)o)->n.ivalue);
#endif
}

static void Float_p(CTX ctx, knh_OutputStream_t *w, Object *o, int level)
{
#if defined(K_USING_SEMANTICS)
	knh_Semantics_t *u = knh_getSemantics(ctx, O_cid(sfp[1].o));
	knh_write_floatx(ctx, sfp[0].w, u, sfp[1].fvalue);
#else
	knh_write_ffmt(ctx, w, K_FLOAT_FMT, ((knh_Number_t*)o)->n.fvalue);
#endif
}

static void Boolean_wdata(CTX ctx, void *pkr, Object *o, const knh_PackSPI_t *packspi)
{
	packspi->pack_bool(ctx, pkr, ((knh_Boolean_t *)o)->n.bvalue);
}

static void Int_wdata(CTX ctx, void *pkr, Object *o, const knh_PackSPI_t *packspi)
{
	packspi->pack_int(ctx, pkr, ((knh_Int_t *)o)->n.ivalue);
}

static void Float_wdata(CTX ctx, void *pkr, Object *o, const knh_PackSPI_t *packspi)
{
	packspi->pack_float(ctx, pkr, ((knh_Float_t *)o)->n.fvalue);
}

static knh_ClassDef_t BooleanDef = {
	NDATA_init, NDATA_initcopy, DEFAULT_reftrace, NDATA_free,
	DEFAULT_checkin, DEFAULT_checkout, Int_compareTo, Boolean_p,
	ObjectField_getkey, NDATA_hashCode, DEFAULT_toint, DEFAULT_tofloat,
	DEFAULT_findTypeMapNULL, Boolean_wdata, DEFAULT_2, DEFAULT_3,
	"Boolean", CFLAG_Boolean, 0, NULL,
	NULL, DEFAULT_4, DEFAULT_5, DEFAULT_6,
};

static knh_ClassDef_t NumberDef = {
	NDATA_init, NDATA_initcopy, DEFAULT_reftrace, NDATA_free,
	DEFAULT_checkin, DEFAULT_checkout, DEFAULT_compareTo, DEFAULT_p,
	ObjectField_getkey, NDATA_hashCode, DEFAULT_toint, DEFAULT_tofloat,
	DEFAULT_findTypeMapNULL, DEFAULT_1, DEFAULT_2, DEFAULT_3,
	"Number", CFLAG_Number, 0, NULL,
	NULL, DEFAULT_4, DEFAULT_5, DEFAULT_6,
};

static knh_ClassDef_t IntDef = {
	NDATA_init, NDATA_initcopy, DEFAULT_reftrace, NDATA_free,
	DEFAULT_checkin, DEFAULT_checkout, Int_compareTo, Int_p,
	ObjectField_getkey, NDATA_hashCode, Int_toint, Int_tofloat,
	DEFAULT_findTypeMapNULL, Int_wdata, DEFAULT_2, DEFAULT_3,
	"Int", CFLAG_Int, 0, NULL,
	NULL, DEFAULT_4, DEFAULT_5, DEFAULT_6,
};

static knh_ClassDef_t FloatDef = {
	NDATA_init, NDATA_initcopy, DEFAULT_reftrace, NDATA_free,
	DEFAULT_checkin, DEFAULT_checkout, Float_compareTo, Float_p,
	ObjectField_getkey, NDATA_hashCode, Float_toint, Float_tofloat,
	DEFAULT_findTypeMapNULL, Float_wdata, DEFAULT_2, DEFAULT_3,
	"Float", CFLAG_Float, 0, NULL,
	NULL, DEFAULT_4, DEFAULT_5, DEFAULT_6,
};

/* --------------- */
/* String */

static void String_init(CTX ctx, Object *o)
{
	knh_String_t *s = (knh_String_t*)o;
	s->str.text = "";
	s->str.len = 0;
	s->hashCode = 0;
	String_setTextSgm(s, 1);
}

static void String_free(CTX ctx, knh_Object_t *o)
{
	knh_String_t *s = (knh_String_t*)o;
	if(!String_isTextSgm(s)) {
		KNH_FREE(ctx, s->str.ubuf, KNH_SIZE(S_size(s) + 1));
	}
}

static int String_compareTo(const knh_Object_t *o, const knh_Object_t *o2)
{
	knh_String_t *s1 = (knh_String_t*)o;
	knh_String_t *s2 = (knh_String_t*)o2;
#if defined(K_USING_SEMANTICS)
	if(s1->h.cid != CLASS_String && s2->h.cid != CLASS_String) {
		knh_Semantics_t *u = knh_getSemantics(ctx, s1->h.cid);
		return DP(u)->fscmp(u, S_tobytes(s1), S_tobytes(s2));
	}
#else
	return knh_bytes_strcmp(S_tobytes(s1) ,S_tobytes(s2));
#endif
}

static void String_p(CTX ctx, knh_OutputStream_t *w, knh_Object_t *o, int level)
{
	knh_String_t *s = (knh_String_t*)o;
	if(IS_FMTs(level)) {
		knh_write_utf8(ctx, w, S_tobytes(s), !String_isASCII(s));
	}
	else {
		knh_write_quote(ctx, w, '"', S_tobytes(s), !String_isASCII(s));
	}
}

static knh_String_t* String_getkey(CTX ctx, knh_sfp_t *sfp)
{
	return sfp[0].s;
}

static knh_hashcode_t String_hashCode(CTX ctx, knh_sfp_t *sfp)
{
	return (sfp[0].s)->hashCode;
}

static void String_wdata(CTX ctx, void *pkr, Object *o, const knh_PackSPI_t *packspi)
{
	knh_String_t *s = (knh_String_t *)o;
	packspi->pack_string(ctx, pkr, s->str.text, s->str.len);
}

static knh_ClassDef_t StringDef = {
	String_init, DEFAULT_initcopy, DEFAULT_reftrace, String_free,
	DEFAULT_checkin, DEFAULT_checkout, String_compareTo, String_p,
	String_getkey, String_hashCode, DEFAULT_toint, DEFAULT_tofloat,
	DEFAULT_findTypeMapNULL, String_wdata, DEFAULT_2, DEFAULT_3,
	"String", CFLAG_String, 0, NULL,
	NULL, DEFAULT_4, DEFAULT_5, DEFAULT_6,
};

/* --------------- */
/* Bytes */

static const knh_dim_t dimINIT = {
	0, 0, 0, 0, 0, 0, 0,
};

static void Bytes_init(CTX ctx, Object *o)
{
	knh_Bytes_t *ba = (knh_Bytes_t*)o;
	ba->bu.len = 0;
	ba->bu.ubuf = NULL;
	ba->dim = &dimINIT;
	ba->DBG_name = NULL;
}

static const knh_dim_t* dim_copy(CTX ctx, const knh_dim_t *dim_src)
{
	if(dim_src->capacity == 0) {
		return dim_src;
	}
	else {
		knh_dim_t *dim = (knh_dim_t*)KNH_MALLOC(ctx, sizeof(knh_dim_t));
		dim = (knh_dim_t*)dim_src;
		return (const knh_dim_t*)dim;
	}
}

static void Bytes_initcopy(CTX ctx, Object *o, const Object *src)
{
	knh_Bytes_t *ba = (knh_Bytes_t*)o, *ba_src = (knh_Bytes_t*)src;
	if(ba_src->dim->capacity > 0) {
		ba->bu.len = ba_src->bu.len;
		ba->bu.ubuf = (knh_uchar_t*)KNH_MALLOC(ctx, ba_src->dim->capacity);
		knh_memcpy(ba->bu.ubuf, ba_src->bu.ubuf, ba_src->dim->capacity);
	}
	else {
		ba->bu.len = 0;
		ba->bu.ubuf = NULL;
	}
	ba->dim = dim_copy(ctx, ba_src->dim);
}

static void Bytes_free(CTX ctx, Object *o)
{
	knh_Bytes_t *ba = (knh_Bytes_t*)o;
	knh_dimfree(ctx, ba->bu.ubuf, ba->dim);
}

static int Bytes_compareTo(const knh_Object_t *o, const knh_Object_t *o2)
{
	knh_Bytes_t *s1 = (knh_Bytes_t*)o;
	knh_Bytes_t *s2 = (knh_Bytes_t*)o2;
	return knh_bytes_strcmp(BA_tobytes(s1) , BA_tobytes(s2));
}

static void Bytes_p(CTX ctx, knh_OutputStream_t *w, knh_Object_t *o, int level)
{
	knh_Bytes_t *ba = (knh_Bytes_t*)o;
	if(IS_FMTs(level)) {
		knh_printf(ctx, w, "byte[%d]", BA_size(ba));
	}
	else if(IS_FMTdump(level)) {
		size_t i, j, n;
		char buf[40];
		for(j = 0; j * 16 < ba->bu.len; j++) {
			knh_snprintf(buf, sizeof(buf), "%08x", (int)(j*16));
			knh_write(ctx, w, B(buf));
			for(i = 0; i < 16; i++) {
				n = j * 16 + i;
				if(n < ba->bu.len) {
					knh_snprintf(buf, sizeof(buf), " %2x", (int)ba->bu.utext[n]);
					knh_write(ctx, w, B(buf));
				}
				else {
					knh_write(ctx, w, STEXT("   "));
				}
			}
			knh_write(ctx, w, STEXT("    "));
			for(i = 0; i < 16; i++) {
				n = j * 16 + i;
				if(n < ba->bu.len && isprint(ba->bu.utext[n])) {
					knh_snprintf(buf, sizeof(buf), "%c", (int)ba->bu.utext[n]);
					knh_write(ctx, w, B(buf));
				}
				else {
					knh_write(ctx, w, STEXT(" "));
				}
			}
			knh_write_EOL(ctx, w);
		}
	}
}

static knh_hashcode_t Bytes_hashCode(CTX ctx, knh_sfp_t *sfp)
{
	knh_Bytes_t *ba = sfp[0].ba;
	return knh_hash(0, ba->bu.text, ba->bu.len);
}

static knh_ClassDef_t BytesDef = {
	Bytes_init, Bytes_initcopy, DEFAULT_reftrace, Bytes_free,
	DEFAULT_checkin, DEFAULT_checkout, Bytes_compareTo, Bytes_p,
	DEFAULT_getkey, Bytes_hashCode, DEFAULT_toint, DEFAULT_tofloat,
	DEFAULT_findTypeMapNULL, DEFAULT_1, DEFAULT_2, DEFAULT_3,
	"Bytes", CFLAG_Bytes, 0, NULL,
	NULL, DEFAULT_4, DEFAULT_5, DEFAULT_6,
};

static knh_ClassDef_t BytesImDef = {
	Bytes_init, DEFAULT_initcopy, DEFAULT_reftrace, Bytes_free,
	DEFAULT_checkin, DEFAULT_checkout, Bytes_compareTo, Bytes_p,
	DEFAULT_getkey, Bytes_hashCode, DEFAULT_toint, DEFAULT_tofloat,
	DEFAULT_findTypeMapNULL, DEFAULT_1, DEFAULT_2, DEFAULT_3,
	"BytesIm", CFLAG_BytesIm, 0, NULL,
	NULL, DEFAULT_4, DEFAULT_5, DEFAULT_6,
};

/* --------------------------------------------------------------------------*/
/* Tuple */

static void Tuple_init(CTX ctx, Object *o)
{
	knh_Tuple_t *of = (knh_Tuple_t*)o;
	const knh_ClassTBL_t *t = O_cTBL(o);
	if(t->fsize > 0) {
		Object **v = &(of->smallobject);
		if(t->fsize > K_SMALLOBJECT_FIELDSIZE) {
			v = (Object**)KNH_MALLOC(ctx, t->fsize * sizeof(knh_Object_t*));
		}
		of->fields = v;
		knh_memcpy(v, t->defnull->ref, t->fsize * sizeof(knh_Object_t*));
#ifdef K_USING_RCGC
		size_t i;
		for(i = 0; i < t->fsize; i++) {
			if(t->fields[i].israw == 0) {
				knh_Object_RCinc(of->fields[i]);
			}
		}
#endif
	}
	else {
		of->fields = NULL;
	}
}

static void TUPLE_p(CTX ctx, knh_OutputStream_t *w, Object *o, int level)
{
	knh_putc(ctx, w, '(');
	{
		knh_fields_t *tf = O_cTBL(o)->fields;
		size_t i, fsize = O_cTBL(o)->fsize;
		Object **v = (Object**)o->ref;
		knh_write_TObject(ctx, w, tf[0].type, v, 0, level);
		for(i = 1; i < fsize; i++) {
			if(tf[i].type == TYPE_void) continue;
			knh_write_delim(ctx, w);
			knh_write_TObject(ctx, w, tf[i].type, v, i, level);
		}
	}
	knh_putc(ctx, w, ')');
}

static knh_ClassDef_t TupleDef = {
	Tuple_init, ObjectField_initcopy, ObjectField_reftrace, ObjectField_free,
	DEFAULT_checkin, DEFAULT_checkout, DEFAULT_compareTo, TUPLE_p,
	DEFAULT_getkey, DEFAULT_hashCode, DEFAULT_toint, DEFAULT_tofloat,
	DEFAULT_findTypeMapNULL, DEFAULT_1, DEFAULT_2, DEFAULT_3,
	"Tuple", 0, 0, NULL,
	NULL, DEFAULT_4, DEFAULT_5, DEFAULT_6,
};

/* --------------- */
/* Range */

static void Range_init(CTX ctx, Object *o)
{
	knh_Range_t *rng = (knh_Range_t*)o;
	if(Range_isNDATA(o)) {
		rng->nstart = 0;
		rng->nend = 0;
	}
	else {
		KNH_INITv(rng->ostart, KNH_NULVAL(O_p1(rng)));
		KNH_INITv(rng->oend, rng->ostart);
	}
}

static void Range_reftrace(CTX ctx, Object *o FTRARG)
{
	knh_Range_t *rng = (knh_Range_t*)o;
	if(!Range_isNDATA(o)) {
		KNH_ADDREF(ctx, rng->ostart);
		KNH_ADDREF(ctx, rng->oend);
		KNH_SIZEREF(ctx);
	}
}

static void Range_p(CTX ctx, knh_OutputStream_t *w, Object *o, int level)
{
	knh_Range_t *range = (knh_Range_t*)o;
	knh_class_t p1 = O_p1(range);
	knh_putc(ctx, w, '[');
	knh_write_TObject(ctx, w, p1, &range->ostart, 0, level);
	knh_write(ctx, w, STEXT(" to "));
	knh_write_TObject(ctx, w, p1, &range->oend, 0, level);
	knh_putc(ctx, w, ']');
}

static knh_ClassDef_t RangeDef = {
	Range_init, DEFAULT_initcopy, Range_reftrace, DEFAULT_free,
	DEFAULT_checkin, DEFAULT_checkout, DEFAULT_compareTo, Range_p,
	DEFAULT_getkey, DEFAULT_hashCode, DEFAULT_toint, DEFAULT_tofloat,
	DEFAULT_findTypeMapNULL, DEFAULT_1, DEFAULT_2, DEFAULT_3,
	"Range", CFLAG_Range, 0, NULL,
	NULL, DEFAULT_4, DEFAULT_5, DEFAULT_6,
};

/* --------------- */
/* Array */

static void Array_init(CTX ctx, Object *o)
{
	knh_Array_t *a = (knh_Array_t*)o;
	knh_Array_initAPI(ctx, a);
	a->dim = &dimINIT;
	a->list = NULL;
	a->size = 0;
}

static void Array_initcopy(CTX ctx, Object *o, const Object *src)
{
	knh_Array_t *a = (knh_Array_t*)o, *a_src = (knh_Array_t*)src;
	a->api = a_src->api;
	if(a_src->dim->capacity > 0) {
		a->size = a_src->size;
		a->list = (knh_Object_t**)KNH_MALLOC(ctx, a_src->dim->capacity * a_src->dim->wsize);
		knh_memcpy(a->list, a_src->list, a_src->dim->capacity * a_src->dim->wsize);
		Array_setNDATA(a, Array_isNDATA(a_src));
		if(!Array_isNDATA(a)) {
			size_t i;
			for(i = 0; i < a->size; i++) {
				knh_Object_RCinc(a->list[i]);
			}
		}
	}
	else {
		a->list = NULL;
		a->size = 0;
	}
	a->dim = dim_copy(ctx, a_src->dim);
}

static void Array_reftrace(CTX ctx, Object *o FTRARG)
{
	knh_Array_t *a = (knh_Array_t*)o;
	if(!Array_isNDATA(a)) {
#ifdef K_USING_FASTREFS_
		KNH_SETREF(ctx, a->list, a->size);
#else
		size_t i;
		for(i = 0; i < a->size; i++) {
			KNH_ADDREF(ctx, a->list[i]);
		}
		KNH_SIZEREF(ctx);
#endif
	}
}

static void Array_free(CTX ctx, Object *o)
{
	knh_Array_t *a = (knh_Array_t*)o;
	knh_dimfree(ctx, a->list, a->dim);
}

static void Array_p(CTX ctx, knh_OutputStream_t *w, Object *o, int level)
{
	knh_putc(ctx, w, '[');
	{
		knh_Array_t *a = (knh_Array_t*)o;
		knh_class_t p1 = O_p1(a);
		size_t c, size = knh_Array_size(a);
		if(size > 0) {
			if(IS_Tunbox(p1)) {
				if(IS_Tint(p1)) {
					knh_write_ifmt(ctx, w, K_INT_FMT, a->ilist[0]);
					if(IS_FMTline(level)) {
						knh_write_dots(ctx, w);
					}
					else {
						for(c = 1; c < size; c++) {
							knh_write_delim(ctx, w);
							knh_write_ifmt(ctx, w, K_INT_FMT, a->ilist[c]);
						}
					}
				}
				else if(p1 == TYPE_Boolean) {
					knh_write_bool(ctx, w, (int)a->ilist[0]);
					if(IS_FMTline(level)) {
						knh_write_dots(ctx, w);
					}
					else {
						for(c = 1; c < knh_Array_size(a); c++) {
							knh_write_delim(ctx, w);
							knh_write_bool(ctx, w, (int)a->ilist[c]);
						}
					}
				}
				else { /* IS_Tfloat(p1) */
					knh_write_ffmt(ctx, w, K_FLOAT_FMT, a->flist[0]);
					if(IS_FMTline(level)) {
						knh_write_dots(ctx, w);
					}
					else {
						for(c = 1; c < knh_Array_size(a); c++) {
							knh_write_delim(ctx, w);
							knh_write_ffmt(ctx, w, K_FLOAT_FMT, a->flist[c]);
						}
					}
				}
			}
			else {
				knh_write_InObject(ctx, w, a->list[0], level);
				if(IS_FMTline(level)) {
					knh_write_dots(ctx, w);
				}
				else {
					for(c = 1; c < size; c++) {
						knh_write_delim(ctx, w);
						knh_write_InObject(ctx, w, a->list[c], level);
					}
				}
			}
		}
	}
	knh_putc(ctx, w, ']');
}

static void Array_wdata(CTX ctx, void *pkr, Object *o, const knh_PackSPI_t *packspi)
{
	knh_Array_t *a = (knh_Array_t *)o;
	packspi->pack_beginmap(ctx, pkr, 2);
	packspi->pack_string(ctx, pkr, "ks:atype", sizeof("ks:atype"));
	packspi->pack_int(ctx, pkr, O_cTBL(a)->p1);//for type check
	packspi->pack_string(ctx, pkr, "abody", sizeof("abody"));
	packspi->pack_beginarray(ctx, pkr, a->size);
	int i = 0;
	for (i = 0; i < a->size; i++) {
		switch (O_cTBL(a)->p1) {
		case CLASS_Int:
			packspi->pack_int(ctx, pkr, a->ilist[i]);
			break;
		case CLASS_Float:
			packspi->pack_float(ctx, pkr, a->flist[i]);
			break;
		case CLASS_Boolean:
			packspi->pack_bool(ctx, pkr, a->ilist[i]);
			break;
		default:
			if (O_cTBL(a->list[i])->ospi->wdata != NULL) {
				O_cTBL(a->list[i])->ospi->wdata(ctx, pkr, a->list[i], packspi);
			}
			break;
		}
	}
	fprintf(stderr, " ] }");
}

static knh_ClassDef_t ArrayDef = {
	Array_init, Array_initcopy, Array_reftrace, Array_free,
	DEFAULT_checkin, DEFAULT_checkout, DEFAULT_compareTo, Array_p,
	DEFAULT_getkey, DEFAULT_hashCode, DEFAULT_toint, DEFAULT_tofloat,
	DEFAULT_findTypeMapNULL, Array_wdata, DEFAULT_2, DEFAULT_3,
	"Array", CFLAG_Array, 0, NULL,
	NULL, DEFAULT_4, DEFAULT_5, DEFAULT_6,
};

static knh_ClassDef_t ArrayImDef = {
	Array_init, DEFAULT_initcopy, Array_reftrace, Array_free,
	DEFAULT_checkin, DEFAULT_checkout, DEFAULT_compareTo, Array_p,
	DEFAULT_getkey, DEFAULT_hashCode, DEFAULT_toint, DEFAULT_tofloat,
	DEFAULT_findTypeMapNULL, DEFAULT_1, DEFAULT_2, DEFAULT_3,
	"ArrayIm", CFLAG_ArrayIm, 0, NULL,
	NULL, DEFAULT_4, DEFAULT_5, DEFAULT_6,
};

/* --------------- */
/* Iterator */

static ITRNEXT Fitrnext_single(CTX ctx, knh_sfp_t *sfp, long rtnidx)
{
	knh_Iterator_t *itr = ITR(sfp);
	if(DP(itr)->mitr.index == 0) {
		DP(itr)->mitr.index = 1;
		ITRNEXT_(DP(itr)->source);
	}
	ITREND_();
}

static void Iterator_init(CTX ctx, Object *o)
{
	knh_Iterator_t *itr = (knh_Iterator_t*)o;
	knh_IteratorEX_t *b = knh_bodymalloc(ctx, Iterator);
	itr->fnext_1  =  Fitrnext_single;
	KNH_INITv(b->source, KNH_NULL);
	b->nptr     =  NULL;
	b->mitr.index = 0;
	b->mitr.ptr = NULL;
	b->freffree = NULL;
	itr->b = b;
}

static void Iterator_reftrace(CTX ctx, Object *o FTRARG)
{
	knh_Iterator_t *itr = (knh_Iterator_t*)o;
	KNH_ADDREF(ctx, DP(itr)->source);
	KNH_SIZEREF(ctx);
}

static void Iterator_free(CTX ctx, Object *o)
{
	knh_Iterator_t *itr = (knh_Iterator_t*)o;
	knh_Iterator_close(ctx, itr);
	knh_bodyfree(ctx, itr->b, Iterator);
}

static void Iterator_p(CTX ctx, knh_OutputStream_t *w, Object *o, int level)
{
	knh_Iterator_t *it = (knh_Iterator_t*)o;
	knh_class_t p1 = O_p1(it);
	if(IS_FMTdump(level)) {
		knh_sfp_t *lsfp = ctx->esp;
		size_t c = 0;
		while(it->fnext_1(ctx, lsfp+1, /*1+*/1)) {
			if(c > 0) {
				knh_write_EOL(ctx, w);
			}
			if(IS_Tint(p1)) {
				knh_write_ifmt(ctx, w, K_INT_FMT, lsfp[2].ivalue);
			}
			else if(IS_Tfloat(p1)) {
				knh_write_ffmt(ctx, w, K_FLOAT_FMT, lsfp[2].fvalue);
			}
			else {
				knh_write_Object(ctx, w, lsfp[2].o, level);
			}
			c++;
		}
	}
}

static knh_ClassDef_t IteratorDef = {
	Iterator_init, DEFAULT_initcopy, Iterator_reftrace, Iterator_free,
	DEFAULT_checkin, DEFAULT_checkout, DEFAULT_compareTo, Iterator_p,
	DEFAULT_getkey, DEFAULT_hashCode, DEFAULT_toint, DEFAULT_tofloat,
	DEFAULT_findTypeMapNULL, DEFAULT_1, DEFAULT_2, DEFAULT_3,
	"Iterator", CFLAG_Iterator, sizeof(knh_IteratorEX_t), NULL,
	NULL, DEFAULT_4, DEFAULT_5, DEFAULT_6,
};

/* --------------- */
/* Map */

static const knh_MapDSPI_t* NULLMAP_config(CTX ctx, knh_class_t p1, knh_class_t p2);
static knh_map_t *NULLMAP_init(CTX ctx, size_t init, const char *path, void *option) { return NULL; }
static void NULLMAP_reftrace(CTX ctx, knh_map_t *m FTRARG){}
static void NULLMAP_free(CTX ctx, knh_map_t *m){}
static knh_bool_t NULLMAP_get(CTX ctx, knh_map_t* m, knh_sfp_t *ksfp, knh_sfp_t *rsfp) { return 0; }
static void NULLMAP_set(CTX ctx, knh_map_t* m, knh_sfp_t *ksfp) {}
static void NULLMAP_remove(CTX ctx, knh_map_t* m, knh_sfp_t *ksfp) {}
static size_t NULLMAP_size(CTX ctx, knh_map_t* m) { return 0; }
static knh_bool_t NULLMAP_next(CTX ctx, knh_map_t* m, knh_mapitr_t *mitr, knh_sfp_t *rsfp) { return 0; }

static const knh_MapDSPI_t NULLMAP = {
	K_DSPI_MAP, "NULL",
	NULLMAP_config, NULLMAP_init, NULLMAP_reftrace, NULLMAP_free,
	NULLMAP_get, NULLMAP_set, NULLMAP_remove, NULLMAP_size, NULLMAP_next,
};

static const knh_MapDSPI_t* NULLMAP_config(CTX ctx, knh_class_t p1, knh_class_t p2)
{
	return &NULLMAP;
}

static void Map_init(CTX ctx, Object *o)
{
	knh_Map_t *m = (knh_Map_t*)o;
	m->dspi = &NULLMAP;
	m->map = NULL;
}

static void TODO_initcopy(CTX ctx, Object *d, const Object *s)
{
	KNH_TODO(__FUNCTION__);
}

static void Map_reftrace(CTX ctx, Object *o FTRARG)
{
	knh_Map_t *m = (knh_Map_t*)o;
	m->dspi->reftrace(ctx, m->map FTRDATA);
}

static void Map_free(CTX ctx, Object *o)
{
	knh_Map_t *m = (knh_Map_t*)o;
	m->dspi->freemap(ctx, m->map);
}

static void Map_p(CTX ctx, knh_OutputStream_t *w, Object *o, int level)
{
	knh_Map_t *m = (knh_Map_t*)o;
	size_t n = m->dspi->size(ctx, m->map);
	knh_putc(ctx, w, '{');
	if(n > 0) {
		BEGIN_LOCAL(ctx, lsfp, 2);
		knh_class_t p1 = O_cTBL(o)->p1, p2 = O_cTBL(o)->p2;
		knh_mapitr_t mitrbuf = K_MAPITR_INIT, *mitr = &mitrbuf;
		if(m->dspi->next(ctx, m->map, mitr, lsfp)) {
			knh_write_sfp(ctx, w, p1, lsfp, FMT_line);
			knh_write(ctx, w, STEXT(": "));
			knh_write_sfp(ctx, w, p2, lsfp+1, FMT_line);
			if(!IS_FMTline(level)) {
				while(m->dspi->next(ctx, m->map, mitr, lsfp)) {
					knh_write(ctx, w, STEXT(", "));
					knh_write_sfp(ctx, w, p1, lsfp, FMT_line);
					knh_write(ctx, w, STEXT(": "));
					knh_write_sfp(ctx, w, p2, lsfp+1, FMT_line);
				}
			}
			else {
				knh_write_dots(ctx, w);
			}
		}
		END_LOCAL_NONGC(ctx, lsfp);
	}
	knh_putc(ctx, w, '}');
}

static knh_ClassDef_t MapDef = {
	Map_init, TODO_initcopy, Map_reftrace, Map_free,
	DEFAULT_checkin, DEFAULT_checkout, DEFAULT_compareTo, Map_p,
	DEFAULT_getkey, DEFAULT_hashCode, DEFAULT_toint, DEFAULT_tofloat,
	DEFAULT_findTypeMapNULL, DEFAULT_1, DEFAULT_2, DEFAULT_3,
	"Map", CFLAG_Map, 0, NULL,
	NULL, DEFAULT_4, DEFAULT_5, DEFAULT_6,
};

static knh_ClassDef_t MapImDef = {
	Map_init, DEFAULT_initcopy, Map_reftrace, Map_free,
	DEFAULT_checkin, DEFAULT_checkout, DEFAULT_compareTo, Map_p,
	DEFAULT_getkey, DEFAULT_hashCode, DEFAULT_toint, DEFAULT_tofloat,
	DEFAULT_findTypeMapNULL, DEFAULT_1, DEFAULT_2, DEFAULT_3,
	"MapIm", CFLAG_MapIm, 0, NULL,
	NULL, DEFAULT_4, DEFAULT_5, DEFAULT_6,
};

/* --------------- */
/* Class */

static int Class_compareTo(const knh_Object_t *o, const knh_Object_t *o2)
{
	knh_Class_t *c = (knh_Class_t*)o;
	knh_Class_t *c2 = (knh_Class_t*)o2;
	return knh_strcmp(S_tochar(c->cTBL->lname), S_tochar(c2->cTBL->lname));
}

static knh_String_t *Class_getkey(CTX ctx,knh_sfp_t *sfp)
{
	knh_Class_t *c = (knh_Class_t*)sfp[0].o;
	return ClassTBL(c->cid)->lname;
}

static knh_hashcode_t Class_hashCode(CTX ctx,knh_sfp_t *sfp)
{
	knh_Class_t *c = (knh_Class_t*)sfp[0].o;
	return (knh_hashcode_t)c->cid;
}

static void Class_p(CTX ctx, knh_OutputStream_t *w, Object *o, int level)
{
	knh_write_cid(ctx, w, knh_Class_cid((knh_Class_t*)o));
}

static knh_ClassDef_t ClassDef = {
	DEFAULT_init, DEFAULT_initcopy, DEFAULT_reftrace, DEFAULT_free,
	DEFAULT_checkin, DEFAULT_checkout, Class_compareTo, Class_p,
	Class_getkey, Class_hashCode, DEFAULT_toint, DEFAULT_tofloat,
	DEFAULT_findTypeMapNULL, DEFAULT_1, DEFAULT_2, DEFAULT_3,
	"Class", CFLAG_Class, 0, NULL,
	NULL, DEFAULT_4, DEFAULT_5, DEFAULT_6,
};

/* --------------- */
/* ParamArray */

static void ParamArray_init(CTX ctx, Object *o)
{
	knh_ParamArray_t *pa = (knh_ParamArray_t*)o;
	pa->psize = 0;
	pa->rsize = 0;
	pa->params = NULL;
	pa->capacity = 0;
}

static void ParamArray_free(CTX ctx, Object *o)
{
	knh_ParamArray_t *pa = (knh_ParamArray_t*)o;
	if(pa->psize + pa->rsize > 3) {
		KNH_FREE(ctx, pa->params, pa->capacity * sizeof(knh_param_t));
	}
}

static void ParamArray_p(CTX ctx, knh_OutputStream_t *w, Object *o, int level)
{
	knh_ParamArray_t *pa = (knh_ParamArray_t*)o;
	size_t i;
	for(i = 0; i < pa->psize; i++) {
		knh_param_t *p = knh_ParamArray_get(pa, i);
		if(i > 0) knh_write_delim(ctx, w);
		knh_write_type(ctx, w, p->type);
		if(IS_FMTdump(level)) {
			knh_putc(ctx, w, ' ');
			knh_write_fn(ctx, w, p->fn);
		}
	}
	if(pa->rsize > 0) {
		knh_write(ctx, w, STEXT(" => "));
		for(i = 0; i < pa->rsize; i++) {
			knh_param_t *p = knh_ParamArray_rget(pa, i);
			knh_write_type(ctx, w, p->type);
		}
	}
}

static knh_ClassDef_t ParamArrayDef = {
	ParamArray_init, TODO_initcopy, DEFAULT_reftrace, ParamArray_free,
	DEFAULT_checkin, DEFAULT_checkout, DEFAULT_compareTo, ParamArray_p,
	DEFAULT_getkey, DEFAULT_hashCode, DEFAULT_toint, DEFAULT_tofloat,
	DEFAULT_findTypeMapNULL, DEFAULT_1, DEFAULT_2, DEFAULT_3,
	"ParamArray", CFLAG_ParamArray, 0, NULL,
	NULL, DEFAULT_4, DEFAULT_5, DEFAULT_6,
};

/* --------------- */
/* Method */

static void Method_init(CTX ctx, Object *o)
{
	knh_Method_t *mtd = (knh_Method_t*)o;
	knh_MethodEX_t *b = knh_bodymalloc(ctx, Method);
	KNH_INITv(b->mp, KNH_NULVAL(CLASS_ParamArray));
	KNH_INITv(b->kcode, KNH_NULL);
//	b->flag   = 0;
//	b->delta  = 0;
//	b->uri  = 0;  b->domain = 0;
	mtd->b = b;
}

static void Method_reftrace(CTX ctx, Object *o FTRARG)
{
	knh_Method_t *mtd = (knh_Method_t*)o;
	knh_MethodEX_t *b = DP(mtd);
	KNH_ADDREF(ctx, b->mp);
	KNH_ADDREF(ctx, b->kcode);
	KNH_SIZEREF(ctx);
}

static void BODY_free(CTX ctx, Object *o)
{
	const knh_ClassTBL_t *ct = O_cTBL(o);
	DBG_ASSERT(ct->ospi->struct_size > 0);
	KNH_FREE(ctx, o->ref, ct->ospi->struct_size);
}

static void Method_p(CTX ctx, knh_OutputStream_t *w, Object *o, int level)
{
	knh_Method_t *mtd = (knh_Method_t*)o;
	if(!(IS_FMTline(level))) {
		if(Method_isAbstract(mtd)) {
			knh_write(ctx, w, STEXT("@Abstract"));
			knh_putc(ctx, w, ' ');
		}
		if(Method_isStatic(mtd)) {
			knh_write(ctx, w, STEXT("@Static"));
			knh_putc(ctx, w, ' ');
		}
		knh_write_type(ctx, w, knh_ParamArray_rtype(DP(mtd)->mp));
		knh_putc(ctx, w, ' ');
	}
	knh_write_type(ctx, w, (mtd)->cid);
	knh_putc(ctx, w, '.');
	knh_write_mn(ctx, w, (mtd)->mn);
	if(!(IS_FMTline(level))) {
		size_t i;
		knh_putc(ctx, w, '(');
		for(i = 0; i < knh_Method_psize(mtd); i++) {
			knh_param_t *p = knh_ParamArray_get(DP(mtd)->mp, i);
			if(i > 0) {
				knh_write_delim(ctx, w);
			}
			knh_write_type(ctx, w, p->type);
			knh_putc(ctx, w, ' ');
			knh_write(ctx, w, B(FN__(p->fn)));
		}
		if(ParamArray_isVARGs(DP(mtd)->mp)) {
			knh_write_delim(ctx, w);
			knh_write_dots(ctx, w);
		}
		knh_putc(ctx, w, ')');
	}
	if(IS_FMTdump(level)) {
		knh_write_EOL(ctx, w);
		if(Method_isObjectCode(mtd)) {
			knh_write_InObject(ctx, w, DP(mtd)->objdata, level);
		}
	}
}

static knh_ClassDef_t MethodDef = {
	Method_init, TODO_initcopy, Method_reftrace, BODY_free,
	DEFAULT_checkin, DEFAULT_checkout, DEFAULT_compareTo, Method_p,
	DEFAULT_getkey, DEFAULT_hashCode, DEFAULT_toint, DEFAULT_tofloat,
	DEFAULT_findTypeMapNULL, DEFAULT_1, DEFAULT_2, DEFAULT_3,
	"Method", CFLAG_Method, sizeof(knh_MethodEX_t), NULL,
	NULL, DEFAULT_4, DEFAULT_5, DEFAULT_6,
};

/* --------------- */
/* TypeMap */

static void TypeMap_init(CTX ctx, Object *o)
{
	knh_TypeMap_t *tmr = (knh_TypeMap_t*)o;
	tmr->scid = 0;
	tmr->tcid = 0;
	KNH_INITv(tmr->mapdata, KNH_NULL);
	KNH_INITv(tmr->tmr2, KNH_NULL);
}

static void TypeMap_reftrace(CTX ctx, Object *o FTRARG)
{
	knh_TypeMap_t *tmr = (knh_TypeMap_t*)o;
	KNH_ADDREF(ctx, tmr->mapdata);
	KNH_ADDREF(ctx, tmr->tmr2);
	KNH_SIZEREF(ctx);
}

static void TypeMap_p(CTX ctx, knh_OutputStream_t *w, Object *o, int level)
{
	knh_TypeMap_t *tmr = (knh_TypeMap_t*)o;
	knh_write_type(ctx, w, tmr->scid);
	knh_write(ctx, w, STEXT("=>"));
	knh_write_type(ctx, w, tmr->tcid);
}

static knh_ClassDef_t TypeMapDef = {
	TypeMap_init, TODO_initcopy, TypeMap_reftrace, DEFAULT_free,
	DEFAULT_checkin, DEFAULT_checkout, DEFAULT_compareTo, TypeMap_p,
	DEFAULT_getkey, DEFAULT_hashCode, DEFAULT_toint, DEFAULT_tofloat,
	DEFAULT_findTypeMapNULL, DEFAULT_1, DEFAULT_2, DEFAULT_3,
	"TypeMap", CFLAG_TypeMap, 0, NULL,
	NULL, DEFAULT_4, DEFAULT_5, DEFAULT_6,
};

/* --------------- */
/* Func */

static METHOD Fmethod_funcRTYPE(CTX ctx, knh_sfp_t *sfp _RIX)
{
	knh_type_t rtype = knh_ParamArray_rtype(DP(sfp[K_MTDIDX].mtdNC)->mp);
	if(rtype != TYPE_void) {
		if(IS_Tunbox(rtype)) {
			RETURNi_(K_INT0);  // same results in Float, Boolean
		}
		else {
			RETURN_(KNH_NULVAL(CLASS_t(rtype)));
		}
	}
}

static void Func_init(CTX ctx, Object *o)
{
	knh_Func_t *fo = (knh_Func_t*)o;
	const knh_ClassTBL_t *t = O_cTBL(o);
	const knh_Method_t *mtd;
	if(t->defnull == NULL) {
		mtd = new_Method(ctx, 0, O_cid(o), MN_LAMBDA, Fmethod_funcRTYPE);
		KNH_SETv(ctx, DP(mtd)->mp, t->cparam);
	}
	else {
		mtd = t->deffunc->mtd;
	}
	KNH_INITv(fo->mtd, mtd);
	fo->baseNULL = NULL;
}

static void Func_reftrace(CTX ctx, Object *o FTRARG)
{
	knh_Func_t *fo = (knh_Func_t*)o;
	KNH_ADDREF(ctx, (fo->mtd));
	KNH_ADDNNREF(ctx, fo->baseNULL);
//		size_t i, stacksize = (fo)->hstacksize[-1];
//		for(i = 0; i < stacksize; i++) {
//			KNH_ADDREF(ctx, (fo)->envsfp[i].o);
//		}
	KNH_SIZEREF(ctx);
}

static void Func_free(CTX ctx, Object *o)
{
//	knh_Func_t *cc = (knh_Func_t*)o;
//	if(Func_isStoredEnv(cc)) {
//		size_t stacksize = (cc)->hstacksize[-1];
//		KNH_FREE(ctx, (cc)->envsfp, (sizeof(knh_sfp_t) * stacksize) + sizeof(size_t));
//		(cc)->envsfp = NULL;
//		Func_setStoredEnv(cc, 0);
//	}
}

static void Func_p(CTX ctx, knh_OutputStream_t *w, Object *o, int level)
{

}

static knh_ClassDef_t FuncDef = {
	Func_init, TODO_initcopy, Func_reftrace, Func_free,
	DEFAULT_checkin, DEFAULT_checkout, DEFAULT_compareTo, Func_p,
	DEFAULT_getkey, DEFAULT_hashCode, DEFAULT_toint, DEFAULT_tofloat,
	DEFAULT_findTypeMapNULL, DEFAULT_1, DEFAULT_2, DEFAULT_3,
	"Func", CFLAG_Func, 0, NULL,
	NULL, DEFAULT_4, DEFAULT_5, DEFAULT_6,
};

/* --------------- */
/* Thunk */

static void Thunk_init(CTX ctx, Object *o)
{
	knh_Thunk_t *thunk = (knh_Thunk_t*)o;
	thunk->envsfp = NULL;
	thunk->envsize = 0;
}

static void Thunk_reftrace(CTX ctx, Object *o FTRARG)
{
	knh_Thunk_t *thunk = (knh_Thunk_t*)o;
	if(thunk->envsize > 0) {
		size_t i;
		for(i = 0; i < (thunk)->envsize; i++) {
			KNH_ADDREF(ctx, (thunk)->envsfp[i].o);
		}
		KNH_SIZEREF(ctx);
	}
}

static void Thunk_free(CTX ctx, Object *o)
{
	knh_Thunk_t *thunk = (knh_Thunk_t*)o;
	if(thunk->envsize > 0) {
		KNH_FREE(ctx, thunk->envsfp, sizeof(knh_sfp_t) * thunk->envsize);
		thunk->envsize = 0;
		thunk->envsfp = NULL;
	}
}

static knh_ClassDef_t ThunkDef = {
	Thunk_init, TODO_initcopy, Thunk_reftrace, Thunk_free,
	DEFAULT_checkin, DEFAULT_checkout, DEFAULT_compareTo, DEFAULT_p,
	DEFAULT_getkey, DEFAULT_hashCode, DEFAULT_toint, DEFAULT_tofloat,
	DEFAULT_findTypeMapNULL, DEFAULT_1, DEFAULT_2, DEFAULT_3,
	"Thunk", CFLAG_Thunk, 0, NULL,
	NULL, DEFAULT_4, DEFAULT_5, DEFAULT_6,
};

/* --------------- */
/* Exception */

static void Exception_init(CTX ctx, Object *o)
{
	knh_ExceptionEX_t *b = knh_bodymalloc(ctx, Exception);
	b->eid  = 1; b->flag = 0;
	KNH_INITv(b->event, O_cTBL(o)->sname);
	KNH_INITv(b->msg, TS_EMPTY);
	KNH_INITv(b->bag, KNH_NULL);
	b->tracesNULL = NULL;
	b->uline = 0;
	b->sysloglevel = LOG_ERR;
	o->ref = b;
}

static void Exception_reftrace(CTX ctx, Object *o FTRARG)
{
	knh_ExceptionEX_t *b = DP((knh_Exception_t*)o);
	KNH_ADDREF(ctx, b->event);
	KNH_ADDREF(ctx, b->msg);
	KNH_ADDREF(ctx, b->bag);
	KNH_ADDNNREF(ctx, b->tracesNULL);
	KNH_SIZEREF(ctx);
}

static void Exception_p(CTX ctx, knh_OutputStream_t *w, Object *o, int level)
{
	knh_Exception_t *e = (knh_Exception_t*)o;
	if(!IS_FMTdump(level)) {
		knh_write_ascii(ctx, w, EBI__(DP(e)->eid));
		knh_write_ascii(ctx, w, "!!");
	}
	else {
		knh_write_EOL(ctx, w);
		if(DP(e)->uline != 0) {
			knh_write_uline(ctx, w, DP(e)->uline);
		}
		{
//			knh_bytes_t emsg = S_tobytes(DP(e)->event);
			knh_bytes_t msg = S_tobytes(DP(e)->msg);
//			const char *fmt = (DP(e)->eid <= 1) ? "%B: %B" : "%BException: %B";
			knh_printf(ctx, w, "Exception: %B", msg);
		}
		if(DP(e)->tracesNULL != NULL) {
			knh_Array_t *a = DP(e)->tracesNULL;
			size_t i, size = knh_Array_size(a), c = 0;
			knh_bytes_t prev = STEXT("?");
			for(i = 0; i < size; i++) {
				knh_String_t *s = a->strings[i];
				if(S_startsWith(s, prev)) {
					c++; continue;
				}
				if(c > 0) {
					knh_write_EOL(ctx, w);
					knh_printf(ctx, w, "  *** called %d times recursively ***", c);
					c = 0;
				}
				knh_write_EOL(ctx, w);
				knh_printf(ctx, w, "  at %s", S_tochar(s));
				prev = S_tobytes(s);
				prev = knh_bytes_first(prev, knh_bytes_rindex(prev, '('));
			}
		}
		knh_write_EOL(ctx, w);
	}
}

static knh_ClassDef_t ExceptionDef = {
	Exception_init, TODO_initcopy, Exception_reftrace, BODY_free,
	DEFAULT_checkin, DEFAULT_checkout, DEFAULT_compareTo, Exception_p,
	DEFAULT_getkey, DEFAULT_hashCode, DEFAULT_toint, DEFAULT_tofloat,
	DEFAULT_findTypeMapNULL, DEFAULT_1, DEFAULT_2, DEFAULT_3,
	"Exception", CFLAG_Exception, sizeof(knh_ExceptionEX_t), NULL,
	NULL, DEFAULT_4, DEFAULT_5, DEFAULT_6,
};

/* --------------- */
/* ExceptionHandler */

static void ExceptionHandler_init(CTX ctx, Object *o)
{
	knh_ExceptionHandler_t *hdr = (knh_ExceptionHandler_t*)o;
	knh_ExceptionHandlerEX_t *b = knh_bodymalloc(ctx, ExceptionHandler);
	knh_bzero(b, sizeof(knh_ExceptionHandlerEX_t));
	o->ref = b;
	KNH_INITv(hdr->stacklist, new_Array0(ctx, 0));
}

static void ExceptionHandler_reftrace(CTX ctx, Object *o FTRARG)
{
	knh_ExceptionHandler_t *hdr = (knh_ExceptionHandler_t*)o;
	KNH_ADDREF(ctx, hdr->stacklist);
	KNH_SIZEREF(ctx);
}

static knh_ClassDef_t ExceptionHandlerDef = {
	ExceptionHandler_init, TODO_initcopy, ExceptionHandler_reftrace, BODY_free,
	DEFAULT_checkin, DEFAULT_checkout, DEFAULT_compareTo, DEFAULT_p,
	DEFAULT_getkey, DEFAULT_hashCode, DEFAULT_toint, DEFAULT_tofloat,
	DEFAULT_findTypeMapNULL, DEFAULT_1, DEFAULT_2, DEFAULT_3,
	"ExceptionHandler", CFLAG_ExceptionHandler, sizeof(knh_ExceptionHandlerEX_t), NULL,
	NULL, DEFAULT_4, DEFAULT_5, DEFAULT_6,
};

/* --------------- */
/* Regex */

static void Regex_init(CTX ctx, Object *o)
{
	knh_Regex_t *re = (knh_Regex_t*)o;
	KNH_INITv(re->pattern, TS_EMPTY);
	re->spi = knh_getStrRegexSPI();
	re->reg = (knh_regex_t*)TS_EMPTY;
}

static void Regex_reftrace(CTX ctx, Object *o FTRARG)
{
	knh_Regex_t *re = (knh_Regex_t*)o;
	KNH_ADDREF(ctx, (re->pattern));
	KNH_SIZEREF(ctx);
}

static void Regex_free(CTX ctx, Object *o)
{
	knh_Regex_t *re = (knh_Regex_t*)o;
	if(re->reg != NULL) {
		re->spi->regfree(ctx, re->reg);
		re->spi = NULL;
		re->reg = NULL;
	}
}

static void Regex_p(CTX ctx, knh_OutputStream_t *w, Object *o, int level)
{
	knh_Regex_t *re = (knh_Regex_t*)o;
	knh_write_quote(ctx, w, '/', S_tobytes(re->pattern), !String_isASCII(re->pattern));
}

static knh_ClassDef_t RegexDef = {
	Regex_init, DEFAULT_initcopy, Regex_reftrace, Regex_free,
	DEFAULT_checkin, DEFAULT_checkout, DEFAULT_compareTo, Regex_p,
	DEFAULT_getkey, DEFAULT_hashCode, DEFAULT_toint, DEFAULT_tofloat,
	DEFAULT_findTypeMapNULL, DEFAULT_1, DEFAULT_2, DEFAULT_3,
	"Regex", CFLAG_Regex, 0, NULL,
	NULL, DEFAULT_4, DEFAULT_5, DEFAULT_6,
};

/* --------------- */
/* StringEncoder */

static knh_bool_t conv_NOCONV(CTX ctx, knh_conv_t *c, knh_bytes_t t, knh_Bytes_t *tobuf)
{
	knh_Bytes_write(ctx, tobuf, t);  // this is necessary for default StringEncoder
	return 1;
}

static knh_ConvDSPI_t NOCONV_DSPI = {
	K_DSPI_CONVTO, "NOP",
	NULL,
	conv_NOCONV,
	conv_NOCONV,
	conv_NOCONV,
	conv_NOCONV,
	NULL/*knh_close_NOCONV*/,
	NULL/*knh_conv_NOSET*/,
};

static void Converter_init(CTX ctx, Object *o)
{
	knh_Converter_t *bc = (knh_Converter_t*)o;
	bc->conv = NULL;
	bc->dspi = &NOCONV_DSPI;
}

static void Converter_free(CTX ctx, Object *o)
{
	knh_Converter_t *bc = (knh_Converter_t*)o;
	if(bc->conv != NULL) {
		bc->dspi->close(ctx, bc->conv);
		bc->conv = NULL;
		bc->dspi = &NOCONV_DSPI;
	}
}

static knh_ClassDef_t ConverterDef = {
	Converter_init, DEFAULT_initcopy, DEFAULT_reftrace, Converter_free,
	DEFAULT_checkin, DEFAULT_checkout, DEFAULT_compareTo, DEFAULT_p,
	DEFAULT_getkey, DEFAULT_hashCode, DEFAULT_toint, DEFAULT_tofloat,
	DEFAULT_findTypeMapNULL, DEFAULT_1, DEFAULT_2, DEFAULT_3,
	"Converter", CFLAG_Converter, 0, NULL,
	NULL, DEFAULT_4, DEFAULT_5, DEFAULT_6,
};

static knh_ClassDef_t StringEncoderDef = {
	Converter_init, DEFAULT_initcopy, DEFAULT_reftrace, Converter_free,
	DEFAULT_checkin, DEFAULT_checkout, DEFAULT_compareTo, DEFAULT_p,
	DEFAULT_getkey, DEFAULT_hashCode, DEFAULT_toint, DEFAULT_tofloat,
	DEFAULT_findTypeMapNULL, DEFAULT_1, DEFAULT_2, DEFAULT_3,
	"StringEncoder", CFLAG_StringEncoder, 0, NULL,
	NULL, DEFAULT_4, DEFAULT_5, DEFAULT_6,
};

static knh_ClassDef_t StringDecoderDef = {
	Converter_init, DEFAULT_initcopy, DEFAULT_reftrace, Converter_free,
	DEFAULT_checkin, DEFAULT_checkout, DEFAULT_compareTo, DEFAULT_p,
	DEFAULT_getkey, DEFAULT_hashCode, DEFAULT_toint, DEFAULT_tofloat,
	DEFAULT_findTypeMapNULL, DEFAULT_1, DEFAULT_2, DEFAULT_3,
	"StringDecoder", CFLAG_StringDecoder, 0, NULL,
	NULL, DEFAULT_4, DEFAULT_5, DEFAULT_6,
};

static knh_ClassDef_t StringConverterDef = {
	Converter_init, DEFAULT_initcopy, DEFAULT_reftrace, Converter_free,
	DEFAULT_checkin, DEFAULT_checkout, DEFAULT_compareTo, DEFAULT_p,
	DEFAULT_getkey, DEFAULT_hashCode, DEFAULT_toint, DEFAULT_tofloat,
	DEFAULT_findTypeMapNULL, DEFAULT_1, DEFAULT_2, DEFAULT_3,
	"StringConverter", CFLAG_StringConverter, 0, NULL,
	NULL, DEFAULT_4, DEFAULT_5, DEFAULT_6,
};

/* --------------- */
/* Semantics */

#ifdef K_USING_SEMANTICS

static int knh_fichk__nop(knh_Semantics_t *u, knh_int_t v)
{
	return 1;
}

static int knh_ficmp__signed(knh_Semantics_t *u, knh_int_t v1, knh_int_t v2)
{
	return (int)(v1 - v2);
}

static int knh_ffchk__default(knh_Semantics_t *u, knh_float_t v)
{
	return 1;
}

static int knh_ffcmp__default(knh_Semantics_t *u, knh_float_t v1, knh_float_t v2)
{
	knh_float_t delta = v1 - v2;
	if(delta == K_FLOAT_ZERO) return 0;
	return delta < 0 ? -1 : 1;
}

static knh_String_t *knh_fsnew__default(CTX ctx, knh_class_t cid, knh_bytes_t t, knh_String_t *orig, int *foundError)
{
	return new_String_(ctx, cid, t, orig);
}

static int knh_fscmp__default(knh_Semantics_t *u, knh_bytes_t v1, knh_bytes_t v2)
{
	return knh_bytes_strcmp(v1, v2);
}

#define FUNC(f)     f
#else
#define FUNC(f)     NULL

#endif

static void Semantics_init(CTX ctx, Object *o)
{
	knh_SemanticsEX_t *b = knh_bodymalloc(ctx, Semantics);
	// common
	b->flag = 0;
	b->ucid  = 0;
	KNH_INITv(b->urn, TS_EMPTY);
	KNH_INITv(b->tag,  TS_EMPTY);
	b->ivalue = NULL;
	b->fvalue = NULL;
	b->svalue = NULL;

	// int
	b->imax  = K_INT_MAX;
	b->imin  = K_INT_MIN;
	b->fichk = FUNC(knh_fichk__nop);
	b->ficmp = FUNC(knh_ficmp__signed);

	// float
#ifndef K_USED_TO_BE_ON_LKM
	b->fstep = 0.001;
#else
	b->fstep = 1;
#endif
	b->fmax  = K_FLOAT_MAX;
	b->fmin  = K_FLOAT_MIN;
	b->ffchk = FUNC(knh_ffchk__default);
	b->ffcmp = FUNC(knh_ffcmp__default);
//	b->ffmt = knh_funitfmt__default;
//	b->FMT  = K_FLOAT_FMT;

	// String
	b->fsnew = FUNC(knh_fsnew__default);
	b->fscmp = FUNC(knh_fscmp__default);
	//b->fsconv = NULL;
	KNH_INITv(b->bconv, KNH_NULL);
	b->charlen = 0;
	b->bytelen = 0;
	KNH_INITv(b->pattern, KNH_NULL);
//	KNH_INITv(b->vocabDictIdx, KNH_NULL);
	o->ref = b;
}

static void Semantics_reftrace(CTX ctx, Object *o FTRARG)
{
	knh_Semantics_t *u = (knh_Semantics_t*)o;
	knh_SemanticsEX_t *b = DP(u);
	KNH_ADDREF(ctx, (b->urn));
	KNH_ADDREF(ctx, (b->tag));
	KNH_ADDNNREF(ctx, (b->ivalue));
	KNH_ADDNNREF(ctx, (b->fvalue));
	KNH_ADDNNREF(ctx, (b->svalue));
	KNH_ADDREF(ctx, (b->bconv));
	KNH_ADDREF(ctx, (b->pattern));
//	KNH_ADDREF(ctx, (b->vocabDictIdx));
	KNH_SIZEREF(ctx);
}

static knh_ClassDef_t SemanticsDef = {
	Semantics_init, TODO_initcopy, Semantics_reftrace, BODY_free,
	DEFAULT_checkin, DEFAULT_checkout, DEFAULT_compareTo, DEFAULT_p,
	DEFAULT_getkey, DEFAULT_hashCode, DEFAULT_toint, DEFAULT_tofloat,
	DEFAULT_findTypeMapNULL, DEFAULT_1, DEFAULT_2, DEFAULT_3,
	"Semantics", CFLAG_Semantics, sizeof(knh_SemanticsEX_t), NULL,
	NULL, DEFAULT_4, DEFAULT_5, DEFAULT_6,
};

/* --------------- */
/* InputStream */

static void InputStream_init(CTX ctx, Object *o)
{
	knh_InputStream_t *in = (knh_InputStream_t*)o;
	in->uline = 1;
	in->decNULL = NULL;
	knh_InputStreamEX_t *b = knh_bodymalloc(ctx, InputStream);
	in->dspi = knh_getDefaultStreamDSPI();
	b->fio = IO_BUF;
	KNH_INITv(b->ba, new_Bytes(ctx, "stream", 0));
	KNH_INITv(b->urn, TS_DEVNULL);
	KNH_INITv(b->mon, KNH_NULL);
	b->pos = 0; b->posend = 0;
	b->stat_size = 0;
	in->b = b;
}

static void InputStream_reftrace(CTX ctx, Object *o FTRARG)
{
	knh_InputStream_t *in = (knh_InputStream_t*)o;
	knh_InputStreamEX_t *b = DP(in);
	KNH_ADDREF(ctx, (b->ba));
	KNH_ADDNNREF(ctx, in->decNULL);
	KNH_ADDREF(ctx, (b->urn));
	KNH_SIZEREF(ctx);
}

static void InputStream_free(CTX ctx, Object *o)
{
	knh_InputStream_t *in = (knh_InputStream_t*)o;
	knh_InputStreamEX_t *b = DP(in);
	if(b->fio != IO_NULL) {
		in->dspi->fclose(ctx, b->fio);
		b->fio = IO_NULL;
	}
	knh_bodyfree(ctx, b, InputStream);
}

static void InputStream_p(CTX ctx, knh_OutputStream_t *w, Object *o, int level)
{
	knh_InputStream_t *ins = (knh_InputStream_t*)o;
	knh_write_quote(ctx, w, '\'', S_tobytes(DP(ins)->urn), !String_isASCII(DP(ins)->urn));
}

static knh_ClassDef_t InputStreamDef = {
	InputStream_init, DEFAULT_initcopy, InputStream_reftrace, InputStream_free,
	DEFAULT_checkin, DEFAULT_checkout, DEFAULT_compareTo, InputStream_p,
	DEFAULT_getkey, DEFAULT_hashCode, DEFAULT_toint, DEFAULT_tofloat,
	DEFAULT_findTypeMapNULL, DEFAULT_1, DEFAULT_2, DEFAULT_3,
	"InputStream", CFLAG_InputStream, sizeof(knh_InputStreamEX_t), NULL,
	NULL, DEFAULT_4, DEFAULT_5, DEFAULT_6,
};

/* --------------- */
/* OutputStream */

static void OutputStream_init(CTX ctx, Object *o)
{
	knh_OutputStream_t *w = (knh_OutputStream_t*)o;
	knh_OutputStreamEX_t *b = knh_bodymalloc(ctx, OutputStream);
	w->dspi = knh_getDefaultStreamDSPI();
	b->fio = IO_NULL;
	KNH_INITv(b->ba, new_Bytes(ctx, "stream", 0));
	KNH_INITv(b->urn, TS_DEVNULL);
	b->stat_size = 0;
	KNH_INITv(b->NEWLINE, TS_EOL);
	KNH_INITv(b->TAB, TS_TAB);
	b->indent = 0;
	w->encNULL = NULL;
	w->uline = 0;
	w->b = b;
	OutputStream_setBOL(w,1);
}

static void OutputStream_reftrace(CTX ctx, Object *o FTRARG)
{
	knh_OutputStream_t *w = (knh_OutputStream_t*)o;
	knh_OutputStreamEX_t *b = DP(w);
	KNH_ADDREF(ctx, (b->ba));
	KNH_ADDNNREF(ctx, (w->encNULL));
	KNH_ADDREF(ctx, (b->urn));
	KNH_ADDREF(ctx, (b->NEWLINE));
	KNH_ADDREF(ctx, (b->TAB));
	KNH_SIZEREF(ctx);
}

static void OutputStream_free(CTX ctx, Object *o)
{
	knh_OutputStream_t *w = (knh_OutputStream_t*)o;
	knh_OutputStreamEX_t *b = DP(w);
	if(b->fio != IO_NULL) {
		w->dspi->fclose(ctx, b->fio);
		b->fio = IO_NULL;
	}
	knh_bodyfree(ctx, b, OutputStream);
}

static void OutputStream_p(CTX ctx, knh_OutputStream_t *w, Object *o, int level)
{
	knh_OutputStream_t *ous = (knh_OutputStream_t*)o;
	knh_write_quote(ctx, w, '\'', S_tobytes(DP(ous)->urn) , !String_isASCII(DP(ous)->urn));
}

static knh_ClassDef_t OutputStreamDef = {
	OutputStream_init, DEFAULT_initcopy, OutputStream_reftrace, OutputStream_free,
	DEFAULT_checkin, DEFAULT_checkout, DEFAULT_compareTo, OutputStream_p,
	DEFAULT_getkey, DEFAULT_hashCode, DEFAULT_toint, DEFAULT_tofloat,
	DEFAULT_findTypeMapNULL, DEFAULT_1, DEFAULT_2, DEFAULT_3,
	"OutputStream", CFLAG_OutputStream, sizeof(knh_OutputStreamEX_t), NULL,
	NULL, DEFAULT_4, DEFAULT_5, DEFAULT_6,
};

/* --------------- */
/* Connection */

static void Connection_init(CTX ctx, Object *o)
{
	knh_Connection_t *c = (knh_Connection_t*)o;
	c->conn = NULL;
	c->dspi = knh_getQueryDSPI(ctx, NULL, K_DEFAULT_DSPI);
	KNH_INITv(c->urn, TS_EMPTY);
}

static void Connection_reftrace(CTX ctx, Object *o FTRARG)
{
	knh_Connection_t *c = (knh_Connection_t*)o;
	KNH_ADDREF(ctx, (c->urn));
	KNH_SIZEREF(ctx);
}

static void Connection_free(CTX ctx, Object *o)
{
	knh_Connection_t *c = (knh_Connection_t*)o;
	if(c->conn != NULL) {
		c->dspi->qclose(ctx, c->conn);
	}
}

static knh_ClassDef_t ConnectionDef = {
	Connection_init, DEFAULT_initcopy, Connection_reftrace, Connection_free,
	DEFAULT_checkin, DEFAULT_checkout, DEFAULT_compareTo, DEFAULT_p,
	DEFAULT_getkey, DEFAULT_hashCode, DEFAULT_toint, DEFAULT_tofloat,
	DEFAULT_findTypeMapNULL, DEFAULT_1, DEFAULT_2, DEFAULT_3,
	"Connection", CFLAG_Connection, 0, NULL,
	NULL, DEFAULT_4, DEFAULT_5, DEFAULT_6,
};

/* --------------- */
/* ResultSet */

static void ResultSet_init(CTX ctx, Object *o)
{
	knh_ResultSetEX_t *b = knh_bodymalloc(ctx, ResultSet);
	b->qcur = NULL;
	b->tcid = CLASS_ResultSet;
	b->column_size = 0;
	b->column = NULL;
	KNH_INITv(b->databuf, new_Bytes(ctx, "resultset", 0));
	KNH_INITv(b->conn, KNH_NULL);
	b->qcurfree = knh_getQueryDSPI(ctx, NULL, K_DEFAULT_DSPI)->qcurfree;
	b->count = 0;
	o->ref = b;
}

static void ResultSet_reftrace(CTX ctx, Object *o FTRARG)
{
	size_t i = 0;
	knh_ResultSetEX_t *b = DP((knh_ResultSet_t*)o);
	KNH_ADDREF(ctx, (b->databuf));
	for(i = 0; i < b->column_size; i++) {
		KNH_ADDREF(ctx, (b->column[i].name));
	}
	KNH_ADDREF(ctx, (b->conn));
	KNH_SIZEREF(ctx);
}

static void ResultSet_free(CTX ctx, Object *o)
{
	knh_ResultSetEX_t *b = DP((knh_ResultSet_t*)o);
	if(b->column != NULL) {
		KNH_FREE(ctx, b->column, sizeof(knh_dbschema_t) * b->column_size);
		b->column = NULL;
	}
	if(b->qcur != NULL) {
		b->qcurfree(b->qcur);
		b->qcur = NULL;
	}
	knh_bodyfree(ctx, b, ResultSet);
}

static knh_ClassDef_t ResultSetDef = {
	ResultSet_init, DEFAULT_initcopy, ResultSet_reftrace, ResultSet_free,
	DEFAULT_checkin, DEFAULT_checkout, DEFAULT_compareTo, DEFAULT_p,
	DEFAULT_getkey, DEFAULT_hashCode, DEFAULT_toint, DEFAULT_tofloat,
	DEFAULT_findTypeMapNULL, DEFAULT_1, DEFAULT_2, DEFAULT_3,
	"ResultSet", CFLAG_ResultSet, sizeof(knh_ResultSetEX_t), NULL,
	NULL, DEFAULT_4, DEFAULT_5, DEFAULT_6,
};

/* --------------- */
/* Script */

static void Script_init(CTX ctx, Object *o)
{
	knh_Script_t *scr = (knh_Script_t*)o;
	knh_class_t cid = new_ClassId(ctx);
	knh_ClassTBL_t *ct = varClassTBL(cid);
	scr->h.cTBL = (const knh_ClassTBL_t*)ct;
	DBG_ASSERT(ct->ospi == NULL);
	ct->cflag  = CFLAG_Script;
	ct->magicflag = KNH_MAGICFLAG(ct->cflag);
	ct->bcid   = CLASS_Script;
	ct->baseTBL = ClassTBL(CLASS_Script);
	ct->supcid = CLASS_Script;
	ct->supTBL = ClassTBL(CLASS_Script);
	knh_setClassDef(ct, ClassTBL(CLASS_Script)->ospi);
	KNH_INITv(ct->methods, K_EMPTYARRAY);
	KNH_INITv(ct->typemaps, K_EMPTYARRAY);
	knh_setClassName(ctx, cid, ClassTBL(CLASS_Script)->sname, ClassTBL(CLASS_Script)->sname);
	DBG_ASSERT(ct->defnull == NULL);
	scr->fields = NULL;
	knh_setClassDefaultValue(ctx, cid, scr, NULL);
	KNH_INITv(scr->ns, new_NameSpace(ctx, ctx->share->rootns));
}

static void Script_p(CTX ctx, knh_OutputStream_t *w, Object *o, int level)
{
	if(IS_FMTdump(level)) {
		const knh_ClassTBL_t *ct = O_cTBL(o);
		knh_Script_t *scr = (knh_Script_t*)o;
		size_t i;
		for(i = 0; i < ct->fsize; i++) {
			knh_fields_t *cf = ct->fields + i;
			if(cf->type == TYPE_void) continue;
			{
				knh_type_t type = knh_type_tocid(ctx, cf->type, ct->cid);
				knh_printf(ctx, w, "[%d] %T %s=", i, type, FN__(cf->fn));
				knh_write_TObject(ctx, w, type, scr->fields, i, FMT_line);
				knh_write_EOL(ctx, w);
			}
		}
	}
	else {
		ObjectField_p(ctx, w, o, level);
	}
}

static void Script_reftrace(CTX ctx, Object *o FTRARG)
{
	knh_Script_t *scr = (knh_Script_t*)o;
	KNH_ADDREF(ctx, scr->ns);
	ObjectField_reftrace(ctx, o FTRDATA);
}

static knh_ClassDef_t ScriptDef = {
	Script_init, DEFAULT_initcopy, Script_reftrace, ObjectField_free,
	DEFAULT_checkin, DEFAULT_checkout, DEFAULT_compareTo, Script_p,
	DEFAULT_getkey, DEFAULT_hashCode, DEFAULT_toint, DEFAULT_tofloat,
	DEFAULT_findTypeMapNULL, DEFAULT_1, DEFAULT_2, DEFAULT_3,
	"Script", CFLAG_Script, 0, NULL,
	NULL, DEFAULT_4, DEFAULT_5, DEFAULT_6,
};

/* --------------- */
/* NameSpace */

static void NameSpace_init(CTX ctx, Object *o)
{
	knh_NameSpace_t *ns = (knh_NameSpace_t*)o;
	knh_NameSpaceEX_t *b = knh_bodymalloc(ctx, NameSpace);
	KNH_INITv(b->nsname, TS_main);
	b->macroDictMapNULL = NULL;
	b->name2cidDictSetNULL = NULL;
	b->constDictCaseMapNULL = NULL;
	b->func2cidDictSetNULL = NULL;
	b->formattersNULL = NULL;
	b->methodsNULL = NULL;
	b->dspiDictSetNULL = NULL;
	ns->b = b;
	ns->parentNULL = NULL;
	ns->dlhdr = NULL;
	KNH_INITv(ns->rpath, TS_EMPTY);
}

static void NameSpace_reftrace(CTX ctx, Object *o FTRARG)
{
	knh_NameSpace_t *ns = (knh_NameSpace_t*)o;
	knh_NameSpaceEX_t *b = DP(ns);
	KNH_ADDNNREF(ctx, ns->parentNULL);
	KNH_ADDREF(ctx, ns->rpath);
	KNH_ADDREF(ctx, b->nsname);
	KNH_ADDNNREF(ctx, b->dspiDictSetNULL);
	KNH_ADDNNREF(ctx, b->macroDictMapNULL);
	KNH_ADDNNREF(ctx, b->name2cidDictSetNULL);
	KNH_ADDNNREF(ctx, b->func2cidDictSetNULL);
	KNH_ADDNNREF(ctx, b->constDictCaseMapNULL);
	KNH_ADDNNREF(ctx, b->formattersNULL);
	KNH_ADDNNREF(ctx, b->methodsNULL);
	KNH_SIZEREF(ctx);
}

static void NameSpace_free(CTX ctx, Object *o)
{
	BODY_free(ctx, o);
	if(((knh_NameSpace_t*)o)->dlhdr) {
		knh_dlclose(ctx, ((knh_NameSpace_t*)o)->dlhdr);
	}
}

static void NameSpace_p(CTX ctx, knh_OutputStream_t *w, Object *o, int level)
{
	knh_NameSpace_t *ns = (knh_NameSpace_t*)o;
	knh_write_ascii(ctx, w, "ns:");
	knh_write_ascii(ctx, w, S_tochar(ns->rpath));
}

static knh_ClassDef_t NameSpaceDef = {
	NameSpace_init, TODO_initcopy, NameSpace_reftrace, NameSpace_free,
	DEFAULT_checkin, DEFAULT_checkout, DEFAULT_compareTo, NameSpace_p,
	DEFAULT_getkey, DEFAULT_hashCode, DEFAULT_toint, DEFAULT_tofloat,
	DEFAULT_findTypeMapNULL, DEFAULT_1, DEFAULT_2, DEFAULT_3,
	"NameSpace", CFLAG_NameSpace, sizeof(knh_NameSpaceEX_t), NULL,
	NULL, DEFAULT_4, DEFAULT_5, DEFAULT_6,
};

/* --------------- */
/* System */

static knh_uintptr_t knh_autoSystemId = 0;

#ifdef K_USING_NOFILE
#define stdin  NULL
#define stdout NULL
#define stderr NULL
#endif

static void System_init(CTX ctx, Object *o)
{
	knh_SystemEX_t *sys = knh_bodymalloc(ctx, System);
	sys->sysid = knh_autoSystemId++;
	sys->ctxcount = 0;

	KNH_INITv(sys->ClassNameDictSet, new_DictSet0(ctx, 128, 1/*isCaseMap*/, "System.ClassNameDictSet"));
	KNH_INITv(sys->EventDictCaseSet, new_DictSet0(ctx, 32, 1/*isCaseMap*/, "System.EventDictSet"));
	KNH_INITv(sys->enc,   new_T(knh_getSystemEncoding()));
	KNH_INITv(sys->in,    new_InputStreamSTDIO(ctx, stdin, sys->enc));
	KNH_INITv(sys->out,   new_OutputStreamSTDIO(ctx, stdout, sys->enc));
	KNH_INITv(sys->err,   new_OutputStreamSTDIO(ctx, stderr, sys->enc));

	KNH_INITv(sys->props, new_DictMap0(ctx, 20, 1/*isCaseMap*/, "System.props"));
	KNH_INITv(sys->nameDictCaseSet, new_DictSet0(ctx, K_TFIELD_SIZE + 10, 1/*isCaseMap*/, "System.nameDictSet"));
	sys->namecapacity = k_goodsize2(K_TFIELD_SIZE + 10, sizeof(knh_nameinfo_t));
	sys->nameinfo = (knh_nameinfo_t*)KNH_REALLOC(ctx, "nameinfo", NULL, 0, sys->namecapacity, sizeof(knh_nameinfo_t));
	KNH_INITv(sys->urnDictSet, new_DictSet0(ctx, 0, 0/*isCaseMap*/, "System.urnDictSet"));
	KNH_INITv(sys->urns, new_Array0(ctx, 1));
	KNH_INITv(sys->tokenDictSet, new_DictSet0(ctx, (TT_MAX - STT_MAX), 0/*isCaseMap*/, "System.tokenDictSet"));
	KNH_INITv(sys->dspiDictSet, new_DictSet0(ctx, 32, 0/*isCaseMap*/, "System.dspiDictSet"));
	KNH_INITv(sys->PackageDictMap, new_DictMap0(ctx, 0, 0/*isCaseMap*/, "System.PackageDictMap"));
	KNH_INITv(sys->URNAliasDictMap, new_DictMap0(ctx, 0, 0/*isCaseMap*/, "System.URNAliasDictMap"));
	o->ref = sys;
}

static void System_reftrace(CTX ctx, Object *o FTRARG)
{
	knh_SystemEX_t *sys = DP((knh_System_t*)o);
	size_t i, size = knh_DictSet_size(sys->nameDictCaseSet);
	KNH_ADDREF(ctx, (sys->enc));
	KNH_ADDREF(ctx, (sys->in));
	KNH_ADDREF(ctx, (sys->out));
	KNH_ADDREF(ctx, (sys->err));
	KNH_ADDREF(ctx, (sys->props));
	KNH_ADDREF(ctx, (sys->EventDictCaseSet));
	KNH_ADDREF(ctx, (sys->ClassNameDictSet));
	KNH_ADDREF(ctx, (sys->nameDictCaseSet));
	KNH_ADDREF(ctx, (sys->urnDictSet));
	KNH_ADDREF(ctx, (sys->urns));
	KNH_ADDREF(ctx, (sys->tokenDictSet));
	KNH_ADDREF(ctx, (sys->PackageDictMap));
	KNH_ADDREF(ctx, (sys->URNAliasDictMap));
	KNH_ADDREF(ctx, (sys->dspiDictSet));
	KNH_ENSUREREF(ctx, size);
	for(i = 0; i < size; i++) {
		KNH_ADDREF(ctx, sys->nameinfo[i].name);
	}
	KNH_SIZEREF(ctx);
}

static void System_free(CTX ctx, Object *o)
{
	knh_SystemEX_t *sys = DP((knh_System_t*)o);
	KNH_FREE(ctx, sys->nameinfo, sizeof(knh_nameinfo_t) * sys->namecapacity);
	sys->nameinfo = NULL;
	knh_bodyfree(ctx, sys, System);
}

static knh_ClassDef_t SystemDef = {
	System_init, DEFAULT_initcopy, System_reftrace, System_free,
	DEFAULT_checkin, DEFAULT_checkout, DEFAULT_compareTo, DEFAULT_p,
	DEFAULT_getkey, DEFAULT_hashCode, DEFAULT_toint, DEFAULT_tofloat,
	DEFAULT_findTypeMapNULL, DEFAULT_1, DEFAULT_2, DEFAULT_3,
	"System", CFLAG_System, sizeof(knh_SystemEX_t), NULL,
	NULL, DEFAULT_4, DEFAULT_5, DEFAULT_6,
};

/* --------------- */
/* Context */

static void Context_init(CTX ctx, Object *o)
{
	KNH_TODO(__FUNCTION__);
}

static void Context_reftrace(CTX ctx, Object *o FTRARG)
{
	KNH_TODO(__FUNCTION__);
}

static void Context_free(CTX ctx, Object *o)
{
	KNH_TODO(__FUNCTION__);
}

static knh_ClassDef_t ContextDef = {
	Context_init, DEFAULT_initcopy, Context_reftrace, Context_free,
	DEFAULT_checkin, DEFAULT_checkout, DEFAULT_compareTo, DEFAULT_p,
	DEFAULT_getkey, DEFAULT_hashCode, DEFAULT_toint, DEFAULT_tofloat,
	DEFAULT_findTypeMapNULL, DEFAULT_1, DEFAULT_2, DEFAULT_3,
	"Context", CFLAG_Context, 0, NULL,
	NULL, DEFAULT_4, DEFAULT_5, DEFAULT_6,
};

/* --------------- */
/* Monitor */

static void mtrace(CTX ctx, knh_sfp_t *sfp, struct knh_Monitor_t *mon, const char *ns, const char *evt, const char *fmt, ...)
{
	va_list ap;
	va_start(ap , fmt);
	knh_vtrace(ctx, sfp, (mon)->loglevel, ns, evt, Monitor_isThrowable(mon), fmt, ap);
	va_end(ap);
}

static void Monitor_init(CTX ctx, Object *o)
{
	knh_Monitor_t *mon = (knh_Monitor_t*)o;
	mon->loglevel = LOG_NOTICE;
	mon->trace = mtrace;
}

static knh_ClassDef_t MonitorDef = {
	Monitor_init, DEFAULT_initcopy, DEFAULT_reftrace, DEFAULT_free,
	DEFAULT_checkin, DEFAULT_checkout, DEFAULT_compareTo, DEFAULT_p,
	DEFAULT_getkey, DEFAULT_hashCode, DEFAULT_toint, DEFAULT_tofloat,
	DEFAULT_findTypeMapNULL, DEFAULT_1, DEFAULT_2, DEFAULT_3,
	"Monitor", CFLAG_Monitor, 0, NULL,
	NULL, DEFAULT_4, DEFAULT_5, DEFAULT_6,
};

/* --------------- */
/* Assurance */

static void Assurance_init(CTX ctx, Object *o)
{
	knh_Assurance_t *g = (knh_Assurance_t*)o;
	KNH_INITv(g->msg, TS_EMPTY);
	g->aid = 0;
	g->stime = 0;
}

static void Assurance_reftrace(CTX ctx, Object *o FTRARG)
{
	knh_Assurance_t *g = (knh_Assurance_t*)o;
	KNH_ADDREF(ctx, g->msg);
	KNH_SIZEREF(ctx);
}

static void Assurance_p(CTX ctx, knh_OutputStream_t *w, Object *o, int level)
{
	knh_Assurance_t *g = (knh_Assurance_t*)o;
	knh_write_quote(ctx, w, '\'', S_tobytes(g->msg), !String_isASCII(g->msg));
}

static void Assurance_checkin(CTX ctx, knh_sfp_t *sfp, Object *o)
{
	static knh_uintptr_t uid = 0;
	knh_Assurance_t *g = (knh_Assurance_t*)o;
	g->aid = uid++;
	g->stime = (knh_getTimeMilliSecond() / 1000);
	KNH_SYSLOG_(ctx, sfp, LOG_NOTICE, "ac", "CHECKIN", "id=%d, case='%s'", g->aid, S_tochar(g->msg));
}

static void Assurance_checkout(CTX ctx, Object *o, int isFailed)
{
	knh_Assurance_t *g = (knh_Assurance_t*)o;
	if(isFailed) {
		KNH_SYSLOG_(ctx, NULL, LOG_WARNING, "ac", "FAILED", "id=%d, case='%s'", g->aid, S_tochar(g->msg));
	}
	else {
		knh_intptr_t t = (knh_getTimeMilliSecond() / 1000) - g->stime;
		if(t > 1) {
			KNH_SYSLOG_(ctx, NULL, LOG_NOTICE, "ac", "CHECKOUT", "id=%d, case='%s', time=%ds", g->aid, S_tochar(g->msg), t);
		}
		else {
			KNH_SYSLOG_(ctx, NULL, LOG_NOTICE, "ac", "CHECKOUT", "id=%d, case='%s'", g->aid, S_tochar(g->msg));
		}
	}
	if(knh_getUFILE() != NULL) {
		const char *results = (isFailed) ? "FAILED" : "PASSED";
		fprintf(knh_getUFILE(), "%s @%s\n", results, S_tochar(g->msg));
	}
}

static knh_ClassDef_t AssuranceDef = {
	Assurance_init, DEFAULT_initcopy, Assurance_reftrace, DEFAULT_free,
	Assurance_checkin, Assurance_checkout, DEFAULT_compareTo, Assurance_p,
	DEFAULT_getkey, DEFAULT_hashCode, DEFAULT_toint, DEFAULT_tofloat,
	DEFAULT_findTypeMapNULL, DEFAULT_1, DEFAULT_2, DEFAULT_3,
	"Assurance", CFLAG_Assurance, 0, NULL,
	NULL, DEFAULT_4, DEFAULT_5, DEFAULT_6,
};

/* --------------- */
/* Token */

static void Token_init(CTX ctx, Object *o)
{
	knh_Token_t *tk = (knh_Token_t*)o;
	tk->tt        =  TT_ASIS;
	tk->type      =  TYPE_var;
	tk->uline     =   0;
	tk->flag0 = 0;
	tk->index = 0;
	KNH_INITv(tk->data, KNH_NULL);
}

static void Token_reftrace(CTX ctx, Object *o FTRARG)
{
	knh_Token_t *tk = (knh_Token_t*)o;
	KNH_ADDREF(ctx, tk->data);
	KNH_SIZEREF(ctx);
}

const char* TT__(knh_term_t tt);

static void Token_p(CTX ctx, knh_OutputStream_t *w, Object *o, int level)
{
	knh_Token_t *tk = (knh_Token_t*)o;
	knh_term_t tt = tk->tt;
	if(tt < TT_NUM) {
		knh_write_ascii(ctx, w, TT__(tt));
		if(tt == TT_PARENTHESIS || tt == TT_BRACE || tt == TT_BRANCET) {
			if(IS_Token(tk->data)) {
				knh_write_InObject(ctx, w, tk->data, level);
			}
			else if(IS_Array(tk->data)) {
				size_t i;
				for(i = 0; i < knh_Array_size(tk->list); i++) {
					if(i > 0) knh_putc(ctx, w, ' ');
					knh_write_InObject(ctx, w, tk->list->list[i], level);
				}
			}
			if(tt == TT_PARENTHESIS) {
				knh_putc(ctx, w, ']');
			}else if(tt == TT_BRACE) {
				knh_putc(ctx, w, '}');
			}else {
				knh_putc(ctx, w, ']');
			}
		}
	}
	else {
		knh_bytes_t t = S_tobytes(tk->text);
		int hasUTF8 = !(String_isASCII(tk->text));
		switch(tt) {
		case TT_NUM: knh_write(ctx, w, t); break;
		case TT_STR:  knh_write_quote(ctx, w, '"', t, hasUTF8); break;
		case TT_TSTR: knh_write_quote(ctx, w, '\'', t, hasUTF8); break;
		case TT_ESTR: knh_write_quote(ctx, w, '`', t, hasUTF8); break;
		case TT_REGEX: knh_write_quote(ctx, w, '/', t, hasUTF8); break;
//		case TT_DOC:
		case TT_METAN: knh_putc(ctx, w, '@'); knh_write(ctx, w, t); break;
		case TT_PROPN: knh_putc(ctx, w, '$'); knh_write(ctx, w, t); break;
		case TT_URN: case TT_TSCHEME:
			knh_write_utf8(ctx, w, t, hasUTF8); break;
		case TT_NAME: case TT_UNAME:
			if(Token_isDOT(tk)) knh_putc(ctx, w, '.');
			if(Token_isGetter(tk)) knh_write(ctx, w, STEXT("get_"));
			else if(Token_isSetter(tk)) knh_write(ctx, w, STEXT("set_"));
			else if(Token_isISBOOL(tk)) knh_write(ctx, w, STEXT("is_"));
			knh_write(ctx, w, t); break;
			if(Token_isExceptionType(tk)) {
				knh_write(ctx, w, STEXT("!!"));
			}
			break;
		case TT_FUNCNAME: case TT_UFUNCNAME:
			knh_write(ctx, w, t); break;
		case TT_PTYPE:
			knh_write_Object(ctx, w, tk->data, FMT_line); break;
		case TT_CID:
			knh_write_cid(ctx, w, tk->mn); break;
		case TT_MN:
			knh_write_mn(ctx, w, tk->mn); break;
		case TT_CONST:
			knh_write_Object(ctx, w, tk->data, FMT_line);
			break;
		case TT_SYSVAL: case TT_LOCAL: case TT_FUNCVAR:
		case TT_XLOCAL: case TT_FIELD: case TT_SCRFIELD:
			if(IS_FMTdump(level) || !IS_String(tk->data)) {
				knh_write_ascii(ctx, w, TT__(tt));
				knh_putc(ctx, w, '=');
				knh_write_ifmt(ctx, w, K_INT_FMT, (knh_int_t)tk->index);
				break;
			}
		case TT_ERR:
			knh_write(ctx, w, t); break;
		default:
			fprintf(stderr, "DEFINE %s in Token_stmt", TT__(tt));
		}
	}
	if(IS_FMTdump(level)) {
		if(SP(tk)->uline != 0) {
			knh_write(ctx, w, STEXT("+L"));
			knh_write_ifmt(ctx, w, K_INT_FMT, ULINE_line(tk->uline));
		}
		if(SP(tk)->type != TYPE_var) {
			knh_write(ctx, w, STEXT("+:")); knh_write_type(ctx, w, SP(tk)->type);
		}
	}
}

static knh_ClassDef_t TokenDef = {
	Token_init, TODO_initcopy, Token_reftrace, DEFAULT_free,
	DEFAULT_checkin, DEFAULT_checkout, DEFAULT_compareTo, Token_p,
	DEFAULT_getkey, DEFAULT_hashCode, DEFAULT_toint, DEFAULT_tofloat,
	DEFAULT_findTypeMapNULL, DEFAULT_1, DEFAULT_2, DEFAULT_3,
	"Token", CFLAG_Token, 0, NULL,
	NULL, DEFAULT_4, DEFAULT_5, DEFAULT_6,
};

/* --------------- */
/* Stmt */

static void Stmt_init(CTX ctx, Object *o)
{
	knh_Stmt_t *stmt = (knh_Stmt_t*)o;
	knh_StmtEX_t *b = knh_bodymalloc(ctx, Stmt);
	SP(stmt)->uline = 0;
	SP(stmt)->stt   = STT_DONE;
	SP(stmt)->type = TYPE_var;
	(stmt)->terms = NULL;
	b->flag0 = 0;
	b->espidx = 0;
	b->size = 0;
	b->capacity = 0;
	b->nextNULL = NULL;
	KNH_INITv(b->metaDictCaseMap,  KNH_NULL);
	o->ref = b;
}

static void Stmt_reftrace(CTX ctx, Object *o FTRARG)
{
	knh_Stmt_t *stmt = (knh_Stmt_t*)o;
	knh_StmtEX_t *b = DP((knh_Stmt_t*)o);
	KNH_ADDREF(ctx, (b->metaDictCaseMap));
	KNH_ADDNNREF(ctx, (b->nextNULL));
	if(stmt->terms != NULL) {
		size_t i;
		KNH_ENSUREREF(ctx, b->size);
		for(i = 0; i < b->size; i++) {
			KNH_ADDREF(ctx, stmt->terms[i]);
		}
	}
	KNH_SIZEREF(ctx);
}

static void Stmt_free(CTX ctx, Object *o)
{
	knh_Stmt_t *stmt = (knh_Stmt_t*)o;
	knh_StmtEX_t *b = DP((knh_Stmt_t*)o);
	if(stmt->terms != NULL) {
		KNH_FREE(ctx, stmt->terms, sizeof(knh_Term_t*) * b->capacity);
	}
	knh_bodyfree(ctx, b, Stmt);
}

static void Stmt_p(CTX ctx, knh_OutputStream_t *w, Object *o, int level)
{
	knh_Stmt_t *stmt = (knh_Stmt_t*)o;
	knh_intptr_t i, size;
	L_TAILCALLED:;
	knh_putc(ctx, w, '(');
	if(!IS_FMTs(level)) {
		if(IS_Map(DP(stmt)->metaDictCaseMap)) {
			size = knh_DictMap_size(DP(stmt)->metaDictCaseMap);
			for(i = 0; i < size; i++) {
				knh_String_t *k = knh_DictMap_keyAt(DP(stmt)->metaDictCaseMap, i);
				knh_String_t *v = (knh_String_t*)knh_DictMap_valueAt(DP(stmt)->metaDictCaseMap, i);
				if(k == v) {
					knh_printf(ctx, w, "@%s ", S_tochar(k));
				}
				else {
					knh_printf(ctx, w, "@%s(%O) ", S_tochar(k), v);
				}
			}
		}
	}
	knh_write_ascii(ctx, w, TT__(stmt->stt));
	for(i = 0; i < DP(stmt)->size; i++) {
		knh_putc(ctx, w, ' ');
		knh_write_InObject(ctx, w, (Object*)(stmt->terms[i]), FMT_line);
	}
	knh_putc(ctx, w, ')');
	if(IS_FMTdump(level)) {
		if(SP(stmt)->type != TYPE_var) {
			knh_write(ctx, w, STEXT("+:")); knh_write_type(ctx, w, SP(stmt)->type);
		}
	}
	if(DP(stmt)->nextNULL != NULL) {
		if(IS_FMTdump(level)) {
			knh_write_EOL(ctx, w);
			stmt = DP(stmt)->nextNULL;
			goto L_TAILCALLED;
		}
		else {
			knh_write_dots(ctx, w);
		}
	}
}

static knh_ClassDef_t StmtDef = {
	Stmt_init, TODO_initcopy, Stmt_reftrace, Stmt_free,
	DEFAULT_checkin, DEFAULT_checkout, DEFAULT_compareTo, Stmt_p,
	DEFAULT_getkey, DEFAULT_hashCode, DEFAULT_toint, DEFAULT_tofloat,
	DEFAULT_findTypeMapNULL, DEFAULT_1, DEFAULT_2, DEFAULT_3,
	"Stmt", CFLAG_Stmt, sizeof(knh_StmtEX_t), NULL,
	NULL, DEFAULT_4, DEFAULT_5, DEFAULT_6,
};

/* Gamma */

static void Gamma_init(CTX ctx, Object *o)
{
	knh_GammaEX_t *b = knh_bodymalloc(ctx, Gamma);
	knh_bzero(b, sizeof(knh_GammaEX_t));
	b->cflag = FLAG_Gamma_InlineFunction | FLAG_Gamma_TailRecursion;
	KNH_INITv(b->mtd, KNH_NULL);
	KNH_INITv(b->stmt, KNH_NULL);
	KNH_INITv(b->lstacks, new_Array0(ctx, 0));
	KNH_INITv(b->insts, new_Array0(ctx, 0));
	KNH_INITv(b->errmsgs, new_Array0(ctx, 0));
	KNH_INITv(b->finallyStmt, KNH_NULL);
	o->ref = b;
	KNH_INITv(((knh_Gamma_t*)o)->scr, ctx->script);
}

static void Gamma_reftrace(CTX ctx, Object *o FTRARG)
{
	size_t i;
	knh_GammaEX_t *b = DP((knh_Gamma_t*)o);
	KNH_ENSUREREF(ctx, b->gcapacity * 2);
	for(i = 0; i < b->gcapacity; i++) {
		KNH_ADDREF(ctx, b->gf[i].value);
		KNH_ADDREF(ctx, b->gf[i].tkIDX);
	}
	KNH_ADDREF(ctx, (b->mtd));
	KNH_ADDREF(ctx, (b->stmt));
	KNH_ADDREF(ctx, (b->lstacks));
	KNH_ADDREF(ctx, (b->insts));
	KNH_ADDREF(ctx, (b->errmsgs));
	KNH_ADDREF(ctx, (b->finallyStmt));
	KNH_ADDREF(ctx, ((knh_Gamma_t*)o)->scr);
	KNH_SIZEREF(ctx);
}

static void Gamma_free(CTX ctx, Object *o)
{
	knh_GammaEX_t *b = DP((knh_Gamma_t*)o);
	if(b->gcapacity) {
		KNH_FREE(ctx, b->gf, b->gcapacity * sizeof(knh_gmafields_t));
	}
	knh_bodyfree(ctx, b, Gamma);
}

static knh_ClassDef_t GammaDef = {
	Gamma_init, TODO_initcopy, Gamma_reftrace, Gamma_free,
	DEFAULT_checkin, DEFAULT_checkout, DEFAULT_compareTo, DEFAULT_p,
	DEFAULT_getkey, DEFAULT_hashCode, DEFAULT_toint, DEFAULT_tofloat,
	DEFAULT_findTypeMapNULL, DEFAULT_1, DEFAULT_2, DEFAULT_3,
	"Gamma", CFLAG_Gamma, sizeof(knh_GammaEX_t), NULL,
	NULL, DEFAULT_4, DEFAULT_5, DEFAULT_6,
};

/* --------------- */
/* BasicBlock */

static void BasicBlock_init(CTX ctx, Object *o)
{
	knh_BasicBlock_t *bb = (knh_BasicBlock_t*)o;
	bb->b = knh_bodymalloc(ctx, BasicBlock);
	knh_bzero(bb->b, sizeof(knh_BasicBlockEX_t));
	bb->listNC  = NULL;
	bb->nextNC  = NULL;
	bb->jumpNC  = NULL;
}

static void BasicBlock_reftrace(CTX ctx, Object *o FTRARG)
{
	knh_BasicBlock_t *bb = (knh_BasicBlock_t*)o;
	size_t i;
	KNH_ENSUREREF(ctx, DP(bb)->size);
	for(i = 0; i < DP(bb)->size; i++) {
#ifdef K_USING_CSTACK_TRAVERSE_
		knh_opline_reftrace(ctx, DP(bb)->opbuf + i FTRDATA);
#else
		tail_ = knh_opline_reftrace(ctx, DP(bb)->opbuf + i FTRDATA);
#endif
	}
	KNH_SIZEREF(ctx);
}

static void BasicBlock_free(CTX ctx, Object *o)
{
	knh_BasicBlock_t *bb = (knh_BasicBlock_t*)o;
	if(DP(bb)->capacity > 0) {
		KNH_FREE(ctx, DP(bb)->opbuf, DP(bb)->capacity * sizeof(knh_opline_t));
	}
	knh_bodyfree(ctx, DP(bb), BasicBlock);
}

static knh_ClassDef_t BasicBlockDef = {
	BasicBlock_init, TODO_initcopy, BasicBlock_reftrace, BasicBlock_free,
	DEFAULT_checkin, DEFAULT_checkout, DEFAULT_compareTo, DEFAULT_p,
	DEFAULT_getkey, DEFAULT_hashCode, DEFAULT_toint, DEFAULT_tofloat,
	DEFAULT_findTypeMapNULL, DEFAULT_1, DEFAULT_2, DEFAULT_3,
	"BasicBlock", CFLAG_BasicBlock, sizeof(knh_BasicBlockEX_t), NULL,
	NULL, DEFAULT_4, DEFAULT_5, DEFAULT_6,
};

/* --------------- */
/* KonohaCode */

static void KonohaCode_init(CTX ctx, Object *o)
{
	knh_KonohaCode_t *b = (knh_KonohaCode_t*)o;
	b->codesize = 0;
	b->code = NULL;
	b->uri = 0;
	KNH_INITv(b->source, TS_EMPTY);
}

static void KonohaCode_reftrace(CTX ctx, Object *o FTRARG)
{
	knh_KonohaCode_t *b = (knh_KonohaCode_t*)o;
	knh_opline_t *pc = b->code;
	KNH_ADDREF(ctx, b->source);
	KNH_ENSUREREF(ctx, b->codesize / sizeof(knh_opline_t));
	while(pc->opcode != OPCODE_RET) {
#ifdef K_USING_CSTACK_TRAVERSE_
		knh_opline_reftrace(ctx, pc FTRDATA);
#else
		tail_ = knh_opline_reftrace(ctx, pc FTRDATA);
#endif
		pc++;
	}
	KNH_SIZEREF(ctx);
}

static void KonohaCode_free(CTX ctx, Object *o)
{
	knh_KonohaCode_t *b = (knh_KonohaCode_t*)o;
#ifdef K_USING_VMCOUNT  /* Who added this ?*/
	knh_opline_t *pc = b->code;
	while(pc->opcode != OPCODE_RET) {
		knh_opcode_count(ctx, pc);
		pc++;
	}
#endif
	KNH_FREE(ctx, b->code, b->codesize);
}

static void KonohaCode_p(CTX ctx, knh_OutputStream_t *w, Object *o, int level)
{
	knh_KonohaCode_t *kcode = (knh_KonohaCode_t*)o;
	knh_opline_t *pc = kcode->code + 1;
	while(1) {
		knh_opcode_dump(ctx, pc, w, kcode->code + 1);
		if(pc->opcode == OPCODE_RET) break;
		pc++;
	}
}

static knh_ClassDef_t KonohaCodeDef = {
	KonohaCode_init, TODO_initcopy, KonohaCode_reftrace, KonohaCode_free,
	DEFAULT_checkin, DEFAULT_checkout, DEFAULT_compareTo, KonohaCode_p,
	DEFAULT_getkey, DEFAULT_hashCode, DEFAULT_toint, DEFAULT_tofloat,
	DEFAULT_findTypeMapNULL, DEFAULT_1, DEFAULT_2, DEFAULT_3,
	"KonohaCode", CFLAG_KonohaCode, 0, NULL,
	NULL, DEFAULT_4, DEFAULT_5, DEFAULT_6,
};

static knh_ClassDef_t ImmutableDef = {
	DEFAULT_init, DEFAULT_initcopy, DEFAULT_reftrace, DEFAULT_free,
	DEFAULT_checkin, DEFAULT_checkout, DEFAULT_compareTo, DEFAULT_p,
	DEFAULT_getkey, DEFAULT_hashCode, DEFAULT_toint, DEFAULT_tofloat,
	DEFAULT_findTypeMapNULL, DEFAULT_1, DEFAULT_2, DEFAULT_3,
	"Immutable", CFLAG_Immutable, 0, NULL,
	NULL, DEFAULT_4, DEFAULT_5, DEFAULT_6,
};

static knh_ClassDef_t KindOfDef = {
	DEFAULT_init, DEFAULT_initcopy, DEFAULT_reftrace, DEFAULT_free,
	DEFAULT_checkin, DEFAULT_checkout, DEFAULT_compareTo, DEFAULT_p,
	DEFAULT_getkey, DEFAULT_hashCode, DEFAULT_toint, DEFAULT_tofloat,
	DEFAULT_findTypeMapNULL, DEFAULT_1, DEFAULT_2, DEFAULT_3,
	"KindOf", CFLAG_KonohaCode, 0, NULL,
	NULL, DEFAULT_4, DEFAULT_5, DEFAULT_6,
};

/* --------------- */
/* @data*/

typedef struct {
	const char   *name;
	int           index;
} knh_StringData0_t ;

typedef struct {
	const char *name;
	knh_fieldn_t fn;
} knh_FieldNameData0_t ;

/* --------------- */

#ifdef __cplusplus
}
#endif

#define K_INCLUDE_BUILTINAPI

#include"operator.c"
#include"class.c"
#include"number.c"
#include"string.c"
#include"bytes.c"
#include"array.c"
#include"map.c"

#include"system.c"
#include"stream.c"
#include"query.c"

#include"structdata.h"

#ifdef __cplusplus
extern "C" {
#endif

/* ------------------------------------------------------------------------ */

static Object *knh_Context_fdefault(CTX ctx, knh_class_t cid)
{
	KNH_TODO(__FUNCTION__);
	return (Object*)ctx;
}

static void knh_setDefaultValues(CTX ctx)
{
	knh_ClassTBL_t *ct = (knh_ClassTBL_t *)ClassTBL(CLASS_Tuple);
	DBG_P("Tuple's super=%s", CLASS__(ct->supcid));
	ct->supcid = CLASS_Object;
	ct->supTBL = ClassTBL(CLASS_Object);
	knh_setClassDefaultValue(ctx, CLASS_Object, KNH_NULL, NULL);
	knh_setClassDefaultValue(ctx, CLASS_Tdynamic, KNH_NULL, NULL);
	knh_setClassDefaultValue(ctx, CLASS_Boolean, KNH_FALSE, NULL);
#if defined(K_USING_SEMANTICS)
	{
		knh_Semantics_t *u = new_(Semantics);
		KNH_INITv(DP(u)->ivalue, KNH_INT0);
		KNH_INITv(DP(u)->fvalue, KNH_FLOAT0);
		KNH_INITv(DP(u)->svalue, TS_EMPTY);
		knh_setClassDefaultValue(ctx, CLASS_Semantics, u, NULL);
	}
#endif
	// load file/Channel/regex/db drivers
	knh_setClassDefaultValue(ctx, CLASS_Context, KNH_NULL, knh_Context_fdefault);
	knh_setClassDefaultValue(ctx, CLASS_NameSpace, UPCAST(ctx->share->rootns), NULL);
	knh_setClassDefaultValue(ctx, CLASS_System, UPCAST(ctx->sys), NULL);
	knh_loadScriptDriver(ctx, ctx->share->rootns);
	{
		knh_Token_t *tk = KNH_TNULL(Token);
		tk->tt = TT_FUNCVAR;
		(tk)->index = 0;
	}
}

static void knh_loadScriptFieldNameData0(CTX ctx, knh_FieldNameData0_t *data)
{
	while(data->name != NULL) {
		knh_String_t *name = new_T(data->name);
#if defined(K_USING_DEBUG)
		knh_fieldn_t fn = knh_addname(ctx, name, knh_DictSet_append);
		DBG_ASSERT(fn == data->fn - MN_OPSIZE);
#else
		knh_addname(ctx, name, knh_DictSet_append);
#endif
		data++;
	}
}

/* ------------------------------------------------------------------------ */
/* @data */

static const knh_IntData_t IntConstData0[] = {
	{"Int.MAX", K_INT_MAX},
	{"Int.MIN", K_INT_MIN},
	{NULL, 0}
};

static const knh_FloatData_t FloatConstData0[] = {
	{"Float.MAX", K_FLOAT_MAX},
	{"Float.MIN", K_FLOAT_MIN},
	{NULL, K_FLOAT_ZERO}
};

static const knh_StringData_t StringConstData0[] = {
	{"$konoha.version", K_VERSION},
	{NULL, NULL},
};

#define FN_K FN_k
#define FN_V FN_v
#define FN_T FN_t
#define FN_U FN_u
#define FN_P FN_p
#define FN_R FN_r

static const knh_data_t CParamData0[] = {
	DATA_CPARAM, CLASS_Iterator,  (knh_data_t)"konoha.Iterator<dyn>", 1, 0, TYPE_dyn, FN_V,
	DATA_CPARAM, CLASS_Range,     (knh_data_t)"konoha.Range<dyn>", 1, 0, TYPE_dyn, FN_V,
	DATA_CPARAM, CLASS_Array,     (knh_data_t)"konoha.Array<dyn>", 1, 0, TYPE_dyn, FN_V,
	DATA_CPARAM, CLASS_Map,       (knh_data_t)"konoha.Map<konoha.String,dyn>", 2, 0, TYPE_String, FN_K, TYPE_dyn, FN_V,
	DATA_CPARAM, CLASS_Tuple,     (knh_data_t)"konoha.Map<konoha.String,dyn>", 2, 0, TYPE_String, FN_K, TYPE_dyn, FN_V,
	DATA_CPARAM, CLASS_Func,      (knh_data_t)"konoha.Func<void>", 0, 0,
	DATA_CPARAM, CLASS_Thunk,     (knh_data_t)"konoha.Thunk<dyn>", 1, 0, TYPE_dyn, FN_V,
	DATA_CPARAM, CLASS_Immutable, (knh_data_t)"konoha.Immutable<dyn>", 1, 0, TYPE_dyn, FN_V,
	DATA_CPARAM, CLASS_KindOf,    (knh_data_t)"konoha.KindOf<void>", 1, 0, TYPE_void, FN_V,
	0,
};

void knh_loadScriptSystemStructData(CTX ctx, const knh_PackageLoaderAPI_t *kapi)
{
	kapi->loadData(ctx, StructData0, NULL);
	{
		knh_ClassTBL_t *t = varClassTBL(CLASS_Tvoid);
		t->supcid = CLASS_Object;
		t->supTBL = ClassTBL(CLASS_Object);
		t = varClassTBL(CLASS_Tvar);
		t->supcid = CLASS_Object;
		t->supTBL = ClassTBL(CLASS_Object);
	}
}

void knh_loadScriptSystemString(CTX ctx)
{
	const char **data = StringData0;
	size_t i = 0;
	for(i = 0; *data != NULL; i++) {
		DBG_ASSERT(ctx->share->tString[i] == NULL);
		DBG_ASSERT(i < (SIZEOF_TSTRING / sizeof(knh_String_t*)));
		KNH_INITv(ctx->share->tString[i], new_T(*data));
		data++;
	}
}

void knh_loadScriptSystemData(CTX ctx, knh_NameSpace_t *ns, const knh_PackageLoaderAPI_t *kapi)
{
	kapi->loadData(ctx, ClassData0, NULL);
	kapi->loadData(ctx, CParamData0, NULL);
	kapi->loadIntData(ctx, ns, IntConstData0);
	kapi->loadFloatData(ctx, ns, FloatConstData0);
	kapi->loadStringData(ctx, ns, StringConstData0);
	knh_getURI(ctx, STEXT("(eval)"));  // URI_EVAL
	knh_setDefaultValues(ctx);
	knh_loadScriptDefaultMapDSPI(ctx, ns);
}

void knh_loadScriptSystemMethod(CTX ctx, const knh_PackageLoaderAPI_t *kapi)
{
	knh_ParamArray_t *pools[K_PARAM0_SIZE];
	knh_loadScriptFieldNameData0(ctx, FieldNameData0);
	kapi->loadData(ctx, ParamArrayData0, pools);
	kapi->loadData(ctx, APIData0, pools);
}

/* ------------------------------------------------------------------------ */

#ifdef __cplusplus
}
#endif
