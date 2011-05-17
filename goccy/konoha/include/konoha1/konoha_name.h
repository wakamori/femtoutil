// THIS FILE WAS AUTOMATICALLY GENERATED

/* ------------------------------------------------------------------------ */
/* MACROS */
#ifndef K_REVISION
#define K_REVISION                      185
#endif
#define K_BUILDID                       1098

/* ------------------------------------------------------------------------ */
/* STRUCT */

/* ------------------------------------------------------------------------ */
/* Tvoid */
#define CLASS_Tvoid             ((knh_class_t)0)
#define IS_Tvoid(o)             (O_cid(o) == CLASS_Tvoid)
#define IS_bTvoid(o)            (O_bcid(o) == CLASS_Tvoid)
#define TYPE_Tvoid              CLASS_Tvoid
#define CFLAG_Tvoid             ((knh_flag_t)FLAG_Class_TypeVariable)
#define FLAG_Tvoid              KNH_MAGICFLAG(CFLAG_Tvoid)

/* ------------------------------------------------------------------------ */
/* Tvar */
#define CLASS_Tvar              ((knh_class_t)1)
#define IS_Tvar(o)              (O_cid(o) == CLASS_Tvar)
#define IS_bTvar(o)             (O_bcid(o) == CLASS_Tvar)
#define TYPE_Tvar               CLASS_Tvar
#define CFLAG_Tvar              ((knh_flag_t)FLAG_Class_TypeVariable)
#define FLAG_Tvar               KNH_MAGICFLAG(CFLAG_Tvar)

/* ------------------------------------------------------------------------ */
/* Object */
#define CLASS_Object            ((knh_class_t)2)
#define IS_Object(o)            (O_cid(o) == CLASS_Object)
#define TYPE_Object             CLASS_Object
#define CFLAG_Object            ((knh_flag_t)0)
#define FLAG_Object             KNH_MAGICFLAG(CFLAG_Object)

/* ------------------------------------------------------------------------ */
/* Boolean */
#define CLASS_Boolean           ((knh_class_t)3)
#define IS_Boolean(o)           (O_cid(o) == CLASS_Boolean)
#define IS_bBoolean(o)          (O_bcid(o) == CLASS_Boolean)
#define TYPE_Boolean            CLASS_Boolean
#define CFLAG_Boolean           ((knh_flag_t)FLAG_Class_Immutable)
#define FLAG_Boolean            KNH_MAGICFLAG(CFLAG_Boolean)

/* ------------------------------------------------------------------------ */
/* Number */
#define CLASS_Number            ((knh_class_t)4)
#define IS_Number(o)            (O_cid(o) == CLASS_Number)
#define IS_bNumber(o)           (O_bcid(o) == CLASS_Number)
#define TYPE_Number             CLASS_Number
#define CFLAG_Number            ((knh_flag_t)FLAG_Class_Immutable)
#define FLAG_Number             KNH_MAGICFLAG(CFLAG_Number)

/* ------------------------------------------------------------------------ */
/* Int */
#define CLASS_Int               ((knh_class_t)5)
#define IS_Int(o)               (O_cid(o) == CLASS_Int)
#define IS_bInt(o)              (O_bcid(o) == CLASS_Int)
#define TYPE_Int                CLASS_Int
#define CFLAG_Int               ((knh_flag_t)FLAG_Class_Immutable)
#define FLAG_Int                KNH_MAGICFLAG(CFLAG_Int)

/* ------------------------------------------------------------------------ */
/* Float */
#define CLASS_Float             ((knh_class_t)6)
#define IS_Float(o)             (O_cid(o) == CLASS_Float)
#define IS_bFloat(o)            (O_bcid(o) == CLASS_Float)
#define TYPE_Float              CLASS_Float
#define CFLAG_Float             ((knh_flag_t)FLAG_Class_Immutable)
#define FLAG_Float              KNH_MAGICFLAG(CFLAG_Float)

/* ------------------------------------------------------------------------ */
/* String */
#define CLASS_String            ((knh_class_t)7)
#define IS_String(o)            (O_cid(o) == CLASS_String)
#define IS_bString(o)           (O_bcid(o) == CLASS_String)
#define TYPE_String             CLASS_String
#define CFLAG_String            ((knh_flag_t)FLAG_Class_Immutable)
#define FLAG_String             KNH_MAGICFLAG(CFLAG_String)

/* ------------------------------------------------------------------------ */
/* Bytes */
#define CLASS_Bytes             ((knh_class_t)8)
#define IS_Bytes(o)             (O_cid(o) == CLASS_Bytes)
#define IS_bBytes(o)            (O_bcid(o) == CLASS_Bytes)
#define TYPE_Bytes              CLASS_Bytes
#define CFLAG_Bytes             ((knh_flag_t)0)
#define FLAG_Bytes              KNH_MAGICFLAG(CFLAG_Bytes)

/* ------------------------------------------------------------------------ */
/* BytesIm */
#define CLASS_BytesIm           ((knh_class_t)9)
#define IS_BytesIm(o)           (O_cid(o) == CLASS_BytesIm)
#define IS_bBytesIm(o)          (O_bcid(o) == CLASS_BytesIm)
#define TYPE_BytesIm            CLASS_BytesIm
#define CFLAG_BytesIm           ((knh_flag_t)FLAG_Class_Immutable)
#define FLAG_BytesIm            KNH_MAGICFLAG(CFLAG_BytesIm)

/* ------------------------------------------------------------------------ */
/* Iterator */
#define CLASS_Iterator          ((knh_class_t)10)
#define IS_Iterator(o)          (O_cid(o) == CLASS_Iterator)
#define IS_bIterator(o)         (O_bcid(o) == CLASS_Iterator)
#define TYPE_Iterator           CLASS_Iterator
#define CFLAG_Iterator          ((knh_flag_t)0)
#define FLAG_Iterator           KNH_MAGICFLAG(CFLAG_Iterator)

/* ------------------------------------------------------------------------ */
/* Tuple */
#define CLASS_Tuple             ((knh_class_t)11)
#define IS_Tuple(o)             (O_cid(o) == CLASS_Tuple)
#define IS_bTuple(o)            (O_bcid(o) == CLASS_Tuple)
#define TYPE_Tuple              CLASS_Tuple
#define CFLAG_Tuple             ((knh_flag_t)0)
#define FLAG_Tuple              KNH_MAGICFLAG(CFLAG_Tuple)

/* ------------------------------------------------------------------------ */
/* Range */
#define CLASS_Range             ((knh_class_t)12)
#define IS_Range(o)             (O_cid(o) == CLASS_Range)
#define IS_bRange(o)            (O_bcid(o) == CLASS_Range)
#define TYPE_Range              CLASS_Range
#define CFLAG_Range             ((knh_flag_t)FLAG_Class_Immutable)
#define FLAG_Range              KNH_MAGICFLAG(CFLAG_Range)

/* ------------------------------------------------------------------------ */
/* Array */
#define CLASS_Array             ((knh_class_t)13)
#define IS_Array(o)             (O_cid(o) == CLASS_Array)
#define IS_bArray(o)            (O_bcid(o) == CLASS_Array)
#define TYPE_Array              CLASS_Array
#define CFLAG_Array             ((knh_flag_t)0)
#define FLAG_Array              KNH_MAGICFLAG(CFLAG_Array)

/* ------------------------------------------------------------------------ */
/* ArrayIm */
#define CLASS_ArrayIm           ((knh_class_t)14)
#define IS_ArrayIm(o)           (O_cid(o) == CLASS_ArrayIm)
#define IS_bArrayIm(o)          (O_bcid(o) == CLASS_ArrayIm)
#define TYPE_ArrayIm            CLASS_ArrayIm
#define CFLAG_ArrayIm           ((knh_flag_t)0)
#define FLAG_ArrayIm            KNH_MAGICFLAG(CFLAG_ArrayIm)

/* ------------------------------------------------------------------------ */
/* Map */
#define CLASS_Map               ((knh_class_t)15)
#define IS_Map(o)               (O_cid(o) == CLASS_Map)
#define IS_bMap(o)              (O_bcid(o) == CLASS_Map)
#define TYPE_Map                CLASS_Map
#define CFLAG_Map               ((knh_flag_t)0)
#define FLAG_Map                KNH_MAGICFLAG(CFLAG_Map)

/* ------------------------------------------------------------------------ */
/* MapIm */
#define CLASS_MapIm             ((knh_class_t)16)
#define IS_MapIm(o)             (O_cid(o) == CLASS_MapIm)
#define IS_bMapIm(o)            (O_bcid(o) == CLASS_MapIm)
#define TYPE_MapIm              CLASS_MapIm
#define CFLAG_MapIm             ((knh_flag_t)FLAG_Class_Immutable)
#define FLAG_MapIm              KNH_MAGICFLAG(CFLAG_MapIm)

