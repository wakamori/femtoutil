#ifndef KONOHA_CODE_H_
#define KONOHA_CODE_H_
// THIS FILE WAS AUTOMATICALLY GENERATED

//#include "konoha_vm.h"


#define OPCODE_HALT ((knh_opcode_t)0)
typedef struct klr_HALT_t {
	KCODE_HEAD;
} klr_HALT_t;

#define OPCODE_THCODE ((knh_opcode_t)1)
typedef struct klr_THCODE_t {
	KCODE_HEAD;
	klr_Fth th;
	knh_uintptr_t uri;
} klr_THCODE_t;

#define OPCODE_ENTER ((knh_opcode_t)2)
typedef struct klr_ENTER_t {
	KCODE_HEAD;
} klr_ENTER_t;

#define OPCODE_VEXEC ((knh_opcode_t)3)
typedef struct klr_VEXEC_t {
	KCODE_HEAD;
} klr_VEXEC_t;

#define OPCODE_YIELD ((knh_opcode_t)4)
typedef struct klr_YIELD_t {
	KCODE_HEAD;
	knh_sfpidx_t n;
} klr_YIELD_t;

#define OPCODE_FUNCCALL ((knh_opcode_t)5)
typedef struct klr_FUNCCALL_t {
	KCODE_HEAD;
} klr_FUNCCALL_t;

#define OPCODE_TRY ((knh_opcode_t)6)
typedef struct klr_TRY_t {
	KCODE_HEAD;
	knh_opline_t  *jumppc;
	knh_ro_t hn;
} klr_TRY_t;

#define OPCODE_TRYEND ((knh_opcode_t)7)
typedef struct klr_TRYEND_t {
	KCODE_HEAD;
	knh_ro_t hn;
} klr_TRYEND_t;

#define OPCODE_THROW ((knh_opcode_t)8)
typedef struct klr_THROW_t {
	KCODE_HEAD;
	knh_sfpidx_t start;
} klr_THROW_t;

#define OPCODE_ASSERT ((knh_opcode_t)9)
typedef struct klr_ASSERT_t {
	KCODE_HEAD;
	knh_sfpidx_t start;
	knh_uintptr_t uline;
} klr_ASSERT_t;

#define OPCODE_CATCH ((knh_opcode_t)10)
typedef struct klr_CATCH_t {
	KCODE_HEAD;
	knh_opline_t  *jumppc;
	knh_ro_t en;
	knh_String_t* msg;
} klr_CATCH_t;

#define OPCODE_CHKIN ((knh_opcode_t)11)
typedef struct klr_CHKIN_t {
	KCODE_HEAD;
	knh_ro_t on;
	klr_Fcheckin checkin;
} klr_CHKIN_t;

#define OPCODE_CHKOUT ((knh_opcode_t)12)
typedef struct klr_CHKOUT_t {
	KCODE_HEAD;
	knh_ro_t on;
	klr_Fcheckout checkout;
} klr_CHKOUT_t;

#define OPCODE_ERROR ((knh_opcode_t)13)
typedef struct klr_ERROR_t {
	KCODE_HEAD;
	knh_sfpidx_t start;
	knh_String_t* msg;
} klr_ERROR_t;

#define OPCODE_P ((knh_opcode_t)14)
typedef struct klr_P_t {
	KCODE_HEAD;
	klr_Fprint print;
	knh_uintptr_t flag;
	knh_String_t* msg;
	knh_sfpidx_t n;
} klr_P_t;

#define OPCODE_PROBE ((knh_opcode_t)15)
typedef struct klr_PROBE_t {
	KCODE_HEAD;
	klr_Fprobe probe;
	knh_uintptr_t n;
} klr_PROBE_t;

#define OPCODE_EXIT ((knh_opcode_t)16)
typedef struct klr_EXIT_t {
	KCODE_HEAD;
} klr_EXIT_t;

#define OPCODE_NSET ((knh_opcode_t)17)
typedef struct klr_NSET_t {
	KCODE_HEAD;
	knh_rn_t a;
	knh_int_t n;
} klr_NSET_t;

#define OPCODE_NMOV ((knh_opcode_t)18)
typedef struct klr_NMOV_t {
	KCODE_HEAD;
	knh_rn_t a;
	knh_rn_t b;
} klr_NMOV_t;

#define OPCODE_NNMOV ((knh_opcode_t)19)
typedef struct klr_NNMOV_t {
	KCODE_HEAD;
	knh_rn_t a;
	knh_rn_t b;
	knh_rn_t c;
	knh_rn_t d;
} klr_NNMOV_t;

#define OPCODE_NSET2 ((knh_opcode_t)20)
typedef struct klr_NSET2_t {
	KCODE_HEAD;
	knh_rn_t a;
	knh_int_t n;
	knh_int_t n2;
} klr_NSET2_t;

#define OPCODE_NSET3 ((knh_opcode_t)21)
typedef struct klr_NSET3_t {
	KCODE_HEAD;
	knh_rn_t a;
	knh_uintptr_t n;
	knh_uintptr_t n2;
	knh_uintptr_t n3;
} klr_NSET3_t;

