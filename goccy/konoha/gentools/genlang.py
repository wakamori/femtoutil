#!/usr/bin/python
import os, sys
from pygenlib2 import *

TSTMT = '''
DONE      _NOCHECK
BLOCK     _BLOCK
pragma    _NOCHECK
namespace _BLOCK
script    _BLOCK
defmacro  _NAME _PARAM _BLOCK
include   _NOCHECK
allow     _CNAME _MEXPR
deny      _CNAME _MEXPR
using     _NOCHECK
class     _CNAME _PARAM _CNAME _NOCHECK _
format    _NOCHECK
return    _EXPR
yield     _EXPR
if        _EXPR  _BLOCK _BLOCK
switch    _EXPR  _BLOCK  
case      _EXPR  _BLOCK
while     _EXPR  _BLOCK
do        _BLOCK _EXPR
for       _BLOCK _EXPR _BLOCK _BLOCK
foreach   _NOCHECK
break     _LABEL
continue  _LABEL
try       _BLOCK _CATCH
catch     _DECL  _BLOCK
throw     _EXPR
print     _MEXPR
assure    _EXPR  _BLOCK
assert    _EXPR
register  _MEXPR
function  _PARAM _BLOCK

METHOD    _TYPE  _CNAME _NAME _PARAM _BLOCK
DECL      _TYPE  _NAME  _EXPR
CHKOUT    _NOCHECK
LETM      _NOCHECK
SWAP      _NOCHECK
CONST     _NOCHECK
ERR       _ERR

# @expr: let - call1
LET         _      _NAME  _EXPR
TCAST       _CNAME _      _EXPR
OPR         _OP    _EXPR  _EXPR
CALL        _NAME  _EXPR  _MEXPR
NEW         _NAME  _CNAME _MEXPR
FUNCCALL    _EXPR  _      _MEXPR
ACALL       _NAME  _      _MEXPR
TSCHEME       _
ALT         _EXPR  _EXPR
TRI         _EXPR  _EXPR  _EXPR
AND         _MEXPR
OR          _MEXPR
W1          _STR _ _EXPR
SEND        _STR _ _MEXPR
FMTCALL     _W1  _ _MEXPR

CALL1       _EXPR
'''

PSTMT = '''
#keyword  FLAG  _PARSE
pragma    0     _FUTURE
namespace 0     _STMT
script    0     _STMT
include   0     _FILE
allow     0     _CNAME _MEXPR
deny      0     _CNAME _MEXPR
using     0     _FUTURE
class     0     _CLASS
return    0     _EXPR
yield     0     _EXPR
if        0     _PEXPR   _STMT _ELSE
switch    0     _PEXPR   _STMT
case      0     _EXPR    _CASESTMT
while     0     _PEXPR   _STMT
do        0     _STMT    _WHILE _PEXPR
for       0     _PSTMT3  _STMT
foreach   0     _PEACH   _STMT
break     0     _LABEL
continue  0     _LABEL
#goto     0     _FUTURE
try       0     _STMT _CATCH
catch     0     _PEXPR  _STMT
throw     0     _EXPR
print     0     _MEXPR
assure    0     _EXPR _STMT
assert    0     _EXPR
register  0     _MEXPR
go        0     _STMT
function  0     _FUNCTION
'''

TOKEN = '''
# name    TT_NAME    flag
# begin_stmt
pragma    -          0
namespace -          0
script    -          0
include   -          0
import    -          0
using     -          0
class     -          0
format    -          0
return    -          0
yield     -          0
if        -          0
switch    -          0
case      -          0
while     -          0
do        -          0
for       -          0
foreach   -          0
break     -          0
continue  -          0
goto      -          0
try       -          0
catch     -          0
throw     -          0
print     -          0
assure    -          0
assert    -          0
register  -          0
go        -          0
#end_stmt

extends    -         0
implements -         0
else       -         0
default    -         0
finally    -         0

;          TT_SEMICOLON  0
,          TT_COMMA      0
<>         TT_DIAMOND    0
->         TT_ARROW      0
=>         TT_DARROW     0
-->        TT_TARROW     0
==>        TT_TDARROW    0
!!         TT_EXPT       0
<<<        TT_LSEND      0
>>>        TT_RSEND      0
...        TT_DOTS       0
**         TT_DMUL       0

#beg_inexpr
_    TT_ASIS       0
function   _       0
new        -       0
null       -       0
#define isTYPE(tt) return (tt == TT_PTYPE || tt == TT_UNAME || (TT_TYPEOF <= tt && tt <= TT_DYNAMIC));
typeof     -       0
void       -       0
var        -       0
byte       -       0
dyn        -       0
true       -       0
false      -       0

{ TT_BRACE         0
( TT_PARENTHESIS   0
[ TT_BRANCET       0

#end_inexpr

:=    TT_DECLLET   0
<<=   TT_LSFTE     0    TT_LSFT
>>=   TT_RSFTE     0    TT_RSFT
+=    TT_ADDE      0    TT_ADD
-=    TT_SUBE      0    TT_SUB
/=    TT_DIVE      0    TT_DIV
%=    TT_MODE      0    TT_MOD
*=    TT_MULE      0    TT_MUL
&=    TT_LANDE     0    TT_LAND
|=    TT_LORE      0    TT_LOR
?\?=  TT_ALTLET    0    TT_ALT
'''

