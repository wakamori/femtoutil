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


#ifdef K_INCLUDE_BUILTINAPI

/* ------------------------------------------------------------------------ */
/* [new] */

//## @Virtual method This Object.new();

static METHOD Object_new(CTX ctx, knh_sfp_t *sfp _RIX)
{
	RETURN_(sfp[0].o);
}

/* ------------------------------------------------------------------------ */

//static void ObjectField_setValue(CTX ctx, knh_ObjectField_t *of, knh_index_t idx, knh_type_t type, Object *value)
//{
//	knh_class_t tcid = CLASS_t(type);
//	knh_class_t scid = O_cid(value);
//	DBG_ASSERT_cid(tcid);
//	DBG_ASSERT_cid(scid);
//	if(scid == tcid || knh_class_instanceof(ctx, scid, tcid)) {
//		goto L_SETVAL;
//	}
//	DBG_P("COERCION %s -> %s", CLASS__(scid), CLASS__(tcid));
//	TODO();
////		BEGIN_LOCAL(ctx, lsfp);
////		KNH_LPUSH(ctx, o);
////		VM_MAP(ctx, tcid);
////		o = ctx->esp[0].o;
////		END_LOCAL(ctx, lsfp);
//	return ;
//
//	L_SETVAL:;
//	if(IS_Tint(type)) {
//		knh_int_t *data = (knh_int_t*)(of->fields + idx);
//		data[0] = N_toint(value);
//	}
//	else if(IS_Tfloat(type)) {
//		knh_float_t *data = (knh_float_t*)(of->fields + idx);
//		data[0] = N_tofloat(value);
//	}
//	else if(IS_Tbool(type)) {
//		knh_boolean_t *data = (knh_boolean_t*)(of->fields +idx);
//		data[0] = N_tobool(value);
//	}
//	else {
//		DBG_ASSERT(of->fields[idx] != NULL);
//		KNH_SETv(ctx, of->fields[idx], value);
//	}
//}

/* ------------------------------------------------------------------------ */
//## @Hidden method This Object.new:MAP(dynamic value, ...);

static METHOD Object_newMAP(CTX ctx, knh_sfp_t *sfp _RIX)
{
//	knh_ObjectField_t *of = (knh_ObjectField_t*)sfp[0].o;
//	knh_class_t cid = O_cid(of);
//	knh_sfp_t *v = sfp + 1;
//	size_t i, ac = knh_stack_argc(ctx, v);
//	for(i = 0; i < ac; i+= 2) {
//		if(IS_bString(v[i].s)) {
//			knh_fieldn_t fn = knh_getfnq(ctx, S_tobytes(v[i].s), FN_NONAME);
//			if(fn == FN_NONAME) continue;
//			knh_index_t idx = knh_Class_queryField(ctx, cid, fn);
//			if(idx == -1) continue;
//			knh_fields_t *cf = knh_Class_fieldAt(ctx, cid, idx);
//			knh_type_t type = knh_type_tocid(ctx, cf->type, cid);
//			if(type == TYPE_void) continue;
//			DBG_P("[%d] %s %s", (int)(idx), TYPE__(type), S_tochar(v[i].s));
//			ObjectField_setValue(ctx, of, idx, type, v[i+1].o);
//		}
//	}
//	RETURN_(of);
}

/* ------------------------------------------------------------------------ */
//## method Bytes Bytes.new(Int init);

static METHOD Bytes_new(CTX ctx, knh_sfp_t *sfp _RIX)
{
	knh_Bytes_t *ba = sfp[0].ba;
	size_t init = sfp[1].ivalue <= 0 ? 0 : k_goodsize(Int_to(size_t, sfp[1]));
	DBG_ASSERT(ba->dim->capacity == 0);
	ba->bu.ubuf = (knh_uchar_t*)KNH_MALLOC(ctx, init);
	knh_bzero(ba->bu.ubuf, init);
	ba->dim = new_dim(ctx, init, 1);
	ba->bu.len = 0;
	RETURN_(ba);
}

/* ------------------------------------------------------------------------ */
//## method Bytes Bytes.new:ARRAY(Int init);

static METHOD Bytes_newARRAY(CTX ctx, knh_sfp_t *sfp _RIX)
{
	size_t size = sfp[1].ivalue <= 0 ? 0 : Int_to(size_t, sfp[1]);
	Bytes_new(ctx, sfp, rix);
	sfp[rix].ba->bu.len = size;
}

/* ------------------------------------------------------------------------ */
//## method String String.new(Bytes buf, StringDecoder c);

static METHOD String_new(CTX ctx, knh_sfp_t *sfp _RIX)
{
	knh_String_t *s;
	if(IS_NULL(sfp[2].o)) {
		s = new_S(ctx, BA_tobytes(sfp[1].ba));
	}
	else {
		knh_cwb_t cwbbuf, *cwb = knh_cwb_open(ctx, &cwbbuf);
		knh_StringDecoder_t *c = (knh_StringDecoder_t*)sfp[2].o;
		c->dspi->dec(ctx, c->conv, BA_tobytes(sfp[1].ba), cwb->ba);
		s = knh_cwb_newString(ctx, cwb);
	}
	RETURN_(s);
}

/* ------------------------------------------------------------------------ */
//## @Const method Regex Regex.new(String pattern, String option);

static METHOD Regex_new(CTX ctx, knh_sfp_t *sfp _RIX)
{
	knh_Regex_t *re = (knh_Regex_t*)sfp[0].o;
	const char *ptn = S_tochar(sfp[1].s);
	const char *opt = IS_NULL(sfp[2].o) ? "" : S_tochar(sfp[2].s);
	KNH_SETv(ctx, re->pattern, sfp[1].s);
	re->spi = knh_getRegexSPI();
	re->reg = re->spi->regmalloc(ctx, sfp[1].s);
	re->spi->regcomp(ctx, re->reg, ptn, re->spi->parse_cflags(ctx, opt));
	re->eflags = re->spi->parse_eflags(ctx, opt);
	RETURN_(sfp[0].o);
}