#define OPCODE_NSET4 ((knh_opcode_t)22)
typedef struct klr_NSET4_t {
	KCODE_HEAD;
	knh_rn_t a;
	knh_uintptr_t n;
	knh_uintptr_t n2;
	knh_uintptr_t n3;
	knh_uintptr_t n4;
} klr_NSET4_t;

#define OPCODE_NMOVx ((knh_opcode_t)23)
typedef struct klr_NMOVx_t {
	KCODE_HEAD;
	knh_rn_t a;
	knh_sfx_t b;
} klr_NMOVx_t;

#define OPCODE_XNSET ((knh_opcode_t)24)
typedef struct klr_XNSET_t {
	KCODE_HEAD;
	knh_sfx_t a;
	knh_int_t b;
} klr_XNSET_t;

#define OPCODE_XNMOV ((knh_opcode_t)25)
typedef struct klr_XNMOV_t {
	KCODE_HEAD;
	knh_sfx_t a;
	knh_rn_t b;
} klr_XNMOV_t;

#define OPCODE_XNMOVx ((knh_opcode_t)26)
typedef struct klr_XNMOVx_t {
	KCODE_HEAD;
	knh_sfx_t a;
	knh_sfx_t b;
} klr_XNMOVx_t;

#define OPCODE_bNUL ((knh_opcode_t)27)
typedef struct klr_bNUL_t {
	KCODE_HEAD;
	knh_rn_t c;
	knh_ro_t a;
} klr_bNUL_t;

#define OPCODE_bNN ((knh_opcode_t)28)
typedef struct klr_bNN_t {
	KCODE_HEAD;
	knh_rn_t c;
	knh_ro_t a;
} klr_bNN_t;

#define OPCODE_bNOT ((knh_opcode_t)29)
typedef struct klr_bNOT_t {
	KCODE_HEAD;
	knh_rn_t c;
	knh_rn_t a;
} klr_bNOT_t;

#define OPCODE_iINC ((knh_opcode_t)30)
typedef struct klr_iINC_t {
	KCODE_HEAD;
	knh_rn_t a;
} klr_iINC_t;

#define OPCODE_iDEC ((knh_opcode_t)31)
typedef struct klr_iDEC_t {
	KCODE_HEAD;
	knh_rn_t a;
} klr_iDEC_t;

#define OPCODE_iNEG ((knh_opcode_t)32)
typedef struct klr_iNEG_t {
	KCODE_HEAD;
	knh_rn_t c;
	knh_rn_t a;
} klr_iNEG_t;

#define OPCODE_fNEG ((knh_opcode_t)33)
typedef struct klr_fNEG_t {
	KCODE_HEAD;
	knh_rn_t c;
	knh_rn_t a;
} klr_fNEG_t;

#define OPCODE_iTR ((knh_opcode_t)34)
typedef struct klr_iTR_t {
	KCODE_HEAD;
	knh_rn_t c;
	knh_rn_t a;
	klr_Finttr inttr;
} klr_iTR_t;

#define OPCODE_fTR ((knh_opcode_t)35)
typedef struct klr_fTR_t {
	KCODE_HEAD;
	knh_rn_t c;
	knh_rn_t a;
	klr_Ffloattr floattr;
} klr_fTR_t;

#define OPCODE_iADD ((knh_opcode_t)36)
typedef struct klr_iADD_t {
	KCODE_HEAD;
	knh_rn_t c;
	knh_rn_t a;
	knh_rn_t b;
} klr_iADD_t;

#define OPCODE_iSUB ((knh_opcode_t)37)
typedef struct klr_iSUB_t {
	KCODE_HEAD;
	knh_rn_t c;
	knh_rn_t a;
	knh_rn_t b;
} klr_iSUB_t;

#define OPCODE_iMUL ((knh_opcode_t)38)
typedef struct klr_iMUL_t {
	KCODE_HEAD;
	knh_rn_t c;
	knh_rn_t a;
	knh_rn_t b;
} klr_iMUL_t;

#define OPCODE_iDIV ((knh_opcode_t)39)
typedef struct klr_iDIV_t {
	KCODE_HEAD;
	knh_rn_t c;
	knh_rn_t a;
	knh_rn_t b;
} klr_iDIV_t;

#define OPCODE_iMOD ((knh_opcode_t)40)
typedef struct klr_iMOD_t {
	KCODE_HEAD;
	knh_rn_t c;
	knh_rn_t a;
	knh_rn_t b;
} klr_iMOD_t;

#define OPCODE_iEQ ((knh_opcode_t)41)
typedef struct klr_iEQ_t {
	KCODE_HEAD;
	knh_rn_t c;
	knh_rn_t a;
	knh_rn_t b;
} klr_iEQ_t;

#define OPCODE_iNEQ ((knh_opcode_t)42)
typedef struct klr_iNEQ_t {
	KCODE_HEAD;
	knh_rn_t c;
	knh_rn_t a;
	knh_rn_t b;
} klr_iNEQ_t;

#define OPCODE_iLT ((knh_opcode_t)43)
typedef struct klr_iLT_t {
	KCODE_HEAD;
	knh_rn_t c;
	knh_rn_t a;
	knh_rn_t b;
} klr_iLT_t;

#define OPCODE_iLTE ((knh_opcode_t)44)
typedef struct klr_iLTE_t {
	KCODE_HEAD;
	knh_rn_t c;
	knh_rn_t a;
	knh_rn_t b;
} klr_iLTE_t;

