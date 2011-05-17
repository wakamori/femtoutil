#!/usr/bin/python 
# This is a final python version of genkonoha.py 

import os, os.path, sys
import time, binascii

####
# print 

__FILE__ = ''
__LINE__ = 0

def perror(msg, data = ""):
    print "[%s:%d] %s\t%s" % (__FILE__,  __LINE__, msg, data)

LINE = '''
/* ------------------------------------------------------------------------ */
'''

DLINE = '''
/* ------------------------------------------------------------------------ */
'''

def write_line(f):
    f.write(LINE)

def write_dline(f):
    f.write(DLINE)

def write_comment(f, msg):
    f.write('/* %s */\n' % msg)

def write_chapter(f, msg):
    f.write(DLINE)
    write_comment(f, msg)

def write_section(f, msg):
    f.write(LINE)
    write_comment(f, msg)

def write_define(f, name, value='', n=40):
    s = '#define %s ' % name
    while(len(s) < n) : s+=' '
    f.write(s)
    f.write(value)
    f.write('\n')
###

def write_ifndefine(f, name, value='', n=40):
    f.write('#ifndef %s\n' % name)
    write_define(f, name, value, n)
    f.write('#endif\n')
###

def write_data(f, type, name, dlist, end='{NULL}'):
    f.write('''
static %s %s[] = {''' % (type, name))
    for d in dlist:
        f.write(d)
    f.write('''
\t%s
};
''' % end)

####
# utils

def classname(name):
    if name.startswith('int') or name.startswith('float') or name.startswith('Boolean'):
        if name.islower(): name = name[0].upper() + name[1:]
    name = name.replace('!', '').replace('?', '')
    name = name.replace('Int[]', 'IArray')
    name = name.replace('Float[]', 'FArray')
    name = name.replace('[]', 'ARRAY').replace('..', 'ITR')
    return name

def STRUCT_(cname): return 'STRUCT_%s' % cname
def CLASS_(cname): return 'CLASS_%s' % cname
def FN_(fn):   return 'FN_%s' % fn.replace(':', '__')
def MN_(mn):  return 'MN_%s' % mn.replace('%', '_').replace(':', '__')
def EBI_(cname):  return 'EBI_%s' % cname.replace('!!', '')

def TYPE_(type):
    p = ''
    #if type.endswith('!'): p = 'NN
    #if type.endswith('?'): p = 'NA'
    return '%sTYPE_%s' % (p, classname(type))

def addflag(flag, meta, cname, key):
    if meta.has_key(key):
        flag += '|FLAG_%s_%s' % (cname, key[1:])
    return flag.replace('0|', '')
 
#### 
# 

class Class :
    def __init__(self, name, cname, parent, base):
        self.meta = {}
        self.name = name
        self.cname = cname
        self.parent = parent
        if base.endswith("_t"):
            self.struct = base
            self.base = base.replace('knh_', '').replace('_t', '')
        else:
            self.struct = None
            self.base = base
        self.cparams = []
        self.method_size = 0
        self.formatter_size = 0
        self.mapper_size = 0
        
    def flag(self): 
        f = '0'
        f = addflag(f, self.meta, 'Class', '@Singleton')
        f = addflag(f, self.meta, 'Class', '@Immutable')
        f = addflag(f, self.meta, 'Class', '@Private')
        f = addflag(f, self.meta, 'Class', '@TypeVariable')
        f = addflag(f, self.meta, 'Class', '@Final')
        return f
        
    def has(self, key):
        return self.meta.has_key(key)

    def StructData(self):
        fmt = '''
\tDATA_STRUCT0, CLASS_$C, _DATA(&$CDef), CFLAG_$C,'''
        fmt = fmt.replace('$C', self.cname)
        fmt = fmt.replace('_Object_', '_ObjectField_')
        return fmt

    def ClassData(self):
        fmt = '''
\tDATA_CLASS0, CLASS_%s, _DATA("konoha.%s"), CFLAG_%s, %s, %s, %s + %s /*%s*/,''' % (self.cname, self.name, self.cname, CLASS_(self.base), CLASS_(self.parent), self.method_size, self.formatter_size, self.mapper_size)
        return fmt

# class Class Object knh_Object_t
# class Int[] IArray Array knh_IArray_t

def parse_Class(meta, tokens, data):
    if len(tokens) == 2:
        c = Class(tokens[0], tokens[0], tokens[1], 'knh_%s_t' % tokens[0])
    elif len(tokens) == 3:
        c = Class(tokens[0], tokens[0], tokens[1], tokens[2])
    else:
        c = Class(tokens[0], tokens[1], tokens[2], tokens[3])
    c.meta = meta
    data.add_Class(c)