/* ------------------------------------------------------------------------ */
/* Class */
#define CLASS_Class             ((knh_class_t)17)
#define IS_Class(o)             (O_cid(o) == CLASS_Class)
#define IS_bClass(o)            (O_bcid(o) == CLASS_Class)
#define TYPE_Class              CLASS_Class
#define CFLAG_Class             ((knh_flag_t)FLAG_Class_Immutable)
#define FLAG_Class              KNH_MAGICFLAG(CFLAG_Class)

/* ------------------------------------------------------------------------ */
/* ParamArray */
#define CLASS_ParamArray        ((knh_class_t)18)
#define IS_ParamArray(o)        (O_cid(o) == CLASS_ParamArray)
#define IS_bParamArray(o)       (O_bcid(o) == CLASS_ParamArray)
#define TYPE_ParamArray         CLASS_ParamArray
#define CFLAG_ParamArray        ((knh_flag_t)FLAG_Class_Immutable|FLAG_Class_Private)
#define FLAG_ParamArray         KNH_MAGICFLAG(CFLAG_ParamArray)

/* ------------------------------------------------------------------------ */
/* Method */
#define CLASS_Method            ((knh_class_t)19)
#define IS_Method(o)            (O_cid(o) == CLASS_Method)
#define IS_bMethod(o)           (O_bcid(o) == CLASS_Method)
#define TYPE_Method             CLASS_Method
#define CFLAG_Method            ((knh_flag_t)0)
#define FLAG_Method             KNH_MAGICFLAG(CFLAG_Method)

/* ------------------------------------------------------------------------ */
/* TypeMap */
#define CLASS_TypeMap           ((knh_class_t)20)
#define IS_TypeMap(o)           (O_cid(o) == CLASS_TypeMap)
#define IS_bTypeMap(o)          (O_bcid(o) == CLASS_TypeMap)
#define TYPE_TypeMap            CLASS_TypeMap
#define CFLAG_TypeMap           ((knh_flag_t)0)
#define FLAG_TypeMap            KNH_MAGICFLAG(CFLAG_TypeMap)

/* ------------------------------------------------------------------------ */
/* Func */
#define CLASS_Func              ((knh_class_t)21)
#define IS_Func(o)              (O_cid(o) == CLASS_Func)
#define IS_bFunc(o)             (O_bcid(o) == CLASS_Func)
#define TYPE_Func               CLASS_Func
#define CFLAG_Func              ((knh_flag_t)FLAG_Class_Immutable)
#define FLAG_Func               KNH_MAGICFLAG(CFLAG_Func)

/* ------------------------------------------------------------------------ */
/* Thunk */
#define CLASS_Thunk             ((knh_class_t)22)
#define IS_Thunk(o)             (O_cid(o) == CLASS_Thunk)
#define IS_bThunk(o)            (O_bcid(o) == CLASS_Thunk)
#define TYPE_Thunk              CLASS_Thunk
#define CFLAG_Thunk             ((knh_flag_t)0)
#define FLAG_Thunk              KNH_MAGICFLAG(CFLAG_Thunk)

/* ------------------------------------------------------------------------ */
/* Exception */
#define CLASS_Exception         ((knh_class_t)23)
#define IS_Exception(o)         (O_cid(o) == CLASS_Exception)
#define IS_bException(o)        (O_bcid(o) == CLASS_Exception)
#define TYPE_Exception          CLASS_Exception
#define CFLAG_Exception         ((knh_flag_t)0)
#define FLAG_Exception          KNH_MAGICFLAG(CFLAG_Exception)

/* ------------------------------------------------------------------------ */
/* ExceptionHandler */
#define CLASS_ExceptionHandler  ((knh_class_t)24)
#define IS_ExceptionHandler(o)  (O_cid(o) == CLASS_ExceptionHandler)
#define IS_bExceptionHandler(o) (O_bcid(o) == CLASS_ExceptionHandler)
#define TYPE_ExceptionHandler   CLASS_ExceptionHandler
#define CFLAG_ExceptionHandler  ((knh_flag_t)FLAG_Class_Private)
#define FLAG_ExceptionHandler   KNH_MAGICFLAG(CFLAG_ExceptionHandler)

/* ------------------------------------------------------------------------ */
/* Regex */
#define CLASS_Regex             ((knh_class_t)25)
#define IS_Regex(o)             (O_cid(o) == CLASS_Regex)
#define IS_bRegex(o)            (O_bcid(o) == CLASS_Regex)
#define TYPE_Regex              CLASS_Regex
#define CFLAG_Regex             ((knh_flag_t)FLAG_Class_Immutable)
#define FLAG_Regex              KNH_MAGICFLAG(CFLAG_Regex)

/* ------------------------------------------------------------------------ */
/* Converter */
#define CLASS_Converter         ((knh_class_t)26)
#define IS_Converter(o)         (O_cid(o) == CLASS_Converter)
#define IS_bConverter(o)        (O_bcid(o) == CLASS_Converter)
#define TYPE_Converter          CLASS_Converter
#define CFLAG_Converter         ((knh_flag_t)FLAG_Class_Immutable)
#define FLAG_Converter          KNH_MAGICFLAG(CFLAG_Converter)

/* ------------------------------------------------------------------------ */
/* StringEncoder */
#define CLASS_StringEncoder     ((knh_class_t)27)
#define IS_StringEncoder(o)     (O_cid(o) == CLASS_StringEncoder)
#define IS_bStringEncoder(o)    (O_bcid(o) == CLASS_StringEncoder)
#define TYPE_StringEncoder      CLASS_StringEncoder
#define CFLAG_StringEncoder     ((knh_flag_t)FLAG_Class_Immutable)
#define FLAG_StringEncoder      KNH_MAGICFLAG(CFLAG_StringEncoder)

/* ------------------------------------------------------------------------ */
/* StringDecoder */
#define CLASS_StringDecoder     ((knh_class_t)28)
#define IS_StringDecoder(o)     (O_cid(o) == CLASS_StringDecoder)
#define IS_bStringDecoder(o)    (O_bcid(o) == CLASS_StringDecoder)
#define TYPE_StringDecoder      CLASS_StringDecoder
#define CFLAG_StringDecoder     ((knh_flag_t)FLAG_Class_Immutable)
#define FLAG_StringDecoder      KNH_MAGICFLAG(CFLAG_StringDecoder)

/* ------------------------------------------------------------------------ */
/* StringConverter */
#define CLASS_StringConverter   ((knh_class_t)29)
#define IS_StringConverter(o)   (O_cid(o) == CLASS_StringConverter)
#define IS_bStringConverter(o)  (O_bcid(o) == CLASS_StringConverter)
#define TYPE_StringConverter    CLASS_StringConverter
#define CFLAG_StringConverter   ((knh_flag_t)FLAG_Class_Immutable)
#define FLAG_StringConverter    KNH_MAGICFLAG(CFLAG_StringConverter)

/* ------------------------------------------------------------------------ */
/* Semantics */
#define CLASS_Semantics         ((knh_class_t)30)
#define IS_Semantics(o)         (O_cid(o) == CLASS_Semantics)
#define IS_bSemantics(o)        (O_bcid(o) == CLASS_Semantics)
#define TYPE_Semantics          CLASS_Semantics
#define CFLAG_Semantics         ((knh_flag_t)FLAG_Class_Private)
#define FLAG_Semantics          KNH_MAGICFLAG(CFLAG_Semantics)

/* ------------------------------------------------------------------------ */
/* InputStream */
#define CLASS_InputStream       ((knh_class_t)31)
#define IS_InputStream(o)       (O_cid(o) == CLASS_InputStream)
#define IS_bInputStream(o)      (O_bcid(o) == CLASS_InputStream)
#define TYPE_InputStream        CLASS_InputStream
#define CFLAG_InputStream       ((knh_flag_t)0)
#define FLAG_InputStream        KNH_MAGICFLAG(CFLAG_InputStream)

/* ------------------------------------------------------------------------ */
/* OutputStream */
#define CLASS_OutputStream      ((knh_class_t)32)
#define IS_OutputStream(o)      (O_cid(o) == CLASS_OutputStream)
#define IS_bOutputStream(o)     (O_bcid(o) == CLASS_OutputStream)
#define TYPE_OutputStream       CLASS_OutputStream
#define CFLAG_OutputStream      ((knh_flag_t)0)
#define FLAG_OutputStream       KNH_MAGICFLAG(CFLAG_OutputStream)

/* ------------------------------------------------------------------------ */
/* Connection */
#define CLASS_Connection        ((knh_class_t)33)
#define IS_Connection(o)        (O_cid(o) == CLASS_Connection)
#define IS_bConnection(o)       (O_bcid(o) == CLASS_Connection)
#define TYPE_Connection         CLASS_Connection
#define CFLAG_Connection        ((knh_flag_t)0)
#define FLAG_Connection         KNH_MAGICFLAG(CFLAG_Connection)