OPTOKEN = '''
#                       FLAG   PRI
=       TT_LET          0      2       MN_NONAME
#<<<     TT_SEND         0      2       MN_send
#>>>     TT_RECV         0      2       MN_NONAME
#a = a >>> function(it) {
#}

?\?     TT_ALT          0      4       MN_NONAME
?       TT_QTN          0      4       MN_NONAME
:       TT_COLON        0      99      MN_NONAME
.       TT_DOT          0      99      MN_NONAME
or      TT_OR           0      5       MN_NONAME
and     TT_AND          0      6       MN_NONAME
not     TT_NOT          0      7       MN_opNOT
exists  TT_EXISTS       0      7       MN_opEXISTS
from    TT_FROM         0      7       MN_NONAME
to      TT_TO           0      7       MN_opTO
as      TT_AS           0      7       MN_NONAME
until   TT_UNTIL        0      7       MN_opUNTIL

#define    TT_isBINARY(tt) (TT_IS <= (tt) && (tt) <= TT_RSFT)
is?        TT_IS        _BIN    10      MN_opIS
<:         TT_OF        _BIN    10      MN_opOF
in?        TT_IN        _BIN    10      MN_opHAS
==         TT_EQ        _BIN    10      MN_opEQ
!=         TT_NEQ       _BIN    10      MN_opNOTEQ
<          TT_LT        _BIN    10      MN_opLT
<=         TT_LTE       _BIN    10      MN_opLTE
>          TT_GT        _BIN    10      MN_opGT
>=         TT_GTE       _BIN    10      MN_opGTE

+          TT_ADD       _BIN    20      MN_opADD
-          TT_SUB       _BIN    20      MN_opSUB
/          TT_DIV       _BIN    25      MN_opDIV
%          TT_MOD       _BIN    25      MN_opMOD
*          TT_MUL       _BIN    25      MN_opMUL
&          TT_LAND      _BIN    30      MN_opLAND
|          TT_LOR       _BIN    31      MN_opLOR
^          TT_XOR       _BIN    33      MN_opLXOR
<<         TT_LSFT      _BIN    35      MN_opLSFT
>>         TT_RSFT      _BIN    35      MN_opRSFT

~          TT_LNOT      0       40      MN_opLNOT
++         TT_NEXT      0       40      MN_opNEXT
--         TT_PREV      0       40      MN_opPREV
..         TT_ITR       0       40      MN_opITR
&&&        TT_TAND      0       40      MN_opADDR
+++        TT_TADD      0       40      MN_opPLUS
---        TT_TSUB      0       40      MN_opNEG
'''

VTOKEN = '''
CODE     - 
DOCU     - 
NUM      - 
STR      - 
TSTR     - 
ESTR     - 
#FMTSTR   - 
REGEX    - 
DOC      - 
METAN    - 
PROPN    - 
URN      - 
TSCHEME    - 
NAME     - 
UNAME    - 
FUNCNAME - 
UFUNCNAME -
PTYPE    - 
CID      - 
MN       - 
FMT      - 
#MT       - 

#Typed Token
CONST  -
SYSVAL -
LOCAL  -
FUNCVAR -
XLOCAL -
FIELD  -
SCRFIELD -
ERR -
EOT -
'''

