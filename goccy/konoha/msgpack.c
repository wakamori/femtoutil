#include <konoha1.h>
#include <msgpack.h>
#include <stdio.h>

typedef struct _knh_MessagePack_t {
	msgpack_sbuffer *buf;
	msgpack_packer *pk;
	msgpack_unpacker *upk;
	msgpack_unpacked *result;
} knh_MessagePack_t;

static knh_MessagePack_t *new_knh_MessagePack()
{
	knh_MessagePack_t *mp = (knh_MessagePack_t *)malloc(sizeof(knh_MessagePack_t));
	memset(mp, 0, sizeof(knh_MessagePack_t));
	mp->buf = msgpack_sbuffer_new();
	mp->pk = msgpack_packer_new(mp->buf, msgpack_sbuffer_write);
	mp->upk = (msgpack_unpacker *)malloc(sizeof(msgpack_unpacker));
	mp->result = (msgpack_unpacked *)malloc(sizeof(msgpack_unpacked));
	return mp;
}

static void knh_MessagePack_setUnpakcer(CTX ctx, knh_MessagePack_t *mp)
{
	msgpack_unpacker_init(mp->upk, MSGPACK_UNPACKER_INIT_BUFFER_SIZE);
	msgpack_unpacker_reserve_buffer(mp->upk, mp->buf->size);
	memcpy(msgpack_unpacker_buffer(mp->upk), mp->buf->data, mp->buf->size);
	msgpack_unpacker_buffer_consumed(mp->upk, mp->buf->size);
	msgpack_unpacked_init(mp->result);
}

static knh_Object_t* knh_getObjectFromFieldIndex(CTX ctx, knh_class_t cid, knh_ObjectField_t *of, knh_index_t fldidx)
{
	knh_Object_t *o = KNH_NULL;
	const knh_ClassTBL_t *tbl = ClassTBL(cid);
	int i, len = tbl->fsize;
	for (i = 0; i < len; i++) {
		knh_fields_t *cur_fld = tbl->fields + i;
		if (cur_fld->fn == fldidx) {
			knh_index_t fidx = i;
			knh_class_t ftype = cur_fld->type;
			//o = knh_transObject_FromField(ctx, of, fidx, ftype);
			//KNH_SETv(ctx, o, of->fields[fldidx]);
			break;
		}
	}
	return o;
}

static void knh_MessagePack_addObject(CTX ctx, knh_MessagePack_t *mp, Object *obj)
{
	int cid = obj->h.cTBL->bcid;
	switch (cid) {
	case CLASS_Int:
		msgpack_pack_int64(mp->pk, ((knh_Int_t *)obj)->n.ivalue);
		break;
	case CLASS_Float:
		msgpack_pack_double(mp->pk, ((knh_Float_t *)obj)->n.fvalue);
		break;
	case CLASS_Boolean:
		(((knh_Boolean_t *)obj)->n.bvalue) ? msgpack_pack_true(mp->pk) : msgpack_pack_false(mp->pk);
		break;
	case CLASS_String: {
		knh_String_t *s = (knh_String_t *)obj;
		msgpack_pack_raw(mp->pk, s->str.len);
		msgpack_pack_raw_body(mp->pk, s->str.text, s->str.len);
		break;
	}
	case CLASS_Array: {
		knh_Array_t *a = (knh_Array_t *)obj;
		msgpack_pack_array(mp->pk, a->size);
		int i = 0;
		while (a->list[i] != NULL) {
			knh_MessagePack_addObject(ctx, mp, a->list[i]);
			i++;
		}
		break;
	}
	default: {
		//Object
		asm("int3");
		const knh_ClassTBL_t *tbl = obj->h.cTBL;
		int i = 0;
		for (i = 0; i < tbl->fsize - 1; i++) {
			knh_fields_t *field = tbl->fields + i;
			knh_String_t *key = knh_getFieldName(ctx, field->fn);
			fprintf(stderr, "key = [%s]\n", key->str.text);
			knh_DictSet_t *dset = DP(ctx->sys)->nameDictCaseSet;
			knh_bytes_t name = S_tobytes(key);
			knh_index_t fldidx = knh_DictSet_index(dset, name);
			if (fldidx != -1) {
				fldidx = knh_DictSet_valueAt(dset, fldidx) - 1;
				fldidx += MN_OPSIZE;
			}
			Object *o = knh_getObjectFromFieldIndex(ctx, tbl->cid, (knh_ObjectField_t *)obj, fldidx);
		}
		break;
	}
	}
}

static int knh_MessagePack_nextObject(CTX ctx, knh_MessagePack_t *mp)
{
	return msgpack_unpacker_next(mp->upk, mp->result);
}

static int knh_MessagePack_getObjectType(CTX ctx, knh_MessagePack_t *mp)
{
	int cid = 0;
	msgpack_object obj = mp->result->data;
	switch (obj.type) {
	case MSGPACK_OBJECT_POSITIVE_INTEGER:
	case MSGPACK_OBJECT_NEGATIVE_INTEGER:
		cid = CLASS_Int;
		break;
	case MSGPACK_OBJECT_BOOLEAN:
		cid = CLASS_Boolean;
		break;
	case MSGPACK_OBJECT_DOUBLE:
		cid = CLASS_Float;
		break;
	case MSGPACK_OBJECT_RAW:
		cid = CLASS_String;
		break;
	case MSGPACK_OBJECT_ARRAY:
		cid = CLASS_Array;
		break;
	default:
		break;
	}
	return cid;
}