/* ------------------------------------------------------------------------ */
/* ResultSet */
#define CLASS_ResultSet         ((knh_class_t)34)
#define IS_ResultSet(o)         (O_cid(o) == CLASS_ResultSet)
#define IS_bResultSet(o)        (O_bcid(o) == CLASS_ResultSet)
#define TYPE_ResultSet          CLASS_ResultSet
#define CFLAG_ResultSet         ((knh_flag_t)0)
#define FLAG_ResultSet          KNH_MAGICFLAG(CFLAG_ResultSet)

/* ------------------------------------------------------------------------ */
/* NameSpace */
#define CLASS_NameSpace         ((knh_class_t)35)
#define IS_NameSpace(o)         (O_cid(o) == CLASS_NameSpace)
#define IS_bNameSpace(o)        (O_bcid(o) == CLASS_NameSpace)
#define TYPE_NameSpace          CLASS_NameSpace
#define CFLAG_NameSpace         ((knh_flag_t)0)
#define FLAG_NameSpace          KNH_MAGICFLAG(CFLAG_NameSpace)

/* ------------------------------------------------------------------------ */
/* Script */
#define CLASS_Script            ((knh_class_t)36)
#define IS_Script(o)            (O_cid(o) == CLASS_Script)
#define IS_bScript(o)           (O_bcid(o) == CLASS_Script)
#define TYPE_Script             CLASS_Script
#define CFLAG_Script            ((knh_flag_t)FLAG_Class_Singleton|FLAG_Class_Private)
#define FLAG_Script             KNH_MAGICFLAG(CFLAG_Script)

/* ------------------------------------------------------------------------ */
/* System */
#define CLASS_System            ((knh_class_t)37)
#define IS_System(o)            (O_cid(o) == CLASS_System)
#define IS_bSystem(o)           (O_bcid(o) == CLASS_System)
#define TYPE_System             CLASS_System
#define CFLAG_System            ((knh_flag_t)FLAG_Class_Singleton)
#define FLAG_System             KNH_MAGICFLAG(CFLAG_System)

/* ------------------------------------------------------------------------ */
/* Context */
#define CLASS_Context           ((knh_class_t)38)
#define IS_Context(o)           (O_cid(o) == CLASS_Context)
#define IS_bContext(o)          (O_bcid(o) == CLASS_Context)
#define TYPE_Context            CLASS_Context
#define CFLAG_Context           ((knh_flag_t)0)
#define FLAG_Context            KNH_MAGICFLAG(CFLAG_Context)

/* ------------------------------------------------------------------------ */
/* Monitor */
#define CLASS_Monitor           ((knh_class_t)39)
#define IS_Monitor(o)           (O_cid(o) == CLASS_Monitor)
#define IS_bMonitor(o)          (O_bcid(o) == CLASS_Monitor)
#define TYPE_Monitor            CLASS_Monitor
#define CFLAG_Monitor           ((knh_flag_t)0)
#define FLAG_Monitor            KNH_MAGICFLAG(CFLAG_Monitor)

/* ------------------------------------------------------------------------ */
/* Assurance */
#define CLASS_Assurance         ((knh_class_t)40)
#define IS_Assurance(o)         (O_cid(o) == CLASS_Assurance)
#define IS_bAssurance(o)        (O_bcid(o) == CLASS_Assurance)
#define TYPE_Assurance          CLASS_Assurance
#define CFLAG_Assurance         ((knh_flag_t)0)
#define FLAG_Assurance          KNH_MAGICFLAG(CFLAG_Assurance)

/* ------------------------------------------------------------------------ */
/* Token */
#define CLASS_Token             ((knh_class_t)41)
#define IS_Token(o)             (O_cid(o) == CLASS_Token)
#define IS_bToken(o)            (O_bcid(o) == CLASS_Token)
#define TYPE_Token              CLASS_Token
#define CFLAG_Token             ((knh_flag_t)0)
#define FLAG_Token              KNH_MAGICFLAG(CFLAG_Token)

/* ------------------------------------------------------------------------ */
/* Stmt */
#define CLASS_Stmt              ((knh_class_t)42)
#define IS_Stmt(o)              (O_cid(o) == CLASS_Stmt)
#define IS_bStmt(o)             (O_bcid(o) == CLASS_Stmt)
#define TYPE_Stmt               CLASS_Stmt
#define CFLAG_Stmt              ((knh_flag_t)0)
#define FLAG_Stmt               KNH_MAGICFLAG(CFLAG_Stmt)

/* ------------------------------------------------------------------------ */
/* Gamma */
#define CLASS_Gamma             ((knh_class_t)43)
#define IS_Gamma(o)             (O_cid(o) == CLASS_Gamma)
#define IS_bGamma(o)            (O_bcid(o) == CLASS_Gamma)
#define TYPE_Gamma              CLASS_Gamma
#define CFLAG_Gamma             ((knh_flag_t)FLAG_Class_Private)
#define FLAG_Gamma              KNH_MAGICFLAG(CFLAG_Gamma)

/* ------------------------------------------------------------------------ */
/* BasicBlock */
#define CLASS_BasicBlock        ((knh_class_t)44)
#define IS_BasicBlock(o)        (O_cid(o) == CLASS_BasicBlock)
#define IS_bBasicBlock(o)       (O_bcid(o) == CLASS_BasicBlock)
#define TYPE_BasicBlock         CLASS_BasicBlock
#define CFLAG_BasicBlock        ((knh_flag_t)0)
#define FLAG_BasicBlock         KNH_MAGICFLAG(CFLAG_BasicBlock)

/* ------------------------------------------------------------------------ */
/* KonohaCode */
#define CLASS_KonohaCode        ((knh_class_t)45)
#define IS_KonohaCode(o)        (O_cid(o) == CLASS_KonohaCode)
#define IS_bKonohaCode(o)       (O_bcid(o) == CLASS_KonohaCode)
#define TYPE_KonohaCode         CLASS_KonohaCode
#define CFLAG_KonohaCode        ((knh_flag_t)0)
#define FLAG_KonohaCode         KNH_MAGICFLAG(CFLAG_KonohaCode)

/* ------------------------------------------------------------------------ */
/* Immutable */
#define CLASS_Immutable         ((knh_class_t)46)
#define IS_Immutable(o)         (O_cid(o) == CLASS_Immutable)
#define IS_bImmutable(o)        (O_bcid(o) == CLASS_Immutable)
#define TYPE_Immutable          CLASS_Immutable
#define CFLAG_Immutable         ((knh_flag_t)FLAG_Class_Immutable)
#define FLAG_Immutable          KNH_MAGICFLAG(CFLAG_Immutable)

/* ------------------------------------------------------------------------ */
/* KindOf */
#define CLASS_KindOf            ((knh_class_t)47)
#define IS_KindOf(o)            (O_cid(o) == CLASS_KindOf)
#define IS_bKindOf(o)           (O_bcid(o) == CLASS_KindOf)
#define TYPE_KindOf             CLASS_KindOf
#define CFLAG_KindOf            ((knh_flag_t)0)
#define FLAG_KindOf             KNH_MAGICFLAG(CFLAG_KindOf)

/* ------------------------------------------------------------------------ */
/* Tdynamic */
#define CLASS_Tdynamic          ((knh_class_t)48)
#define IS_Tdynamic(o)          (O_cid(o) == CLASS_Tdynamic)
#define TYPE_Tdynamic           CLASS_Tdynamic
#define CFLAG_Tdynamic          ((knh_flag_t)0)
#define FLAG_Tdynamic           KNH_MAGICFLAG(CFLAG_Tdynamic)

/* ------------------------------------------------------------------------ */
/* CLASS */

/* ------------------------------------------------------------------------ */
/* CLASS */

/* ------------------------------------------------------------------------ */
/* RangeInt */
#define CLASS_RangeInt          ((knh_class_t)49)
#define IS_RangeInt(o)          (O_cid(o) == CLASS_RangeInt)
#define TYPE_RangeInt           CLASS_RangeInt

/* ------------------------------------------------------------------------ */
/* CmprT1 */
#define CLASS_CmprT1            ((knh_class_t)50)
#define IS_CmprT1(o)            (O_cid(o) == CLASS_CmprT1)
#define TYPE_CmprT1             CLASS_CmprT1

/* ------------------------------------------------------------------------ */
/* T1ITR */
#define CLASS_T1ITR             ((knh_class_t)51)
#define IS_T1ITR(o)             (O_cid(o) == CLASS_T1ITR)
#define TYPE_T1ITR              CLASS_T1ITR