/* ------------------------------------------------------------------------ */
//## @Const mapper String Regex!;

static METHOD String_Regex(CTX ctx, knh_sfp_t *sfp _RIX)
{
	knh_Regex_t *re = new_(Regex);
	const char *ptn = S_tochar(sfp[0].s);
	KNH_SETv(ctx, re->pattern, sfp[0].s);
	re->reg = re->spi->regmalloc(ctx, sfp[0].s);
	re->spi->regcomp(ctx, re->reg, ptn, 0);
	re->eflags = 0;
	RETURN_(re);
}

/* ------------------------------------------------------------------------ */
/* [Pair, Tuple, Range] */

//## @Hidden @Const method This Tuple.new:TUPLE(dynamic value, ...);

static METHOD Tuple_newTUPLE(CTX ctx, knh_sfp_t *sfp _RIX)
{
	knh_Tuple_t *tpl = (knh_Tuple_t*)sfp[0].o;
	knh_sfp_t *v = sfp + 1;
	size_t i, ti = 0, ac = knh_stack_argc(ctx, v);
	knh_fields_t *tf = O_cTBL(tpl)->fields;
	for(i = 0; i < ac; i++) {
		if(tf[ti].israw == 1) {
			knh_ndata_t *n = (knh_ndata_t*)(tpl->fields + ti);
			n[0] = v[i].ndata;
			ti++;
			DBLNDATA_(ti++);
		}
		else {
			KNH_SETv(ctx, tpl->fields[ti], v[i].o);
			ti++;
		}
	}
	RETURN_(tpl);
}

/* ------------------------------------------------------------------------ */
//## @Hidden @Const method This Range.new:LIST(T1 start, T1 end);

static METHOD Range_newLIST(CTX ctx, knh_sfp_t *sfp _RIX)
{
	knh_Range_t *rng = (knh_Range_t*)sfp[0].o;
	if(Range_isNDATA(rng)) {
		rng->nstart = sfp[1].ndata;
		rng->nend =   sfp[2].ndata;
	}
	else {
		KNH_SETv(ctx, rng->ostart, sfp[1].o);
		KNH_SETv(ctx, rng->oend, sfp[2].o);
	}
	RETURN_(rng);
}

/* ------------------------------------------------------------------------ */
/* [Array] */

//## method This Array.new(Int initCapacity);

static METHOD Array_new(CTX ctx, knh_sfp_t *sfp _RIX)
{
	knh_Array_grow(ctx, sfp[0].a, Int_to(size_t, sfp[1]), 0);
	DBG_ASSERT((sfp[0].a)->size == 0);
	RETURN_(sfp[0].a);
}

/* ------------------------------------------------------------------------ */
//## @Hidden method This Array.new:ARRAY(Int size, ...);

static METHOD Array_newARRAY(CTX ctx, knh_sfp_t *sfp _RIX)
{
	knh_Array_t *a = sfp[0].a;
	knh_int_t init, x = 0, y = 1, z = 1, w = 1;
	size_t dim = knh_stack_argc(ctx, sfp+1);
	switch(dim) {
		case 4: w = sfp[4].ivalue;
		case 3: z = sfp[3].ivalue;
		case 2: y = sfp[2].ivalue;
		case 1: x = sfp[1].ivalue;
	}
	init = x * y * z * w;
	if(0 <= init && init < LONG_MAX) {
		knh_Array_grow(ctx, a, (size_t)init, 0);
		a->size = (size_t)init;
		if(!Array_isNDATA(a)) {
			knh_class_t p1 = O_p1(a);
			Object *v = knh_getClassDefaultValue(ctx, p1);
			size_t i;
			for(i = 0; i < a->size; i++) {
				KNH_INITv(a->list[i], v);
			}
		}
	}
	else if(unlikely(init < 0)) {
		THROW_OutOfRange(ctx, sfp, init, 0);
	}
	if(init > 0 && dim > 1) {
		((knh_dim_t*)a->dim)->dim = dim;
		((knh_dim_t*)a->dim)->x = (size_t)x;
		((knh_dim_t*)a->dim)->xy = (size_t)(x * y);
		((knh_dim_t*)a->dim)->xyz = (size_t)(x * y * z);
	}
	RETURN_(a);
}

/* ------------------------------------------------------------------------ */
//## @Hidden method This Array.new:LIST(T1 value, ...);

static METHOD Array_newLIST(CTX ctx, knh_sfp_t *sfp _RIX)
{
	knh_Array_t *a = sfp[0].a;
	a->api->add(ctx, a, sfp+1);
	RETURN_(a);
}

/* ------------------------------------------------------------------------ */
//## method This Map.new(Int init, String path, NameSpace ns);

static METHOD Map_new(CTX ctx, knh_sfp_t *sfp _RIX)
{
	knh_Map_t *m = (knh_Map_t *)sfp[0].o;
	size_t init = sfp[1].ivalue <= 0 ? 0: Int_to(size_t, sfp[1]);
	knh_bytes_t path = S_tobytes(sfp[2].s);
	knh_class_t cid = O_cid(m);
	const knh_MapDSPI_t *dspi = knh_NameSpace_getMapDSPI(ctx, sfp[3].ns, path);
	m->dspi = dspi->config(ctx, knh_class_p1(cid), knh_class_p2(cid));
	m->map = m->dspi->init(ctx, init, path.text, NULL);
	RETURN_(m);
}