ALIAS = '''
public  @Public
private @Private
final   @Final
virtual @Virtual
static  @Static
const   @Const

mod     %
||      or
&&      and
!       not
===     is
instanceof   <:

char    Int
short   Int
int     Int
long    Int
float   Float
double  Float
boolean Boolean
string  String
def     function
ArrayList Array
Integer Int

TRUE    true
FALSE   false
NULL    null
True    true
False   false
'''

#--------------------------------------------------------------------------

STT_ = {}
STMT_LIST = []
TT_  = {}
TOKEN_LIST = []
MN_ = []
MN_LIST = []
ALIASMAP = {}
OP_LIST = []

class Stmt:
	def __init__(self, name, tt):
		self.name = name
		self.tt = tt
		self.TT = 'STT_%s' % name.upper()
		self.cname = name
		self.flag='0'
		self.TR = '-1'

class Token:
	def __init__(self, name, tt):
		self.name = name
		self.tt = tt
		self.TT = 'TT_%s' % name.upper()
		self.cname = name
		self.flag = '0'
		self.TR = '-1'

def readData():
	tt = 0
	for line in TSTMT.split('\n'):
		if line == '' or line.startswith('#'): continue
		t = line.split()
		stmt = Stmt(t[0], tt)
		tt += 1
		stmt.tcheck = t[1:]
		STT_[t[0]] = stmt
		STMT_LIST.append(stmt)
	
	for line in PSTMT.split('\n'):
		if line == '' or line.startswith('#'): continue
		t = line.split()
		if STT_.has_key(t[0]):
			stmt = STT_[t[0]]
		else:
			stmt = Stmt(t[0], tt)
			tt += 1
			stmt.tcheck = None
			stmt.cname = '*' + t[0]
			STT_[t[0]] = stmt
			STMT_LIST.append(stmt)
		stmt.level = int(t[1])
		stmt.parse = t[2:]
	
	for line in TOKEN.split('\n'):
		if line == '' or line.startswith('#'): continue
		t = line.split()
		tk = Token(t[0], tt)
		tt += 1
		TT_[t[0]] = tk
		TOKEN_LIST.append(tk)
		if t[1].startswith('TT_'): tk.TT = t[1]
		tk.flag = t[2]
		if STT_.has_key(t[0]):
			stmt = STT_[t[0]]
			tk.TR = stmt.TT
		if len(t) == 4: tk.TR = t[3]

	for line in OPTOKEN.split('\n'):
		if line == '' or line.startswith('#'): continue
		t = line.split()
		tk = Token(t[0], tt)
		tt += 1
		TT_[t[0]] = tk
		TOKEN_LIST.append(tk)
		OP_LIST.append(tk)
		if t[1].startswith('TT_'): tk.TT = t[1]
		tk.flag = t[2]
		tk.PRI = t[3]
		tk.TR = t[4]
		if t[4] != 'MN_NONAME':
			MN_.append(t[4])
			MN_LIST.append(tk)

	for line in VTOKEN.split('\n'):
		if line == '' or line.startswith('#'): continue
		t = line.split()
		tk = Token(t[0], tt)
		tt += 1
		TT_[t[0]] = tk
		TOKEN_LIST.append(tk)
	
	for line in ALIAS.split('\n'):
		if line == '' or line.startswith('#'): continue
		t = line.split()
		if not len(t) > 1: continue
		name = t[0]
		TT = t[1]
		ALIASMAP[name] = TT