#define OPCODE_iGT ((knh_opcode_t)45)
typedef struct klr_iGT_t {
	KCODE_HEAD;
	knh_rn_t c;
	knh_rn_t a;
	knh_rn_t b;
} klr_iGT_t;

#define OPCODE_iGTE ((knh_opcode_t)46)
typedef struct klr_iGTE_t {
	KCODE_HEAD;
	knh_rn_t c;
	knh_rn_t a;
	knh_rn_t b;
} klr_iGTE_t;

#define OPCODE_iAND ((knh_opcode_t)47)
typedef struct klr_iAND_t {
	KCODE_HEAD;
	knh_rn_t c;
	knh_rn_t a;
	knh_rn_t b;
} klr_iAND_t;

#define OPCODE_iOR ((knh_opcode_t)48)
typedef struct klr_iOR_t {
	KCODE_HEAD;
	knh_rn_t c;
	knh_rn_t a;
	knh_rn_t b;
} klr_iOR_t;

#define OPCODE_iXOR ((knh_opcode_t)49)
typedef struct klr_iXOR_t {
	KCODE_HEAD;
	knh_rn_t c;
	knh_rn_t a;
	knh_rn_t b;
} klr_iXOR_t;

#define OPCODE_iLSFT ((knh_opcode_t)50)
typedef struct klr_iLSFT_t {
	KCODE_HEAD;
	knh_rn_t c;
	knh_rn_t a;
	knh_rn_t b;
} klr_iLSFT_t;

#define OPCODE_iRSFT ((knh_opcode_t)51)
typedef struct klr_iRSFT_t {
	KCODE_HEAD;
	knh_rn_t c;
	knh_rn_t a;
	knh_rn_t n;
} klr_iRSFT_t;

#define OPCODE_iADDC ((knh_opcode_t)52)
typedef struct klr_iADDC_t {
	KCODE_HEAD;
	knh_rn_t c;
	knh_rn_t a;
	knh_int_t n;
} klr_iADDC_t;

#define OPCODE_iSUBC ((knh_opcode_t)53)
typedef struct klr_iSUBC_t {
	KCODE_HEAD;
	knh_rn_t c;
	knh_rn_t a;
	knh_int_t n;
} klr_iSUBC_t;

#define OPCODE_iMULC ((knh_opcode_t)54)
typedef struct klr_iMULC_t {
	KCODE_HEAD;
	knh_rn_t c;
	knh_rn_t a;
	knh_int_t n;
} klr_iMULC_t;

#define OPCODE_iDIVC ((knh_opcode_t)55)
typedef struct klr_iDIVC_t {
	KCODE_HEAD;
	knh_rn_t c;
	knh_rn_t a;
	knh_int_t n;
} klr_iDIVC_t;

#define OPCODE_iMODC ((knh_opcode_t)56)
typedef struct klr_iMODC_t {
	KCODE_HEAD;
	knh_rn_t c;
	knh_rn_t a;
	knh_int_t n;
} klr_iMODC_t;

#define OPCODE_iEQC ((knh_opcode_t)57)
typedef struct klr_iEQC_t {
	KCODE_HEAD;
	knh_rn_t c;
	knh_rn_t a;
	knh_int_t n;
} klr_iEQC_t;

#define OPCODE_iNEQC ((knh_opcode_t)58)
typedef struct klr_iNEQC_t {
	KCODE_HEAD;
	knh_rn_t c;
	knh_rn_t a;
	knh_int_t n;
} klr_iNEQC_t;

#define OPCODE_iLTC ((knh_opcode_t)59)
typedef struct klr_iLTC_t {
	KCODE_HEAD;
	knh_rn_t c;
	knh_rn_t a;
	knh_int_t n;
} klr_iLTC_t;

#define OPCODE_iLTEC ((knh_opcode_t)60)
typedef struct klr_iLTEC_t {
	KCODE_HEAD;
	knh_rn_t c;
	knh_rn_t a;
	knh_int_t n;
} klr_iLTEC_t;

#define OPCODE_iGTC ((knh_opcode_t)61)
typedef struct klr_iGTC_t {
	KCODE_HEAD;
	knh_rn_t c;
	knh_rn_t a;
	knh_int_t n;
} klr_iGTC_t;

#define OPCODE_iGTEC ((knh_opcode_t)62)
typedef struct klr_iGTEC_t {
	KCODE_HEAD;
	knh_rn_t c;
	knh_rn_t a;
	knh_int_t n;
} klr_iGTEC_t;

#define OPCODE_iANDC ((knh_opcode_t)63)
typedef struct klr_iANDC_t {
	KCODE_HEAD;
	knh_rn_t c;
	knh_rn_t a;
	knh_int_t n;
} klr_iANDC_t;

#define OPCODE_iORC ((knh_opcode_t)64)
typedef struct klr_iORC_t {
	KCODE_HEAD;
	knh_rn_t c;
	knh_rn_t a;
	knh_int_t n;
} klr_iORC_t;

#define OPCODE_iXORC ((knh_opcode_t)65)
typedef struct klr_iXORC_t {
	KCODE_HEAD;
	knh_rn_t c;
	knh_rn_t a;
	knh_int_t n;
} klr_iXORC_t;