class CParam :
    def __init__(self, cname):
        self.meta = {}
        self.cname = cname

    def CParamData(self):
        fmt = '''
\tDATA_CPARAM, %s, %s,''' % (CLASS_(self.cname), )
        return fmt

# cparam Array T dynamic

def parse_cparam(meta, tokens, data):
	#print tokens, len(tokens)
	#c = CParam(tokens[0])    
	#c.meta = meta
	#data.add_cparam(c)
	pass

class PType :
    def __init__(self, cname, bname, rsize):
        self.meta = {}
        self.cname = cname
        self.bname = bname
        self.rsize = int(rsize)
        self.cparams = None

    def ClassData(self):
        args = ''
        for a in self.cparams:
            args += ', %s, FN_' % (TYPE_(a))
        psize = len(self.cparams) - self.rsize
        fmt = '''
\tDATA_GENERICS/*%s*/, %s, %d, %d%s,''' % (self.cname, CLASS_(self.bname), psize, self.rsize, args)
        return fmt

# type ICmpr Func 1 Boolean Int Int

def parse_type(meta, tokens, data):
    #print len(tokens), tokens
    c = PType(tokens[0], tokens[1], tokens[2])
    c.meta = meta
    c.cparams = tokens[3:]
    data.add_ptype(c)

def write_class_h(f, c, cid):
    write_section(f, c.cname)
    if c.struct != None:
        write_define(f, CLASS_(c.cname), '((knh_class_t)%d)' % cid, 32)
        #write_define(f, STRUCT_(c.cname), '((knh_struct_t)%d)' % cid, 32)
    else :
        write_define(f, CLASS_(c.cname), '((knh_class_t)%d)' % cid, 32)
        #write_define(f, STRUCT_(c.cname), STRUCT_(c.base), 32)
    write_define(f, 'IS_%s(o)' % c.cname,  '(O_cid(o) == %s)' % CLASS_(c.cname), 32)
    if(c.cname == c.base) :
        write_define(f, 'IS_b%s(o)' % c.cname,  '(O_bcid(o) == %s)' % CLASS_(c.cname), 32)
    write_define(f, 'TYPE_%s' % c.cname, CLASS_(c.cname), 32)
    write_define(f, 'CFLAG_%s' % c.cname, '((knh_flag_t)%s)' % c.flag(), 32)
    write_define(f, 'FLAG_%s' % c.cname, 'KNH_MAGICFLAG(CFLAG_%s)' % c.cname, 32)

def write_ptype_h(f, c, cid):
    write_section(f, c.cname)
    write_define(f, CLASS_(c.cname), '((knh_class_t)%d)' % cid, 32)
    write_define(f, 'IS_%s(o)' % c.cname,  '(O_cid(o) == %s)' % CLASS_(c.cname), 32)
    write_define(f, 'TYPE_%s' % c.cname, CLASS_(c.cname), 32)
    #write_define(f, 'CFLAG_%s' % c.cname, 'CFLAG_%s' % c.bname, 32)
    #write_define(f, 'FLAG_%s' % c.cname, 'KNH_MAGICFLAG(CFLAG_%s)' % c.cname, 32)


class Expt:
    def __init__(self, cname, parent):
        self.cname = cname.replace("!!", "")
        if parent == '-': self.parent = 'Exception'
        else: self.parent = parent

    def ExptData(self):
        fmt = '''
\tDATA_EXPT, _DATA("%s"), 0, %s, %s, ''' % (self.cname, EBI_(self.cname), EBI_(self.parent))
        return fmt

def parse_Expt(meta, tokens, data):
    expt = Expt(tokens[0], tokens[1])
    expt.meta = meta
    data.add_Expt(expt)

class Flag:
    def __init__(self, cname, flagname, index, code):
        self.cname = cname
        tt = flagname.split('!')
        self.poname = tt[0]
        self.ngname = None
        if len(tt) == 2: self.ngname = tt[1]
        self.index = index
        self.code = code
        self.attrs = []
        
def parse_Flag(meta, tokens, data):
    if len(tokens) != 8 :
        perror("flag", tokens)
    else:
        fg = Flag(tokens[0], tokens[1], int(tokens[2]), tokens[3])
        fg.attrs = tokens[4:]
        fg.meta = meta
        data.add_Flag(fg)