/* ------------------------------------------------------------------------ */
/* T1ARRAY */
#define CLASS_T1ARRAY           ((knh_class_t)52)
#define IS_T1ARRAY(o)           (O_cid(o) == CLASS_T1ARRAY)
#define TYPE_T1ARRAY            CLASS_T1ARRAY

/* ------------------------------------------------------------------------ */
/* StringARRAY */
#define CLASS_StringARRAY       ((knh_class_t)53)
#define IS_StringARRAY(o)       (O_cid(o) == CLASS_StringARRAY)
#define TYPE_StringARRAY        CLASS_StringARRAY

/* ------------------------------------------------------------------------ */
/* StringITR */
#define CLASS_StringITR         ((knh_class_t)54)
#define IS_StringITR(o)         (O_cid(o) == CLASS_StringITR)
#define TYPE_StringITR          CLASS_StringITR
#define K_CLASS_INITSIZE                56

/* ------------------------------------------------------------------------ */
/* FLAG */

#define FLAG_Object_Ref ((knh_uintptr_t)(1<<0))
#define Object_hasRef(o)  (TFLAG_is(knh_uintptr_t,(o)->h.magicflag,FLAG_Object_Ref))
#define Object_setRef(o,b) TFLAG_set(knh_uintptr_t,(o)->h.magicflag,FLAG_Object_Ref,b)
#define FLAG_Object_NullObject ((knh_uintptr_t)(1<<1))
#define Object_isNullObject(o)  (TFLAG_is(knh_uintptr_t,(o)->h.magicflag,FLAG_Object_NullObject))
#define Object_setNullObject(o,b) TFLAG_set(knh_uintptr_t,(o)->h.magicflag,FLAG_Object_NullObject,b)
#define FLAG_Object_Classified ((knh_uintptr_t)(1<<2))
#define Object_isClassified(o)  (TFLAG_is(knh_uintptr_t,(o)->h.magicflag,FLAG_Object_Classified))
#define FLAG_Object_Modified ((knh_uintptr_t)(1<<3))
#define Object_isModified(o)  (TFLAG_is(knh_uintptr_t,(o)->h.magicflag,FLAG_Object_Modified))
#define Object_setModified(o,b) TFLAG_set(knh_uintptr_t,(o)->h.magicflag,FLAG_Object_Modified,b)
#define FLAG_Object_Local1 ((knh_uintptr_t)(1<<12))
#define Object_isLocal1(o)  (TFLAG_is(knh_uintptr_t,(o)->h.magicflag,FLAG_Object_Local1))
#define Object_setLocal1(o,b) TFLAG_set(knh_uintptr_t,(o)->h.magicflag,FLAG_Object_Local1,b)
#define FLAG_Object_Local2 ((knh_uintptr_t)(1<<13))
#define Object_isLocal2(o)  (TFLAG_is(knh_uintptr_t,(o)->h.magicflag,FLAG_Object_Local2))
#define Object_setLocal2(o,b) TFLAG_set(knh_uintptr_t,(o)->h.magicflag,FLAG_Object_Local2,b)
#define FLAG_Object_Local3 ((knh_uintptr_t)(1<<14))
#define Object_isLocal3(o)  (TFLAG_is(knh_uintptr_t,(o)->h.magicflag,FLAG_Object_Local3))
#define Object_setLocal3(o,b) TFLAG_set(knh_uintptr_t,(o)->h.magicflag,FLAG_Object_Local3,b)
#define FLAG_Object_Local4 ((knh_uintptr_t)(1<<15))
#define Object_isLocal4(o)  (TFLAG_is(knh_uintptr_t,(o)->h.magicflag,FLAG_Object_Local4))
#define Object_setLocal4(o,b) TFLAG_set(knh_uintptr_t,(o)->h.magicflag,FLAG_Object_Local4,b)
#define FLAG_String_TextSgm FLAG_Object_Local1
#define String_isTextSgm(o)  (TFLAG_is(knh_uintptr_t,(o)->h.magicflag,FLAG_String_TextSgm))
#define String_setTextSgm(o,b) TFLAG_set(knh_uintptr_t,(o)->h.magicflag,FLAG_String_TextSgm,b)
#define FLAG_String_ASCII FLAG_Object_Local2
#define String_isASCII(o)  (TFLAG_is(knh_uintptr_t,(o)->h.magicflag,FLAG_String_ASCII))
#define String_setASCII(o,b) TFLAG_set(knh_uintptr_t,(o)->h.magicflag,FLAG_String_ASCII,b)
#define FLAG_Range_NDATA FLAG_Object_Local1
#define Range_isNDATA(o)  (TFLAG_is(knh_uintptr_t,(o)->h.magicflag,FLAG_Range_NDATA))
#define Range_setNDATA(o,b) TFLAG_set(knh_uintptr_t,(o)->h.magicflag,FLAG_Range_NDATA,b)
#define FLAG_Range_Inclusive FLAG_Object_Local2
#define Range_isInclusive(o)  (TFLAG_is(knh_uintptr_t,(o)->h.magicflag,FLAG_Range_Inclusive))
#define Range_setInclusive(o,b) TFLAG_set(knh_uintptr_t,(o)->h.magicflag,FLAG_Range_Inclusive,b)
#define FLAG_Array_NDATA FLAG_Object_Local1
#define Array_isNDATA(o)  (TFLAG_is(knh_uintptr_t,(o)->h.magicflag,FLAG_Array_NDATA))
#define Array_setNDATA(o,b) TFLAG_set(knh_uintptr_t,(o)->h.magicflag,FLAG_Array_NDATA,b)
#define FLAG_ArrayIm_NDATA FLAG_Object_Local1
#define ArrayIm_isNDATA(o)  (TFLAG_is(knh_uintptr_t,(o)->h.magicflag,FLAG_ArrayIm_NDATA))
#define ArrayIm_setNDATA(o,b) TFLAG_set(knh_uintptr_t,(o)->h.magicflag,FLAG_ArrayIm_NDATA,b)
#define FLAG_Class_Ref ((knh_flag_t)(1<<0))
#define class_isRef(o)  (TFLAG_is(knh_flag_t,(ClassTBL(o))->cflag,FLAG_Class_Ref))
#define FLAG_Class_Immutable ((knh_flag_t)(1<<2))
#define class_isImmutable(o)  (TFLAG_is(knh_flag_t,(ClassTBL(o))->cflag,FLAG_Class_Immutable))
#define FLAG_Class_Expando ((knh_flag_t)(1<<3))
#define class_isExpando(o)  (TFLAG_is(knh_flag_t,(ClassTBL(o))->cflag,FLAG_Class_Expando))
#define FLAG_Class_Private ((knh_flag_t)(1<<4))
#define class_isPrivate(o)  (TFLAG_is(knh_flag_t,(ClassTBL(o))->cflag,FLAG_Class_Private))
#define class_isPublic(o)  (!TFLAG_is(knh_flag_t,(ClassTBL(o))->cflag,FLAG_Class_Private))
#define FLAG_Class_Final ((knh_flag_t)(1<<5))
#define class_isFinal(o)  (TFLAG_is(knh_flag_t,(ClassTBL(o))->cflag,FLAG_Class_Final))
#define FLAG_Class_Singleton ((knh_flag_t)(1<<6))
#define class_isSingleton(o)  (TFLAG_is(knh_flag_t,(ClassTBL(o))->cflag,FLAG_Class_Singleton))
#define FLAG_Class_Unique ((knh_flag_t)(1<<7))
#define class_isUnique(o)  (TFLAG_is(knh_flag_t,(ClassTBL(o))->cflag,FLAG_Class_Unique))
#define FLAG_Class_Interface ((knh_flag_t)(1<<8))
#define class_isInterface(o)  (TFLAG_is(knh_flag_t,(ClassTBL(o))->cflag,FLAG_Class_Interface))
#define FLAG_Class_TypeVariable ((knh_flag_t)(1<<9))
#define class_isTypeVariable(o)  (TFLAG_is(knh_flag_t,(ClassTBL(o))->cflag,FLAG_Class_TypeVariable))
#define class_setTypeVariable(o,b) TFLAG_set(knh_flag_t,(ClassTBL(o))->cflag,FLAG_Class_TypeVariable,b)
#define FLAG_ParamArray_VARGs FLAG_Object_Local1
#define ParamArray_isVARGs(o)  (TFLAG_is(knh_uintptr_t,(o)->h.magicflag,FLAG_ParamArray_VARGs))
#define ParamArray_setVARGs(o,b) TFLAG_set(knh_uintptr_t,(o)->h.magicflag,FLAG_ParamArray_VARGs,b)
#define FLAG_ParamArray_RVAR FLAG_Object_Local2
#define ParamArray_isRVAR(o)  (TFLAG_is(knh_uintptr_t,(o)->h.magicflag,FLAG_ParamArray_RVAR))
#define ParamArray_setRVAR(o,b) TFLAG_set(knh_uintptr_t,(o)->h.magicflag,FLAG_ParamArray_RVAR,b)
#define FLAG_Method_Private ((knh_flag_t)(1<<0))
#define Method_isPrivate(o)  (TFLAG_is(knh_flag_t,DP(o)->flag,FLAG_Method_Private))
#define Method_isPublic(o)  (!TFLAG_is(knh_flag_t,DP(o)->flag,FLAG_Method_Private))
#define Method_setPrivate(o,b) TFLAG_set(knh_flag_t,DP(o)->flag,FLAG_Method_Private,b)
#define Method_setPublic(o,b)  TFLAG_set(knh_flag_t,DP(o)->flag,FLAG_Method_Private,(!(b)))
#define FLAG_Method_Virtual ((knh_flag_t)(1<<1))
#define Method_isVirtual(o)  (TFLAG_is(knh_flag_t,DP(o)->flag,FLAG_Method_Virtual))
#define Method_isFinal(o)  (!TFLAG_is(knh_flag_t,DP(o)->flag,FLAG_Method_Virtual))
#define Method_setVirtual(o,b) TFLAG_set(knh_flag_t,DP(o)->flag,FLAG_Method_Virtual,b)
#define Method_setFinal(o,b)  TFLAG_set(knh_flag_t,DP(o)->flag,FLAG_Method_Virtual,(!(b)))
#define FLAG_Method_Debug ((knh_flag_t)(1<<2))
#define Method_isDebug(o)  (TFLAG_is(knh_flag_t,DP(o)->flag,FLAG_Method_Debug))
#define Method_setDebug(o,b) TFLAG_set(knh_flag_t,DP(o)->flag,FLAG_Method_Debug,b)
#define FLAG_Method_Const ((knh_flag_t)(1<<3))
#define Method_isConst(o)  (TFLAG_is(knh_flag_t,DP(o)->flag,FLAG_Method_Const))
#define FLAG_Method_Static ((knh_flag_t)(1<<4))
#define Method_isStatic(o)  (TFLAG_is(knh_flag_t,DP(o)->flag,FLAG_Method_Static))
#define FLAG_Method_ObjectCode ((knh_flag_t)(1<<5))
#define Method_isObjectCode(o)  (TFLAG_is(knh_flag_t,DP(o)->flag,FLAG_Method_ObjectCode))
#define Method_setObjectCode(o,b) TFLAG_set(knh_flag_t,DP(o)->flag,FLAG_Method_ObjectCode,b)
#define FLAG_Method_Hidden ((knh_flag_t)(1<<6))
#define Method_isHidden(o)  (TFLAG_is(knh_flag_t,DP(o)->flag,FLAG_Method_Hidden))
#define Method_setHidden(o,b) TFLAG_set(knh_flag_t,DP(o)->flag,FLAG_Method_Hidden,b)
#define FLAG_Method_Dynamic ((knh_flag_t)(1<<7))
#define Method_isDynamic(o)  (TFLAG_is(knh_flag_t,DP(o)->flag,FLAG_Method_Dynamic))
#define Method_setDynamic(o,b) TFLAG_set(knh_flag_t,DP(o)->flag,FLAG_Method_Dynamic,b)
#define FLAG_Method_Immutable ((knh_flag_t)(1<<8))
#define Method_isImmutable(o)  (TFLAG_is(knh_flag_t,DP(o)->flag,FLAG_Method_Immutable))
#define Method_setImmutable(o,b) TFLAG_set(knh_flag_t,DP(o)->flag,FLAG_Method_Immutable,b)
#define FLAG_Method_Message ((knh_flag_t)(1<<9))
#define Method_isMessage(o)  (TFLAG_is(knh_flag_t,DP(o)->flag,FLAG_Method_Message))
#define Method_setMessage(o,b) TFLAG_set(knh_flag_t,DP(o)->flag,FLAG_Method_Message,b)
#define FLAG_Method_Controlled ((knh_flag_t)(1<<10))
#define Method_isControlled(o)  (TFLAG_is(knh_flag_t,DP(o)->flag,FLAG_Method_Controlled))
#define Method_setControlled(o,b) TFLAG_set(knh_flag_t,DP(o)->flag,FLAG_Method_Controlled,b)
#define FLAG_TypeMap_Interface FLAG_Object_Local1
#define TypeMap_isInterface(o)  (TFLAG_is(knh_uintptr_t,(o)->h.magicflag,FLAG_TypeMap_Interface))
#define TypeMap_setInterface(o,b) TFLAG_set(knh_uintptr_t,(o)->h.magicflag,FLAG_TypeMap_Interface,b)
#define FLAG_TypeMap_Semantic FLAG_Object_Local2
#define TypeMap_isSemantic(o)  (TFLAG_is(knh_uintptr_t,(o)->h.magicflag,FLAG_TypeMap_Semantic))
#define TypeMap_setSemantic(o,b) TFLAG_set(knh_uintptr_t,(o)->h.magicflag,FLAG_TypeMap_Semantic,b)
#define FLAG_TypeMap_Const FLAG_Object_Local3
#define TypeMap_isConst(o)  (TFLAG_is(knh_uintptr_t,(o)->h.magicflag,FLAG_TypeMap_Const))
#define TypeMap_setConst(o,b) TFLAG_set(knh_uintptr_t,(o)->h.magicflag,FLAG_TypeMap_Const,b)
#define FLAG_Func_StoredEnv FLAG_Object_Local1
#define Func_isStoredEnv(o)  (TFLAG_is(knh_uintptr_t,(o)->h.magicflag,FLAG_Func_StoredEnv))
#define Func_setStoredEnv(o,b) TFLAG_set(knh_uintptr_t,(o)->h.magicflag,FLAG_Func_StoredEnv,b)
#define FLAG_Thunk_Evaluated FLAG_Object_Local1
#define Thunk_isEvaluated(o)  (TFLAG_is(knh_uintptr_t,(o)->h.magicflag,FLAG_Thunk_Evaluated))
#define Thunk_setEvaluated(o,b) TFLAG_set(knh_uintptr_t,(o)->h.magicflag,FLAG_Thunk_Evaluated,b)
#define FLAG_ExceptionHandler_Catching FLAG_Object_Local1
#define ExceptionHandler_isCatching(o)  (TFLAG_is(knh_uintptr_t,(o)->h.magicflag,FLAG_ExceptionHandler_Catching))
#define ExceptionHandler_setCatching(o,b) TFLAG_set(knh_uintptr_t,(o)->h.magicflag,FLAG_ExceptionHandler_Catching,b)
#define FLAG_OutputStream_BOL FLAG_Object_Local1
#define OutputStream_isBOL(o)  (TFLAG_is(knh_uintptr_t,(o)->h.magicflag,FLAG_OutputStream_BOL))
#define OutputStream_setBOL(o,b) TFLAG_set(knh_uintptr_t,(o)->h.magicflag,FLAG_OutputStream_BOL,b)
#define FLAG_OutputStream_AutoFlush FLAG_Object_Local2
#define OutputStream_isAutoFlush(o)  (TFLAG_is(knh_uintptr_t,(o)->h.magicflag,FLAG_OutputStream_AutoFlush))
#define OutputStream_setAutoFlush(o,b) TFLAG_set(knh_uintptr_t,(o)->h.magicflag,FLAG_OutputStream_AutoFlush,b)
#define FLAG_OutputStream_UTF8 FLAG_Object_Local3
#define OutputStream_hasUTF8(o)  (TFLAG_is(knh_uintptr_t,(o)->h.magicflag,FLAG_OutputStream_UTF8))
#define OutputStream_setUTF8(o,b) TFLAG_set(knh_uintptr_t,(o)->h.magicflag,FLAG_OutputStream_UTF8,b)
#define FLAG_Context_Debug ((knh_flag_t)(1<<1))
#define CTX_isDebug(o)  (TFLAG_is(knh_flag_t,((knh_context_t*)o)->flag,FLAG_Context_Debug))
#define CTX_setDebug(o,b) TFLAG_set(knh_flag_t,((knh_context_t*)o)->flag,FLAG_Context_Debug,b)
#define FLAG_Context_CompileOnly ((knh_flag_t)(1<<2))
#define CTX_isCompileOnly(o)  (TFLAG_is(knh_flag_t,((knh_context_t*)o)->flag,FLAG_Context_CompileOnly))
#define CTX_setCompileOnly(o,b) TFLAG_set(knh_flag_t,((knh_context_t*)o)->flag,FLAG_Context_CompileOnly,b)
#define FLAG_Context_Interactive ((knh_flag_t)(1<<3))
#define CTX_isInteractive(o)  (TFLAG_is(knh_flag_t,((knh_context_t*)o)->flag,FLAG_Context_Interactive))
#define CTX_setInteractive(o,b) TFLAG_set(knh_flag_t,((knh_context_t*)o)->flag,FLAG_Context_Interactive,b)
#define FLAG_Monitor_Throwable FLAG_Object_Local1
#define Monitor_isThrowable(o)  (TFLAG_is(knh_uintptr_t,(o)->h.magicflag,FLAG_Monitor_Throwable))
#define Monitor_setThrowable(o,b) TFLAG_set(knh_uintptr_t,(o)->h.magicflag,FLAG_Monitor_Throwable,b)
#define FLAG_Token_BOL ((knh_flag_t)(1<<0))
#define Token_isBOL(o)  (TFLAG_is(knh_flag_t,SP(o)->flag0,FLAG_Token_BOL))
#define Token_setBOL(o,b) TFLAG_set(knh_flag_t,SP(o)->flag0,FLAG_Token_BOL,b)
#define FLAG_Token_DOT ((knh_flag_t)(1<<1))
#define Token_isDOT(o)  (TFLAG_is(knh_flag_t,SP(o)->flag0,FLAG_Token_DOT))
#define Token_setDOT(o,b) TFLAG_set(knh_flag_t,SP(o)->flag0,FLAG_Token_DOT,b)
#define FLAG_Token_LCASE ((knh_flag_t)(1<<2))
#define Token_hasLCASE(o)  (TFLAG_is(knh_flag_t,SP(o)->flag0,FLAG_Token_LCASE))
#define Token_setLCASE(o,b) TFLAG_set(knh_flag_t,SP(o)->flag0,FLAG_Token_LCASE,b)
#define FLAG_Token_ISBOOL ((knh_flag_t)(1<<3))
#define Token_isISBOOL(o)  (TFLAG_is(knh_flag_t,SP(o)->flag0,FLAG_Token_ISBOOL))
#define Token_setISBOOL(o,b) TFLAG_set(knh_flag_t,SP(o)->flag0,FLAG_Token_ISBOOL,b)
#define FLAG_Token_Getter ((knh_flag_t)(1<<4))
#define Token_isGetter(o)  (TFLAG_is(knh_flag_t,SP(o)->flag0,FLAG_Token_Getter))
#define Token_setGetter(o,b) TFLAG_set(knh_flag_t,SP(o)->flag0,FLAG_Token_Getter,b)
#define FLAG_Token_Setter ((knh_flag_t)(1<<5))
#define Token_isSetter(o)  (TFLAG_is(knh_flag_t,SP(o)->flag0,FLAG_Token_Setter))
#define Token_setSetter(o,b) TFLAG_set(knh_flag_t,SP(o)->flag0,FLAG_Token_Setter,b)
#define FLAG_Token_ExceptionType ((knh_flag_t)(1<<6))
#define Token_isExceptionType(o)  (TFLAG_is(knh_flag_t,SP(o)->flag0,FLAG_Token_ExceptionType))
#define Token_setExceptionType(o,b) TFLAG_set(knh_flag_t,SP(o)->flag0,FLAG_Token_ExceptionType,b)
#define FLAG_Token_Immutable ((knh_flag_t)(1<<7))
#define Token_isImmutable(o)  (TFLAG_is(knh_flag_t,SP(o)->flag0,FLAG_Token_Immutable))
#define Token_setImmutable(o,b) TFLAG_set(knh_flag_t,SP(o)->flag0,FLAG_Token_Immutable,b)
#define FLAG_Token_MEMO1 ((knh_flag_t)(1<<8))
#define Token_isMEMO1(o)  (TFLAG_is(knh_flag_t,SP(o)->flag0,FLAG_Token_MEMO1))
#define Token_setMEMO1(o,b) TFLAG_set(knh_flag_t,SP(o)->flag0,FLAG_Token_MEMO1,b)
#define FLAG_Stmt_STOPITR ((knh_flag_t)(1<<1))
#define Stmt_isSTOPITR(o)  (TFLAG_is(knh_flag_t,DP(o)->flag0,FLAG_Stmt_STOPITR))
#define Stmt_setSTOPITR(o,b) TFLAG_set(knh_flag_t,DP(o)->flag0,FLAG_Stmt_STOPITR,b)
#define FLAG_Stmt_CONST ((knh_flag_t)(1<<2))
#define Stmt_isCONST(o)  (TFLAG_is(knh_flag_t,DP(o)->flag0,FLAG_Stmt_CONST))
#define Stmt_setCONST(o,b) TFLAG_set(knh_flag_t,DP(o)->flag0,FLAG_Stmt_CONST,b)
#define FLAG_Stmt_Memo1 ((knh_flag_t)(1<<4))
#define Stmt_isMemo1(o)  (TFLAG_is(knh_flag_t,DP(o)->flag0,FLAG_Stmt_Memo1))
#define Stmt_setMemo1(o,b) TFLAG_set(knh_flag_t,DP(o)->flag0,FLAG_Stmt_Memo1,b)
#define FLAG_Stmt_Memo2 ((knh_flag_t)(1<<5))
#define Stmt_isMemo2(o)  (TFLAG_is(knh_flag_t,DP(o)->flag0,FLAG_Stmt_Memo2))
#define Stmt_setMemo2(o,b) TFLAG_set(knh_flag_t,DP(o)->flag0,FLAG_Stmt_Memo2,b)
#define FLAG_Gamma_Quiet ((knh_flag_t)(1<<1))
#define Gamma_isQuiet(o)  (TFLAG_is(knh_flag_t,DP(o)->flag,FLAG_Gamma_Quiet))
#define Gamma_setQuiet(o,b) TFLAG_set(knh_flag_t,DP(o)->flag,FLAG_Gamma_Quiet,b)
#define FLAG_Gamma_Throwable ((knh_flag_t)(1<<2))
#define Gamma_isThrowable(o)  (TFLAG_is(knh_flag_t,DP(o)->flag,FLAG_Gamma_Throwable))
#define Gamma_setThrowable(o,b) TFLAG_set(knh_flag_t,DP(o)->flag,FLAG_Gamma_Throwable,b)
#define FLAG_Gamma_PROCEED ((knh_flag_t)(1<<3))
#define Gamma_hasPROCEED(o)  (TFLAG_is(knh_flag_t,DP(o)->flag,FLAG_Gamma_PROCEED))
#define Gamma_foundPROCEED(o,b) TFLAG_set(knh_flag_t,DP(o)->flag,FLAG_Gamma_PROCEED,b)
#define FLAG_Gamma_REGISTER ((knh_flag_t)(1<<4))
#define Gamma_hasREGISTER(o)  (TFLAG_is(knh_flag_t,DP(o)->flag,FLAG_Gamma_REGISTER))
#define Gamma_foundREGISTER(o,b) TFLAG_set(knh_flag_t,DP(o)->flag,FLAG_Gamma_REGISTER,b)
#define FLAG_Gamma_YEILD ((knh_flag_t)(1<<5))
#define Gamma_hasYEILD(o)  (TFLAG_is(knh_flag_t,DP(o)->flag,FLAG_Gamma_YEILD))
#define Gamma_foundYEILD(o,b) TFLAG_set(knh_flag_t,DP(o)->flag,FLAG_Gamma_YEILD,b)
#define FLAG_Gamma_FIELD ((knh_flag_t)(1<<6))
#define Gamma_hasFIELD(o)  (TFLAG_is(knh_flag_t,DP(o)->flag,FLAG_Gamma_FIELD))
#define Gamma_foundFIELD(o,b) TFLAG_set(knh_flag_t,DP(o)->flag,FLAG_Gamma_FIELD,b)
#define FLAG_Gamma_XLOCAL ((knh_flag_t)(1<<7))
#define Gamma_hasXLOCAL(o)  (TFLAG_is(knh_flag_t,DP(o)->flag,FLAG_Gamma_XLOCAL))
#define Gamma_foundXLOCAL(o,b) TFLAG_set(knh_flag_t,DP(o)->flag,FLAG_Gamma_XLOCAL,b)
#define FLAG_Gamma_SCRIPT ((knh_flag_t)(1<<8))
#define Gamma_hasSCRIPT(o)  (TFLAG_is(knh_flag_t,DP(o)->flag,FLAG_Gamma_SCRIPT))
#define Gamma_foundSCRIPT(o,b) TFLAG_set(knh_flag_t,DP(o)->flag,FLAG_Gamma_SCRIPT,b)
#define FLAG_Gamma_InlineFunction ((knh_flag_t)(1<<0))
#define Gamma_isInlineFunction(o)  (TFLAG_is(knh_flag_t,DP(o)->cflag,FLAG_Gamma_InlineFunction))
#define Gamma_setInlineFunction(o,b) TFLAG_set(knh_flag_t,DP(o)->cflag,FLAG_Gamma_InlineFunction,b)
#define FLAG_Gamma_TailRecursion ((knh_flag_t)(1<<1))
#define Gamma_isTailRecursion(o)  (TFLAG_is(knh_flag_t,DP(o)->cflag,FLAG_Gamma_TailRecursion))
#define Gamma_setTailRecursion(o,b) TFLAG_set(knh_flag_t,DP(o)->cflag,FLAG_Gamma_TailRecursion,b)
#define FLAG_BasicBlock_Visited FLAG_Object_Local1
#define BasicBlock_isVisited(o)  (TFLAG_is(knh_uintptr_t,(o)->h.magicflag,FLAG_BasicBlock_Visited))
#define BasicBlock_setVisited(o,b) TFLAG_set(knh_uintptr_t,(o)->h.magicflag,FLAG_BasicBlock_Visited,b)
#define FLAG_BasicBlock_StackChecked FLAG_Object_Local2
#define BasicBlock_isStackChecked(o)  (TFLAG_is(knh_uintptr_t,(o)->h.magicflag,FLAG_BasicBlock_StackChecked))
#define BasicBlock_setStackChecked(o,b) TFLAG_set(knh_uintptr_t,(o)->h.magicflag,FLAG_BasicBlock_StackChecked,b)
#define FLAG_KonohaCode_NativeCompiled FLAG_Object_Local1
#define KonohaCode_isNativeCompiled(o)  (TFLAG_is(knh_uintptr_t,(o)->h.magicflag,FLAG_KonohaCode_NativeCompiled))
#define KonohaCode_setNativeCompiled(o,b) TFLAG_set(knh_uintptr_t,(o)->h.magicflag,FLAG_KonohaCode_NativeCompiled,b)
/* ------------------------------------------------------------------------ */
/* EXPT */
#define EBI_Exception           1
#define EBI_Fatal               2
#define EBI_System              3
#define EBI_Security            4
#define EBI_SourceCode          5
#define EBI_Type                6
#define EBI_Assertion           7