/* ------------------------------------------------------------------------ */
//## @Hidden method This Map.new:MAP(T1 value, ...);

static METHOD Map_newMAP(CTX ctx, knh_sfp_t *sfp _RIX)
{
	knh_Map_t *m = sfp[0].m;
	knh_sfp_t *v = sfp + 1;
	size_t i, ac = knh_stack_argc(ctx, v);
	m->dspi = knh_getDefaultMapDSPI(ctx, O_p1(m), O_p2(m));
	KNH_ASSERT(m->dspi != NULL); // if NULL, it is unsupported
	m->map  = m->dspi->init(ctx, 0, NULL, NULL);
	for(i = 0; i < ac; i+=2) {
		m->dspi->set(ctx, m->map, v + i);
	}
	RETURN_(m);
}

/* ------------------------------------------------------------------------ */
//## method Exception! Exception.new(String event, String msg, dynamic bag);

static METHOD Exception_new(CTX ctx, knh_sfp_t *sfp _RIX)
{
	Exception_setup(ctx, sfp[0].e, sfp[1].s, sfp[2].s, sfp[3].o);
	RETURN_(sfp[0].e);
}

/* ------------------------------------------------------------------------ */
//## method This Func.new(dynamic base, Method method);

static METHOD Func_new(CTX ctx, knh_sfp_t *sfp _RIX)
{
	knh_Func_t   *fo = sfp[0].fo;
	knh_Method_t *mtd = sfp[2].mtd;
	KNH_SETv(ctx, (fo)->mtd, sfp[2].mtd);
	if(!Method_isStatic(mtd)) {
		KNH_INITv((fo)->baseNULL, sfp[1].o);
	}
	//(sfp[0].cc)->envsfp = NULL;
	RETURN_(fo);
}

/* ------------------------------------------------------------------------ */
//## @Const method Assurance Assurance.new(String msg);

static METHOD Assurance_new(CTX ctx, knh_sfp_t *sfp _RIX)
{
	knh_Assurance_t *Assurance = sfp[0].Assurance;
	KNH_SETv(ctx, Assurance->msg, sfp[1].s);
	RETURN_(sfp[0].o);
}

/* ------------------------------------------------------------------------ */
/* [Operator] */

/* ------------------------------------------------------------------------ */
//## @Const method Int Object.opADDR();

static METHOD Object_opADDR(CTX ctx, knh_sfp_t *sfp _RIX)
{
	void *p = NULL;
	if(IS_Boolean(sfp[0].o)) {
		p = (sfp[0].bvalue) ? (void*)(KNH_TRUE) : (void*)(KNH_FALSE);
	}
	else {
		p = (void*)sfp[0].o;
	}
	RETURNi_((knh_uintptr_t)p);
}

/* ------------------------------------------------------------------------ */
//## @Const method Boolean Object.opOF(Class c);

static METHOD Object_opOF(CTX ctx, knh_sfp_t *sfp _RIX)
{
	knh_type_t reqt = Class_tocid(sfp[1]);
	DBG_P("cid=%s", O__(sfp[0].o));
	RETURNb_(ClassTBL_isa(O_cTBL(sfp[0].o), reqt));
}

/* ------------------------------------------------------------------------ */
//## @Const method Boolean Object.opEQ(dynamic value);

static METHOD Object_opEQ(CTX ctx, knh_sfp_t *sfp _RIX)
{
	RETURNb_(knh_Object_compareTo(sfp[0].o, sfp[1].o) == 0);
}

/* ------------------------------------------------------------------------ */
//## @Const method Boolean Object.opNOTEQ(dynamic value);

static METHOD Object_opNOTEQ(CTX ctx, knh_sfp_t *sfp _RIX)
{
	RETURNb_(knh_Object_compareTo(sfp[0].o, sfp[1].o) != 0);
}

/* ------------------------------------------------------------------------ */
//## @Const method Boolean Object.opLT(dynamic value);

static METHOD Object_opLT(CTX ctx, knh_sfp_t *sfp _RIX)
{
	RETURNb_(knh_Object_compareTo(sfp[0].o, sfp[1].o) < 0);
}

/* ------------------------------------------------------------------------ */
//## @Const method Boolean Object.opLTE(dynamic value);

static METHOD Object_opLTE(CTX ctx, knh_sfp_t *sfp _RIX)
{
	RETURNb_(knh_Object_compareTo(sfp[0].o, sfp[1].o) <= 0);
}

/* ------------------------------------------------------------------------ */
//## @Const method Boolean Object.opGT(dynamic value);

static METHOD Object_opGT(CTX ctx, knh_sfp_t *sfp _RIX)
{
	RETURNb_(knh_Object_compareTo(sfp[0].o, sfp[1].o) > 0);
}

/* ------------------------------------------------------------------------ */
//## @Const method Boolean Object.opGTE(dynamic value);

static METHOD Object_opGTE(CTX ctx, knh_sfp_t *sfp _RIX)
{
	RETURNb_(knh_Object_compareTo(sfp[0].o, sfp[1].o) >= 0);
}

/* ------------------------------------------------------------------------ */
//## @Const method Boolean Int.opEQ(Int value);

static METHOD Int_opEQ(CTX ctx, knh_sfp_t *sfp _RIX)
{
	RETURNb_(sfp[0].ivalue == sfp[1].ivalue);
}

/* ------------------------------------------------------------------------ */
//## @Const  method Boolean Int.opNOTEQ(Int value);

static METHOD Int_opNOTEQ(CTX ctx, knh_sfp_t *sfp _RIX)
{
	RETURNb_(sfp[0].ivalue != sfp[1].ivalue);
}

/* ------------------------------------------------------------------------ */
//## @Const method Boolean Int.opLT(Int value);

