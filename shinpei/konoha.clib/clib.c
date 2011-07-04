/* Glue 
 *  ffi package, for DEOS
 *  written by shinpei(c)2011
 */

#include <konoha1.h>


//#define MACOSX
#if defined(K_USING_MACOSX_)
#include <signal.h>
#include <ffi/ffi.h>
#elif defined(K_USING_LINUX_)
#include <ffi.h>
#include <sys/wait.h>
#include <signal.h>
#endif

#include <unistd.h>

#ifdef __cplusplus
extern "C" {
#endif

/* ------------------------------------------------------------------------ */
/* [Glue] */
#include <dglue.h> // for DEOS
// #include <glue.h>

/* ------------------------------------------------------------------------ */
/* [Structure] */

typedef struct knh_Structure_t {
  size_t param_size;
  ffi_type **params;
  char **field_name;
  void *test;
} knh_Structure_t ;

static void Structure_init(CTX ctx, knh_RawPtr_t *po)
{
  po->rawptr = NULL;
}

static void Structure_free(CTX ctx, knh_RawPtr_t *po)
{
	if (po->rawptr != NULL) {
	  po->rawptr = NULL;
	}
}

DEFAPI(void) defStructure(CTX ctx, knh_class_t cid, knh_ClassDef_t *cdef)
{
	cdef->name = "Structure";
	cdef->init = Structure_init;
	cdef->free = Structure_free;
}


/* ------------------------------------------------------------------------ */
/* [Clib] */

#define CLIB_ARGMAX 3

typedef struct knh_CLib_t {
  void *handler;
} knh_CLib_t;

static void Clib_init(CTX ctx, knh_RawPtr_t *po)
{
  po->rawptr = NULL;
}

static void Clib_free(CTX ctx, knh_RawPtr_t *po)
{
	if (po->rawptr != NULL) {
	  knh_CLib_t *clib = (knh_CLib_t*)po->rawptr;
	  if (clib->handler != NULL) {
		knh_dlclose(ctx, clib->handler);
	  }
	  KNH_FREE(ctx, clib, sizeof(knh_CLib_t));
	  po->rawptr = NULL;
	}
}

DEFAPI(void) defClib(CTX ctx, knh_class_t cid, knh_ClassDef_t *cdef)
{
	cdef->name = "Clib";
	cdef->init = Clib_init;
	cdef->free = Clib_free;
}

/* ------------------------------------------------------------------------ */
/* [Dglue:internal] */

#define DGLUE_NOT_UNBOXED 0
#define DGLUE_UNBOXED 1

typedef struct knh_ClibGlue_t {
  void *fptr;
  ffi_cif cif;
  int argCount;
  ffi_type *retT;
  ffi_type *argT[CLIB_ARGMAX];
  int argT_isUnboxed[CLIB_ARGMAX];
  void *argV[CLIB_ARGMAX];
  void *retV;
} knh_ClibGlue_t;

static void ClibGlue_init(CTX ctx, knh_ClibGlue_t *cglue)
{
  int i;
  cglue->fptr = NULL;
  cglue->argCount = 0;
  for (i = 0; i < CLIB_ARGMAX; i++) {
	cglue->argT[i] = NULL;
	cglue->argT_isUnboxed[i] = 0;
	cglue->argV[i] = NULL;
  }
  cglue->retV = NULL;
}

static void ClibGlue_free(CTX ctx, void *ptr)
{
  if (ptr != NULL) {
	knh_ClibGlue_t *cglue = (knh_ClibGlue_t*)ptr;
	KNH_FREE(ctx, cglue, sizeof(knh_ClibGlue_t));
	ptr = NULL;
  }
}
/* ------------------------------------------------------------------------ */

static ffi_type *ffi_type_map[] = {
  &ffi_type_pointer,
  &ffi_type_void,
  &ffi_type_uint8,
  &ffi_type_sint8,
  &ffi_type_uint16,
  &ffi_type_sint16,
  &ffi_type_uint32,
  &ffi_type_sint32,
  &ffi_type_uint64,
  &ffi_type_sint64,
  &ffi_type_float,
  &ffi_type_double,
  &ffi_type_uchar,
  &ffi_type_schar,
  &ffi_type_ushort,
  &ffi_type_sshort,
  &ffi_type_uint,
  &ffi_type_sint,
  &ffi_type_ulong,
  &ffi_type_slong,
  &ffi_type_longdouble,
  &ffi_type_sint64, // KNHINT
  &ffi_type_double // KNHFLOAT
};

static knh_IntData_t StructureConstInt[] = {
  {"FFI_POINTER", 1},
  {"FFI_VOID", 2},
  {"FFI_UINT8", 3},
  {"FFI_SINT8", 4},
  {"FFI_UINT16", 5},
  {"FFI_SINT16", 6},
  {"FFI_UINT32", 7},
  {"FFI_SINT32", 8},
  {"FFI_UINT64", 9},
  {"FFI_SINT64", 11},
  {"FFI_FLOAT", 12},
  {"FFI_DOUBLE", 13},
  {"FFI_UCHAR", 14},
  {"FFI_SCHAR", 15},
  {"FFI_USHORT", 16},
  {"FFI_SSHORT", 17},
  {"FFI_UINT", 18},
  {"FFI_SINT", 19},
  {"FFI_ULONG", 20},
  {"FFI_SLONG", 21},
  {"FFI_LONGDOUBLE", 22},

  {"FFI_KNHINT",23 },
  {"FFI_KNhFLOAT", 24},

  {NULL, 0}
};

DEFAPI(void) constStructure(CTX ctx, knh_class_t cid, const knh_PackageLoaderAPI_t *kapi)
{
  kapi->loadIntClassConst(ctx, cid, StructureConstInt);
}

/* ------------------------------------------------------------------------ */

//@Native Structure Structure.new(Array<Array<dynamic> values, Structure _)
METHOD Structure_new(CTX ctx, knh_sfp_t *sfp _RIX)
{
  knh_Array_t *a = sfp[1].a;
  knh_Structure_t *strc = (knh_Structure_t*)KNH_MALLOC(ctx, sizeof(knh_Structure_t));
  int i = 0, j = 0;
  size_t a_size = knh_Array_size(a);
  strc->param_size = a_size;
  strc->params = (ffi_type**)KNH_MALLOC(ctx, sizeof(ffi_type*) * strc->param_size);

  for (i = 0; i < a_size; i++) {
	// param size;
	knh_Array_t *aa = (knh_Array_t *)knh_Array_n(a, i);
	size_t aa_size = knh_Array_size(aa);
	{
	  // suppose its [<String>, <Int>]
	  knh_String_t *v1 = (knh_String_t *)knh_Array_n(aa, 0);
	  knh_Int_t *v2 = (knh_Int_t *)knh_Array_n(aa, 1);
	  //	  fprintf(stderr, "HERE>>%s:%d\n", S_tochar(v1), v2->n.ivalue);
	  strc->params[j] = ffi_type_map[v2->n.ivalue];
	}
  }
  knh_RawPtr_t *po = new_RawPtr(ctx, sfp[2].p, strc);
  RETURN_(po);
}

/*
//@Native Structure Structure.new(Array<Tuple<String, Int>> values, Structure _);
METHOD Structure_new(CTX ctx, knh_sfp_t *sfp _RIX)
{
  knh_Array_t *a = sfp[1].a;
  knh_Structure_t *strc = (knh_Structure_t*)KNH_MALLOC(ctx, sizeof(knh_Structure_t));
  strc->test = NULL;

  int i = 0;
  int j = 0;
  for (i = 0; i < knh_Array_size(a); i++) {
	knh_Map_t *map = (knh_Map_t *)knh_Array_n(a, i);
	fprintf(stderr, "%s\n", CLASS__(O_cid(map)));
	knh_ParamArray_t *pa = ClassTBL(O_cid(map))->cparam;
	size_t psize = pa->psize;
	for (j = 0; j < psize; j++) {
	  knh_param_t *p = knh_ParamArray_get(pa, j);
	  fprintf(stderr, "%s\n", TYPE__(p->type));
	}
	asm("int3");
	knh_String_t *v1 = map->dspi->get(ctx, map->map, sfp, sfp);
	//	knh_String_t *v1 = knh_Map_get(ctx, map, 
#if 0
	knh_DictMap_t *dmap = (knh_DictMap_t *)knh_Array_n(a, i);
	knh_Int_t *v2 = (knh_Int_t *)knh_DictMap_valueAt(dmap, 0);
	fprintf(stderr, "%s\n", CLASS__(O_cid(v2)));
#endif
#if 0
	knh_Tuple_t *tpl = (knh_Tuple_t*)knh_Array_n(a, i);
	fprintf(stderr, "%s\n", CLASS__(O_cid(tpl)));
	knh_String_t *v1 = tpl->fields[0];
	asm("int3");
	knh_Int_t *v2 = tpl->fields[1];
	fprintf(stderr, "%s\n", CLASS__(O_cid(v1)));
	fprintf(stderr, "%s\n", CLASS__(O_cid(v2)));
#endif	
  }
  knh_RawPtr_t *po = new_RawPtr(ctx, sfp[2].p, strc);
  RETURN_(po);
}

*/
/* ------------------------------------------------------------------------ */
//@Native Clib Clib.new(String libname, Clib _);
METHOD Clib_new(CTX ctx, knh_sfp_t *sfp _RIX)
{
  const char *libname = String_to(const char *, sfp[1]);
  knh_CLib_t *clib = (knh_CLib_t*)KNH_MALLOC(ctx, sizeof(knh_CLib_t));
  clib->handler = knh_dlopen(ctx, libname);
  knh_RawPtr_t *po = new_RawPtr(ctx, sfp[2].p, clib);
  RETURN_(po);
}

static METHOD Fmethod_wrapCLib(CTX ctx, knh_sfp_t *sfp _RIX)
{
  knh_type_t rtype = knh_ParamArray_rtype(DP(sfp[K_MTDIDX].mtdNC)->mp);
  knh_Func_t *fo = sfp[0].fo;
  knh_ClibGlue_t *cglue = (knh_ClibGlue_t*)(((fo->mtd)->b)->cfunc);
  //  fprintf(stderr, "fptr:%p, %p, %d, %p, %p\n", 
  //		  dg->fptr,
  //		  &(dg->cif), dg->argCount,
  //		  dg->retT, dg->argT);
  
  int idx = 0;
  for (idx = 0; idx < cglue->argCount; idx++) {
	if (cglue->argT_isUnboxed[idx] == DGLUE_UNBOXED) {
	  cglue->argV[idx] = &(sfp[idx+1].ndata);
	} else {
	  //TODO: now, we cannot distinguish object from string
	  if (cglue->argT[idx] == &ffi_type_pointer) {
		cglue->argV[idx] = &((sfp[idx+1].s)->str.text);
	  } else {
		// TODO? : array? map? char?
		cglue->argV[idx] = &(sfp[idx+1].o);
	  }
	}
  } /* for loop for argT*/

  if(rtype != TYPE_void) {
	if(IS_Tunbox(rtype)) {
	  if (rtype == TYPE_Int || rtype == TYPE_Boolean) {
		knh_int_t return_i = 0;
		if (ffi_prep_cif(&(cglue->cif), FFI_DEFAULT_ABI, cglue->argCount,
						 cglue->retT, cglue->argT) == FFI_OK) {
		  ffi_call(&(cglue->cif), cglue->fptr, &(cglue->retV), cglue->argV);
		  return_i = *(knh_int_t*)(&cglue->retV);
		} else {
		  fprintf(stderr, "prep_cif FAILED\n:");
		}
		RETURNi_(return_i);
	  } else if (rtype == TYPE_Float) {
		double return_f = 0.0;
		if (ffi_prep_cif(&(cglue->cif), FFI_DEFAULT_ABI, cglue->argCount,
						 cglue->retT, cglue->argT) == FFI_OK) {
		  ffi_call(&(cglue->cif), cglue->fptr, &(cglue->retV), cglue->argV);
		  return_f = *(double*)(&cglue->retV);
		} else {
		  fprintf(stderr, "prep_cif FAILED\n:");
		}
		RETURNf_(return_f);
	  } 
	} else { // IS not unbox 
	  if (rtype == TYPE_String) {
		// its String
		char *return_s = NULL;
		if (ffi_prep_cif(&(cglue->cif), FFI_DEFAULT_ABI, cglue->argCount,
						 cglue->retT, cglue->argT) == FFI_OK) {
		  ffi_call(&(cglue->cif), cglue->fptr, &(cglue->retV), cglue->argV);
		  return_s = *(char**)(&cglue->retV);
		} else {
		  fprintf(stderr, "prep_cif FAILED\n:");
		}
		RETURN_(new_String(ctx, return_s));
	  } else {
		// its Object
		void *return_ptr = NULL;
		if (ffi_prep_cif(&(cglue->cif), FFI_DEFAULT_ABI, cglue->argCount,
						 cglue->retT, cglue->argT) == FFI_OK) {
		  ffi_call(&(cglue->cif), cglue->fptr, &(cglue->retV), cglue->argV);
		  return_ptr = *(void**)(&cglue->retV);
		} else {
		  fprintf(stderr, "prep_cif FAILED\n:");
		}
		RETURN_(new_RawPtr(ctx, (knh_RawPtr_t*)KNH_NULVAL(CLASS_Tvar), return_ptr));
	  }
	} // end of IS_Tunbox 
  } // end of is_VOID
  RETURNvoid_();
}

static knh_RawPtr_t *ClibGlue_getFunc(CTX ctx, knh_sfp_t *sfp _RIX)
{
  knh_Glue_t *glue = (knh_Glue_t*)((sfp[0].p)->rawptr);
  knh_ClibGlue_t *cglue = (knh_ClibGlue_t*)(glue->glueInfo);
  knh_CLib_t *clib = (knh_CLib_t*)glue->componentInfo;
  if (clib == NULL) {
	fprintf(stderr, "invalid Dglue\n");
	return (knh_RawPtr_t*)(sfp[3].o);
  }
  const char *symstr = String_to(const char *, sfp[1]);
  knh_Class_t *klass = (knh_Class_t*)sfp[2].o;
  knh_Func_t *fo = (knh_Func_t *)sfp[3].o;

  // requested type
  //  fprintf(stderr, "%s %s\n", CLASS__(O_cid(klass)), CLASS__(klass->cid));
  const knh_ClassTBL_t *tbl = ClassTBL(klass->cid);
  knh_ParamArray_t *pa = tbl->cparam;

  if ((cglue->fptr = knh_dlsym(ctx, clib->handler, symstr, 0))== NULL) {
	fprintf(stderr, "dlsym_ERROR!!!\n");
  }
  // type a method from requested type
  size_t argCount = pa->psize;
  cglue->argCount = argCount;
  // retT
  DBG_ASSERT(pa->rsize == 1);
  knh_param_t *p = knh_ParamArray_rget(pa, 0);
  if (p->type == TYPE_Int || p->type == TYPE_Boolean) {
	cglue->retT = &ffi_type_sint64;
  } else if (p->type == TYPE_Float) {
	cglue->retT = &ffi_type_double;
  } else {
	cglue->retT = &ffi_type_pointer;
	//	TODO();
  }

  //argT
  size_t idx = 0;
  for (idx = 0; idx < argCount; idx++) {
	knh_param_t *p = knh_ParamArray_get(pa, idx);
	if (p->type == TYPE_Int || p->type == TYPE_Boolean) {
	  cglue->argT[idx] = &ffi_type_sint64;
	  cglue->argT_isUnboxed[idx] = DGLUE_UNBOXED;
	} else if (p->type == TYPE_Float) {
	  cglue->argT[idx] = &ffi_type_double;
	  cglue->argT_isUnboxed[idx] = DGLUE_UNBOXED;
	} else if (p->type == TYPE_String) {
	  cglue->argT[idx] = &ffi_type_pointer;
	  cglue->argT_isUnboxed[idx] = DGLUE_NOT_UNBOXED;
	} else {
	  cglue->argT[idx] = &ffi_type_pointer;
	  cglue->argT_isUnboxed[idx] = DGLUE_NOT_UNBOXED;
	  //	  TODO();
	}
  }

  // type Func object
  fo->h.cTBL= tbl;
  //cid = knh_class_Generics(ctx, CLASS_Func, pa);
  
  // set wrapper method
  knh_Method_t *mtd = new_Method(ctx, 0, O_cid(klass), MN_LAMBDA, Fmethod_wrapCLib);
  mtd->b->cfunc = (void*)cglue;
  KNH_SETv(ctx, ((mtd)->b)->mp, tbl->cparam);
  KNH_INITv(fo->mtd, mtd);
  return (knh_RawPtr_t*)fo;
}


static knh_GlueSPI_t CLibGlueSPI = {
  ClibGlue_getFunc,
  /*  Clib_component_free*/ NULL,
  ClibGlue_free
};

// @Native Glue Clib_genGlue (Glue _)
METHOD Clib_genGlue(CTX ctx, knh_sfp_t *sfp _RIX)
{
  knh_CLib_t *clib = (knh_CLib_t *)((sfp[0].p)->rawptr);
  if (clib != NULL) {
	knh_Glue_t *glue = new_Glue(ctx);
	glue->glueType = GLUE_TYPE_INTERNAL;
	glue->gapi = &CLibGlueSPI;
	glue->componentInfo = (void*)clib;
	knh_ClibGlue_t *cglue = (knh_ClibGlue_t*)KNH_MALLOC(ctx, sizeof(knh_ClibGlue_t));
	ClibGlue_init(ctx, cglue);
	glue->glueInfo = (void*)cglue;
	RETURN_(new_RawPtr(ctx, sfp[1].p, glue));
  }
  RETURN_(sfp[1].p);
}

// @Native void Clib_makeClass (String classname)
METHOD Clib_makeClass(CTX ctx, knh_sfp_t *sfp _RIX)
{
  char *classname = String_to(char *, sfp[1]);
  knh_String_t *str_classname = sfp[1].s;
  knh_ClassTBL_t *ct = NULL;
  knh_NameSpace_t *ns = sfp[2].ns;
  knh_class_t cid;
  knh_cwb_t cwbbuf, *cwb = knh_cwb_open(ctx, &cwbbuf);
  knh_Bytes_write(ctx, cwb->ba, S_tobytes(DP(ns)->nsname));
  knh_Bytes_putc(ctx, cwb->ba, '.');
  knh_Bytes_write(ctx, cwb->ba, B(classname));
  cid = knh_getcid(ctx, knh_cwb_tobytes(cwb));

  if  (cid == CLASS_unknown) {
	//new class
	cid = new_ClassId(ctx);
	ct = varClassTBL(cid);
	knh_String_t *name = knh_cwb_newString(ctx, cwb);
	knh_setClassName(ctx, cid, name, str_classname);
	//	ct->cflag  = knh_StmtCLASS_flag(ctx, stmt);
	ct->cflag = 0; // none
	ct->magicflag  = KNH_MAGICFLAG(ct->cflag);
	//	NameSpace_setcid(ctx, ns, knh_cwb_newString(ctx, cwb), cid);
	{
	  if(DP(ns)->name2cidDictSetNULL == NULL) {
		KNH_INITv(DP(ns)->name2cidDictSetNULL, new_DictSet0(ctx, 0, 1/*isCaseMap*/, "NameSpace.name2cid"));
	  }
	  else {
		knh_uintptr_t oldcid = knh_DictSet_get(ctx, DP(ns)->name2cidDictSetNULL, S_tobytes(name));
		if(oldcid != 0 && cid != oldcid - 1) {
		  WARN_AlreadyDefinedClass(ctx, cid, (knh_class_t)(oldcid - 1));
		  return;
		}
	  }
	  //	  fprintf(stderr, "set!!:%d, name:%s, ns:%p, n2cid:%p\n", cid, S_tochar(name), ns, DP(ns)->name2cidDictSetNULL);
	  knh_DictSet_set(ctx, DP(ns)->name2cidDictSetNULL, str_classname, (knh_uintptr_t)(cid+1));
	}
	KNH_INITv(ct->methods, K_EMPTYARRAY);
	KNH_INITv(ct->typemaps, K_EMPTYARRAY);
	//// class C extends E ..
	//	ct->supcid = knh_Token_cid(ctx, tkE, CLASS_unknown);
	//ct->supcid = CLASS_unknown;
	//if(ct->supcid == CLASS_unknown) {
	//  knh_Stmt_toERR(ctx, stmt, ERROR_Undefined(ctx, "class", ct->supcid, tkE));
	//  return K_BREAK;
	//}
	//if(class_isFinal(ct->supcid)) {
	//  knh_Stmt_toERR(ctx, stmt, ErrorExtendingFinalClass(ctx, ct->supcid));
	//  return K_BREAK;
	//}
	ct->supTBL = ClassTBL(ct->supcid);
	ct->keyidx = ct->supTBL->keyidx;
	ct->metaidx = ct->supTBL->keyidx;
	((knh_ClassTBL_t*)ct->supTBL)->subclass += 1;
	{
	  LOGSFPDATA = {cDATA("name", cid), iDATA("cid", cid)};
	  LIB_OK("konoha:new_class");
	}
	knh_Object_t *obj = new_hObject_(ctx, ct);
	knh_Object_t *tmp = new_hObject_(ctx, ct);
	Object_setNullObject(obj, 1);
	ct->bcid = CLASS_Object;
	ct->baseTBL = ClassTBL(CLASS_Object);
	knh_setClassDef(ct, ct->baseTBL->cdef);
	obj->ref = NULL; tmp->ref = NULL;
	knh_setClassDefaultValue(ctx, cid, obj, NULL);
	KNH_INITv(ct->protoNULL, tmp);
	if(IS_Tfield(cid)) {
		DBG_P("superclass=%s, fsize=%d, fcapacity=%d", CLASS__(ct->supcid), ct->supTBL->fsize, ct->supTBL->fcapacity);
		if(ct->supTBL->fcapacity > 0 && ct->fcapacity == 0) {
			ct->fields = (knh_fields_t*)KNH_MALLOC(ctx, ct->supTBL->fcapacity * sizeof(knh_fields_t));
			knh_memcpy(ct->fields, ct->supTBL->fields, ct->supTBL->fcapacity * sizeof(knh_fields_t));
			ct->fsize = ct->supTBL->fsize;
			ct->fcapacity = ct->supTBL->fcapacity;
			if(ct->fsize > 0) {
				knh_Object_t *suptmp = (knh_Object_t*)ct->supTBL->protoNULL, *supobj = ct->supTBL->defnull;
				knh_ObjectField_expand(ctx, ct->protoNULL, 0, ct->fsize);
				knh_ObjectField_expand(ctx, ct->defobj, 0, ct->fsize);
				knh_memcpy(ct->protoNULL->fields, suptmp->ref, ct->fsize*sizeof(knh_Object_t*));
				knh_memcpy(ct->defnull->ref, supobj->ref, ct->fsize*sizeof(knh_Object_t*));
#ifdef K_USING_RCGC
				ct->supTBL->cdef->reftrace(ctx, suptmp, ctx->refs);
				knh_RefTraverse(ctx, RCinc);
				ct->supTBL->cdef->reftrace(ctx, supobj, ctx->refs);
				knh_RefTraverse(ctx, RCinc);
#endif
			}
		}
	}

  } else {
	ct = varClassTBL(cid);
	if (!(ct->bcid == CLASS_Object && ct->fields == NULL)) {
	  fprintf(stderr, "redefining class %s\n", classname);
	  return;
	} else {
	  //TODO()
	  fprintf(stderr, "alreaady class!\n");
	}
  }

}

/* ------------------------------------------------------------------------ */
// @Native void Clib_defineClass (String classname, Structure strc, Namespace _)
METHOD Clib_defineClass(CTX ctx, knh_sfp_t *sfp _RIX)
{
  char *classname = String_to(char *, sfp[1]);
  knh_String_t *str_classname = sfp[1].s;
  knh_ClassTBL_t *ct = NULL;
  knh_RawPtr_t *po = sfp[2].p;
  knh_Structure_t *strc = (knh_Structure_t*)po->rawptr;
  knh_NameSpace_t *ns = sfp[3].ns;
  knh_class_t cid;
  int i = 0;
  knh_cwb_t cwbbuf, *cwb = knh_cwb_open(ctx, &cwbbuf);
  knh_Bytes_write(ctx, cwb->ba, S_tobytes(DP(ns)->nsname));
  knh_Bytes_putc(ctx, cwb->ba, '.');
  knh_Bytes_write(ctx, cwb->ba, B(classname));
  cid = knh_getcid(ctx, knh_cwb_tobytes(cwb));

  if  (cid == CLASS_unknown) {
	//new class
	cid = new_ClassId(ctx);
	ct = varClassTBL(cid);
	knh_String_t *name = knh_cwb_newString(ctx, cwb);
	knh_setClassName(ctx, cid, name, str_classname);
	//	ct->cflag  = knh_StmtCLASS_flag(ctx, stmt);
	ct->cflag = 0; // none
	ct->magicflag  = KNH_MAGICFLAG(ct->cflag);
	//	NameSpace_setcid(ctx, ns, knh_cwb_newString(ctx, cwb), cid);
	{
	  if(DP(ns)->name2cidDictSetNULL == NULL) {
		KNH_INITv(DP(ns)->name2cidDictSetNULL, new_DictSet0(ctx, 0, 1/*isCaseMap*/, "NameSpace.name2cid"));
	  }
	  else {
		knh_uintptr_t oldcid = knh_DictSet_get(ctx, DP(ns)->name2cidDictSetNULL, S_tobytes(name));
		if(oldcid != 0 && cid != oldcid - 1) {
		  WARN_AlreadyDefinedClass(ctx, cid, (knh_class_t)(oldcid - 1));
		  return;
		}
	  }
	  //	  fprintf(stderr, "set!!:%d, name:%s, ns:%p, n2cid:%p\n", cid, S_tochar(name), ns, DP(ns)->name2cidDictSetNULL);
	  knh_DictSet_set(ctx, DP(ns)->name2cidDictSetNULL, str_classname, (knh_uintptr_t)(cid+1));
	}
	KNH_INITv(ct->methods, K_EMPTYARRAY);
	KNH_INITv(ct->typemaps, K_EMPTYARRAY);
	//// class C extends E ..
	//	ct->supcid = knh_Token_cid(ctx, tkE, CLASS_unknown);
	//ct->supcid = CLASS_unknown;
	//if(ct->supcid == CLASS_unknown) {
	//  knh_Stmt_toERR(ctx, stmt, ERROR_Undefined(ctx, "class", ct->supcid, tkE));
	//  return K_BREAK;
	//}
	//if(class_isFinal(ct->supcid)) {
	//  knh_Stmt_toERR(ctx, stmt, ErrorExtendingFinalClass(ctx, ct->supcid));
	//  return K_BREAK;
	//}
	ct->supTBL = ClassTBL(ct->supcid);
	ct->keyidx = ct->supTBL->keyidx;
	ct->metaidx = ct->supTBL->keyidx;
	((knh_ClassTBL_t*)ct->supTBL)->subclass += 1;
	{
	  LOGSFPDATA = {cDATA("name", cid), iDATA("cid", cid)};
	  LIB_OK("konoha:new_class");
	}
	knh_Object_t *obj = new_hObject_(ctx, ct);
	knh_Object_t *tmp = new_hObject_(ctx, ct);
	Object_setNullObject(obj, 1);
	ct->bcid = CLASS_Object;
	ct->baseTBL = ClassTBL(CLASS_Object);
	knh_setClassDef(ct, ct->baseTBL->cdef);
	obj->ref = NULL; tmp->ref = NULL;
	knh_setClassDefaultValue(ctx, cid, obj, NULL);
	KNH_INITv(ct->protoNULL, tmp);
	if(IS_Tfield(cid)) {
		DBG_P("superclass=%s, fsize=%d, fcapacity=%d", CLASS__(ct->supcid), ct->supTBL->fsize, ct->supTBL->fcapacity);
		if(ct->supTBL->fcapacity > 0 && ct->fcapacity == 0) {
			ct->fields = (knh_fields_t*)KNH_MALLOC(ctx, ct->supTBL->fcapacity * sizeof(knh_fields_t));
			knh_memcpy(ct->fields, ct->supTBL->fields, ct->supTBL->fcapacity * sizeof(knh_fields_t));
			ct->fsize = ct->supTBL->fsize;
			ct->fcapacity = ct->supTBL->fcapacity;
			if(ct->fsize > 0) {
				knh_Object_t *suptmp = (knh_Object_t*)ct->supTBL->protoNULL, *supobj = ct->supTBL->defnull;
				knh_ObjectField_expand(ctx, ct->protoNULL, 0, ct->fsize);
				knh_ObjectField_expand(ctx, ct->defobj, 0, ct->fsize);
				knh_memcpy(ct->protoNULL->fields, suptmp->ref, ct->fsize*sizeof(knh_Object_t*));
				knh_memcpy(ct->defnull->ref, supobj->ref, ct->fsize*sizeof(knh_Object_t*));
#ifdef K_USING_RCGC
				ct->supTBL->cdef->reftrace(ctx, suptmp, ctx->refs);
				knh_RefTraverse(ctx, RCinc);
				ct->supTBL->cdef->reftrace(ctx, supobj, ctx->refs);
				knh_RefTraverse(ctx, RCinc);
#endif
			}
		}
	}

  } else {
	ct = varClassTBL(cid);
	if (!(ct->bcid == CLASS_Object && ct->fields == NULL)) {
	  fprintf(stderr, "redefining class %s\n", classname);
	  return;
	} else {
	  //TODO()
	  fprintf(stderr, "alreaady class!\n");
	}
  }
  
  /* make fields */
  for (i = 0; i < strc->param_size; i++) {
	

  }

  
}

/* ------------------------------------------------------------------------ */

#ifdef __cplusplus
}
#endif