#define OPCODE_iLSFTC ((knh_opcode_t)66)
typedef struct klr_iLSFTC_t {
	KCODE_HEAD;
	knh_rn_t c;
	knh_rn_t a;
	knh_int_t n;
} klr_iLSFTC_t;

#define OPCODE_iRSFTC ((knh_opcode_t)67)
typedef struct klr_iRSFTC_t {
	KCODE_HEAD;
	knh_rn_t c;
	knh_rn_t a;
	knh_int_t n;
} klr_iRSFTC_t;

#define OPCODE_fADD ((knh_opcode_t)68)
typedef struct klr_fADD_t {
	KCODE_HEAD;
	knh_rn_t c;
	knh_rn_t a;
	knh_rn_t b;
} klr_fADD_t;

#define OPCODE_fSUB ((knh_opcode_t)69)
typedef struct klr_fSUB_t {
	KCODE_HEAD;
	knh_rn_t c;
	knh_rn_t a;
	knh_rn_t b;
} klr_fSUB_t;

#define OPCODE_fMUL ((knh_opcode_t)70)
typedef struct klr_fMUL_t {
	KCODE_HEAD;
	knh_rn_t c;
	knh_rn_t a;
	knh_rn_t b;
} klr_fMUL_t;

#define OPCODE_fDIV ((knh_opcode_t)71)
typedef struct klr_fDIV_t {
	KCODE_HEAD;
	knh_rn_t c;
	knh_rn_t a;
	knh_rn_t b;
} klr_fDIV_t;

#define OPCODE_fEQ ((knh_opcode_t)72)
typedef struct klr_fEQ_t {
	KCODE_HEAD;
	knh_rn_t c;
	knh_rn_t a;
	knh_rn_t b;
} klr_fEQ_t;

#define OPCODE_fNEQ ((knh_opcode_t)73)
typedef struct klr_fNEQ_t {
	KCODE_HEAD;
	knh_rn_t c;
	knh_rn_t a;
	knh_rn_t b;
} klr_fNEQ_t;

#define OPCODE_fLT ((knh_opcode_t)74)
typedef struct klr_fLT_t {
	KCODE_HEAD;
	knh_rn_t c;
	knh_rn_t a;
	knh_rn_t b;
} klr_fLT_t;

#define OPCODE_fLTE ((knh_opcode_t)75)
typedef struct klr_fLTE_t {
	KCODE_HEAD;
	knh_rn_t c;
	knh_rn_t a;
	knh_rn_t b;
} klr_fLTE_t;

#define OPCODE_fGT ((knh_opcode_t)76)
typedef struct klr_fGT_t {
	KCODE_HEAD;
	knh_rn_t c;
	knh_rn_t a;
	knh_rn_t b;
} klr_fGT_t;

#define OPCODE_fGTE ((knh_opcode_t)77)
typedef struct klr_fGTE_t {
	KCODE_HEAD;
	knh_rn_t c;
	knh_rn_t a;
	knh_rn_t b;
} klr_fGTE_t;

#define OPCODE_fADDC ((knh_opcode_t)78)
typedef struct klr_fADDC_t {
	KCODE_HEAD;
	knh_rn_t c;
	knh_rn_t a;
	knh_float_t n;
} klr_fADDC_t;

#define OPCODE_fSUBC ((knh_opcode_t)79)
typedef struct klr_fSUBC_t {
	KCODE_HEAD;
	knh_rn_t c;
	knh_rn_t a;
	knh_float_t n;
} klr_fSUBC_t;

#define OPCODE_fMULC ((knh_opcode_t)80)
typedef struct klr_fMULC_t {
	KCODE_HEAD;
	knh_rn_t c;
	knh_rn_t a;
	knh_float_t n;
} klr_fMULC_t;

#define OPCODE_fDIVC ((knh_opcode_t)81)
typedef struct klr_fDIVC_t {
	KCODE_HEAD;
	knh_rn_t c;
	knh_rn_t a;
	knh_float_t n;
} klr_fDIVC_t;

#define OPCODE_fEQC ((knh_opcode_t)82)
typedef struct klr_fEQC_t {
	KCODE_HEAD;
	knh_rn_t c;
	knh_rn_t a;
	knh_float_t n;
} klr_fEQC_t;

#define OPCODE_fNEQC ((knh_opcode_t)83)
typedef struct klr_fNEQC_t {
	KCODE_HEAD;
	knh_rn_t c;
	knh_rn_t a;
	knh_float_t n;
} klr_fNEQC_t;

#define OPCODE_fLTC ((knh_opcode_t)84)
typedef struct klr_fLTC_t {
	KCODE_HEAD;
	knh_rn_t c;
	knh_rn_t a;
	knh_float_t n;
} klr_fLTC_t;

#define OPCODE_fLTEC ((knh_opcode_t)85)
typedef struct klr_fLTEC_t {
	KCODE_HEAD;
	knh_rn_t c;
	knh_rn_t a;
	knh_float_t n;
} klr_fLTEC_t;

#define OPCODE_fGTC ((knh_opcode_t)86)
typedef struct klr_fGTC_t {
	KCODE_HEAD;
	knh_rn_t c;
	knh_rn_t a;
	knh_float_t n;
} klr_fGTC_t;