/* ------------------------------------------------------------------------ */
/* FIELDN */
#define FN_             (MN_OPSIZE+0)
#define FN_2            (MN_OPSIZE+1)
#define FN_3            (MN_OPSIZE+2)
#define FN_4            (MN_OPSIZE+3)
#define FN_abstract     (MN_OPSIZE+4)
#define FN_add          (MN_OPSIZE+5)
#define FN_all          (MN_OPSIZE+6)
#define FN_aSCII        (MN_OPSIZE+7)
#define FN_ascii        (MN_OPSIZE+7)
#define FN_autoFlush    (MN_OPSIZE+8)
#define FN_autoflush    (MN_OPSIZE+8)
#define FN_bag          (MN_OPSIZE+9)
#define FN_base         (MN_OPSIZE+10)
#define FN_bits         (MN_OPSIZE+11)
#define FN_buf          (MN_OPSIZE+12)
#define FN_c            (MN_OPSIZE+13)
#define FN_ch           (MN_OPSIZE+14)
#define FN_char         (MN_OPSIZE+15)
#define FN_charset      (MN_OPSIZE+16)
#define FN_class        (MN_OPSIZE+17)
#define FN_clear        (MN_OPSIZE+18)
#define FN_close        (MN_OPSIZE+19)
#define FN_closed       (MN_OPSIZE+20)
#define FN_cmd          (MN_OPSIZE+21)
#define FN_concat       (MN_OPSIZE+22)
#define FN_const        (MN_OPSIZE+23)
#define FN_convert      (MN_OPSIZE+24)
#define FN_copy         (MN_OPSIZE+25)
#define FN_d            (MN_OPSIZE+26)
#define FN_data         (MN_OPSIZE+27)
#define FN_debug        (MN_OPSIZE+28)
#define FN_decode       (MN_OPSIZE+29)
#define FN_defined      (MN_OPSIZE+30)
#define FN_delegate     (MN_OPSIZE+31)
#define FN_dump         (MN_OPSIZE+32)
#define FN_e            (MN_OPSIZE+33)
#define FN_empty        (MN_OPSIZE+34)
#define FN_enc          (MN_OPSIZE+35)
#define FN_encode       (MN_OPSIZE+36)
#define FN_end          (MN_OPSIZE+37)
#define FN_endsWith     (MN_OPSIZE+38)
#define FN_endswith     (MN_OPSIZE+38)
#define FN_equals       (MN_OPSIZE+39)
#define FN_err          (MN_OPSIZE+40)
#define FN_event        (MN_OPSIZE+41)
#define FN_exec         (MN_OPSIZE+42)
#define FN_f            (MN_OPSIZE+43)
#define FN_first        (MN_OPSIZE+44)
#define FN_flag         (MN_OPSIZE+45)
#define FN_float        (MN_OPSIZE+46)
#define FN_flush        (MN_OPSIZE+47)
#define FN_fmt          (MN_OPSIZE+48)
#define FN_format       (MN_OPSIZE+49)
#define FN_gc           (MN_OPSIZE+50)
#define FN_hashCode     (MN_OPSIZE+51)
#define FN_hashcode     (MN_OPSIZE+51)
#define FN_in           (MN_OPSIZE+52)
#define FN_indexOf      (MN_OPSIZE+53)
#define FN_indexof      (MN_OPSIZE+53)
#define FN_init         (MN_OPSIZE+54)
#define FN_initCapacity (MN_OPSIZE+55)
#define FN_initcapacity (MN_OPSIZE+55)
#define FN_insert       (MN_OPSIZE+56)
#define FN_int          (MN_OPSIZE+57)
#define FN_invoke       (MN_OPSIZE+58)
#define FN_it           (MN_OPSIZE+59)
#define FN_k            (MN_OPSIZE+60)
#define FN_key          (MN_OPSIZE+61)
#define FN_keys         (MN_OPSIZE+62)
#define FN_lastIndexOf  (MN_OPSIZE+63)
#define FN_lastindexof  (MN_OPSIZE+63)
#define FN_length       (MN_OPSIZE+64)
#define FN_listProperties (MN_OPSIZE+65)
#define FN_listproperties (MN_OPSIZE+65)
#define FN_m            (MN_OPSIZE+66)
#define FN_main         (MN_OPSIZE+67)
#define FN_match        (MN_OPSIZE+68)
#define FN_method       (MN_OPSIZE+69)
#define FN_mode         (MN_OPSIZE+70)
#define FN_mon          (MN_OPSIZE+71)
#define FN_msg          (MN_OPSIZE+72)
#define FN_n            (MN_OPSIZE+73)
#define FN_name         (MN_OPSIZE+74)
#define FN_new          (MN_OPSIZE+75)
#define FN_new__ARRAY   (MN_OPSIZE+76)
#define FN_new__array   (MN_OPSIZE+76)
#define FN_new__LIST    (MN_OPSIZE+77)
#define FN_new__list    (MN_OPSIZE+77)
#define FN_new__MAP     (MN_OPSIZE+78)
#define FN_new__map     (MN_OPSIZE+78)
#define FN_new__TUPLE   (MN_OPSIZE+79)
#define FN_new__tuple   (MN_OPSIZE+79)
#define FN_next         (MN_OPSIZE+80)
#define FN_notNull      (MN_OPSIZE+81)
#define FN_notnull      (MN_OPSIZE+81)
#define FN_ns           (MN_OPSIZE+82)
#define FN_null         (MN_OPSIZE+83)
#define FN_offset       (MN_OPSIZE+84)
#define FN_option       (MN_OPSIZE+85)
#define FN_out          (MN_OPSIZE+86)
#define FN_path         (MN_OPSIZE+87)
#define FN_pattern      (MN_OPSIZE+88)
#define FN_pop          (MN_OPSIZE+89)
#define FN_print        (MN_OPSIZE+90)
#define FN_println      (MN_OPSIZE+91)
#define FN_property     (MN_OPSIZE+92)
#define FN_putc         (MN_OPSIZE+93)
#define FN_query        (MN_OPSIZE+94)
#define FN_random       (MN_OPSIZE+95)
#define FN_randomSeed   (MN_OPSIZE+96)
#define FN_randomseed   (MN_OPSIZE+96)
#define FN_re           (MN_OPSIZE+97)
#define FN_readLine     (MN_OPSIZE+98)
#define FN_readline     (MN_OPSIZE+98)
#define FN_readObject   (MN_OPSIZE+99)
#define FN_readobject   (MN_OPSIZE+99)
#define FN_remove       (MN_OPSIZE+100)
#define FN_replace      (MN_OPSIZE+101)
#define FN_reqt         (MN_OPSIZE+102)
#define FN_reset        (MN_OPSIZE+103)
#define FN_reverse      (MN_OPSIZE+104)
#define FN_s            (MN_OPSIZE+105)
#define FN_scheme       (MN_OPSIZE+106)
#define FN_search       (MN_OPSIZE+107)
#define FN_second       (MN_OPSIZE+108)
#define FN_seed         (MN_OPSIZE+109)
#define FN_send         (MN_OPSIZE+110)
#define FN_shuffle      (MN_OPSIZE+111)
#define FN_size         (MN_OPSIZE+112)
#define FN_split        (MN_OPSIZE+113)
#define FN_start        (MN_OPSIZE+114)
#define FN_startsWith   (MN_OPSIZE+115)
#define FN_startswith   (MN_OPSIZE+115)
#define FN_string       (MN_OPSIZE+116)
#define FN_substring    (MN_OPSIZE+117)
#define FN_super        (MN_OPSIZE+118)
#define FN_swap         (MN_OPSIZE+119)
#define FN_third        (MN_OPSIZE+120)
#define FN_this         (MN_OPSIZE+121)
#define FN_time         (MN_OPSIZE+122)
#define FN_trim         (MN_OPSIZE+123)
#define FN_u            (MN_OPSIZE+124)
#define FN_urn          (MN_OPSIZE+125)
#define FN_v            (MN_OPSIZE+126)
#define FN_value        (MN_OPSIZE+127)
#define FN_vargs        (MN_OPSIZE+128)
#define FN_w            (MN_OPSIZE+129)
#define FN_write        (MN_OPSIZE+130)
#define FN_writeASCII   (MN_OPSIZE+131)
#define FN_writeascii   (MN_OPSIZE+131)
#define FN_writeData    (MN_OPSIZE+132)
#define FN_writedata    (MN_OPSIZE+132)
#define FN_writeObject  (MN_OPSIZE+133)
#define FN_writeobject  (MN_OPSIZE+133)
#define FN_x            (MN_OPSIZE+134)
#define FN_y            (MN_OPSIZE+135)
#define FN_z            (MN_OPSIZE+136)
#define K_TFIELD_SIZE   137