static METHOD Int_opLT(CTX ctx, knh_sfp_t *sfp _RIX)
{
	RETURNb_(sfp[0].ivalue < sfp[1].ivalue);
}

/* ------------------------------------------------------------------------ */
//## @Const method Boolean Int.opLTE(Int value);

static METHOD Int_opLTE(CTX ctx, knh_sfp_t *sfp _RIX)
{
	RETURNb_(sfp[0].ivalue <= sfp[1].ivalue);
}

/* ------------------------------------------------------------------------ */
//## @Const method Boolean Int.opGT(Int value);

static METHOD Int_opGT(CTX ctx, knh_sfp_t *sfp _RIX)
{
	RETURNb_(sfp[0].ivalue > sfp[1].ivalue);
}

/* ------------------------------------------------------------------------ */
//## @Const method Boolean Int.opGTE(Int value);

static METHOD Int_opGTE(CTX ctx, knh_sfp_t *sfp _RIX)
{
	RETURNb_(sfp[0].ivalue >= sfp[1].ivalue);
}

/* ------------------------------------------------------------------------ */
//## @Const method Boolean Float.opEQ(Float value);

static METHOD Float_opEQ(CTX ctx, knh_sfp_t *sfp _RIX)
{
	RETURNb_(sfp[0].fvalue == sfp[1].fvalue);
}

/* ------------------------------------------------------------------------ */
//## @Const method Boolean Float.opNOTEQ(Float value);

static METHOD Float_opNOTEQ(CTX ctx, knh_sfp_t *sfp _RIX)
{
	RETURNb_(sfp[0].fvalue != sfp[1].fvalue);
}

/* ------------------------------------------------------------------------ */
//## @Const method Boolean Float.opLT(Float! value);

static METHOD Float_opLT(CTX ctx, knh_sfp_t *sfp _RIX)
{
	RETURNb_(sfp[0].fvalue < sfp[1].fvalue);
}

/* ------------------------------------------------------------------------ */
//## @Const method Boolean Float.opLTE(Float! value);

static METHOD Float_opLTE(CTX ctx, knh_sfp_t *sfp _RIX)
{
	RETURNb_(sfp[0].fvalue <= sfp[1].fvalue);
}

/* ------------------------------------------------------------------------ */
//## @Const method Boolean Float.opGT(Float! value);

static METHOD Float_opGT(CTX ctx, knh_sfp_t *sfp _RIX)
{
	RETURNb_(sfp[0].fvalue > sfp[1].fvalue);
}

/* ------------------------------------------------------------------------ */
//## @Const method Boolean Float.opGTE(Float! value);

static METHOD Float_opGTE(CTX ctx, knh_sfp_t *sfp _RIX)
{
	RETURNb_(sfp[0].fvalue >= sfp[1].fvalue);
}

/* ------------------------------------------------------------------------ */
/* [opHAS] */

/* ------------------------------------------------------------------------ */
//## @Const method Boolean String.opHAS(String s);

static METHOD String_opHAS(CTX ctx, knh_sfp_t *sfp _RIX)
{
	RETURNb_(knh_bytes_indexOf(S_tobytes(sfp[0].s), S_tobytes(sfp[1].s)) != -1);
}

///* ------------------------------------------------------------------------ */
////## method Boolean Range.opHAS(dynamic v);
//
//static METHOD Range_opHAS(CTX ctx, knh_sfp_t *sfp _RIX)
//{
//	int res = 0/*NotFound*/;
//	knh_Range_t *o = sfp[0].range;
//	if(IS_Int((o)->start) && IS_Float(sfp[1].o)) {
//		KNH_SETv(ctx, sfp[1].o, new_Int(ctx, CLASS_Int, (knh_int_t)sfp[1].fvalue));
//	}
//	else if(IS_Float((o)->start) && IS_Int(sfp[1].o)) {
//		KNH_SETv(ctx, sfp[1].o, new_Float(ctx, CLASS_Float, (knh_float_t)sfp[1].ivalue));
//	}
//	else {
//		knh_stack_boxing(ctx, sfp + 1);
//	}
//	if(knh_Object_compareTo(ctx, o->start, sfp[1].o) <= 0) {
//		if(Range_isInclusive(o)) {
//			if(knh_Object_compareTo(ctx, sfp[1].o, o->end) <= 0) {
//				res = 1;
//			}
//		}
//		else {
//			if(knh_Object_compareTo(ctx, sfp[1].o, o->end) < 0) {
//				res = 1;
//			}
//		}
//	}
//	RETURNb_(res);
//}

/* ------------------------------------------------------------------------ */
//## method Boolean Array.opHAS(dynamic v);

static METHOD Array_opHAS(CTX ctx, knh_sfp_t *sfp _RIX)
{
	knh_Array_t *a = (knh_Array_t*)sfp[0].o;
	size_t i, res = 0/*NotFound*/;
	if(Array_isNDATA(a)) {
		knh_ndata_t d = O_data(sfp[1].o);
		for(i = 0; i < knh_Array_size(a); i++) {
			if(d == a->nlist[i]) { res = 1; break; }
		}
	}
	else {
		for(i = 0; i < knh_Array_size(a); i++) {
			if(knh_Object_compareTo(a->list[i], sfp[1].o) == 0) {
				res = 1; break;
			}
		}
	}
	RETURNb_(res);
}

///* ------------------------------------------------------------------------ */
////## method Boolean Tuple.opHAS(dynamic v);

