// THIS FILE WAS AUTOMATICALLY GENERATED

#ifdef __cplusplus
extern "C" {
#endif



#ifdef K_INTERNAL
KNHAPI2(knh_Object_t*) new_ObjectNS(CTX ctx, knh_NameSpace_t *ns, const char *sname);
KNHAPI2(knh_status_t) knh_load(CTX ctx, const char *scheme, knh_bytes_t path, knh_Array_t *resultsNULL);
KNHAPI2(knh_Array_t*) new_Array(CTX ctx, knh_class_t p1, size_t capacity);
KNHAPI2(void) knh_Array_add_(CTX ctx, knh_Array_t *a, knh_Object_t *value);
KNHAPI2(void) knh_Array_swap(CTX ctx, knh_Array_t *a, size_t n, size_t m);
KNHAPI2(knh_Iterator_t*) new_Iterator(CTX ctx, knh_class_t p1, knh_Object_t *source, knh_Fitrnext fnext);
KNHAPI2(void) knh_write_cid(CTX ctx, knh_OutputStream_t *w, knh_class_t cid);
KNHAPI2(void) knh_write_type(CTX ctx, knh_OutputStream_t *w, knh_type_t type);
KNHAPI2(Object*) knh_getClassDefaultValue(CTX ctx, knh_class_t cid);
KNHAPI2(knh_class_t) knh_type_tocid(CTX ctx, knh_type_t ptype, knh_class_t this_cid);
KNHAPI2(knh_param_t*) knh_ParamArray_get(knh_ParamArray_t *pa, size_t n);
KNHAPI2(knh_type_t) knh_ParamArray_rtype(knh_ParamArray_t *pa);
KNHAPI2(void) knh_write_mn(CTX ctx, knh_OutputStream_t *w, knh_methodn_t mn);
KNHAPI2(knh_bool_t) Method_isAbstract(knh_Method_t *mtd);
KNHAPI2(void) knh_addTypeMap(CTX ctx, knh_TypeMap_t *trl);
KNHAPI2(void) knh_TypeMap_exec(CTX ctx, knh_TypeMap_t *tmr, knh_sfp_t *sfp, long rix);
KNHAPI2(knh_TypeMap_t*) new_TypeMap(CTX ctx, knh_flag_t flag, knh_class_t scid, knh_class_t tcid, knh_Ftypemap func);
KNHAPI2(void) knh_invoke(CTX ctx, knh_Func_t *fo, knh_sfp_t *sfp /*rtnidx*/, int argc);
KNHAPI2(knh_text_t*) knh_cwb_tochar(CTX ctx, knh_cwb_t *cwb);
KNHAPI2(knh_String_t*) knh_DictMap_keyAt(knh_DictMap_t *m, size_t n);
KNHAPI2(Object*) knh_DictMap_valueAt(knh_DictMap_t *m, size_t n);
KNHAPI2(Object*) new_Boxing(CTX ctx, knh_sfp_t *sfp, const knh_ClassTBL_t *ct);
KNHAPI2(knh_Int_t*) new_Int(CTX ctx, knh_class_t cid, knh_int_t value);
KNHAPI2(knh_Float_t*) new_Float(CTX ctx, knh_class_t cid, knh_float_t value);
KNHAPI2(void) knh_ResultSet_initColumn(CTX ctx, knh_ResultSet_t *o, size_t column_size);
KNHAPI2(void) ResultSet_setName(CTX ctx, knh_ResultSet_t *o, size_t n, knh_String_t *name);
KNHAPI2(void) ResultSet_setInt(CTX ctx, knh_ResultSet_t *rs, size_t n, knh_int_t value);
KNHAPI2(void) ResultSet_setFloat(CTX ctx, knh_ResultSet_t *rs, size_t n, knh_float_t value);
KNHAPI2(void) ResultSet_setText(CTX ctx, knh_ResultSet_t *o, size_t n, knh_bytes_t t);
KNHAPI2(void) ResultSet_setBlob(CTX ctx, knh_ResultSet_t *o, size_t n, knh_bytes_t t);
KNHAPI2(void) ResultSet_setNULL(CTX ctx, knh_ResultSet_t *o, size_t n);
KNHAPI2(knh_bool_t) knh_isSystemVerbose(void);
KNHAPI2(knh_InputStream_t*) new_InputStreamDSPI(CTX ctx, knh_io_t fio, const knh_StreamDSPI_t *dspi);
KNHAPI2(knh_InputStream_t*) new_InputStreamNULL(CTX ctx, knh_NameSpace_t *ns, knh_String_t *urn, const char *mode, knh_Monitor_t *mon);
KNHAPI2(knh_OutputStream_t*) new_OutputStreamDSPI(CTX ctx, knh_io_t fio, const knh_StreamDSPI_t *dspi);
KNHAPI2(knh_OutputStream_t*) new_OutputStreamNULL(CTX ctx, knh_NameSpace_t *ns, knh_String_t *urn, const char *mode, knh_Monitor_t *mon);
KNHAPI2(knh_OutputStream_t*) new_BytesOutputStream(CTX ctx, knh_Bytes_t *ba);
KNHAPI2(void) knh_OutputStream_flush(CTX ctx, knh_OutputStream_t *w, int isNEWLINE);
KNHAPI2(void) knh_OutputStream_putc(CTX ctx, knh_OutputStream_t *w, int ch);
KNHAPI2(void) knh_OutputStream_write(CTX ctx, knh_OutputStream_t *w, knh_bytes_t buf);
KNHAPI2(void) knh_write_BOL(CTX ctx, knh_OutputStream_t *w);
KNHAPI2(void) knh_write_EOL(CTX ctx, knh_OutputStream_t *w);
KNHAPI2(void) knh_write_TAB(CTX ctx, knh_OutputStream_t *w);
KNHAPI2(void) knh_write_ascii(CTX ctx, knh_OutputStream_t *w, const char *text);
KNHAPI2(void) knh_write_utf8(CTX ctx, knh_OutputStream_t *w, knh_bytes_t t, int hasUTF8);
KNHAPI2(void) knh_printf(CTX ctx, knh_OutputStream_t *w, const char *fmt, ...);
KNHAPI2(knh_String_t*) new_String_(CTX ctx, knh_class_t cid, knh_bytes_t t, knh_String_t *memoNULL);
KNHAPI2(knh_String_t*) new_String(CTX ctx, const char *str);
KNHAPI2(knh_ClassDef_t*) knh_getDefaultClassDef(void);
KNHAPI2(void) knh_setPropertyText(CTX ctx, char *key, char *value);
KNHAPI2(knh_String_t*) knh_getFieldName(CTX ctx, knh_fieldn_t fn);
#endif

typedef struct knh_api2_t {
	size_t crc32;
	Object*  (*DictMap_valueAt)(knh_DictMap_t *m, size_t n);
	Object*  (*getClassDefaultValue)(CTX ctx, knh_class_t cid);
	Object* (*new_Boxing)(CTX ctx, knh_sfp_t *sfp, const knh_ClassTBL_t *ct);
	knh_Array_t* (*new_Array)(CTX ctx, knh_class_t p1, size_t capacity);
	knh_ClassDef_t*  (*getDefaultClassDef)(void);
	knh_Float_t* (*new_Float)(CTX ctx, knh_class_t cid, knh_float_t value);
	knh_InputStream_t* (*new_InputStreamDSPI)(CTX ctx, knh_io_t fio, const knh_StreamDSPI_t *dspi);
	knh_InputStream_t* (*new_InputStreamNULL)(CTX ctx, knh_NameSpace_t *ns, knh_String_t *urn, const char *mode, knh_Monitor_t *mon);
	knh_Int_t* (*new_Int)(CTX ctx, knh_class_t cid, knh_int_t value);
	knh_Iterator_t* (*new_Iterator)(CTX ctx, knh_class_t p1, knh_Object_t *source, knh_Fitrnext fnext);
	knh_Object_t* (*new_ObjectNS)(CTX ctx, knh_NameSpace_t *ns, const char *sname);
	knh_OutputStream_t* (*new_BytesOutputStream)(CTX ctx, knh_Bytes_t *ba);
	knh_OutputStream_t* (*new_OutputStreamDSPI)(CTX ctx, knh_io_t fio, const knh_StreamDSPI_t *dspi);
	knh_OutputStream_t* (*new_OutputStreamNULL)(CTX ctx, knh_NameSpace_t *ns, knh_String_t *urn, const char *mode, knh_Monitor_t *mon);
	knh_String_t*  (*DictMap_keyAt)(knh_DictMap_t *m, size_t n);
	knh_String_t*  (*getFieldName)(CTX ctx, knh_fieldn_t fn);
	knh_String_t* (*new_String)(CTX ctx, const char *str);
	knh_String_t* (*new_String_)(CTX ctx, knh_class_t cid, knh_bytes_t t, knh_String_t *memoNULL);
	knh_TypeMap_t* (*new_TypeMap)(CTX ctx, knh_flag_t flag, knh_class_t scid, knh_class_t tcid, knh_Ftypemap func);
	knh_bool_t (*Method_isAbstract)(knh_Method_t *mtd);
	knh_bool_t  (*isSystemVerbose)(void);
	knh_class_t  (*type_tocid)(CTX ctx, knh_type_t ptype, knh_class_t this_cid);
	knh_param_t*  (*ParamArray_get)(knh_ParamArray_t *pa, size_t n);
	knh_status_t  (*load)(CTX ctx, const char *scheme, knh_bytes_t path, knh_Array_t *resultsNULL);
	knh_text_t*  (*cwb_tochar)(CTX ctx, knh_cwb_t *cwb);
	knh_type_t  (*ParamArray_rtype)(knh_ParamArray_t *pa);
	void (*ResultSet_setBlob)(CTX ctx, knh_ResultSet_t *o, size_t n, knh_bytes_t t);
	void (*ResultSet_setFloat)(CTX ctx, knh_ResultSet_t *rs, size_t n, knh_float_t value);
	void (*ResultSet_setInt)(CTX ctx, knh_ResultSet_t *rs, size_t n, knh_int_t value);
	void (*ResultSet_setNULL)(CTX ctx, knh_ResultSet_t *o, size_t n);
	void (*ResultSet_setName)(CTX ctx, knh_ResultSet_t *o, size_t n, knh_String_t *name);
	void (*ResultSet_setText)(CTX ctx, knh_ResultSet_t *o, size_t n, knh_bytes_t t);
	void  (*Array_add_)(CTX ctx, knh_Array_t *a, knh_Object_t *value);
	void  (*Array_swap)(CTX ctx, knh_Array_t *a, size_t n, size_t m);
	void  (*OutputStream_flush)(CTX ctx, knh_OutputStream_t *w, int isNEWLINE);
	void  (*OutputStream_putc)(CTX ctx, knh_OutputStream_t *w, int ch);
	void  (*OutputStream_write)(CTX ctx, knh_OutputStream_t *w, knh_bytes_t buf);
	void  (*ResultSet_initColumn)(CTX ctx, knh_ResultSet_t *o, size_t column_size);
	void  (*TypeMap_exec)(CTX ctx, knh_TypeMap_t *tmr, knh_sfp_t *sfp, long rix);
	void  (*addTypeMap)(CTX ctx, knh_TypeMap_t *trl);
	void  (*invoke)(CTX ctx, knh_Func_t *fo, knh_sfp_t *sfp /*rtnidx*/, int argc);
	void  (*printf)(CTX ctx, knh_OutputStream_t *w, const char *fmt, ...);
	void  (*setPropertyText)(CTX ctx, char *key, char *value);
	void  (*write_BOL)(CTX ctx, knh_OutputStream_t *w);
	void  (*write_EOL)(CTX ctx, knh_OutputStream_t *w);
	void  (*write_TAB)(CTX ctx, knh_OutputStream_t *w);
	void  (*write_ascii)(CTX ctx, knh_OutputStream_t *w, const char *text);
	void  (*write_cid)(CTX ctx, knh_OutputStream_t *w, knh_class_t cid);
	void  (*write_mn)(CTX ctx, knh_OutputStream_t *w, knh_methodn_t mn);
	void  (*write_type)(CTX ctx, knh_OutputStream_t *w, knh_type_t type);
	void  (*write_utf8)(CTX ctx, knh_OutputStream_t *w, knh_bytes_t t, int hasUTF8);
} knh_api2_t;
	
#define K_API2_CRC32 ((size_t)1562612672)
#ifdef K_DEFINE_API2
static const knh_api2_t* getapi2(void) {
	static const knh_api2_t DATA_API2 = {
		K_API2_CRC32,
		knh_DictMap_valueAt,
		knh_getClassDefaultValue,
		new_Boxing,
		new_Array,
		knh_getDefaultClassDef,
		new_Float,
		new_InputStreamDSPI,
		new_InputStreamNULL,
		new_Int,
		new_Iterator,
		new_ObjectNS,
		new_BytesOutputStream,
		new_OutputStreamDSPI,
		new_OutputStreamNULL,
		knh_DictMap_keyAt,
		knh_getFieldName,
		new_String,
		new_String_,
		new_TypeMap,
		Method_isAbstract,
		knh_isSystemVerbose,
		knh_type_tocid,
		knh_ParamArray_get,
		knh_load,
		knh_cwb_tochar,
		knh_ParamArray_rtype,
		ResultSet_setBlob,
		ResultSet_setFloat,
		ResultSet_setInt,
		ResultSet_setNULL,
		ResultSet_setName,
		ResultSet_setText,
		knh_Array_add_,
		knh_Array_swap,
		knh_OutputStream_flush,
		knh_OutputStream_putc,
		knh_OutputStream_write,
		knh_ResultSet_initColumn,
		knh_TypeMap_exec,
		knh_addTypeMap,
		knh_invoke,
		knh_printf,
		knh_setPropertyText,
		knh_write_BOL,
		knh_write_EOL,
		knh_write_TAB,
		knh_write_ascii,
		knh_write_cid,
		knh_write_mn,
		knh_write_type,
		knh_write_utf8,
	};
	return &DATA_API2;
};
#endif/*K_DEFINE_API2*/

#ifndef K_INTERNAL
#define knh_DictMap_valueAt   ctx->api2->DictMap_valueAt
#define knh_getClassDefaultValue   ctx->api2->getClassDefaultValue
#define new_Boxing   ctx->api2->new_Boxing
#define new_Array   ctx->api2->new_Array
#define knh_getDefaultClassDef   ctx->api2->getDefaultClassDef
#define new_Float   ctx->api2->new_Float
#define new_InputStreamDSPI   ctx->api2->new_InputStreamDSPI
#define new_InputStreamNULL   ctx->api2->new_InputStreamNULL
#define new_Int   ctx->api2->new_Int
#define new_Iterator   ctx->api2->new_Iterator
#define new_ObjectNS   ctx->api2->new_ObjectNS
#define new_BytesOutputStream   ctx->api2->new_BytesOutputStream
#define new_OutputStreamDSPI   ctx->api2->new_OutputStreamDSPI
#define new_OutputStreamNULL   ctx->api2->new_OutputStreamNULL
#define knh_DictMap_keyAt   ctx->api2->DictMap_keyAt
#define knh_getFieldName   ctx->api2->getFieldName
#define new_String   ctx->api2->new_String
#define new_String_   ctx->api2->new_String_
#define new_TypeMap   ctx->api2->new_TypeMap
#define Method_isAbstract   ctx->api2->Method_isAbstract
#define knh_isSystemVerbose   ctx->api2->isSystemVerbose
#define knh_type_tocid   ctx->api2->type_tocid
#define knh_ParamArray_get   ctx->api2->ParamArray_get
#define knh_load   ctx->api2->load
#define knh_cwb_tochar   ctx->api2->cwb_tochar
#define knh_ParamArray_rtype   ctx->api2->ParamArray_rtype
#define ResultSet_setBlob   ctx->api2->ResultSet_setBlob
#define ResultSet_setFloat   ctx->api2->ResultSet_setFloat
#define ResultSet_setInt   ctx->api2->ResultSet_setInt
#define ResultSet_setNULL   ctx->api2->ResultSet_setNULL
#define ResultSet_setName   ctx->api2->ResultSet_setName
#define ResultSet_setText   ctx->api2->ResultSet_setText
#define knh_Array_add_   ctx->api2->Array_add_
#define knh_Array_swap   ctx->api2->Array_swap
#define knh_OutputStream_flush   ctx->api2->OutputStream_flush
#define knh_OutputStream_putc   ctx->api2->OutputStream_putc
#define knh_OutputStream_write   ctx->api2->OutputStream_write
#define knh_ResultSet_initColumn   ctx->api2->ResultSet_initColumn
#define knh_TypeMap_exec   ctx->api2->TypeMap_exec
#define knh_addTypeMap   ctx->api2->addTypeMap
#define knh_invoke   ctx->api2->invoke
#define knh_printf   ctx->api2->printf
#define knh_setPropertyText   ctx->api2->setPropertyText
#define knh_write_BOL   ctx->api2->write_BOL
#define knh_write_EOL   ctx->api2->write_EOL
#define knh_write_TAB   ctx->api2->write_TAB
#define knh_write_ascii   ctx->api2->write_ascii
#define knh_write_cid   ctx->api2->write_cid
#define knh_write_mn   ctx->api2->write_mn
#define knh_write_type   ctx->api2->write_type
#define knh_write_utf8   ctx->api2->write_utf8
#endif


/* ------------------------------------------------------------------------ */
/* DEVELOPERS API */
knh_BasicBlock_t* new_BasicBlockLABEL(CTX ctx);
void knh_BasicBlock_add_(CTX ctx, knh_BasicBlock_t *bb, knh_ushort_t line, knh_opline_t *op, size_t size);
METHOD knh_Fmethod_runVM(CTX ctx, knh_sfp_t *sfp _RIX);
void knh_Method_asm(CTX ctx, knh_Method_t *mtd, knh_Stmt_t *stmtP, knh_type_t ittype, knh_Stmt_t *stmtB, knh_Ftyping typing);
void knh_loadScriptSystemKonohaCode(CTX ctx);
void knh_write_vmfunc(CTX ctx, knh_OutputStream_t *w, void *f);
const char* TERM_BBOLD(CTX ctx);
const char* TERM_EBOLD(CTX ctx);
const char* TERM_BNOTE(CTX ctx, int pe);
const char* TERM_ENOTE(CTX ctx, int pe);
knh_Token_t* ERROR_NotFound(CTX ctx, const char *whatis, const char *t);
void WARN_NotFound(CTX ctx, const char *whatis, const char *t);
knh_Token_t* ERROR_IncompatiblePackage(CTX ctx, knh_bytes_t path, const knh_PackageDef_t *pkgdef);
knh_Token_t* ErrorRedefinedClass(CTX ctx, knh_bytes_t cname, knh_class_t cid);
knh_Token_t* ErrorExtendingFinalClass(CTX ctx, knh_class_t cid);
void WARN_MuchBetter(CTX ctx, const char *token);
void WarningMethodName(CTX ctx, const char *name);
void BadPracticeSemicolon(CTX ctx);
void WarningUnexpectedCharacter(CTX ctx);
void WarningMisplaced(CTX ctx);
knh_Token_t *ErrorHazardousStatement(CTX ctx);
void WarningNoEffect(CTX ctx);
knh_Token_t* ErrorMisplaced(CTX ctx);
knh_Token_t* ErrorStaticType(CTX ctx, const char *msg);
knh_Token_t* ErrorFieldAddition(CTX ctx, knh_class_t cid);
knh_Token_t* ERROR_Block(CTX ctx, const char* block);
knh_Token_t* ERROR_text(CTX ctx, const char *keyword K_TRACEARGV);
knh_Token_t* ERROR_Token(CTX ctx, knh_Token_t *tk K_TRACEARGV);
knh_Token_t* ERROR_Stmt(CTX ctx, knh_Stmt_t *stmt K_TRACEARGV);
knh_Token_t* SyntaxErrorTokenIsNot(CTX ctx, knh_Token_t *tk, const char* whatis);
knh_Token_t* ERROR_Required(CTX ctx, knh_Token_t *tk, const char *stmtexpr, const char *token);
void WarningNotInitialized(CTX ctx, knh_Token_t *tk, const char *tool);
knh_Token_t* ErrorRegexCompilation(CTX ctx, knh_Token_t *tk, const char *regname, const char *regdata);
knh_Token_t* ERROR_Undefined(CTX ctx, const char *whatis, knh_class_t cid, knh_Token_t *tk);
knh_Token_t* ERROR_UndefinedName(CTX ctx, knh_Token_t *tk);
void WARN_UndefinedName(CTX ctx, knh_Token_t *tk);
void WARN_AlreadyDefined(CTX ctx, knh_Token_t *tk);
void WARN_AlreadyDefinedClass(CTX ctx, knh_class_t cid, knh_class_t oldcid);
knh_Token_t* ERROR_Denied(CTX ctx, const char *why, knh_Token_t *tk);
void WarningUndefined(CTX ctx, const char *whatis, const char *prefix, const char *msg);
void WarningUndefinedOfClass(CTX ctx, const char *whatis, knh_class_t cid, const char *prefix, const char *msg);
void WarningUnknownClass(CTX ctx, knh_Token_t *tk, knh_class_t defc);
knh_Token_t* ErrorTokenCannotAssign(CTX ctx, knh_Token_t *tk);
knh_Token_t* ErrorUnsupportedConstructor(CTX ctx, knh_class_t mtd_cid);
knh_Token_t* ErrorUndefinedBehavior(CTX ctx, const char *token);
knh_Token_t* ERROR_RequiredParameter(CTX ctx);
void WARN_WrongTypeParam(CTX ctx, knh_class_t cid);
void InfoType(CTX ctx, const char *prefix, knh_bytes_t name, knh_type_t type);
knh_Token_t* ErrorNumericType(CTX ctx, knh_type_t reqt);
void WarningOverflow(CTX ctx, const char *floatorint, knh_bytes_t t);
void WarningUnusedName(CTX ctx, knh_Token_t *tk, knh_fieldn_t fn);
knh_Token_t* ErrorDifferentlyDeclaredType(CTX ctx, knh_fieldn_t fn, knh_type_t type);
knh_Token_t* ErrorTooManyVariables(CTX ctx);
void WARN_UseDefaultValue(CTX ctx, const char *whatis, knh_type_t type);
knh_Token_t* ErrorUndefinedMethod(CTX ctx, knh_Token_t *tkMN);
void WarningNoFmt(CTX ctx, const char *fmt);
void WarningIllegalFormatting(CTX ctx, const char *fmt);
knh_Token_t* ErrorNotStaticMethod(CTX ctx, knh_Method_t *mtd);
knh_Token_t *ERROR_Unsupported(CTX ctx, const char *whatis, knh_class_t cid, const char *symbol);
void WARN_Unsupported(CTX ctx, const char *msg);
void WARN_Ignored(CTX ctx, const char *whatis, knh_class_t cid, const char *symbol);
void WarningUnnecessaryOperation(CTX ctx, const char *msg);
void WarningTooManyParameters(CTX ctx);
knh_Token_t* ERROR_Needs(CTX ctx, const char *whatis);
knh_Token_t* ERROR_MustBe(CTX ctx, const char *whatis, const char* token);
void WarningNullable(CTX ctx, knh_class_t cid);
knh_Token_t* ErrorComparedDiffrentType(CTX ctx, knh_type_t t1, knh_type_t t2);
knh_Token_t *TERROR_Token(CTX ctx, knh_Token_t *tk, knh_class_t type, knh_class_t reqt);
knh_Token_t *TypeErrorStmtNN(CTX ctx, knh_Stmt_t *stmt, int n, knh_type_t reqt, knh_type_t type);
knh_Token_t* TypeErrorCallParam(CTX ctx, int n, knh_Method_t *mtd, knh_class_t reqt, knh_class_t type);
knh_Token_t *TypeErrorNotFuncion(CTX ctx, knh_type_t reqt);
void WarningUpcast(CTX ctx, knh_class_t tcid, knh_class_t scid);
void WarningDowncast(CTX ctx, knh_class_t tcid, knh_class_t scid);
knh_Token_t* ErrorNoSuchTransCast(CTX ctx, knh_class_t tcid, knh_class_t scid);
void WarningDuplicatedDefault(CTX ctx);
void WarningNotConstant(CTX ctx);
void WarningAlwaysFalseAssertion(CTX ctx);
void WarningDifferentMethodClass(CTX ctx, knh_bytes_t name, knh_class_t cid);
void WarningDeprecated(CTX ctx, const char *msg);
knh_Token_t* ErrorFinalMethod(CTX ctx, knh_class_t cid, knh_methodn_t mn);
knh_Token_t* ErrorDifferentlyDefinedMethod(CTX ctx, knh_class_t mtd_cid, knh_methodn_t mn);
knh_Token_t* ERROR_OnlyTopLevel(CTX ctx, const char* stmt);
knh_Token_t* ErrorUndefinedLabel(CTX ctx, knh_Token_t *tk);
void WarningAbstractMethod(CTX ctx, knh_Method_t *mtd);
void WarningDividedByZero(CTX ctx);
void WarningUndefinedFmt(CTX ctx, knh_class_t cid, knh_methodn_t mn);
void NoticeInliningMethod(CTX ctx, knh_Method_t *mtd);
knh_NameSpace_t* new_NameSpace(CTX ctx, knh_NameSpace_t *parent);
knh_class_t knh_NameSpace_getcid(CTX ctx, knh_NameSpace_t *ns, knh_bytes_t sname);
knh_flag_t knh_Stmt_flag_(CTX ctx, knh_Stmt_t *stmt, knh_bytes_t name, knh_flag_t flag);
knh_class_t knh_NameSpace_getFuncClass(CTX ctx, knh_NameSpace_t *ns, knh_methodn_t mn);
knh_type_t knh_NameSpace_gettype(CTX ctx, knh_NameSpace_t *ns, knh_bytes_t name);
knh_type_t knh_NameSpace_tagcid(CTX ctx, knh_NameSpace_t *o, knh_class_t cid, knh_bytes_t tag);
knh_status_t knh_loadScriptPackage(CTX ctx, knh_bytes_t path);
Object *knh_NameSpace_getConstNULL(CTX ctx, knh_NameSpace_t *ns, knh_bytes_t name);
void knh_RefTraverse(CTX ctx, knh_Ftraverse ftr);
knh_bool_t knh_eval(CTX ctx, knh_InputStream_t *in, knh_Array_t *resultsNULL);
knh_status_t konoha_initload(konoha_t konoha, const char *path);
void knh_BasicBlock_optimize(CTX ctx, knh_BasicBlock_t *bb);
knh_Stmt_t* new_Stmt2(CTX ctx, knh_term_t stt, ...);
void knh_Stmt_swap(CTX ctx, knh_Stmt_t *stmt, size_t n, size_t m);
void knh_Stmt_trimToSize(CTX ctx, knh_Stmt_t *stmt, size_t n);
knh_Token_t *knh_Stmt_done(CTX ctx, knh_Stmt_t *stmt);
void knh_Stmt_toERR(CTX ctx, knh_Stmt_t *stmt, knh_Token_t *tkERR);
Object* knh_getConstPools(CTX ctx, void *p);
knh_Term_t *knh_Stmt_add_(CTX ctx, knh_Stmt_t *stmt, ...);
knh_Token_t *new_TokenMN(CTX ctx, knh_methodn_t mn);
knh_Stmt_t *knh_InputStream_parseStmt(CTX ctx, knh_InputStream_t *in);
knh_Stmt_t *knh_Token_parseStmt(CTX ctx, knh_Token_t *tk);
knh_Stmt_t *knh_bytes_parseStmt(CTX ctx, knh_bytes_t expr, knh_uline_t uline);
knh_Token_t* knh_Token_toTYPED(CTX ctx, knh_Token_t *tk, knh_term_t tt, knh_type_t type, knh_short_t nn);
knh_Token_t* new_TokenTYPED(CTX ctx, knh_term_t tt, knh_type_t type, knh_short_t nn);
knh_bool_t StmtMETA_is_(CTX ctx, knh_Stmt_t *stmt, knh_bytes_t name);
knh_class_t knh_Token_cid(CTX ctx, knh_Token_t *tk, knh_type_t reqt);
knh_Token_t* Tn_typing(CTX ctx, knh_Stmt_t *stmt, size_t n, knh_type_t reqt, knh_flag_t opflag);
void knh_class_addField(CTX ctx, knh_class_t bcid, knh_flag_t flag, knh_type_t type, knh_fieldn_t fn);
void knh_ObjectField_expand(CTX ctx, knh_ObjectField_t *of, size_t oldsize, size_t newsize);
void SCRIPT_typing(CTX ctx, knh_Stmt_t *stmt);
knh_bool_t knh_Method_typing(CTX ctx, knh_Method_t *mtd, knh_Stmt_t *stmtP, knh_type_t it, knh_Stmt_t *stmtB);
knh_bool_t knh_Fmt_typing(CTX ctx, knh_Method_t *mtd, knh_Stmt_t *stmtP, knh_type_t it, knh_Stmt_t *stmtB);
void SCRIPT_asm(CTX ctx, knh_Stmt_t *stmt);
void knh_opcode_check(void);
void knh_opcode_stat(CTX ctx);
void knh_opcode_count(CTX ctx, knh_opline_t *c);
const char *OPCODE__(knh_opcode_t opcode);
size_t knh_opcode_size(knh_opcode_t opcode);
knh_bool_t knh_opcode_hasjump(knh_opcode_t opcode);
knh_bool_t knh_opcode_usedef(knh_opcode_t opcode, int i);
knh_Object_t** knh_opline_reftrace(CTX ctx, knh_opline_t *c FTRARG);
void knh_opcode_dump(CTX ctx, knh_opline_t *c, knh_OutputStream_t *w, knh_opline_t *pc_start);
void knh_opcode_shift(knh_opline_t *c, int shift);
knh_opline_t* knh_VirtualMachine_run(CTX ctx, knh_sfp_t *sfp0, knh_opline_t *pc);
void knh_Array_grow(CTX ctx, knh_Array_t *a, size_t newsize, size_t reqsize);
void knh_Array_clear(CTX ctx, knh_Array_t *a, size_t n);
knh_Array_t* new_Array0(CTX ctx, size_t capacity);
void knh_Array_initAPI(CTX ctx, knh_Array_t *a);
void knh_Iterator_close(CTX ctx, knh_Iterator_t *it);
knh_Iterator_t* new_ArrayIterator(CTX ctx, knh_Array_t *a);
knh_bool_t knh_isArrayIterator(knh_Iterator_t *itr);
knh_Array_t* knh_Iterator_toArray(CTX ctx, knh_Iterator_t *itr);
size_t k_goodsize(size_t ss);
size_t k_goodsize2(size_t ss, size_t wsize);
const knh_dim_t *new_dim(CTX ctx, size_t capacity, size_t wsize);
void knh_dimfree(CTX ctx, void *p, const knh_dim_t *dim);
void knh_Bytes_expands(CTX ctx, knh_Bytes_t *ba, size_t newsize);
void knh_Bytes_dispose(CTX ctx, knh_Bytes_t *ba);
knh_Bytes_t* new_Bytes(CTX ctx, const char *name, size_t capacity);
void knh_Bytes_clear(knh_Bytes_t *ba, size_t pos);
void knh_Bytes_ensureSize(CTX ctx, knh_Bytes_t *ba, size_t len);
const char *knh_Bytes_ensureZero(CTX ctx, knh_Bytes_t *ba);
void knh_Bytes_putc(CTX ctx, knh_Bytes_t *ba, int ch);
void knh_Bytes_unputc(knh_Bytes_t *ba, int c);
void knh_Bytes_write(CTX ctx, knh_Bytes_t *ba, knh_bytes_t t);
knh_String_t *knh_cwb_newString(CTX ctx, knh_cwb_t *cwb);
int knh_Object_compareTo(Object *o1, Object *o2);
void knh_Object_toNULL_(CTX ctx, Object *o);
const char *SAFESTRUCT__(CTX ctx, knh_class_t bcid);
const char *SAFETYPE__(CTX ctx, knh_type_t type);
const char *SAFECLASS__(CTX ctx, knh_class_t cid);
const knh_ClassTBL_t* DBG_ClassTBL(CTX ctx, knh_class_t cid);
knh_Class_t *new_Type(CTX ctx, knh_type_t type);
void knh_setClassName(CTX ctx, knh_class_t cid, knh_String_t *lname, knh_String_t *snameNULL);
knh_class_t knh_getcid(CTX ctx, knh_bytes_t lname);
void knh_setClassDefaultValue_(CTX ctx, knh_class_t cid, Object *value, knh_Fdefnull f);
void knh_setClassDef(knh_ClassTBL_t *ct, const knh_ClassDef_t *cdef);
knh_bool_t ClassTBL_isa_(CTX ctx, const knh_ClassTBL_t *ct, const knh_ClassTBL_t *ct2);
int class_isGenerics(CTX ctx, knh_class_t cid);
knh_class_t knh_class_p(CTX ctx, knh_class_t cid, size_t n);
void knh_setClassParam(CTX ctx, knh_ClassTBL_t *t, knh_ParamArray_t *pa);
knh_class_t knh_addGenericsClass(CTX ctx, knh_class_t cid, knh_class_t bcid, knh_ParamArray_t *pa);
knh_class_t knh_class_Generics(CTX ctx, knh_class_t bcid, knh_ParamArray_t *pa);
knh_class_t knh_class_P1(CTX ctx, knh_class_t bcid, knh_type_t p1);
knh_class_t knh_class_P2(CTX ctx, knh_class_t bcid, knh_type_t p1, knh_type_t p2);
knh_bool_t knh_ParamArray_hasTypeVar(knh_ParamArray_t *pa);
void knh_ParamArray_tocid(CTX ctx, knh_ParamArray_t *pa, knh_class_t this_cid, knh_ParamArray_t *npa);
knh_ParamArray_t *new_ParamArrayR0(CTX ctx, knh_type_t t);
knh_ParamArray_t *new_ParamArrayP1(CTX ctx, knh_type_t rtype, knh_type_t p1, knh_fieldn_t fn1);
void knh_ParamArray_add(CTX ctx, knh_ParamArray_t *pa, knh_param_t p);
void knh_ParamArray_radd(CTX ctx, knh_ParamArray_t *pa, knh_param_t p);
knh_type_t knh_ParamArray_getptype(knh_ParamArray_t *pa, size_t n);
knh_bool_t knh_ParamArray_equalsType(knh_ParamArray_t *pa, knh_ParamArray_t *pa2);
void knh_Method_setFunc(CTX ctx, knh_Method_t *mtd, knh_Fmethod func);
void knh_Method_toAbstract(CTX ctx, knh_Method_t *mtd);
knh_Method_t* new_Method(CTX ctx, knh_flag_t flag, knh_class_t cid, knh_methodn_t mn, knh_Fmethod func);
knh_index_t knh_Method_indexOfGetterField(knh_Method_t *o);
knh_index_t knh_Method_indexOfSetterField(knh_Method_t *o);
knh_fieldn_t FN_index(CTX ctx, size_t idx);
knh_Method_t* knh_ClassTBL_getTupleGetter(CTX ctx, const knh_ClassTBL_t *t, knh_methodn_t mn, size_t idx);
void knh_ClassTBL_addMethod(CTX ctx, const knh_ClassTBL_t *t, knh_Method_t *mtd, int isCHECK);
knh_Method_t* knh_ClassTBL_findMethodNULL(CTX ctx, const knh_ClassTBL_t *t, knh_methodn_t mn, int isGEN);
knh_Method_t *knh_ClassTBL_getFmtNULL(CTX ctx, const knh_ClassTBL_t *t, knh_methodn_t mn0);
knh_Method_t *knh_ClassTBL_getFmt(CTX ctx, const knh_ClassTBL_t *t, knh_methodn_t mn0);
void knh_NameSpace_addMethod(CTX ctx, knh_class_t mtd_cid, knh_Method_t *mtd);
knh_Method_t* knh_NameSpace_getMethodNULL(CTX ctx, knh_class_t cid, knh_methodn_t mn);
knh_Method_t* knh_NameSpace_getFmtNULL(CTX ctx, knh_NameSpace_t *ns, knh_class_t cid, knh_methodn_t mn);
void knh_NameSpace_addFmt(CTX ctx, knh_NameSpace_t *ns, knh_Method_t *mtd);
void knh_addTypeMapFunc(CTX ctx, knh_flag_t flag, knh_type_t stype, knh_type_t ttype, knh_Ftypemap fTYPEMAP, Object *mapdata);
knh_bool_t TypeMap_isNoSuchMapping(knh_TypeMap_t *tmr);
knh_TypeMap_t *knh_findTypeMapNULL(CTX ctx, knh_class_t scid0, knh_class_t tcid0, int isT);
knh_class_t new_ClassId(CTX ctx);
void knh_expandEventTBL(CTX ctx);
void pseudo_vsyslog(int p, const char *fmt, va_list ap);
void pseudo_syslog(int p, const char *fmt, ...);
iconv_t iconv_open(const char *t, const char *f);
size_t iconv(iconv_t i, char **t, size_t *ts, char **f, size_t *fs);
int iconv_close(iconv_t i);
knh_Context_t* toContext(CTX ctx);
void knh_context_reftrace(CTX ctx, knh_context_t *o FTRARG);
void knh_Context_free(CTX ctx, knh_context_t* ctxo);
konoha_t konoha_open(size_t stacksize);
void knh_reftraceAll(CTX ctx FTRARG);
void konoha_close(konoha_t konoha);
const knh_StreamDSPI_t *knh_getDefaultStreamDSPI(void);
const knh_StreamDSPI_t *knh_getByteStreamDSPI(void);
const knh_StreamDSPI_t *knh_getStreamDSPI(CTX ctx, knh_NameSpace_t *ns, knh_bytes_t path);
const knh_QueryDSPI_t *knh_getQueryDSPI(CTX ctx, knh_NameSpace_t *ns, knh_bytes_t path);
void knh_loadScriptDriver(CTX ctx, knh_NameSpace_t *ns);
const char* knh_sfile(const char *file);
void dbg_p(const char *file, const char *func, int line, const char *fmt, ...);
void todo_p(const char *file, const char *func, int line, const char *fmt, ...);
void knh_write_uline(CTX ctx, knh_OutputStream_t *w, knh_uline_t uline);
void readuline(FILE *fp, char *buf, size_t bufsiz);
const char* knh_readuline(CTX ctx, knh_uline_t uline, char *buf, size_t bufsiz);
void knh_write_sfp(CTX ctx, knh_OutputStream_t *w, knh_type_t type, knh_sfp_t *sfp, int level);
void knh_throw(CTX ctx, knh_sfp_t *sfp, long start);
void knh_assert(CTX ctx, knh_sfp_t *sfp, long start, knh_uline_t uline);
knh_bool_t isCATCH(CTX ctx, knh_rbp_t *rbp, int en, knh_String_t *event);
void Context_push(CTX ctx, knh_Object_t *o);
knh_Object_t *Context_pop(CTX ctx);
void knh_vtrace(CTX ctx, knh_sfp_t *sfp, int pe, const char *ns, const char *event, int isThrowable, const char *fmt, va_list ap);
void knh_trace(CTX ctx, knh_sfp_t *sfp, int pe,const char *ns, const char *evt, int isThrowable, const char *fmt, ...);
void THROW_Halt(CTX ctx, knh_sfp_t *sfp, const char *msg);
void THROW_Arithmetic(CTX ctx, knh_sfp_t *sfp, const char *msg);
void THROW_OutOfRange(CTX ctx, knh_sfp_t *sfp, knh_int_t n, size_t max);
void THROW_NoSuchMethod(CTX ctx, knh_sfp_t *sfp, knh_class_t cid, knh_methodn_t mn);
void THROW_ParamTypeError(CTX ctx, knh_sfp_t *sfp, size_t n, knh_methodn_t mn, knh_class_t reqt, knh_class_t cid);
const knh_ExportsAPI_t *knh_getExportsAPI(void);
const knh_PackageLoaderAPI_t* knh_getPackageLoaderAPI(void);
knh_Fmethod knh_makeFmethod(CTX ctx, void *func, int argc, knh_ffiparam_t *argv);
void dmap_sort_(knh_dmap_t *dmap, int isforced);
int knh_bytes_strcasecmp2(knh_bytes_t t1, knh_bytes_t t2);
knh_DictMap_t* new_DictMap0_(CTX ctx, size_t capacity, int isCaseMap, const char *DBGNAME);
knh_DictSet_t* new_DictSet0_(CTX ctx, size_t capacity, int isCaseMap, const char *DBGNAME);
knh_uintptr_t knh_DictSet_valueAt(knh_DictSet_t *m, size_t n);
knh_index_t knh_DictMap_index(knh_DictMap_t *m, knh_bytes_t key);
Object *knh_DictMap_getNULL(CTX ctx, knh_DictMap_t *m, knh_bytes_t key);
knh_uintptr_t knh_DictSet_get(CTX ctx, knh_DictSet_t *m, knh_bytes_t key);
void DictMap_set_(CTX ctx, knh_DictMap_t *m, knh_String_t *key, dynamic *v);
void knh_DictSet_set(CTX ctx, knh_DictSet_t *m, knh_String_t *key, knh_uintptr_t n);
void knh_DictMap_append(CTX ctx, knh_DictMap_t *m, knh_String_t *key, knh_Object_t *v);
void knh_DictSet_append(CTX ctx, knh_DictSet_t *m, knh_String_t *key, knh_uintptr_t n);
void knh_DictSet_sort(CTX ctx, knh_DictSet_t *m);
void knh_loadScriptDefaultMapDSPI(CTX ctx, knh_NameSpace_t *ns);
const knh_MapDSPI_t *knh_NameSpace_getMapDSPI(CTX ctx, knh_NameSpace_t *ns, knh_bytes_t path);
const knh_MapDSPI_t *knh_getDefaultMapDSPI(CTX ctx, knh_class_t p1, knh_class_t p2);
void *knh_malloc(CTX ctx, size_t size);
void knh_free(CTX ctx, void *block, size_t size);
void *knh_valloc(CTX ctx, size_t size);
void knh_vfree(CTX ctx, void *block, size_t size);
void *TRACE_malloc(CTX ctx, size_t size K_TRACEARGV);
void TRACE_free(CTX ctx, void *p, size_t size K_TRACEARGV);
void *TRACE_realloc(CTX ctx, void *p, size_t os, size_t ns, size_t wsize K_TRACEARGV);
void *knh_fastmalloc(CTX ctx, size_t size);
void knh_fastfree(CTX ctx, void *block, size_t size);
void* knh_fastrealloc(CTX ctx, void *block, size_t os, size_t ns, size_t wsize);
knh_bool_t knh_isObject(CTX ctx, void *p);
knh_Object_t *new_hObject_(CTX ctx, const knh_ClassTBL_t *ct);
knh_Object_t *new_Object_init2(CTX ctx, const knh_ClassTBL_t *ct);
void TR_NEW(CTX ctx, knh_sfp_t *sfp, knh_sfpidx_t c, const knh_ClassTBL_t *ct);
knh_Object_t** knh_ensurerefs(CTX ctx, knh_Object_t** tail, size_t size);
void knh_Object_RCfree(CTX ctx, Object *o);
void knh_Object_RCsweep(CTX ctx, Object *o);
void knh_ObjectObjectArenaTBL_free(CTX ctx, const knh_ObjectArenaTBL_t *oat);
void knh_showMemoryStat(CTX ctx);
void knh_System_gc(CTX ctx);
void knh_srand(knh_uint_t seed);
knh_uint_t knh_rand(void);
knh_float_t knh_float_rand(void);
METHOD Bytes_getSize(CTX ctx, knh_sfp_t *sfp _RIX);
METHOD Tuple_getSize(CTX ctx, knh_sfp_t *sfp _RIX);
METHOD Map_getSize(CTX ctx, knh_sfp_t *sfp _RIX);
const knh_sysinfo_t* knh_getsysinfo(void);
void dump_sysinfo(CTX ctx, knh_OutputStream_t *w, int isALL);
knh_path_t* knh_path_open_(CTX ctx, const char *scheme, knh_bytes_t t, knh_path_t *ph);
knh_path_t* knh_path_open(CTX ctx, const char *scheme, const char *name, knh_path_t *ph);
knh_cwb_t* knh_cwb_copy(CTX ctx, knh_cwb_t *cwbbuf, knh_path_t *ph, int hasScheme);
void knh_path_reset(CTX ctx, knh_path_t *ph, const char *scheme, knh_bytes_t t);
void knh_path_close(CTX ctx, knh_path_t *ph);
const char* knh_realpath(CTX ctx, const char *path, knh_path_t *ph);
const char* knh_ospath(CTX ctx, knh_path_t *ph);
knh_String_t* knh_path_newString(CTX ctx, knh_path_t *ph, int hasScheme);
knh_bool_t knh_path_isfile(CTX ctx, knh_path_t *ph);
knh_bool_t knh_path_isdir(CTX ctx, knh_path_t *ph);
knh_bool_t knh_path_mkdir(CTX ctx, knh_path_t *ph);
const char* knh_path_reduce(CTX ctx, knh_path_t *ph, int ch);
void knh_path_append(CTX ctx, knh_path_t *ph, int issep, const char *name);
void knh_System_initPath(CTX ctx, knh_System_t *o);
void *knh_dlopen(CTX ctx, const char* path);
void *knh_path_dlopen(CTX ctx, knh_path_t *ph);
void *knh_dlsym(CTX ctx, int pe, void* handler, const char* symbol);
int knh_dlclose(CTX ctx, void* hdr);
const char *knh_getSystemEncoding(void);
void knh_Connection_open(CTX ctx, knh_Connection_t *c, knh_NameSpace_t *ns, knh_String_t *urn);
knh_Connection_t* new_Connection(CTX ctx, knh_NameSpace_t *ns, knh_String_t *urn);
void knh_Connection_close(CTX ctx, knh_Connection_t *c);
knh_bool_t knh_ResultSet_next(CTX ctx, knh_ResultSet_t *o);
void knh_ResultSet_close(CTX ctx, knh_ResultSet_t *o);
void knh_ResultSet_initTargetClass(knh_ResultSet_t *o, knh_class_t tcid);
knh_String_t *knh_ResultSet_getName(CTX ctx, knh_ResultSet_t *o, size_t n);
int knh_ResultSet_findColumn(CTX ctx, knh_ResultSet_t *o, knh_bytes_t name);
knh_type_t knh_ResultSet_get_type(CTX ctx, knh_ResultSet_t *o, size_t n);
void knh_ResultSet_initData(CTX ctx, knh_ResultSet_t *rs);
knh_int_t knh_ResultSet_getInt(CTX ctx, knh_ResultSet_t *o, size_t n);
knh_float_t knh_ResultSet_getFloat(CTX ctx, knh_ResultSet_t *o, size_t n);
knh_String_t* knh_ResultSet_getString(CTX ctx, knh_ResultSet_t *o, size_t n);
void konoha_init(void);
void knh_beginContext(CTX ctx, void **bottom);
void knh_endContext(CTX ctx);
knh_context_t* knh_getCurrentContext(void);
knh_bool_t knh_isAuditLogging(int pe);
knh_bool_t knh_isVerboseLogging(int pe);
knh_bool_t knh_shouldWarnMessage(int pe);
knh_bool_t knh_isCompileOnly(CTX ctx /* for future extension*/);
void knh_loadScriptPackageList(CTX ctx, const char *pkglist);
FILE *knh_getUFILE(void);
knh_bytes_t knh_bytes_nsname(knh_bytes_t t);
void knh_setSecurityAlertMessage(const char *msg, int isNeedFree);
void konoha_main(konoha_t konoha, int argc, const char **argv);
void knh_askSecurityAlert(CTX ctx);
void knh_checkSecurityManager(CTX ctx, knh_sfp_t *sfp);
knh_Int_t* new_IntX(CTX ctx, knh_class_t cid, knh_int_t value);
knh_Float_t* new_FloatX(CTX ctx, knh_class_t cid, knh_float_t value);
void knh_write_intx(CTX ctx, knh_OutputStream_t *w, knh_Semantics_t *u, knh_int_t v);
void knh_write_floatx(CTX ctx, knh_OutputStream_t *w, knh_Semantics_t *u, knh_float_t v);
void knh_Semantics_reuse(CTX ctx, knh_Semantics_t *u, knh_class_t cid);
knh_Semantics_t* new_Enum(CTX ctx, char *tag, knh_bytes_t urn, knh_int_t min, knh_int_t max);
knh_Semantics_t* new_Unit(CTX ctx, char *tag, knh_bytes_t urn, knh_float_t min, knh_float_t max, knh_float_t step);
knh_Semantics_t* new_Vocab(CTX ctx, char *tag, knh_bytes_t urn, int base, char **terms);
knh_bytes_t knh_getURNAlias(CTX ctx, knh_bytes_t aurn);
void knh_loadScriptURNAliasData(CTX ctx, knh_StringData_t *data);
void knh_loadScriptSemanticsFuncData(CTX ctx, knh_NamedPointerData_t *data);
knh_Semantics_t *knh_getSemantics(CTX ctx, knh_class_t cid);
knh_class_t knh_findcidx(CTX ctx, knh_bytes_t lname);
knh_sfp_t* knh_stack_initexpand(CTX ctx, knh_sfp_t *sfp, size_t n);
void knh_stack_clear(CTX ctx, knh_sfp_t *sfp);
knh_sfp_t* knh_stack_local(CTX ctx, size_t n);
void knh_stack_gc(CTX ctx, int isALL);
void knh_stack_typecheck(CTX ctx, knh_sfp_t *sfp, knh_Method_t *mtd, knh_opline_t *pc);
int expt_isa(CTX ctx, knh_ebi_t eid, knh_ebi_t parent);
knh_String_t *knh_getEventName(CTX ctx, knh_ebi_t eid);
knh_ebi_t knh_addEvent(CTX ctx, knh_flag_t flag, knh_class_t eid, knh_String_t *name, knh_class_t peid);
knh_ebi_t knh_geteid(CTX ctx, knh_bytes_t msg, knh_ebi_t def);
knh_Exception_t* Exception_setup(CTX ctx, knh_Exception_t *e, knh_String_t *event, knh_String_t *msg, Object *bag);
knh_Exception_t* new_Error(CTX ctx, knh_ebi_t eid, knh_String_t *msg);
void CTX_setThrowingException(CTX ctx, knh_Exception_t *e);
knh_ExceptionHandler_t* ExceptionHandler_setjmp(CTX ctx, knh_ExceptionHandler_t *hdr);
knh_ExceptionHandler_t *knh_ExceptionHandler_longjmp(CTX ctx, knh_ExceptionHandler_t *hdr);
knh_InputStream_t* new_BytesInputStream(CTX ctx, knh_Bytes_t *ba);
knh_InputStream_t* new_StringInputStream(CTX ctx, knh_String_t *str);
void knh_InputStream_setpos(CTX ctx, knh_InputStream_t *in, size_t s, size_t e);
int knh_InputStream_getc(CTX ctx, knh_InputStream_t *in);
knh_String_t* knh_InputStream_readLine(CTX ctx, knh_InputStream_t *in);
void knh_InputStream_close(CTX ctx, knh_InputStream_t *in);
int InputStream_isClosed(CTX ctx, knh_InputStream_t *in);
void InputStream_setCharset(CTX ctx, knh_InputStream_t *in, knh_StringDecoder_t *c);
void knh_OutputStream_clear(CTX ctx, knh_OutputStream_t *w);
void knh_OutputStream_close(CTX ctx, knh_OutputStream_t *w);
int OutputStream_isClosed(knh_OutputStream_t *w);
void OutputStream_setCharset(CTX ctx, knh_OutputStream_t *w, knh_StringEncoder_t *c);
void knh_write_bool(CTX ctx, knh_OutputStream_t *w, int b);
void knh_write__p(CTX ctx, knh_OutputStream_t *w, void *ptr);
void knh_write_dfmt(CTX ctx, knh_OutputStream_t *w, const char *fmt, knh_intptr_t n);
void knh_write_ifmt(CTX ctx, knh_OutputStream_t *w, const char *fmt, knh_int_t n);
void knh_write_ffmt(CTX ctx, knh_OutputStream_t *w, const char *fmt, knh_float_t n);
void knh_write_flag(CTX ctx, knh_OutputStream_t *w, knh_flag_t flag);
void knh_write_quote(CTX ctx, knh_OutputStream_t *w, int quote, knh_bytes_t t, int hasUTF8);
void knh_write_cap(CTX ctx, knh_OutputStream_t *w, knh_bytes_t t, int hasUTF8);
void knh_write_Object(CTX ctx, knh_OutputStream_t *w, Object *o, int level);
void knh_write_InObject(CTX ctx, knh_OutputStream_t *w, Object *o, int level);
void knh_vprintf(CTX ctx, knh_OutputStream_t *w, const char *fmt, va_list ap);
int knh_bytes_parseint(knh_bytes_t t, knh_int_t *value);
int knh_bytes_parsefloat(knh_bytes_t t, knh_float_t *value);
knh_index_t knh_bytes_indexOf(knh_bytes_t base, knh_bytes_t sub);
int knh_bytes_strcmp(knh_bytes_t v1, knh_bytes_t v2);
knh_bool_t knh_bytes_checkENCODING(knh_bytes_t v);
size_t knh_bytes_mlen(knh_bytes_t v);
knh_bytes_t knh_bytes_mofflen(knh_bytes_t v, size_t moff, size_t mlen);
knh_int_t knh_uchar_toucs4(knh_utext_t *utf8)   /* utf8 -> ucs4 */;
char *knh_format_utf8(char *buf, size_t bufsiz, knh_uint_t ucs4);
knh_String_t *new_TEXT(CTX ctx, knh_class_t cid, knh_TEXT_t text, int isASCII);
int knh_bytes_strcasecmp(knh_bytes_t v1, knh_bytes_t v2);
const knh_RegexSPI_t* knh_getStrRegexSPI(void);
knh_bool_t Regex_isSTRREGEX(knh_Regex_t *re);
size_t pcre_regex_nmatchsize(knh_regex_t *reg);
const knh_RegexSPI_t* knh_getRegexSPI(void);
knh_StringDecoder_t* new_StringDecoderNULL(CTX ctx, knh_bytes_t t);
knh_StringEncoder_t* new_StringEncoderNULL(CTX ctx, knh_bytes_t t);
knh_String_t *knh_cwb_newStringDECODE(CTX ctx, knh_cwb_t *cwb, knh_StringDecoder_t *c);
knh_bool_t knh_class_canObjectCopy(CTX ctx, knh_class_t cid);
knh_TypeMap_t* DEFAULT_findTypeMapNULL(CTX ctx, knh_class_t scid, knh_class_t tcid, int mode);
void knh_ClassTBL_setObjectCSPI(knh_ClassTBL_t *ct);
void knh_loadScriptSystemStructData(CTX ctx, const knh_PackageLoaderAPI_t *kapi);
void knh_loadScriptSystemString(CTX ctx);
void knh_loadScriptSystemData(CTX ctx, knh_NameSpace_t *ns, const knh_PackageLoaderAPI_t *kapi);
void knh_loadScriptSystemMethod(CTX ctx, const knh_PackageLoaderAPI_t *kapi);
knh_String_t* knh_getPropertyNULL(CTX ctx, knh_bytes_t key);
void knh_setProperty(CTX ctx, knh_String_t *key, dynamic *value);
Object *knh_getClassConstNULL(CTX ctx, knh_class_t cid, knh_bytes_t name);
int knh_addClassConst(CTX ctx, knh_class_t cid, knh_String_t* name, Object *value);
knh_fieldn_t knh_addname(CTX ctx, knh_String_t *s, knh_Fdictset f);
knh_nameinfo_t *knh_getnameinfo(CTX ctx, knh_fieldn_t fn);
knh_fieldn_t knh_getfnq(CTX ctx, knh_bytes_t tname, knh_fieldn_t def);
knh_methodn_t knh_getmn(CTX ctx, knh_bytes_t tname, knh_methodn_t def);
const char* knh_getmnname(CTX ctx, knh_methodn_t mn);
knh_uri_t knh_getURI(CTX ctx, knh_bytes_t t);
knh_String_t *knh_getURN(CTX ctx, knh_uri_t uri);
void knh_NameSpace_addDSPI(CTX ctx, knh_NameSpace_t *ns, const char *scheme, const knh_DSPI_t* p);
const knh_DSPI_t *knh_NameSpace_getDSPINULL(CTX ctx, knh_NameSpace_t *ns, int type, knh_bytes_t path);
const knh_PathDSPI_t *knh_NameSpace_getPathDSPINULL(CTX ctx, knh_NameSpace_t *ns, knh_bytes_t path);
const knh_ConvDSPI_t *knh_NameSpace_getConvTODSPINULL(CTX ctx, knh_NameSpace_t *ns, knh_bytes_t path);
const knh_ConvDSPI_t *knh_NameSpace_getConvFROMDSPINULL(CTX ctx, knh_NameSpace_t *ns, knh_bytes_t path);
void knh_exit(CTX ctx, int status);
knh_thread_t knh_thread_self(void);
int knh_thread_create(CTX ctx, knh_thread_t *thread, void *attr, knh_Fthread fgo, void * arg);
int knh_thread_detach(CTX ctx, knh_thread_t th);
int knh_thread_join(CTX ctx, knh_thread_t thread, void **ret);
int knh_mutex_init(knh_mutex_t *m);
int knh_mutex_lock(knh_mutex_t *m);
int knh_mutex_trylock(knh_mutex_t *m);
int knh_mutex_unlock(knh_mutex_t *m);
int knh_mutex_destroy(knh_mutex_t *m);
knh_mutex_t *knh_mutex_malloc(CTX ctx);
void knh_mutex_free(CTX ctx, knh_mutex_t *m);
int knh_thread_key_create(knh_thread_key_t *key);
int thread_setspecific(knh_thread_key_t key, const void *data);
void* knh_thread_getspecific(knh_thread_key_t key);
int knh_thread_key_delete(knh_thread_key_t key);

#ifdef __cplusplus
}
#endif

// THIS FILE WAS AUTOMATICALLY GENERATED. DON'T EDIT.