/* ------------------------------------------------------------------------ */
/* METHODN */
#define MN_exec         FN_exec
#define MN_setAutoFlush MN_toSETTER(FN_autoflush)
#define MN_clear        FN_clear
#define MN_replace      FN_replace
#define MN_gc           FN_gc
#define MN_setDebug     MN_toSETTER(FN_debug)
#define MN_getErr       MN_toGETTER(FN_err)
#define MN_readLine     FN_readline
#define MN_random       FN_random
#define MN_setProperty  MN_toSETTER(FN_property)
#define MN_add          FN_add
#define MN_path         FN_path
#define MN_isNotNull    MN_toISBOOL(FN_notnull)
#define MN_isASCII      MN_toISBOOL(FN_ascii)
#define MN_writeData    FN_writedata
#define MN__empty       MN_toFMT(FN_empty)
#define MN_split        FN_split
#define MN__bits        MN_toFMT(FN_bits)
#define MN_reset        FN_reset
#define MN_isNull       MN_toISBOOL(FN_null)
#define MN_getName      MN_toGETTER(FN_name)
#define MN_get4         MN_toGETTER(FN_4)
#define MN_get3         MN_toGETTER(FN_3)
#define MN_get2         MN_toGETTER(FN_2)
#define MN_flush        FN_flush
#define MN_send         FN_send
#define MN_getChar      MN_toGETTER(FN_char)
#define MN_getSize      MN_toGETTER(FN_size)
#define MN_next         FN_next
#define MN_main         FN_main
#define MN_pop          FN_pop
#define MN_indexOf      FN_indexof
#define MN_super        FN_super
#define MN_getKey       MN_toGETTER(FN_key)
#define MN_insert       FN_insert
#define MN_writeObject  FN_writeobject
#define MN_putc         FN_putc
#define MN_isAutoFlush  MN_toISBOOL(FN_autoflush)
#define MN__x           MN_toFMT(FN_x)
#define MN_             FN_
#define MN_copy         FN_copy
#define MN_swap         FN_swap
#define MN__s           MN_toFMT(FN_s)
#define MN_new__LIST    FN_new__list
#define MN__k           MN_toFMT(FN_k)
#define MN__d           MN_toFMT(FN_d)
#define MN__f           MN_toFMT(FN_f)
#define MN_getOut       MN_toGETTER(FN_out)
#define MN__c           MN_toFMT(FN_c)
#define MN_getTime      MN_toGETTER(FN_time)
#define MN__data        MN_toFMT(FN_data)
#define MN_getIn        MN_toGETTER(FN_in)
#define MN_close        FN_close
#define MN_setCharset   MN_toSETTER(FN_charset)
#define MN_setOut       MN_toSETTER(FN_out)
#define MN_listProperties FN_listproperties
#define MN_substring    FN_substring
#define MN_getFloat     MN_toGETTER(FN_float)
#define MN_first        FN_first
#define MN_match        FN_match
#define MN_defined      FN_defined
#define MN_getProperty  MN_toGETTER(FN_property)
#define MN_setIn        MN_toSETTER(FN_in)
#define MN_isDebug      MN_toISBOOL(FN_debug)
#define MN_concat       FN_concat
#define MN_readObject   FN_readobject
#define MN_setErr       MN_toSETTER(FN_err)
#define MN__dump        MN_toFMT(FN_dump)
#define MN_trim         FN_trim
#define MN_invoke       FN_invoke
#define MN_new__ARRAY   FN_new__array
#define MN_lastIndexOf  FN_lastindexof
#define MN_isClosed     MN_toISBOOL(FN_closed)
#define MN_setAll       MN_toSETTER(FN_all)
#define MN_second       FN_second
#define MN_write        FN_write
#define MN_isAbstract   MN_toISBOOL(FN_abstract)
#define MN_reverse      FN_reverse
#define MN_setConst     MN_toSETTER(FN_const)
#define MN_new__MAP     FN_new__map
#define MN_getClass     MN_toGETTER(FN_class)
#define MN_query        FN_query
#define MN_println      FN_println
#define MN_delegate     FN_delegate
#define MN_third        FN_third
#define MN_convert      FN_convert
#define MN_search       FN_search
#define MN_get          MN_toGETTER(FN_)
#define MN_new__TUPLE   FN_new__tuple
#define MN_print        FN_print
#define MN_endsWith     FN_endswith
#define MN_keys         FN_keys
#define MN_new          FN_new
#define MN_set          MN_toSETTER(FN_)
#define MN_shuffle      FN_shuffle
#define MN_remove       FN_remove
#define MN_this         FN_this
#define MN_encode       FN_encode
#define MN_hashCode     FN_hashcode
#define MN_equals       FN_equals
#define MN_writeASCII   FN_writeascii
#define MN_startsWith   FN_startswith
#define MN__u           MN_toFMT(FN_u)
#define MN_getInt       MN_toGETTER(FN_int)
#define MN_format       FN_format
#define MN_set3         MN_toSETTER(FN_3)
#define MN_set2         MN_toSETTER(FN_2)
#define MN_setRandomSeed MN_toSETTER(FN_randomseed)
#define MN_set4         MN_toSETTER(FN_4)
#define MN_getString    MN_toGETTER(FN_string)
#define MN_decode       FN_decode