#define OPCODE_fGTEC ((knh_opcode_t)87)
typedef struct klr_fGTEC_t {
	KCODE_HEAD;
	knh_rn_t c;
	knh_rn_t a;
	knh_float_t n;
} klr_fGTEC_t;

#define OPCODE_RCINC ((knh_opcode_t)88)
typedef struct klr_RCINC_t {
	KCODE_HEAD;
	knh_ro_t a;
} klr_RCINC_t;

#define OPCODE_RCDEC ((knh_opcode_t)89)
typedef struct klr_RCDEC_t {
	KCODE_HEAD;
	knh_ro_t a;
} klr_RCDEC_t;

#define OPCODE_OSET ((knh_opcode_t)90)
typedef struct klr_OSET_t {
	KCODE_HEAD;
	knh_ro_t a;
	knh_Object_t* o;
} klr_OSET_t;

#define OPCODE_OMOV ((knh_opcode_t)91)
typedef struct klr_OMOV_t {
	KCODE_HEAD;
	knh_ro_t a;
	knh_ro_t b;
} klr_OMOV_t;

#define OPCODE_OOMOV ((knh_opcode_t)92)
typedef struct klr_OOMOV_t {
	KCODE_HEAD;
	knh_ro_t a;
	knh_ro_t b;
	knh_ro_t c;
	knh_ro_t d;
} klr_OOMOV_t;

#define OPCODE_ONMOV ((knh_opcode_t)93)
typedef struct klr_ONMOV_t {
	KCODE_HEAD;
	knh_ro_t a;
	knh_ro_t b;
	knh_rn_t c;
	knh_rn_t d;
} klr_ONMOV_t;

#define OPCODE_OSET2 ((knh_opcode_t)94)
typedef struct klr_OSET2_t {
	KCODE_HEAD;
	knh_ro_t a;
	knh_Object_t* v;
	knh_Object_t* v2;
} klr_OSET2_t;

#define OPCODE_OSET3 ((knh_opcode_t)95)
typedef struct klr_OSET3_t {
	KCODE_HEAD;
	knh_ro_t a;
	knh_Object_t* v;
	knh_Object_t* v2;
	knh_Object_t* v3;
} klr_OSET3_t;

#define OPCODE_OSET4 ((knh_opcode_t)96)
typedef struct klr_OSET4_t {
	KCODE_HEAD;
	knh_ro_t a;
	knh_Object_t* v;
	knh_Object_t* v2;
	knh_Object_t* v3;
	knh_Object_t* v4;
} klr_OSET4_t;

#define OPCODE_RCINCx ((knh_opcode_t)97)
typedef struct klr_RCINCx_t {
	KCODE_HEAD;
	knh_sfx_t a;
} klr_RCINCx_t;

#define OPCODE_RCDECx ((knh_opcode_t)98)
typedef struct klr_RCDECx_t {
	KCODE_HEAD;
	knh_sfx_t a;
} klr_RCDECx_t;

#define OPCODE_OMOVx ((knh_opcode_t)99)
typedef struct klr_OMOVx_t {
	KCODE_HEAD;
	knh_ro_t a;
	knh_sfx_t b;
} klr_OMOVx_t;

#define OPCODE_XMOV ((knh_opcode_t)100)
typedef struct klr_XMOV_t {
	KCODE_HEAD;
	knh_sfx_t a;
	knh_ro_t b;
} klr_XMOV_t;

#define OPCODE_XOSET ((knh_opcode_t)101)
typedef struct klr_XOSET_t {
	KCODE_HEAD;
	knh_sfx_t a;
	knh_Object_t* b;
} klr_XOSET_t;

#define OPCODE_XMOVx ((knh_opcode_t)102)
typedef struct klr_XMOVx_t {
	KCODE_HEAD;
	knh_sfx_t a;
	knh_sfx_t b;
} klr_XMOVx_t;

#define OPCODE_CHKSTACK ((knh_opcode_t)103)
typedef struct klr_CHKSTACK_t {
	KCODE_HEAD;
	knh_sfpidx_t n;
} klr_CHKSTACK_t;

#define OPCODE_LOADMTD ((knh_opcode_t)104)
typedef struct klr_LOADMTD_t {
	KCODE_HEAD;
	knh_sfpidx_t thisidx;
	klr_Fmethod method;
	knh_Method_t* mtdNC;
} klr_LOADMTD_t;

#define OPCODE_CALL ((knh_opcode_t)105)
typedef struct klr_CALL_t {
	KCODE_HEAD;
	knh_r_t a;
	knh_sfpidx_t thisidx;
	knh_sfpidx_t espshift;
} klr_CALL_t;

#define OPCODE_SCALL ((knh_opcode_t)106)
typedef struct klr_SCALL_t {
	KCODE_HEAD;
	knh_r_t a;
	knh_sfpidx_t thisidx;
	knh_sfpidx_t espshift;
	knh_Method_t* mtdNC;
} klr_SCALL_t;

#define OPCODE_VCALL ((knh_opcode_t)107)
typedef struct klr_VCALL_t {
	KCODE_HEAD;
	knh_r_t a;
	knh_sfpidx_t thisidx;
	knh_sfpidx_t espshift;
	knh_Method_t* mtdNC;
} klr_VCALL_t;