def write_flag_h(f, fg):
    fg.MACRO = 'FLAG_%s_%s' % (fg.cname, fg.poname)
    code = fg.code
    tcode = 'knh_flag_t'
    if fg.cname == 'Object' : tcode = 'knh_uintptr_t'
    if fg.code == '-':
        code = '(%s)->h.magicflag'
        tcode = 'knh_uintptr_t'
        f.write('''
#define %s %s''' % (fg.MACRO, 'FLAG_Object_Local%d' % fg.index))
    else:
        f.write('''
#define %s %s''' % (fg.MACRO, '((%s)(1<<%d))' % (tcode, fg.index)))
    args = 'o'
    if code.find('[n]') > 0: args = 'o,n'
    funcbase = '%s' % fg.cname
    if fg.cname == 'Class': funcbase = funcbase.replace('Class', 'class')
    if fg.cname == 'Context': funcbase = funcbase.replace('Context', 'CTX')
    if fg.attrs[0] != '*':  
        f.write('''
#define %s_%s%s(%s)  (TFLAG_is(%s,%s,%s))''' % (funcbase, fg.attrs[0], fg.poname, args, tcode, code % 'o', fg.MACRO))
        if fg.ngname != None:
            f.write('''
#define %s_%s%s(%s)  (!TFLAG_is(%s,%s,%s))''' % (funcbase, fg.attrs[0], fg.ngname, args, tcode, code % 'o', fg.MACRO))
    if fg.attrs[1] != '*':  
        f.write('''
#define %s_%s%s(%s,b) TFLAG_set(%s,%s,%s,b)''' % (funcbase, fg.attrs[1], fg.poname, args, tcode, code % 'o', fg.MACRO))
        if fg.ngname != None:
            f.write('''
#define %s_%s%s(%s,b)  TFLAG_set(%s,%s,%s,(!(b)))''' % (funcbase, fg.attrs[1], fg.ngname, args, tcode, code % 'o', fg.MACRO))

def write_flag_c(f, fg, data):
    funcbase = '%s' % fg.cname
    methodbase = '_%s' % (fg.cname)
    a1 = '(knh_%s_t*)sfp[0].o' % fg.cname
    if fg.cname == 'Class': 
        funcbase = funcbase.replace('Class', 'class')
        a1 = 'knh_Class_cid(sfp[0].c)'
    if fg.cname == 'Context': 
        funcbase = funcbase.replace('Context', 'CTX')
        a1 = '((sfp[0].cx)->ctx)'

    ff = fg.attrs[2]
    if ff != '*':
        ffn = ff + fg.poname
        functype = 'METHOD %s_%s(CTX ctx, knh_sfp_t *sfp _RIX)' % (methodbase, ffn)
        parse_Method({'@Func' : '%s_%s' % (methodbase, ffn)}, 
                     ['Boolean', '%s.%s' % (fg.cname, ffn)], data)
        f.write('''
static METHOD %s_%s(CTX ctx, knh_sfp_t *sfp _RIX)
{
\tRETURNb_(%s_%s(%s));
}
''' % (methodbase, ffn, funcbase, ffn, a1))
        if fg.ngname != None:
            ffn = ff + fg.ngname
            functype = 'METHOD %s_%s(CTX ctx, knh_sfp_t *sfp _RIX)' % (methodbase, ffn)
            parse_Method({'@Func' : '%s_%s' % (methodbase, ffn)}, 
                     ['Boolean', '%s.%s' % (fg.cname, ffn)], data)
            f.write('''
static METHOD %s_%s(CTX ctx, knh_sfp_t *sfp _RIX)
{
\tRETURNb_(!(%s_%s(%s)));
}
''' % (methodbase, ffn, funcbase, ff + fg.poname, a1))
    #
    ff = fg.attrs[3]
    if ff != '*':
        ffn = ff + fg.poname
        functype = 'METHOD %s_%s(CTX ctx, knh_sfp_t *sfp _RIX)' % (methodbase, ffn)
        parse_Method({'@Func' : '%s_%s' % (methodbase, ffn)}, 
                     ['Boolean', '%s.%s' % (fg.cname, ffn), 'Boolean', 'flag'], data)
        f.write('''
static METHOD %s_%s(CTX ctx, knh_sfp_t *sfp _RIX)
{
\t%s_%s(%s, sfp[1].bvalue);
\tRETURNb_(sfp[1].bvalue);
}
''' % (methodbase, ffn, funcbase, ffn, a1))
        if fg.ngname != None:
            ffn = ff + fg.ngname
            functype = 'METHOD %s_%s(CTX ctx, knh_sfp_t *sfp _RIX)' % (methodbase, ffn)
            parse_Method({'@Func' : '%s_%s' % (methodbase, ffn)}, 
                     ['Boolean', '%s.%s' % (fg.cname, ffn), 'Boolean', 'flag'], data)
            f.write('''
static METHOD %s_%s(CTX ctx, knh_sfp_t *sfp _RIX)
{
\t%s_%s(%s, sfp[1].bvalue);
\tRETURNb_(sfp[1].bvalue);
}
''' % (methodbase, ffn, funcbase, ffn, a1))