/* ------------------------------------------------------------------------ */
/* TS */
#define TS_EMPTY        (ctx->share)->tString[0]
#define TS_TAB          (ctx->share)->tString[1]
#define TS_ONE          (ctx->share)->tString[2]
#define TS_EOL          (ctx->share)->tString[3]
#define TS_BEGIN        (ctx->share)->tString[4]
#define TS_END          (ctx->share)->tString[5]
#define TS_true         (ctx->share)->tString[6]
#define TS_false        (ctx->share)->tString[7]
#define TS_null         (ctx->share)->tString[8]
#define TS_void         (ctx->share)->tString[9]
#define TS_main         (ctx->share)->tString[10]
#define TS_ENCODING     (ctx->share)->tString[11]
#define TS_DEVNULL      (ctx->share)->tString[12]
#define TS_DEVSTDIN     (ctx->share)->tString[13]
#define TS_DEVSTDERR    (ctx->share)->tString[14]
#define TS_DEVSTDOUT    (ctx->share)->tString[15]
#define TS_ATlabel      (ctx->share)->tString[16]
#define TS_AssertionException (ctx->share)->tString[17]
#define TS_LP           (ctx->share)->tString[18]
#define TS_RP           (ctx->share)->tString[19]
#define TS_LB           (ctx->share)->tString[20]
#define TS_RB           (ctx->share)->tString[21]
#define TS_LS           (ctx->share)->tString[22]
#define TS_RS           (ctx->share)->tString[23]
#define K_TSTRING_SIZE  24