#define OPCODE_VCALL_ ((knh_opcode_t)108)
typedef struct klr_VCALL__t {
	KCODE_HEAD;
	knh_r_t a;
	knh_sfpidx_t thisidx;
	knh_sfpidx_t espshift;
	knh_Method_t* mtdNC;
} klr_VCALL__t;

#define OPCODE_FASTCALL0 ((knh_opcode_t)109)
typedef struct klr_FASTCALL0_t {
	KCODE_HEAD;
	knh_r_t a;
	knh_sfpidx_t thisidx;
	knh_intptr_t rix;
	klr_Ffcall fcall;
	knh_Method_t* mtdNC;
} klr_FASTCALL0_t;

#define OPCODE_RET ((knh_opcode_t)110)
typedef struct klr_RET_t {
	KCODE_HEAD;
} klr_RET_t;

#define OPCODE_TR ((knh_opcode_t)111)
typedef struct klr_TR_t {
	KCODE_HEAD;
	knh_r_t a;
	knh_sfpidx_t b;
	knh_intptr_t rix;
	const knh_ClassTBL_t* cid;
	klr_Ftr tr;
} klr_TR_t;

#define OPCODE_UNBOX ((knh_opcode_t)112)
typedef struct klr_UNBOX_t {
	KCODE_HEAD;
	knh_rn_t a;
	knh_ro_t b;
	const knh_ClassTBL_t* cid;
} klr_UNBOX_t;

#define OPCODE_SCAST ((knh_opcode_t)113)
typedef struct klr_SCAST_t {
	KCODE_HEAD;
	knh_r_t a;
	knh_sfpidx_t b;
	knh_intptr_t rix;
	knh_TypeMap_t* cast;
} klr_SCAST_t;

#define OPCODE_TCAST ((knh_opcode_t)114)
typedef struct klr_TCAST_t {
	KCODE_HEAD;
	knh_r_t a;
	knh_sfpidx_t b;
	knh_intptr_t rix;
	knh_TypeMap_t* cast;
} klr_TCAST_t;

#define OPCODE_ACAST ((knh_opcode_t)115)
typedef struct klr_ACAST_t {
	KCODE_HEAD;
	knh_r_t a;
	knh_sfpidx_t b;
	knh_intptr_t rix;
	knh_TypeMap_t* cast;
} klr_ACAST_t;

#define OPCODE_iCAST ((knh_opcode_t)116)
typedef struct klr_iCAST_t {
	KCODE_HEAD;
	knh_rn_t a;
	knh_rn_t b;
} klr_iCAST_t;

#define OPCODE_fCAST ((knh_opcode_t)117)
typedef struct klr_fCAST_t {
	KCODE_HEAD;
	knh_rn_t a;
	knh_rn_t b;
} klr_fCAST_t;

#define OPCODE_JMP ((knh_opcode_t)118)
typedef struct klr_JMP_t {
	KCODE_HEAD;
	knh_opline_t  *jumppc;
} klr_JMP_t;

#define OPCODE_JMP_ ((knh_opcode_t)119)
typedef struct klr_JMP__t {
	KCODE_HEAD;
	knh_opline_t  *jumppc;
} klr_JMP__t;

#define OPCODE_JMPF ((knh_opcode_t)120)
typedef struct klr_JMPF_t {
	KCODE_HEAD;
	knh_opline_t  *jumppc;
	knh_rn_t a;
} klr_JMPF_t;

#define OPCODE_NEXT ((knh_opcode_t)121)
typedef struct klr_NEXT_t {
	KCODE_HEAD;
	knh_opline_t  *jumppc;
	knh_r_t a;
	knh_sfpidx_t b;
	knh_intptr_t rix;
} klr_NEXT_t;

#define OPCODE_BGETIDX ((knh_opcode_t)122)
typedef struct klr_BGETIDX_t {
	KCODE_HEAD;
	knh_rn_t c;
	knh_ro_t a;
	knh_rn_t n;
} klr_BGETIDX_t;

#define OPCODE_BSETIDX ((knh_opcode_t)123)
typedef struct klr_BSETIDX_t {
	KCODE_HEAD;
	knh_rn_t c;
	knh_ro_t a;
	knh_rn_t n;
	knh_rn_t v;
} klr_BSETIDX_t;

#define OPCODE_BGETIDXC ((knh_opcode_t)124)
typedef struct klr_BGETIDXC_t {
	KCODE_HEAD;
	knh_rn_t c;
	knh_ro_t a;
	knh_uintptr_t n;
} klr_BGETIDXC_t;

#define OPCODE_BSETIDXC ((knh_opcode_t)125)
typedef struct klr_BSETIDXC_t {
	KCODE_HEAD;
	knh_rn_t c;
	knh_ro_t a;
	knh_uintptr_t n;
	knh_rn_t v;
} klr_BSETIDXC_t;

#define OPCODE_NGETIDX ((knh_opcode_t)126)
typedef struct klr_NGETIDX_t {
	KCODE_HEAD;
	knh_rn_t c;
	knh_ro_t a;
	knh_rn_t n;
} klr_NGETIDX_t;