//static METHOD Tuple_opHAS(CTX ctx, knh_sfp_t *sfp _RIX)
//{
//	if(Tuple_isTriple(sfp[0].tuple)) {
//		knh_Tuple_t *t = sfp[0].tuple;
//		int res;
//		knh_stack_boxing(ctx, sfp + 1);
//		res = ((knh_Object_compareTo(ctx, t->first, sfp[1].o) == 0)
//				|| (knh_Object_compareTo(ctx, t->second, sfp[1].o) == 0)
//				|| (t->thirdNULL != NULL && (knh_Object_compareTo(ctx, t->thirdNULL, sfp[1].o) == 0)));
//		RETURNb_(res);
//	}
//	else {
//		Array_opHAS(ctx, sfp, rix); // reuse
//	}
//}

/* ------------------------------------------------------------------------ */
/* [Semantic Comparator] */

/* ------------------------------------------------------------------------ */
//## @Const method Boolean Object.opIS(dynamic v);

static METHOD Object_opIS(CTX ctx, knh_sfp_t *sfp _RIX)
{
//	knh_class_t scid = O_cid(sfp[0].o);
//	knh_class_t tcid = O_cid(sfp[1].o);
//	DBG_P("Semantic Matching %s === %s", CLASS__(scid), CLASS__(tcid));
//	if(scid == tcid) {
//		Object_opEQ(ctx, sfp. rix);
//	}
//	else if(IS_NULL(sfp[1].o)) {
//		RETURNb_(IS_NULL(sfp[0].o));
//	}
//	else
	{
		TODO();
		RETURNb_(0);
//		BEGIN_LOCAL(ctx, lsfp);
//		KNH_LPUSH(ctx, sfp[1].o);
//		VM_MAP(ctx, scid);
//		KNH_SETv(ctx, sfp[1].o, ctx->esp[0].o);
//		END_LOCAL(ctx, lsfp);
//		if(IS_NULL(sfp[1].o)) {
//			RETURNb_(0);
//		}
//		else {
//			Object_opEQ(ctx, sfp);
//		}
	}
}

/* ------------------------------------------------------------------------ */
/* [Arithemetic] */

/* ------------------------------------------------------------------------ */
/* [Int] */

//## @Const method Int Int.opADD(Int v);

static METHOD Int_opADD(CTX ctx, knh_sfp_t *sfp _RIX)
{
	RETURNi_(sfp[0].ivalue + sfp[1].ivalue);
}

/* ------------------------------------------------------------------------ */
//## @Const method Int Int.opNEG();

static METHOD Int_opNEG(CTX ctx, knh_sfp_t *sfp _RIX)
{
	RETURNi_(-(sfp[0].ivalue));
}

/* ------------------------------------------------------------------------ */
//## @Const method Int Int.opSUB(Int n);

static METHOD Int_opSUB(CTX ctx, knh_sfp_t *sfp _RIX)
{
	RETURNi_(sfp[0].ivalue - sfp[1].ivalue);
}

/* ------------------------------------------------------------------------ */
//## @Const method Int Int.opMUL(Int n);

static METHOD Int_opMUL(CTX ctx, knh_sfp_t *sfp _RIX)
{
	RETURNi_(sfp[0].ivalue * sfp[1].ivalue);
}

/* ------------------------------------------------------------------------ */
//## @Const method Int Int.opDIV(Int n);

static METHOD Int_opDIV(CTX ctx, knh_sfp_t *sfp _RIX)
{
	SYSLOG_iZERODIV(ctx, sfp, sfp[1].ivalue);
	RETURNi_(sfp[0].ivalue / sfp[1].ivalue);
}

/* ------------------------------------------------------------------------ */
//## @Const method Int Int.opMOD(Int n);

static METHOD Int_opMOD(CTX ctx, knh_sfp_t *sfp _RIX)
{
	SYSLOG_iZERODIV(ctx, sfp, sfp[1].ivalue);
	RETURNi_(sfp[0].ivalue % sfp[1].ivalue);
}

/* ------------------------------------------------------------------------ */
/* [Float] */
//## @Const method Float! Float.opADD(Float! v);

static METHOD Float_opADD(CTX ctx, knh_sfp_t *sfp _RIX)
{
	RETURNf_(sfp[0].fvalue + sfp[1].fvalue);
}

/* ------------------------------------------------------------------------ */
//## @Const method Float! Float.opNEG();

static METHOD Float_opNEG(CTX ctx, knh_sfp_t *sfp _RIX)
{
	RETURNf_(-(sfp[0].fvalue));
}

/* ------------------------------------------------------------------------ */
//## @Const method Float! Float.opSUB(Float! n);

static METHOD Float_opSUB(CTX ctx, knh_sfp_t *sfp _RIX)
{
	RETURNf_(sfp[0].fvalue - sfp[1].fvalue);
}

/* ------------------------------------------------------------------------ */
//## @Const method Float! Float.opMUL(Float! n);

static METHOD Float_opMUL(CTX ctx, knh_sfp_t *sfp _RIX)
{
	RETURNf_(sfp[0].fvalue * sfp[1].fvalue);
}

/* ------------------------------------------------------------------------ */
//## @Const method Float! Float.opDIV(Float! n);

static METHOD Float_opDIV(CTX ctx, knh_sfp_t *sfp _RIX)
{
	SYSLOG_fZERODIV(ctx, sfp, sfp[1].fvalue);
	RETURNf_(sfp[0].fvalue / sfp[1].fvalue);
}

/* ------------------------------------------------------------------------ */
//## @Const method String String.opSUB(String s);

