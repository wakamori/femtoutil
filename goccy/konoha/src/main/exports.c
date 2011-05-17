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

#define USE_B 1
#define USE_bytes_rindex 1
#define USE_bytes_first  1

#include"commons.h"

/* ************************************************************************ */

#ifdef __cplusplus
extern "C" {
#endif

/* ------------------------------------------------------------------------ */

static void knh_setsfp(CTX ctx, knh_sfp_t *sfp, void *v)
{
	knh_Object_t *o = (knh_Object_t*)v;
	DBG_ASSERT_ISOBJECT(o);
	knh_Object_RCinc(o);
	knh_Object_RCdec(sfp[0].o);
	if(Object_isRC0(sfp[0].o)) {
		knh_Object_RCfree(ctx, sfp[0].o);
	}
	sfp[0].o = o;
}

KNHAPI2(knh_text_t*) knh_cwb_tochar(CTX ctx, knh_cwb_t *cwb)
{
	return knh_Bytes_ensureZero(ctx, cwb->ba) + cwb->pos;
}

const knh_ExportsAPI_t *knh_getExportsAPI(void)
{
	static knh_ExportsAPI_t exports = {
		knh_fastmalloc, knh_fastfree, /* memory.c */
		knh_setsfp, knh_Iterator_close,
		knh_trace, dbg_p, todo_p,
	};
	return &exports;
}

/* ------------------------------------------------------------------------ */
/* [CONST/PROPERTY DATA] */

static void knh_addConstData(CTX ctx, knh_NameSpace_t *ns, const char *dname, Object *value)
{
	if(dname[0] == '$') {
		knh_String_t *n = new_T(dname + 1);
		DictMap_set_(ctx, DP(ctx->sys)->props, n, value);
	}
	else {
		knh_bytes_t n = {{dname}, knh_strlen(dname)};
		knh_index_t loc = knh_bytes_rindex(n, '.');
		knh_String_t *name = new_T(dname + (loc+1));
		knh_class_t cid = CLASS_Tdynamic;
		if(loc != -1) {
			cid = knh_NameSpace_getcid(ctx, ns, knh_bytes_first(n, loc));
			if(cid == CLASS_unknown) {
				KNH_WARN(ctx, "unknown class constant: %s", dname);
				cid = CLASS_Tdynamic;
			}
		}
		knh_addClassConst(ctx, cid, name, value);
	}
}

static void knh_loadScriptIntData(CTX ctx, knh_NameSpace_t *ns, const knh_IntData_t *data)
{
	while(data->name != NULL) {
		Object *value = UPCAST(new_Int(ctx, CLASS_Int, data->ivalue));
		knh_addConstData(ctx, ns, data->name, value);
		data++;
	}
}

static void knh_loadScriptFloatData(CTX ctx, knh_NameSpace_t *ns, const knh_FloatData_t *data)
{
	while(data->name != NULL) {
		Object *value = UPCAST(new_Float(ctx, CLASS_Float, data->fvalue));
		knh_addConstData(ctx, ns, data->name, value);
		data++;
	}
}

static void knh_loadScriptStringData(CTX ctx, knh_NameSpace_t *ns, const knh_StringData_t *data)
{
	while(data->name != NULL) {
		Object *value = UPCAST(new_T(data->value));
		knh_addConstData(ctx, ns, data->name, value);
		data++;
	}
}

/* ------------------------------------------------------------------------ */

#define _MAX 1024

static knh_bytes_t knh_data_tobytes(knh_data_t data)
{
	char *p = (char*)data;
	return B(p);
}

static knh_ParamArray_t *knh_loadScriptParamArray(CTX ctx, const knh_data_t **d, knh_uintptr_t uflag, int step)
{
	knh_ParamArray_t *pa = new_ParamArray(ctx);
	const knh_data_t *data = (*d) + step;
	long i, psize = (long)data[0];
	long rsize = (long)data[1];
	data += 2;
	for(i = 0; i < psize+rsize; i++) {
		knh_type_t type = (data[0] < _MAX || (TYPE_This <= data[0] && data[0] <= TYPE_T3)) ?
			(knh_type_t)data[0] : knh_NameSpace_gettype(ctx, K_GMANS, knh_data_tobytes(data[0]));
		knh_fieldn_t fn = (data[1] < _MAX) ?
			(knh_fieldn_t)data[1] : knh_getfnq(ctx, knh_data_tobytes(data[1]), FN_NEWID);
		knh_param_t p = {type, fn};
		knh_ParamArray_add(ctx, pa, p);
		data += 2;
	}
	pa->psize = (knh_ushort_t)psize;
	pa->rsize = (knh_ushort_t)rsize;
	*d = data;
	pa->h.magicflag |= uflag;
	return pa;
}

#define _CID(d)  (d < _MAX) ? (knh_class_t)(d) : knh_NameSpace_getcid(ctx, K_GMANS, knh_data_tobytes(d))
#define _EXPTID(d)  (d < _MAX) ? (knh_ebi_t)(d) : knh_geteid(ctx, knh_data_tobytes(d), EBI_newid)

static void knh_loadScriptData(CTX ctx, const knh_data_t *data, knh_ParamArray_t **buf)
{
	size_t c = 0;
	while(1) {
		long datatype = (long)data[0]; data++;
		switch(datatype) {
		case DATA_END: return;
		case DATA_STRUCT0: {
			knh_class_t cid0 = _CID(data[0]);
			knh_ClassDef_t *cspi = (knh_ClassDef_t*)data[1];
			knh_flag_t cflag = (knh_flag_t)data[2];
			data += 3;
			knh_class_t cid = new_ClassId(ctx);
			knh_ClassTBL_t *t = varClassTBL(cid);
			KNH_ASSERT(cid == cid0);
			t->bcid = cid;
			knh_setClassDef(t, cspi);
			t->cflag = cflag;
			t->magicflag = KNH_MAGICFLAG(cflag);
			break;
		}
		//{"Object", FLAG_Object, CLASS_ObjectField, CLASS_Object, a},
		case DATA_CLASS0: {
			knh_class_t cid0 = _CID(data[0]);
			char *name = (char*)data[1];
			knh_ClassTBL_t *ct = varClassTBL(cid0);
			if(ct == NULL) {
				knh_class_t cid = new_ClassId(ctx);
				ct = varClassTBL(cid0);
				KNH_ASSERT(cid == cid0);
			}
			ct->cflag = (knh_flag_t)data[2];
			ct->magicflag = KNH_MAGICFLAG(ct->cflag);
			ct->bcid = _CID(data[3]);
			ct->baseTBL = ClassTBL(ct->bcid);
			if(cid0 != ct->bcid) {
				knh_setClassDef(ct, ClassTBL(ct->bcid)->ospi);
			}
			DBG_ASSERT(ct->ospi != NULL);
			ct->supcid = _CID(data[4]);
			ct->supTBL = ClassTBL(ct->supcid);
			knh_setClassName(ctx, cid0, new_T(name), NULL);
			DBG_ASSERT(ct->methods == NULL);
			if(data[5] > 0/* || cid0 == CLASS_Tvoid*/) {
				KNH_INITv(ct->methods, new_Array0(ctx, data[5]));
			}
			else {
				KNH_INITv(ct->methods, K_EMPTYARRAY);
			}
			if(ct->typemaps == NULL) {
				KNH_INITv(ct->typemaps, K_EMPTYARRAY);
			}
			data += 6;
			break;
		}
		case DATA_CPARAM: {
			knh_class_t cid = _CID(data[0]);
			const char *lname = (const char*)data[1];
			knh_ClassTBL_t *ct = varClassTBL(cid);
			knh_ParamArray_t *pa = knh_loadScriptParamArray(ctx, &data, 0/*hflag*/, +2);
			knh_setClassParam(ctx, ct, pa);
			KNH_SETv(ctx, ct->lname, new_T(lname));
			break;
		}
		case DATA_GENERICS: {
			knh_class_t bcid = _CID(data[0]);
			knh_ParamArray_t *mp = knh_loadScriptParamArray(ctx, &data, 0/*hflag*/, +1);
			knh_addGenericsClass(ctx, CLASS_newid, bcid, mp);
			break;
		}
		case DATA_EXPT: {
			char *name = (char*)data[0];
			knh_flag_t flag = (knh_flag_t)data[1];
			knh_ebi_t eid = _EXPTID(data[2]);
			knh_ebi_t pid = _EXPTID(data[3]);
			knh_addEvent(ctx, flag, eid, new_T(name), pid);
			data += 4;
			break;
		}
		//{Int_opPREV, FLAG_Method_Const, CLASS_Int, MN_opPREV, 0, 20, NULL},
		//{cid, mn, flag, func, mpidx, psize, rsize}
		case DATA_METHOD0:
		case DATA_METHOD : {
			knh_class_t cid = _CID(data[0]);
			knh_methodn_t mn = (datatype == DATA_METHOD)
				? knh_getmn(ctx, knh_data_tobytes(data[1]), MN_NEWID) : (knh_methodn_t)data[1];
			knh_flag_t flag = (knh_flag_t)data[2];
			knh_Fmethod func = (knh_Fmethod)data[3];
			knh_Method_t *mtd;
			if(class_isSingleton(cid)) {
				flag = flag | FLAG_Method_Static;
			}
			mtd = new_Method(ctx, flag, cid, mn, func);
			knh_ClassTBL_addMethod(ctx, ClassTBL(cid), mtd, 0);
			data += 4;
			if(datatype == DATA_METHOD0) {
				KNH_SETv(ctx, DP(mtd)->mp, buf[data[0]]);
				DBG_ASSERT(IS_ParamArray(DP(mtd)->mp));
				data++;
			}
			else {
				knh_ParamArray_t *mp = knh_loadScriptParamArray(ctx, &data, 0/*hflag*/, +0);
				KNH_SETv(ctx, DP(mtd)->mp, mp);
			}
			break;
		}
		case DATA_TYPEMAP: { //{scid, tcid, flag, func},
			knh_class_t scid = _CID(data[0]);
			knh_class_t tcid = _CID(data[1]);
			knh_flag_t  flag = (knh_flag_t)data[2];
			knh_Ftypemap func = (knh_Ftypemap)data[3];
			knh_addTypeMapFunc(ctx, flag, scid, tcid, func, KNH_NULL);
			data += 4;
			break;
		}
		case DATA_PARAM: {
			knh_uintptr_t flag = (knh_uintptr_t)data[0];
			data++;
			buf[c] = knh_loadScriptParamArray(ctx, &data, flag, +0);
			c++;
			break;
		}/*case DATA_PARAM*/
		default :
			KNH_PANIC(ctx, "loading Data unknown dataype=%d", (int)datatype);
		}/*switch*/
	}
}

/* ------------------------------------------------------------------------ */

//static void knh_setRegexSPI(CTX ctx, knh_NameSpace_t *ns, const knh_RegexSPI_t *spi)
//{
//	KNH_ASSERT(ns != NULL);
//	ns->regexSPI = spi;
//}

static void knh_addPathDSPI(CTX ctx, knh_NameSpace_t *ns, const char *scheme, const knh_PathDSPI_t *d)
{
	knh_NameSpace_addDSPI(ctx, ns, scheme, (knh_DSPI_t*)d);
}

static void knh_addStreamDSPI(CTX ctx, knh_NameSpace_t *ns, const char *scheme, const knh_StreamDSPI_t *d)
{
	knh_NameSpace_addDSPI(ctx, ns, scheme, (knh_DSPI_t*)d);
}

static void knh_addQueryDSPI(CTX ctx, knh_NameSpace_t *ns, const char *scheme, const knh_QueryDSPI_t *d)
{
	knh_NameSpace_addDSPI(ctx, ns, scheme, (knh_DSPI_t*)d);
}

static void knh_addConvDSPI(CTX ctx, knh_NameSpace_t *ns, const char *scheme, const knh_ConvDSPI_t *d)
{
	knh_NameSpace_addDSPI(ctx, ns, scheme, (knh_DSPI_t*)d);
}


const knh_PackageLoaderAPI_t* knh_getPackageLoaderAPI(void)
{
	static knh_PackageLoaderAPI_t exports = {
		knh_loadScriptData,
		knh_loadScriptIntData, knh_loadScriptFloatData, knh_loadScriptStringData,
		knh_addPathDSPI, knh_addStreamDSPI, knh_addQueryDSPI, knh_addConvDSPI,
	};
	return &exports;
}

/* ------------------------------------------------------------------------ */

#ifdef __cplusplus
}
#endif