# method void C.f name name

class Method:
    def __init__(self, rtype, cname, mn):
        self.rtype = rtype
        self.cname = cname
        self.mn = mn
        self.mparams = []
        self.mf = 0
        self.delta = 0
        
    def add(self, type, fn):
        p = (type, fn)
        self.mparams.append(p)

    def ParamArrayKey(self):
        args = self.rtype.replace('!', '')
        for p in self.mparams: args += '.%s.%s' % (p[0].replace('!',''), p[1])
        if self.meta.has_key('@VARGs'): args += "..."
        return args

    def ParamArrayData2(self):
        args = TYPE_(self.rtype)
        for p in self.mparams: args += ', %s, %s' % (TYPE_(p[0]), FN_(p[1]))
        fmt = '''
\t{%d, %d, %s},''' % (self.mf, len(self.mparams), args)
        return fmt

    def ParamArrayData(self):
        args = ''
        for p in self.mparams: args += ', %s, %s' % (TYPE_(p[0]), FN_(p[1]))
        rsize = 0
        if self.rtype != 'void':
            rsize = 1
            args += ', %s, FN_return' % (TYPE_(self.rtype))
        if args == '': args = ', '
        flag = '0'
        flag = addflag(flag, self.meta, 'ParamArray', '@VARGs')
        fmt = '''
\tDATA_PARAM/*%d*/, %s, %d, %d%s,''' % (self.mf, flag, len(self.mparams), rsize, args)
        return fmt
    
    def MethodData(self):
        ext = None
        if self.meta.has_key('@Func'):
            func = self.meta['@Func']
            ext = 'NULL'
            if func.startswith('knh_'):
                ext = '(void*)%s' % func
                func = 'knh_fmethod_formatter'
        if self.meta.has_key('@Abstract') or ext == None:
            func = 'knh_fmethod_abstract'
            ext = 'NULL'
        flag = '0'
        flag = addflag(flag, self.meta, 'Method', '@Const')
        flag = addflag(flag, self.meta, 'Method', '@Static')
        flag = addflag(flag, self.meta, 'Method', '@Virtual')
        flag = addflag(flag, self.meta, 'Method', '@Hidden')
        flag = addflag(flag, self.meta, 'Method', '@Private')
        fmt = '''
\tDATA_METHOD0, %s, %s, %s, _DATA(%s), /*P=*/%s,''' % (CLASS_(self.cname), MN_(self.mn), flag, func, self.mf)
        return fmt

def parse_Method(meta, tokens, data):
    rtype = tokens[0]
    cname, mn = tokens[1].split('.')
    data.check_type(rtype)
    data.check_type(cname)
    data.add_mn(mn)
    mtd = Method(rtype, cname, mn)
    mtd.meta = meta
    if tokens[-1] == '...':
        tokens = tokens[:-1]
        mtd.meta['@VARGs'] = True
    tt = tokens[2:]
    c = 0
    while c + 1 < len(tt):
        type = tt[c]
        fn = tt[c+1]
        data.check_type(tt[c])
        data.add_name(fn)
        mtd.add(type, fn)
        c += 2
    data.add_Method(mtd)
    key = mtd.ParamArrayKey()
    if not data.METHODFIELD.has_key(key):
        mf = len(data.METHODFIELD_LIST) + 1
        data.METHODFIELD_LIST.append(mtd)
        data.METHODFIELD[key] = mf
    mtd.mf = data.METHODFIELD[key]

class TypeMap:
    def __init__(self, fcname, tcname):
        self.fcname = fcname
        self.tcname = classname(tcname)

    def TypeMapData(self):
        func = 'NULL'
        if self.meta.has_key('@Func'):
            func = self.meta['@Func']
        flag = '0'
        flag = addflag(flag, self.meta, 'TypeMap', '@Const')
        flag = addflag(flag, self.meta, 'TypeMap', '@Semantic')
        fmt = '''
\tDATA_TYPEMAP, %s, %s, %s, _DATA(%s),''' % (CLASS_(self.fcname), CLASS_(self.tcname), flag, func)
        return fmt