static METHOD String_opSUB(CTX ctx, knh_sfp_t *sfp _RIX)
{
	knh_bytes_t base = S_tobytes(sfp[0].s);
	knh_bytes_t t = S_tobytes(sfp[1].s);
	knh_uchar_t c = t.utext[0];
	knh_cwb_t cwbbuf, *cwb = knh_cwb_open(ctx, &cwbbuf);
	size_t i;
	for(i = 0; i < base.len; i++) {
		if(base.utext[i] == c) {
			size_t j;
			for(j = 1; j < t.len; j++) {
				if(base.utext[i+j] != t.utext[j]) break;
			}
			if(j == t.len) {
				i += t.len - 1;
				continue;
			}
		}
		knh_Bytes_putc(ctx, cwb->ba, base.utext[i]);
	}
	if(base.len == knh_cwb_size(cwb)) {
		knh_cwb_close(cwb);
		RETURN_(sfp[0].o);
	}
	else {
		RETURN_(knh_cwb_newString(ctx, cwb));
	}
}

/* ------------------------------------------------------------------------ */
/* [dynamic] */

//## method dynamic dynamic.opADD(dynamic value);
//## method dynamic dynamic.opSUB(dynamic value);
//## method dynamic dynamic.opMUL(dynamic value);
//## method dynamic dynamic.opDIV(dynamic value);
//## method dynamic dynamic.opMOD(dynamic value);
//## method dynamic dynamic.opNEG();

static METHOD dynamic_opADD(CTX ctx, knh_sfp_t *sfp _RIX)
{
	knh_Method_t *mtd = sfp[K_MTDIDX].mtdNC;
	mtd = ClassTBL_getMethod(ctx, O_cTBL(sfp[0].o), (mtd)->mn);
	klr_setmtdNC(ctx, sfp[K_MTDIDX], mtd);
	knh_stack_typecheck(ctx, sfp, mtd, NULL);
	(sfp[K_MTDIDX].mtdNC)->fcall_1(ctx, sfp, rix);
}

/* ------------------------------------------------------------------------ */
/* [Logical] */

/* ------------------------------------------------------------------------ */
//## @Const method Boolean Boolean.opNOT();

static METHOD Boolean_opNOT(CTX ctx, knh_sfp_t *sfp _RIX)
{
	RETURNb_(!sfp[0].bvalue);
}

/* ------------------------------------------------------------------------ */
//## @Const method Int Int.opLAND(Int n, ...);

static METHOD Int_opLAND(CTX ctx, knh_sfp_t *sfp _RIX)
{
	size_t i, ac = knh_stack_argc(ctx, sfp);
	knh_int_t n = sfp[0].ivalue;
	for(i = 1; i < ac; i++) {
		n = n & sfp[i].ivalue;
	}
	RETURNi_(n);
}

/* ------------------------------------------------------------------------ */
//## @Const method Int Int.opLOR(Int n, ...);

static METHOD Int_opLOR(CTX ctx, knh_sfp_t *sfp _RIX)
{
	size_t i, ac = knh_stack_argc(ctx, sfp);
	knh_int_t n = sfp[0].ivalue;
	for(i = 1; i < ac; i++) {
		n = n | sfp[i].ivalue;
	}
	RETURNi_(n);
}

/* ------------------------------------------------------------------------ */
//## @Const method Int Int.opLXOR(Int n);

static METHOD Int_opLXOR(CTX ctx, knh_sfp_t *sfp _RIX)
{
	RETURNi_(sfp[0].ivalue ^ sfp[1].ivalue);
}

/* ------------------------------------------------------------------------ */
//## @Const method Int Int.opLNOT();

static METHOD Int_opLNOT(CTX ctx, knh_sfp_t *sfp _RIX)
{
	RETURNi_(~(sfp[0].ivalue));
}

/* ------------------------------------------------------------------------ */
//## @Const method Int Int.opLSFT(Int n);

static METHOD Int_opLSFT(CTX ctx, knh_sfp_t *sfp _RIX)
{
	RETURNi_(sfp[0].ivalue << sfp[1].ivalue);
}

/* ------------------------------------------------------------------------ */
//## @Const method Int Int.opRSFT(Int n);

static METHOD Int_opRSFT(CTX ctx, knh_sfp_t *sfp _RIX)
{
	RETURNi_(sfp[0].ivalue >> sfp[1].ivalue);
}

/* ------------------------------------------------------------------------ */
/* [getSize] */

/* ------------------------------------------------------------------------ */
//## method Int Bytes.getSize();
//## method Int Array.getSize();

METHOD Bytes_getSize(CTX ctx, knh_sfp_t *sfp _RIX)
{
	//fprintf(stderr, "** rix=%ld sfp[rix]=%p\n", rix, sfp + rix);
	RETURNi_((sfp[0].ba)->bu.len);
}

/* ------------------------------------------------------------------------ */
//## method Int Tuple.getSize();

METHOD Tuple_getSize(CTX ctx, knh_sfp_t *sfp _RIX)
{
	RETURNi_(O_cTBL(sfp[0].o)->cparam->psize);
}

/* ------------------------------------------------------------------------ */
//## method @Const Int String.getSize();

static METHOD String_getSize(CTX ctx, knh_sfp_t *sfp _RIX)
{
	size_t size = IS_bString(sfp[0].s) ? S_size(sfp[0].s) : 0;
	if(!String_isASCII(sfp[0].s)) {
		size = knh_bytes_mlen(S_tobytes(sfp[0].s));
	}
	RETURNi_(size);
}

/* ------------------------------------------------------------------------ */
//## method Int Map.getSize();
METHOD Map_getSize(CTX ctx, knh_sfp_t *sfp _RIX)
{
	knh_Map_t *m = sfp[0].m;
	RETURNi_(m->dspi->size(ctx, m->map));
}

///* ------------------------------------------------------------------------ */
////## method Int Tuple.getSize();
//
//static METHOD Tuple_getSize(CTX ctx, knh_sfp_t *sfp _RIX)
//{
//	size_t size = (IS_NULL(sfp[0].o)) ? 0 : 2;
//	if(!Tuple_isTriple(sfp[0].tuple)) size = (sfp[0].tuple)->size;
//	RETURNi_(size);
//}