static Object *knh_MessagePack_getObject(CTX ctx, knh_MessagePack_t *mp, int isRecursive)
{
	Object *o;
	msgpack_object obj;
	if (isRecursive) {
		obj = *(msgpack_object *)mp;
	} else {
		obj = mp->result->data;
	}
	switch (obj.type) {
	case MSGPACK_OBJECT_POSITIVE_INTEGER:
		o = (Object *)new_Int(ctx, CLASS_Int, obj.via.u64);
		fprintf(stderr, "[%d]\n", obj.via.u64);
		break;
	case MSGPACK_OBJECT_DOUBLE:
		o = (Object *)new_Float(ctx, CLASS_Float, obj.via.dec);
		fprintf(stderr, "[%f]\n", obj.via.dec);
		break;
	case MSGPACK_OBJECT_BOOLEAN:
		if (obj.via.boolean) {
			o = (Object *)new_Boolean(ctx, KNH_TRUE);
			fprintf(stderr, "[true]\n");
		} else {
			o = (Object *)new_Boolean(ctx, KNH_FALSE);
			fprintf(stderr, "[false]\n");
		}
		break;
	case MSGPACK_OBJECT_RAW:
		o = (Object *)new_String(ctx, obj.via.raw.ptr);
		fprintf(stderr, "[%s]\n", obj.via.raw.ptr);
		break;
	case MSGPACK_OBJECT_ARRAY:
		o = (Object *)new_Array(ctx, CLASS_Int, obj.via.array.size);
		fprintf(stderr, "[");
		if(obj.via.array.size != 0) {
			msgpack_object *p = obj.via.array.ptr;
			Object *elem = knh_MessagePack_getObject(ctx, (knh_MessagePack_t *)p, 1);
			knh_Array_add(ctx, (knh_Array_t *)o, elem);
			++p;
			const msgpack_object* pend = obj.via.array.ptr + obj.via.array.size;
			for (; p < pend; ++p) {
				elem = knh_MessagePack_getObject(ctx, (knh_MessagePack_t *)p, 1);
				knh_Array_add(ctx, (knh_Array_t *)o, elem);
			}
		}
		fprintf(stderr, "]\n");
		break;
	default:
		break;
	}
	return o;
}

static void knh_MessagePack_free(CTX ctx, knh_MessagePack_t *mp)
{
	msgpack_sbuffer_free(mp->buf);
	msgpack_packer_free(mp->pk);
	free(mp->upk);
	free(mp->result);
	free(mp);
}

int main(int argc, char **argv)
{
	konoha_t konoha = konoha_open(4096);
	CTX ctx = konoha.ctx;
	KONOHA_BEGIN(ctx);
	
	knh_MessagePack_t *mp = new_knh_MessagePack();
	knh_Int_t *i = (knh_Int_t *)new_Int(ctx, CLASS_Int, 3);
	knh_Float_t *f = (knh_Float_t *)new_Float(ctx, CLASS_Float, 3.14);
	knh_Boolean_t *b = (knh_Boolean_t *)new_Boolean(ctx, KNH_TRUE);
	knh_String_t *s = (knh_String_t *)new_String(ctx, "hello world");
	knh_Array_t *a = (knh_Array_t *)new_Array(ctx, CLASS_Int, 4);
	size_t j = 0;
	for (j = 0; j < 4; j++) {
		knh_Array_add(ctx, a, new_Int(ctx, CLASS_Int, j));
	}
	//asm("int3");
	const char *script = "class Person {\n"			\
		"    String name;\n"						\
		"    int age = 24;\n"						\
		"}\n";
	knh_String_t *knh_script = new_String(ctx, script);
	knh_InputStream_t *ins = new_StringInputStream(ctx, knh_script);
	knh_eval(ctx, ins, NULL);
	const knh_ClassTBL_t *t = (ctx->share->ClassTBL[K_CLASS_INITSIZE + 1]);
	Object *o = new_Object_init2(ctx, t);
	knh_MessagePack_addObject(ctx, mp, UPCAST(i));
	knh_MessagePack_addObject(ctx, mp, UPCAST(f));
	knh_MessagePack_addObject(ctx, mp, UPCAST(b));
	knh_MessagePack_addObject(ctx, mp, UPCAST(s));
	knh_MessagePack_addObject(ctx, mp, UPCAST(a));
	knh_MessagePack_addObject(ctx, mp, UPCAST(o));
	//=============> send mp
	//receive mp <==========
	knh_MessagePack_setUnpakcer(ctx, mp);
	while (knh_MessagePack_nextObject(ctx, mp)) {
		switch (knh_MessagePack_getObjectType(ctx, mp)) {
		case CLASS_Int: {
			knh_Int_t *o = (knh_Int_t *)knh_MessagePack_getObject(ctx, mp, 0);
			break;
		}
		case CLASS_Float: {
			knh_Float_t *o = (knh_Float_t *)knh_MessagePack_getObject(ctx, mp, 0);
			break;
		}
		case CLASS_Boolean: {
			knh_Boolean_t *o = (knh_Boolean_t *)knh_MessagePack_getObject(ctx, mp, 0);
			break;
		}
		case CLASS_String: {
			knh_String_t *o = (knh_String_t *)knh_MessagePack_getObject(ctx, mp, 0);
			break;
		}
		case CLASS_Array: {
			knh_Array_t *o = (knh_Array_t *)knh_MessagePack_getObject(ctx, mp, 0);
			break;
		}
		default:
			break;
		}
	}
	knh_MessagePack_free(ctx, mp);

	KONOHA_END(ctx);
	konoha_close(konoha);
	return 0;
}