def parse_TypeMap(meta, tokens, data):
    mpr = TypeMap(tokens[0], tokens[1])
    mpr.meta = meta
    data.add_TypeMap(mpr)

class Data:
    def __init__(self):
        self.KNHAPI2_LIST = []
        self.PROTOTYPE_LIST = []
        self.parser = {}
        self.NAME = {"":""}
        self.CLASS = {}
        self.STRUCT_LIST = []
        self.CLASS_LIST = []
        self.CPARAM_LIST = []
        self.PTYPE_LIST = []
        self.EXPT = {}
        self.EBI_LIST = []
        self.FLAG_LIST = []
        self.METHODFIELD = {"void": 0}
        self.METHODFIELD_LIST = []
        self.METHODN = {}
        self.METHOD_LIST = []
        self.TYPEMAP_LIST = []
        self.STEXT = [
                      ("EMPTY", ""), ("TAB", "\\t"), ("ONE", "1"),
                      ("EOL", '"K_OSLINEFEED"'), ("BEGIN", "BEGIN"), ("END", "END"),
                      ("true", "true"), ("false","false"), ("null", "null"), ("void", "void"),
                      ("main", "main"), ("ENCODING", '"K_ENCODING"'),
                      ("DEVNULL", "/dev/null"), ("DEVSTDIN", "/dev/stdin"),
                      ("DEVSTDERR", "/dev/stderr"), ("DEVSTDOUT", "/dev/stdout"),
                      ("ATlabel", "@label"), ("AssertionException", "Assertion!!"), 
                      ("LP", "("), ("RP", ")"),  ("LB", "{"), ("RB", "}"),  ("LS", "["), ("RS", "]"), 
                      ]
        self.add_mn('main')
        self.add_mn('this')
        self.add_mn('super')
        self.add_name('it')
        self.add_name('vargs')
        #self.add_mn('invoke')
        self.add_mn('defined')
        self.add_mn('delegate')
        #self.add_mn('likely')
        #self.add_mn('unlikely')
        self.add_mn('first')
        self.add_mn('second')
        self.add_mn('third')
        
    def add_name(self, name):
        if name.startswith('*'): perror('[bugs?]', name)
        n = name.lower().replace('_', '').replace(':', '__')
        if not self.NAME.has_key(n):
            self.NAME[n] = name
        elif self.NAME[n] != name:
            perror('different name', '%s %s' % (name, self.NAME[n]))
        return n
            
    def add_mn(self, mn):
    	if mn.startswith("op"):
    		return
        if mn.startswith("get"):
            t = mn[3:]
            if len(t) > 1 and t[0].isupper: t = t[0].lower() + t[1:]
            fn = self.add_name(t)
            self.METHODN[MN_(mn)] = 'MN_toGETTER(FN_%s)' % fn
            return
        if mn.startswith("set"):
            t = mn[3:]
            if len(t) > 1 and t[0].isupper: t = t[0].lower() + t[1:]
            fn = self.add_name(t)
            self.METHODN[MN_(mn)] = 'MN_toSETTER(FN_%s)' % fn
            return
        if mn.startswith("is"):
            t = mn[2:]
            if t[0].isupper: t = t[0].lower() + t[1:]
            fn = self.add_name(t)
            self.METHODN[MN_(mn)] = 'MN_toISBOOL(FN_%s)' % fn
            return
        if mn.startswith('%'): 
            fn = self.add_name(mn[1:])
            self.METHODN[MN_(mn)] = 'MN_toFMT(FN_%s)' % fn
            return
        fn = self.add_name(mn)
        self.METHODN[MN_(mn)] = 'FN_%s' % fn

    def add_Class(self, c):
        if(c.struct != None) : self.STRUCT_LIST.append(c)
        self.CLASS_LIST.append(c)
        self.CLASS[c.name]  = c
        self.CLASS[c.cname] = c

    def add_cparam(self, c):
        self.CPARAM_LIST.append(c)
        
    def get_Class(self, name):
        if self.CLASS.has_key(name):
            return self.CLASS[name] 
        if name.endswith('[]'):
            bname = name[:-2]
            cname = bname + 'ARRAY'
            if self.CLASS.has_key(cname): return self.CLASS[cname]
            newc = PType(cname, 'Array', '0')
            perror('generating', '%s ==> %s' % (name, newc.cname))
            newc.cparams = [bname]
            self.add_ptype(newc)
            return newc
        if name.endswith('..'):
            bname = name[:-2]
            cname = bname + 'ITR'
            if self.CLASS.has_key(cname): return self.CLASS[cname]
            newc = PType(cname, 'Iterator', '0')
            perror('generating', '%s ==> %s' % (name, newc.cname))
            newc.cparams = [bname]
            self.add_ptype(newc)
            return newc
        return None

    def add_ptype(self, c):
        self.PTYPE_LIST.append(c)
        self.CLASS[c.cname] = c
    
    def check_type(self, type):
        if type == 'void' : return type
        q = ''
        if type.endswith('?') or type.endswith('!'):
            q = type[-1]
        c = self.get_Class(type.replace('!', '').replace('?',''))
        if c is None:
            #perror('unknown type:', type)
            return type
        return c.cname + q
    
    def add_Flag(self, fg):
        self.FLAG_LIST.append(fg)
        
    def add_Method(self, mtd):
        self.METHOD_LIST.append(mtd)
        c = self.get_Class(mtd.cname)
        if c is not None: 
            if mtd.mn.startswith('%'):
                c.formatter_size +=1
            else:
                c.method_size += 1

    def add_TypeMap(self, mpr):
        self.TYPEMAP_LIST.append(mpr)
        c = self.get_Class(mpr.fcname)
        if c is not None: c.mapper_size += 1

    def add_Expt(self, expt):
        if not self.EXPT.has_key(expt.cname):
            self.EXPT[expt.cname] = expt
            self.EBI_LIST.append(expt)
        else:
            perror('duplicated', expt.cname)
    