/* ------------------------------------------------------------------------ */
/* [get,set] */

/* ------------------------------------------------------------------------ */
//## method Int Bytes.get(Int n);

static METHOD Bytes_get(CTX ctx, knh_sfp_t *sfp _RIX)
{
	knh_Bytes_t *ba = sfp[0].ba;
	size_t n2 = knh_array_index(ctx, sfp, Int_to(size_t, sfp[1]), ba->bu.len);
	RETURNi_(ba->bu.utext[n2]);
}

/* ------------------------------------------------------------------------ */
//## method Int Bytes.set(Int n, Int c);

static METHOD Bytes_set(CTX ctx, knh_sfp_t *sfp _RIX)
{
	knh_Bytes_t *ba = sfp[0].ba;
	size_t n2 = knh_array_index(ctx, sfp, Int_to(size_t, sfp[1]), ba->bu.len);
	ba->bu.ubuf[n2] = Int_to(knh_uchar_t, sfp[2]);
	RETURNi_(ba->bu.utext[n2]);
}

/* ------------------------------------------------------------------------ */
//## method void Bytes.setAll(Int c);

static METHOD Bytes_setAll(CTX ctx, knh_sfp_t *sfp _RIX)
{
	knh_Bytes_t *ba = sfp[0].ba;
	size_t i, n = Int_to(size_t, sfp[1]);
	for(i = 0; i < ba->bu.len; i++) {
		ba->bu.ubuf[i] = n;
	}
	RETURNvoid_();
}

/* ------------------------------------------------------------------------ */
//## @Const method String String.get(Int n);

static METHOD String_get(CTX ctx, knh_sfp_t *sfp _RIX)
{
	knh_bytes_t base = S_tobytes(sfp[0].s);
	knh_String_t *s;
	if(String_isASCII(sfp[0].s)) {
		size_t n = knh_array_index(ctx, sfp, Int_to(size_t, sfp[1]), S_size(sfp[0].s));
		base.utext = base.utext + n;
		base.len = 1;
		s = new_String_(ctx, CLASS_String, base, sfp[0].s);
	}
	else {
		size_t off = knh_array_index(ctx, sfp, Int_to(size_t, sfp[1]), knh_bytes_mlen(base));
		knh_bytes_t sub = knh_bytes_mofflen(base, off, 1);
		s = new_String_(ctx, CLASS_String, sub, sfp[0].s);
	}
	RETURN_(s);
}

///* ------------------------------------------------------------------------ */
////## method dynamic Tuple.get(Int n);
//
//static METHOD Tuple_get(CTX ctx, knh_sfp_t *sfp _RIX)
//{
//	if(Tuple_isTriple(sfp[0].tuple)) {
//		Object *v = KNH_NULL;
//		switch(Int_to(size_t, sfp[1])) {
//		case 0: v = (sfp[0].tuple)->first; break;
//		case 1: v = (sfp[1].tuple)->first; break;
//		case 2: v = (sfp[2].tuple)->first; break;
//		default: knh_array_index(ctx, Int_to(size_t, sfp[1]), 3);
//		}
//		RETURN_(v);
//	}
//	else {
//		Array_get(ctx, sfp, rix); /* reuse */
//	}
//}

/* ------------------------------------------------------------------------ */
/* [range] */

static void _rangeUNTIL(CTX ctx, knh_sfp_t *sfp, size_t size, size_t *s, size_t *e)
{
	*s = sfp[1].ivalue == 0 ? 0 : knh_array_index(ctx, sfp, Int_to(size_t, sfp[1]), size);
	*e = sfp[2].ivalue == 0 ? (size) : knh_array_index(ctx, sfp, Int_to(size_t, sfp[2]), size);
}

static void _rangeTO(CTX ctx, knh_sfp_t *sfp, size_t size, size_t *s, size_t *e)
{
	*s = sfp[1].ivalue == 0 ? 0 : knh_array_index(ctx, sfp, Int_to(size_t, sfp[1]), size);
	*e = sfp[2].ivalue == 0 ? (size) : knh_array_index(ctx, sfp, Int_to(size_t, sfp[2]), size) + 1;
}

/* ------------------------------------------------------------------------ */

static knh_Bytes_t *new_BytesRANGE(CTX ctx, knh_Bytes_t *ba, size_t s, size_t e)
{
	knh_Bytes_t *newa = new_(Bytes);
	if(e < s) {
		size_t t = s; s = e; e = t;
	}
	DBG_ASSERT(e <= BA_size(ba));
	if(s < e) {
		size_t newsize = e - s;
		size_t capacity = newsize;
		if(newsize > 0) {
			if(capacity < 256) capacity = 256;
			newa->bu.ubuf = (knh_uchar_t*)KNH_MALLOC(ctx, capacity);
			knh_bzero(newa->bu.ubuf, capacity);
			knh_memcpy(newa->bu.ubuf, ba->bu.utext + s, newsize);
		}
		else {
			newa->bu.ubuf = NULL;
		}
		newa->dim = new_dim(ctx, capacity, 1);
		newa->bu.len = newsize;
	}
	return newa;
}

/* ------------------------------------------------------------------------ */
//## method Bytes Bytes.opUNTIL(Int s, Int e);

static METHOD Bytes_opUNTIL(CTX ctx, knh_sfp_t *sfp _RIX)
{
	size_t s, e;
	_rangeUNTIL(ctx, sfp, (sfp[0].ba)->bu.len, &s, &e);
	RETURN_(new_BytesRANGE(ctx, sfp[0].ba, s, e));
}