def write_hfile(f):
	f.write('''
#ifndef MN_OPSIZE
#define STT_MAX  (%d)
#define TT_MAX   (%d)
#define MN_OPSIZE  (%d)
''' % (len(STMT_LIST), len(TOKEN_LIST), len(MN_)))
	n = 0
	for mn in MN_:
		write_define(f, mn, '((knh_methodn_t)%d)' % n, 40)
		n += 1

	write_line(f)
	for stmt in STMT_LIST:
		f.write('''
#define %s  ((knh_term_t)%d)''' % (stmt.TT, stmt.tt))
	for tk in TOKEN_LIST:
		f.write('''
#define %s   ((knh_term_t)%d)''' % (tk.TT, tk.tt))
	f.write('''
#endif/*MN_OPSIZE*/
''')
	f.write('''

#ifdef K_USING_LOADDATA

typedef struct {
	const char *name;
	knh_flag_t  flag;
	knh_short_t to;
} TERMDATA_t;

#define _BIN 1

static TERMDATA_t TERMDATA[] = {''')
	for stmt in STMT_LIST:
		f.write('''
	{"%s", %s, -1}, ''' % (stmt.cname, stmt.flag))
	for tk in TOKEN_LIST:
		f.write('''
	{"%s", %s, %s}, ''' % (tk.cname, tk.flag, tk.TR))
	f.write('''
	{NULL, 0, 0}
};

static void knh_loadScriptTokenData(CTX ctx)
{
	knh_DictSet_t *ds = DP(ctx->sys)->tokenDictSet;
	TERMDATA_t *data = TERMDATA + STT_MAX;
	int tt = STT_MAX;
	while(data->name != NULL) {
		if(!isupper(data->name[0])) {
			knh_DictSet_append(ctx, ds, new_T(data->name), tt);
		}
		tt++;
		data++;
	}
	knh_DictSet_sort(ctx, ds);
}

const char *TT__(knh_term_t tt)
{
	if(tt < TT_MAX + STT_MAX) {
		return TERMDATA[tt].name;
	}
	DBG_P("tt=%d", tt);
	return "UNDEFINED";
}

knh_bool_t TT_is(knh_term_t tt, knh_flag_t flag)
{
	return FLAG_is(TERMDATA[tt].flag, flag);
}

knh_short_t TT_to(knh_term_t tt)
{
	return TERMDATA[tt].to;
}

void knh_dumpKeyword(CTX ctx, knh_OutputStream_t *w)
{
	TERMDATA_t *data = TERMDATA + STT_MAX;
	while(data->name != NULL) {
		if(islower(data->name[0])) {
			knh_write_ascii(ctx, w, data->name);
			knh_putc(ctx, w, ' ');
		}
		data++;
	}
	knh_write_EOL(ctx, w);
}

typedef struct {
	const char *name;
	const char *alias;
} ALIASDATA_t;

static ALIASDATA_t __AliasData[] = {''')
	l = ALIASMAP.keys()
	l.sort()
	for n in l:
		f.write('''
	{"%s", "%s"}, ''' % (n, ALIASMAP[n]))
	f.write('''
	{NULL, NULL}
};

void knh_loadScriptAliasTokenData(CTX ctx)
{
	ALIASDATA_t *data = __AliasData;
	knh_DictMap_t *dm = new_DictMap0(ctx, sizeof(__AliasData), 0/*isCaseMap*/, "AliasDictMap");
	KNH_INITv(ctx->wshare->sysAliasDictMapNULL, dm);
	while(data->name != NULL) {
		knh_DictMap_append(ctx, dm, new_T(data->name), UPCAST(new_T(data->alias)));
		data++;
	}
}

typedef struct { 
	knh_ushort_t priority;
	knh_methodn_t mn;
} OPDATA_t;

static OPDATA_t OPDATA[] = {''')
	for tk in OP_LIST:
		f.write('''
	{%s, %s}, /* %s */''' % (tk.PRI, tk.TR, tk.cname))
	f.write('''
};

static const char *OPNAME[] = {''')
	for tk in MN_LIST:
		f.write('''
	"%s",''' % tk.cname);
	f.write('''
};

int TT_priority(knh_term_t tt)
{
	if(TT_LET <= tt && tt <= TT_TSUB) {
		return OPDATA[tt-TT_LET].priority;
	}
	if(TT_ASIS <= tt && tt < TT_ERR) {
		return 99;
	}
	return 0;
}

knh_methodn_t TT_toMN(knh_term_t tt)
{
	if(TT_LET <= tt && tt <= TT_TSUB) {
		return OPDATA[tt - TT_LET].mn;
	}
	return MN_NONAME;
}

const char* knh_getopname(knh_methodn_t mn)
{
	DBG_ASSERT(mn + TT_NOT <= TT_TSUB);
	return OPNAME[mn];
}

#endif/*K_USING_LOADDATA*/

''')

def gen_stmt(bdir):
    f = open('include/konoha1/konohalang.h', 'w')
    f.write('''// THIS FILE WAS AUTOMATICALLY GENERATED
''')
    readData()
    write_hfile(f)
    f.close()

if __name__ == '__main__':
    bdir = '..'
    readData()
    write_hfile(sys.stdout)
    #gen_stmt(bdir)