####
# parser


####
# readfile 

def parsedoc(meta, tokens, data):
    #print 'meta', meta
    #print 'tokens', tokens
    if len(tokens) == 0: return
    if data.parser.has_key(tokens[0]):
        data.parser[tokens[0]](meta, tokens[1:],data)
    else:
        perror("unsupported", tokens[0])
        
def addtokens(tokens, t, isParseParenthesis):
    if isParseParenthesis:
        tt = t.split('(')
        if len(tt) == 2:
            addtokens(tokens, tt[0], isParseParenthesis)
#            tokens.append('(')
            addtokens(tokens, tt[1], isParseParenthesis)
            return
        tt = t.split(')')
        if len(tt) == 2:
            addtokens(tokens, tt[0], isParseParenthesis)
#            tokens.append(')')
            addtokens(tokens, tt[1], isParseParenthesis)
            return
    if t.endswith(','): t = t.replace(',', '')
    if t != '' : tokens.append(t)
    
def readdoc(doc, funcname, data):
    tokens = []
    meta = {'@Func': funcname}
    isParseParenthesis = False
    for t in doc.split():
        if t == 'method': isParseParenthesis = True
        if t.startswith('@'):
            tt = t.split('(')
            if len(tt) == 2:
                meta[tt[0]] = tt[1].replace(')', '')
            else:
                meta[tt[0]] = tt[0]
            continue
        if t.endswith(";"):
            if len(t) > 1: 
                t = t.replace(';', '')
                addtokens(tokens, t, isParseParenthesis)
            parsedoc(meta, tokens, data)
            meta = {'@Func': funcname}
            tokens = []
            isParseParenthesis = False
            continue
        addtokens(tokens, t, isParseParenthesis)
    if(len(tokens) > 0) :
        parsedoc(meta, tokens, data)

def readfunc(funcdata, data):
    funcdata = funcdata.replace('FASTAPI(', 'FASTAPI_')
    funcdata = funcdata.replace('KLRAPI(', 'KLRAPI_')
    t = funcdata.split('(')
    funcdata = funcdata.replace('FASTAPI_', 'FASTAPI(')
    funcdata = funcdata.replace('KLRAPI_', 'KLRAPI(')
    if funcdata.find('KNHAPI2') >= 0:
        data.KNHAPI2_LIST.append(funcdata)
        return ""
    if len(t) != 2:
        perror("not func", funcdata) 
        return ""
    t = t[0].split()
    funcname = t[-1]
    if funcdata.find('static') == -1:
        data.PROTOTYPE_LIST.append(funcdata)
    return funcname

def readfile(filename, data):
    global __FILE__, __LINE__
    file = open(filename)
    __FILE__ = filename
    __LINE__ = 0
    doc  = ''
    func = ''
    prev = ''
    for line in file:
        __LINE__ += 1
        if line.startswith("/* --") or line.startswith("/* =="):
            if doc != '': readdoc(doc, func, data)
            doc = ''
            func = ''
        if line.startswith("//##") :
            doc += line.replace("//## ", "")
            continue
        if line[0] == '{':
            func = readfunc(prev, data)
            prev = ''
        if line[0].isalpha() : prev += line
        else : prev = ''
    file.close
###