/* ------------------------------------------------------------------------ */
//## method Bytes Bytes.opTO(Int s, Int e);

static METHOD Bytes_opTO(CTX ctx, knh_sfp_t *sfp _RIX)
{
	size_t s, e;
	_rangeTO(ctx, sfp, (sfp[0].ba)->bu.len, &s, &e);
	RETURN_(new_BytesRANGE(ctx, sfp[0].ba, s, e));
}

/* ------------------------------------------------------------------------ */
//## @Const method String String.substring(Int offset, Int length);

static METHOD String_substring(CTX ctx, knh_sfp_t *sfp _RIX)
{
	knh_String_t *s;
	knh_bytes_t base = S_tobytes(sfp[0].s);
	knh_bytes_t t;
	if(String_isASCII(sfp[0].s)) {
		size_t offset = knh_array_index(ctx, sfp, (sfp[1].ivalue), base.len);
		t = knh_bytes_last(base, offset);
		if(sfp[2].ivalue != 0) {
			size_t len = (size_t)sfp[2].ivalue;
			if(len < t.len) t = knh_bytes_first(t, len);
		}
	}
	else { // multibytes
		size_t mlen = knh_bytes_mlen(base);
		size_t offset = knh_array_index(ctx, sfp, (sfp[1].ivalue), mlen);
		size_t length = sfp[2].ivalue == 0  ? (mlen - offset) : (size_t)sfp[2].ivalue;
		t = knh_bytes_mofflen(base, offset, length);
	}
	s = new_String_(ctx, CLASS_String, t, sfp[0].s);
	RETURN_(s);
}

/* ------------------------------------------------------------------------ */
//## @Const method String String.opUNTIL(Int s, Int e);

static METHOD String_opUNTIL(CTX ctx, knh_sfp_t *sfp _RIX)
{
	knh_bytes_t t = S_tobytes(sfp[0].s);
	if(sfp[2].ivalue != 0) {
		if(!String_isASCII(sfp[0].s)) {
			size_t mlen = knh_bytes_mlen(t);
			size_t offset = knh_array_index(ctx, sfp, (sfp[1].ivalue), mlen);
			size_t length = knh_array_index(ctx, sfp, (sfp[2].ivalue), mlen) - offset;
			t = knh_bytes_mofflen(t, offset, length);
			RETURN_(new_String_(ctx, CLASS_String, t, sfp[0].s));
		}
		else {
			size_t offset = Int_to(size_t, sfp[1]);
			sfp[2].ivalue = knh_array_index(ctx, sfp, Int_to(knh_intptr_t, sfp[2]), (sfp[0].s)->str.len) - offset;
		}
	}
	String_substring(ctx, sfp, rix);
}

/* ------------------------------------------------------------------------ */
//## @Const method String String.opTO(Int s, Int e);

static METHOD String_opTO(CTX ctx, knh_sfp_t *sfp _RIX)
{
	knh_bytes_t t = S_tobytes(sfp[0].s);
	if(sfp[2].ivalue != 0) {
		if(!String_isASCII(sfp[0].s)) {
			size_t mlen = knh_bytes_mlen(t);
			size_t offset = knh_array_index(ctx, sfp, (sfp[1].ivalue), mlen);
			size_t length = knh_array_index(ctx, sfp, (sfp[2].ivalue), mlen) - offset + 1;
			t = knh_bytes_mofflen(t, offset, length);
			RETURN_(new_String_(ctx, CLASS_String, t, sfp[0].s));
		}
		else {
			size_t offset = Int_to(size_t, sfp[1]);
			sfp[2].ivalue = knh_array_index(ctx, sfp, Int_to(knh_intptr_t, sfp[2]), (sfp[0].s)->str.len) - offset + 1;
		}
	}
	String_substring(ctx, sfp, rix);
}

/* ------------------------------------------------------------------------ */

static knh_Array_t *new_ArrayRANGE(CTX ctx, knh_Array_t *a, size_t s, size_t e /*until*/)
{
	knh_Array_t *newa = new_O(Array, O_cid(a));
	if(e < s) {
		size_t t = s; s = e; e = t;
	}
	if(s < e) {
		DBG_ASSERT(e <= a->size);
		size_t newsize = e - s;
		size_t sizeofdata = a->dim->wsize;
		if(newsize > 0) {
			char *p = (char*)a->list;
			knh_Array_grow(ctx, newa, newsize, newsize);
			knh_memcpy(newa->list, p + (s * sizeofdata), newsize * sizeofdata);
			if(!Array_isNDATA(a)) {
				size_t i;
				for(i = 0; i < newsize; i++) {
					knh_Object_RCinc(newa->list[i]);
				}
			}
		}
		newa->size = newsize;
	}
	return newa;
}

/* ------------------------------------------------------------------------ */
//## method This Array.opUNTIL(Int s, Int e);

static METHOD Array_opUNTIL(CTX ctx, knh_sfp_t *sfp _RIX)
{
	size_t s, e;
	_rangeUNTIL(ctx, sfp, knh_Array_size(sfp[0].a), &s, &e);
	RETURN_(new_ArrayRANGE(ctx, sfp[0].a, s, e));
}

/* ------------------------------------------------------------------------ */
//## method This Array.opTO(Int s, Int e);

static METHOD Array_opTO(CTX ctx, knh_sfp_t *sfp _RIX)
{
	size_t s, e;
	_rangeTO(ctx, sfp, knh_Array_size(sfp[0].a), &s, &e);
	RETURN_(new_ArrayRANGE(ctx, sfp[0].a, s, e));
}

#endif /*K_INCLUDE_BUILTINAPI*/

/* ------------------------------------------------------------------------ */

#ifdef __cplusplus
}
#endif