#define OPCODE_NSETIDX ((knh_opcode_t)127)
typedef struct klr_NSETIDX_t {
	KCODE_HEAD;
	knh_rn_t c;
	knh_ro_t a;
	knh_rn_t n;
	knh_rn_t v;
} klr_NSETIDX_t;

#define OPCODE_NGETIDXC ((knh_opcode_t)128)
typedef struct klr_NGETIDXC_t {
	KCODE_HEAD;
	knh_rn_t c;
	knh_ro_t a;
	knh_uintptr_t n;
} klr_NGETIDXC_t;

#define OPCODE_NSETIDXC ((knh_opcode_t)129)
typedef struct klr_NSETIDXC_t {
	KCODE_HEAD;
	knh_rn_t c;
	knh_ro_t a;
	knh_uintptr_t n;
	knh_rn_t v;
} klr_NSETIDXC_t;

#define OPCODE_OGETIDX ((knh_opcode_t)130)
typedef struct klr_OGETIDX_t {
	KCODE_HEAD;
	knh_ro_t c;
	knh_ro_t a;
	knh_rn_t n;
} klr_OGETIDX_t;

#define OPCODE_OSETIDX ((knh_opcode_t)131)
typedef struct klr_OSETIDX_t {
	KCODE_HEAD;
	knh_ro_t c;
	knh_ro_t a;
	knh_rn_t n;
	knh_ro_t v;
} klr_OSETIDX_t;

#define OPCODE_OGETIDXC ((knh_opcode_t)132)
typedef struct klr_OGETIDXC_t {
	KCODE_HEAD;
	knh_ro_t c;
	knh_ro_t a;
	knh_uintptr_t n;
} klr_OGETIDXC_t;

#define OPCODE_OSETIDXC ((knh_opcode_t)133)
typedef struct klr_OSETIDXC_t {
	KCODE_HEAD;
	knh_ro_t c;
	knh_ro_t a;
	knh_uintptr_t n;
	knh_ro_t v;
} klr_OSETIDXC_t;

#define OPCODE_bJNUL ((knh_opcode_t)134)
typedef struct klr_bJNUL_t {
	KCODE_HEAD;
	knh_opline_t  *jumppc;
	knh_ro_t a;
} klr_bJNUL_t;

#define OPCODE_bJNN ((knh_opcode_t)135)
typedef struct klr_bJNN_t {
	KCODE_HEAD;
	knh_opline_t  *jumppc;
	knh_ro_t a;
} klr_bJNN_t;

#define OPCODE_bJNOT ((knh_opcode_t)136)
typedef struct klr_bJNOT_t {
	KCODE_HEAD;
	knh_opline_t  *jumppc;
	knh_rn_t a;
} klr_bJNOT_t;

#define OPCODE_iJEQ ((knh_opcode_t)137)
typedef struct klr_iJEQ_t {
	KCODE_HEAD;
	knh_opline_t  *jumppc;
	knh_rn_t a;
	knh_rn_t b;
} klr_iJEQ_t;

#define OPCODE_iJNEQ ((knh_opcode_t)138)
typedef struct klr_iJNEQ_t {
	KCODE_HEAD;
	knh_opline_t  *jumppc;
	knh_rn_t a;
	knh_rn_t b;
} klr_iJNEQ_t;

#define OPCODE_iJLT ((knh_opcode_t)139)
typedef struct klr_iJLT_t {
	KCODE_HEAD;
	knh_opline_t  *jumppc;
	knh_rn_t a;
	knh_rn_t b;
} klr_iJLT_t;

#define OPCODE_iJLTE ((knh_opcode_t)140)
typedef struct klr_iJLTE_t {
	KCODE_HEAD;
	knh_opline_t  *jumppc;
	knh_rn_t a;
	knh_rn_t b;
} klr_iJLTE_t;

#define OPCODE_iJGT ((knh_opcode_t)141)
typedef struct klr_iJGT_t {
	KCODE_HEAD;
	knh_opline_t  *jumppc;
	knh_rn_t a;
	knh_rn_t b;
} klr_iJGT_t;

#define OPCODE_iJGTE ((knh_opcode_t)142)
typedef struct klr_iJGTE_t {
	KCODE_HEAD;
	knh_opline_t  *jumppc;
	knh_rn_t a;
	knh_rn_t b;
} klr_iJGTE_t;

#define OPCODE_iJEQC ((knh_opcode_t)143)
typedef struct klr_iJEQC_t {
	KCODE_HEAD;
	knh_opline_t  *jumppc;
	knh_rn_t a;
	knh_int_t n;
} klr_iJEQC_t;

#define OPCODE_iJNEQC ((knh_opcode_t)144)
typedef struct klr_iJNEQC_t {
	KCODE_HEAD;
	knh_opline_t  *jumppc;
	knh_rn_t a;
	knh_int_t n;
} klr_iJNEQC_t;

#define OPCODE_iJLTC ((knh_opcode_t)145)
typedef struct klr_iJLTC_t {
	KCODE_HEAD;
	knh_opline_t  *jumppc;
	knh_rn_t a;
	knh_int_t n;
} klr_iJLTC_t;