def readdir(dir, data):
    if dir.endswith('/ext') or dir.endswith('/gen'): return
    fl = os.listdir(dir)
    for fn in fl:
        if fn.startswith('.'): continue
        if fn.startswith('_'): continue
        fpath = dir + '/' + fn
        if os.path.isdir(fpath): readdir(fpath, data)
        if dir != 'src' and fn.endswith('.c') : 
            readfile(fpath, data)

def svnrev():
    f = os.popen("svn info")
    t = f.read()
    print t
    f.close()
    t = t.split('Revision: ')[1].split()[0]
    return int(t) + 1

def write_name_h(f, data):
    write_chapter(f, 'MACROS')
    f.write('#ifndef K_REVISION\n')
    write_define(f, 'K_REVISION', '%d' % svnrev(), 40)
    f.write('#endif\n')
    write_define(f, 'K_BUILDID', '%d' % data.serial_number, 40)
    #if not '-c' in data.OPTIONS:
        #write_define(f,  'KONOHA_EXPIRE', '%dLL' % (int(time.time()) + 259200), 40)
        #write_define(f,  'KONOHA_EXPIRE', '%dLL' % (int(time.time()) + 2), 40)
    write_chapter(f, 'STRUCT')
    cid = 0
    for c in data.STRUCT_LIST :
        write_class_h(f, c, cid)
        cid += 1    

    write_chapter(f, 'CLASS')
    for c in data.CLASS_LIST :
        if c.struct != None: continue
        if c.base == c.cname: continue
        write_class_h(f, c, cid)
        cid += 1

    write_chapter(f, 'CLASS')
    for c in data.PTYPE_LIST :
        write_ptype_h(f, c, cid)
        cid += 1

    write_define(f, 'K_CLASS_INITSIZE', '%d' % (cid+1), 40)
    
    write_chapter(f, 'FLAG')
    for fg in data.FLAG_LIST :
        write_flag_h(f, fg)

    write_chapter(f, 'EXPT')
    cid = 1
    for c in data.EBI_LIST :
        write_define(f, EBI_(c.cname), '%d' % cid, 32)
        cid += 1
        
    write_chapter(f, 'FIELDN')
    pairs = data.NAME.items()
    pairs.sort()
    fnn = 0
    for n, fn in pairs:
        write_define(f, FN_(fn), '(MN_OPSIZE+%d)' % fnn, 24)
        if n != fn:
            write_define(f, FN_(n), '(MN_OPSIZE+%d)' % fnn, 24)
        fnn += 1
    write_define(f, 'K_TFIELD_SIZE', '%d' % fnn, 24)
    
    write_chapter(f, 'METHODN')
    pairs = data.METHODN.items()
    for mn, macro in pairs:
        write_define(f, mn, macro, 24)

    write_chapter(f, 'TS')
    fnn = 0
    for n, s in data.STEXT :
        write_define(f, 'TS_%s' % n, '(ctx->share)->tString[%d]' % fnn, 24)
        fnn += 1
    write_define(f, 'K_TSTRING_SIZE', '%d' % fnn, 24)

def write_KNHAPI2(f, data):
	f.write('''
#ifdef K_INTERNAL''')
	for p in data.KNHAPI2_LIST:
		p = p.replace('\n', '')
		f.write('''
%s;''' % p)
	f.write('''
#endif

typedef struct knh_api2_t {
	size_t crc32;''')
	data.KNHAPI2_LIST.sort()
	t=''
	for p in data.KNHAPI2_LIST:
		p = p.replace('\n', '')
		t += p
		#if p.endswith('\n'): p = p[:-1]
		p = p.replace('KNHAPI2(', '')
		p = p.replace('(', ')(')
		p = p.replace(') ', ' (*')
		p = p.replace('(*knh_', ' (*')
		f.write('''
	%s;''' % p)
	f.write('''
} knh_api2_t;
	
#define K_API2_CRC32 ((size_t)%d)
#ifdef K_DEFINE_API2
static const knh_api2_t* getapi2(void) {
	static const knh_api2_t DATA_API2 = {
		K_API2_CRC32,''' % binascii.crc32(t))
	for p in data.KNHAPI2_LIST:
		p = p.replace('(', ' ')
		t = p.split();
		f.write('''
		%s,''' % t[2])
	f.write('''
	};
	return &DATA_API2;
};
#endif/*K_DEFINE_API2*/

#ifndef K_INTERNAL''')
	for p in data.KNHAPI2_LIST:
		p = p.replace('(', ' ')
		t = p.split();
		f.write('''
#define %s   ctx->api2->%s''' % (t[2], t[2].replace('knh_', '')))
	f.write('''
#endif

''')