#define OPCODE_iJLTEC ((knh_opcode_t)146)
typedef struct klr_iJLTEC_t {
	KCODE_HEAD;
	knh_opline_t  *jumppc;
	knh_rn_t a;
	knh_int_t n;
} klr_iJLTEC_t;

#define OPCODE_iJGTC ((knh_opcode_t)147)
typedef struct klr_iJGTC_t {
	KCODE_HEAD;
	knh_opline_t  *jumppc;
	knh_rn_t a;
	knh_int_t n;
} klr_iJGTC_t;

#define OPCODE_iJGTEC ((knh_opcode_t)148)
typedef struct klr_iJGTEC_t {
	KCODE_HEAD;
	knh_opline_t  *jumppc;
	knh_rn_t a;
	knh_int_t n;
} klr_iJGTEC_t;

#define OPCODE_fJEQ ((knh_opcode_t)149)
typedef struct klr_fJEQ_t {
	KCODE_HEAD;
	knh_opline_t  *jumppc;
	knh_rn_t a;
	knh_rn_t b;
} klr_fJEQ_t;

#define OPCODE_fJNEQ ((knh_opcode_t)150)
typedef struct klr_fJNEQ_t {
	KCODE_HEAD;
	knh_opline_t  *jumppc;
	knh_rn_t a;
	knh_rn_t b;
} klr_fJNEQ_t;

#define OPCODE_fJLT ((knh_opcode_t)151)
typedef struct klr_fJLT_t {
	KCODE_HEAD;
	knh_opline_t  *jumppc;
	knh_rn_t a;
	knh_rn_t b;
} klr_fJLT_t;

#define OPCODE_fJLTE ((knh_opcode_t)152)
typedef struct klr_fJLTE_t {
	KCODE_HEAD;
	knh_opline_t  *jumppc;
	knh_rn_t a;
	knh_rn_t b;
} klr_fJLTE_t;

#define OPCODE_fJGT ((knh_opcode_t)153)
typedef struct klr_fJGT_t {
	KCODE_HEAD;
	knh_opline_t  *jumppc;
	knh_rn_t a;
	knh_rn_t b;
} klr_fJGT_t;

#define OPCODE_fJGTE ((knh_opcode_t)154)
typedef struct klr_fJGTE_t {
	KCODE_HEAD;
	knh_opline_t  *jumppc;
	knh_rn_t a;
	knh_rn_t b;
} klr_fJGTE_t;

#define OPCODE_fJEQC ((knh_opcode_t)155)
typedef struct klr_fJEQC_t {
	KCODE_HEAD;
	knh_opline_t  *jumppc;
	knh_rn_t a;
	knh_float_t n;
} klr_fJEQC_t;

#define OPCODE_fJNEQC ((knh_opcode_t)156)
typedef struct klr_fJNEQC_t {
	KCODE_HEAD;
	knh_opline_t  *jumppc;
	knh_rn_t a;
	knh_float_t n;
} klr_fJNEQC_t;

#define OPCODE_fJLTC ((knh_opcode_t)157)
typedef struct klr_fJLTC_t {
	KCODE_HEAD;
	knh_opline_t  *jumppc;
	knh_rn_t a;
	knh_float_t n;
} klr_fJLTC_t;

#define OPCODE_fJLTEC ((knh_opcode_t)158)
typedef struct klr_fJLTEC_t {
	KCODE_HEAD;
	knh_opline_t  *jumppc;
	knh_rn_t a;
	knh_float_t n;
} klr_fJLTEC_t;

#define OPCODE_fJGTC ((knh_opcode_t)159)
typedef struct klr_fJGTC_t {
	KCODE_HEAD;
	knh_opline_t  *jumppc;
	knh_rn_t a;
	knh_float_t n;
} klr_fJGTC_t;

#define OPCODE_fJGTEC ((knh_opcode_t)160)
typedef struct klr_fJGTEC_t {
	KCODE_HEAD;
	knh_opline_t  *jumppc;
	knh_rn_t a;
	knh_float_t n;
} klr_fJGTEC_t;

#define OPCODE_CHKIDX ((knh_opcode_t)161)
typedef struct klr_CHKIDX_t {
	KCODE_HEAD;
	knh_ro_t a;
	knh_rn_t n;
} klr_CHKIDX_t;

#define OPCODE_CHKIDXC ((knh_opcode_t)162)
typedef struct klr_CHKIDXC_t {
	KCODE_HEAD;
	knh_ro_t a;
	knh_uintptr_t n;
} klr_CHKIDXC_t;

#define OPCODE_NOP ((knh_opcode_t)163)
typedef struct klr_NOP_t {
	KCODE_HEAD;
} klr_NOP_t;

#define OPCODE_MAX ((knh_opcode_t)164)

#define VMT_VOID     0
#define VMT_ADDR     1
#define VMT_SFPIDX   2
#define VMT_R        3
#define VMT_RN       4
#define VMT_RO       5
#define VMT_SFPIDX2  6
#define VMT_SFX      7
#define VMT_U        8
#define VMT_I        9
#define VMT_F        10
#define VMT_CID      11
#define VMT_MTD      12
#define VMT_TMR      13
#define VMT_OBJECT   14
#define VMT_STRING   15
#define VMT_INT      16
#define VMT_FLOAT    17

#endif /* KONOHA_CODE_H_ */