def write_Data(f, data):
    write_chapter(f, 'flag')
    for fg in data.FLAG_LIST :
        write_flag_c(f, fg, data)
    write_chapter(f, 'data')
    dlist0 = []
    for n, s in data.STEXT :
        fmt = '''
\t"%s", /* TS_%s */''' % (s, n)
        dlist0.append(fmt)
    write_data(f, 'const char *', 'StringData0', dlist0, 'NULL')
    dlist0 = []
    dlist = []
    for c in data.STRUCT_LIST :
        fmt = '''
\t"%s",''' % c.cname
        dlist0.append(fmt)
        dlist.append(c.StructData())
    #write_data(f, 'char *', 'StructNameData', dlist0, 'NULL')
    write_data(f, 'knh_data_t', 'StructData0', dlist, '0')
    #
    dlist=[]
    for c in data.CLASS_LIST :
        if c.struct is None: continue
        dlist.append(c.ClassData())
    for c in data.CLASS_LIST :
        if c.struct is not None: continue
        dlist.append(c.ClassData())
    for c in data.PTYPE_LIST :
        dlist.append(c.ClassData())
    #dlist=[]
    for c in data.EBI_LIST :
        dlist.append(c.ExptData())
    write_data(f, 'knh_data_t', 'ClassData0', dlist, '0')
    #
    dlist = []
    l = data.NAME.items()
    l.sort()
    for n, fn in l:
        fmt = '''
\t{"%s", %s},''' % (fn, FN_(fn))
        dlist.append(fmt)
    write_data(f, 'knh_FieldNameData0_t', 'FieldNameData0', dlist)
    #
    dlist = ['\n\tDATA_PARAM/*0*/, 0, 0, 0, ']
    for mtd in data.METHODFIELD_LIST:
        dlist.append(mtd.ParamArrayData())
    write_data(f, 'knh_data_t', 'ParamArrayData0', dlist, '0')
    write_define(f, 'K_PARAM0_SIZE', '%d' % len(dlist))
    #
    dlist = []
    for mtd in data.METHOD_LIST:
        dlist.append(mtd.MethodData())
    #write_data(f, 'knh_data_t', 'MethodData0', dlist, '0')
    #
    #dlist = []
    for mpr in data.TYPEMAP_LIST:
        dlist.append(mpr.TypeMapData())
    write_data(f, 'knh_data_t', 'APIData0', dlist, '0')
    #write_KNHAPI2(f, data)
    pass

def genkonoha():
    data = Data()
    f = open('src/BUILDID')
    data.serial_number = int(f.readline())
    f.close()
    data.OPTIONS = sys.argv[1:]
    if '-c' in data.OPTIONS: 
        data.serial_number += 1
        f = open('src/BUILDID', 'w')
        f.write('%d' % data.serial_number)
        f.close()
    print 'build=%d' % data.serial_number
    data.parser['class'] = parse_Class
    data.parser['type'] = parse_type
    data.parser['cparam'] = parse_cparam
    data.parser['flag'] = parse_Flag
    data.parser['method'] = parse_Method
    data.parser['mapper'] = parse_TypeMap
    data.parser['expt'] = parse_Expt
    readfile("include/konoha1/konoha_class.h", data)
    readdir("src", data)
    f = open('src/main/structdata.h', 'w')
    f.write('''// THIS FILE WAS AUTOMATICALLY GENERATED
''')
    write_Data(f, data)
    f.close()
    f = open('include/konoha1/konoha_name.h', 'w')
    f.write('''// THIS FILE WAS AUTOMATICALLY GENERATED
''')
    write_name_h(f, data)
    f.close()
    f = open('include/konoha1/konoha_api.h', 'w')
    f.write('''// THIS FILE WAS AUTOMATICALLY GENERATED

#ifdef __cplusplus
extern "C" {
#endif


''')
    write_KNHAPI2(f, data)
    write_chapter(f, 'DEVELOPERS API')
    for p in data.PROTOTYPE_LIST:
        if p.endswith('\n'): p = p[:-1] + ';\n'
        else: p += ';\n'
        f.write(p)

    f.write('''
#ifdef __cplusplus
}
#endif

// THIS FILE WAS AUTOMATICALLY GENERATED. DON'T EDIT.

''')

    
    f.close()

if __name__ == '__main__':
    genkonoha()
    from gen_vm import gen_vm_c
    gen_vm_c(".")
    from genlang import gen_stmt
    gen_stmt(".")
    #from gen_konohac import gen_stmt
    #gen_stmt(".")

