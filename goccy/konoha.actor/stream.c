#include <konoha1.h>

#define LIBNAME "libc"
//============================  MSGPACK  =============================//
#include <msgpack.h>

static void *knh_msgpack_init(CTX ctx, knh_packer_t *pk)
{
	pk->sbuffer = msgpack_sbuffer_new();
	pk->pk = msgpack_packer_new(pk->sbuffer, msgpack_sbuffer_write);
	return pk;
}

static void knh_msgpack_flushfree(CTX ctx, knh_packer_t *pk)
{
	msgpack_sbuffer *sbuffer = pk->sbuffer;
	pk->w->dspi->fwrite(ctx, DP(pk->w)->fio, sbuffer->data, sbuffer->size, NULL);
	fprintf(stderr, "send packing data!!\n");
}

static void knh_msgpack_null(CTX ctx, void *pk)
{

}

static void knh_msgpack_bool(CTX ctx, void *_pk, int b)
{
	knh_packer_t *pk = (knh_packer_t *)_pk;
	(b) ? msgpack_pack_true(pk->pk) : msgpack_pack_false(pk->pk);
	fprintf(stderr, " %d", b);
}

static void knh_msgpack_int(CTX ctx, void *_pk, knh_int_t i)
{
	knh_packer_t *pk = (knh_packer_t *)_pk;
	msgpack_pack_int64(pk->pk, i);
	fprintf(stderr, " %lld", i);
}

static void knh_msgpack_float(CTX ctx, void *_pk, knh_float_t f)
{
	knh_packer_t *pk = (knh_packer_t *)_pk;
	msgpack_pack_double(pk->pk, f);
	fprintf(stderr, " %f", f);
}

static void knh_msgpack_string(CTX ctx, void *_pk, const char *str, size_t len)
{
	knh_packer_t *pk = (knh_packer_t *)_pk;
	msgpack_pack_raw(pk->pk, len + 1);
	msgpack_pack_raw_body(pk->pk, str, len + 1);
	fprintf(stderr, " %s", str);
}

static void knh_msgpack_raw(CTX ctx, void *_pk, const char *str, size_t len)
{
	knh_packer_t *pk = (knh_packer_t *)_pk;
	msgpack_pack_raw(pk->pk, len + 1);
	msgpack_pack_raw_body(pk->pk, str, len + 1);
	fprintf(stderr, " %s", str);
}

static void knh_msgpack_putc(CTX ctx, void *pk, int ch)
{

}

static void knh_msgpack_array(CTX ctx, void *_pk, size_t array_size)
{
	knh_packer_t *pk = (knh_packer_t *)_pk;
	msgpack_pack_array(pk->pk, array_size);
	fprintf(stderr, "[ ");//, array_size);
}

static void knh_msgpack_map(CTX ctx, void *_pk, size_t map_size)
{
	knh_packer_t *pk = (knh_packer_t *)_pk;
	msgpack_pack_map(pk->pk, map_size);
	fprintf(stderr, "{ ");
}

#define MAX_MSG_BUFFER_SIZE 1024
static void knh_msgapck_setUnpacker(msgpack_unpacker *upk, msgpack_unpacked *result, char *buf)
{
	msgpack_unpacker_init(upk, MSGPACK_UNPACKER_INIT_BUFFER_SIZE);
	msgpack_unpacker_reserve_buffer(upk, MAX_MSG_BUFFER_SIZE);
	memcpy(msgpack_unpacker_buffer(upk), buf, MAX_MSG_BUFFER_SIZE);
	msgpack_unpacker_buffer_consumed(upk, MAX_MSG_BUFFER_SIZE);
	msgpack_unpacked_init(result);
}

static Object *knh_msgpack_getObject(CTX ctx, knh_ClassTBL_t *c, msgpack_unpacked *result, bool isRecursive);
static knh_Array_t *knh_msgpack_getArray(CTX ctx, knh_ClassTBL_t *c, msgpack_object_kv *map)
{
	knh_Array_t *o;
	//fprintf(stderr, "[");
	msgpack_object _value = map->val;
	knh_class_t cid = _value.via.u64;
	map++;
	//msgpack_object _key = map->key;
	//const char *key = _key.via.raw.ptr; //key is "abody"
	//fprintf(stderr, "key = [%s]\n", key);
	_value = map->val;
	msgpack_object *a = _value.via.array.ptr;
	size_t asize = _value.via.array.size;
	const msgpack_object *array_end = _value.via.array.ptr + asize;
	switch (cid) {
	case CLASS_Int: case CLASS_Float: case CLASS_Boolean:
	case CLASS_String:
		o = new_Array(ctx, cid, asize);
		break;
	default:
		//Object
		o = new_Array(ctx, c->p1, asize);
		c = (knh_ClassTBL_t *)ClassTBL(c->p1);
		break;
	}
	int i = 0;
	BEGIN_LOCAL(ctx, lsfp, 1);
	for (; a < array_end; a++) {
		Object *elem = knh_msgpack_getObject(ctx, c, (msgpack_unpacked *)a, 1);
		knh_class_t cid = O_cTBL(elem)->cid;
		switch (cid) {
		case CLASS_Int:
			lsfp[0].ivalue = ((knh_Int_t *)elem)->n.ivalue;
			o->api->add(ctx, o, lsfp);
			break;
		case CLASS_Float:
			lsfp[0].fvalue = ((knh_Float_t *)elem)->n.fvalue;
			o->api->add(ctx, o, lsfp);
			break;
		case CLASS_Boolean:
			lsfp[0].bvalue = ((knh_Boolean_t *)elem)->n.bvalue;
			o->api->add(ctx, o, lsfp);
			break;
		default:
			knh_Array_add(ctx, o, elem);
			break;
		}
		i++;
	}
	END_LOCAL_(ctx, lsfp);
	//fprintf(stderr, "]\n");
	return o;
}

typedef struct _knh_FieldInfo_t {
	int isFind;
	int flocate;
	int type;
} knh_FieldInfo_t;

static void knh_msgpack_setFieldInfo(CTX ctx, knh_FieldInfo_t *info, knh_ClassTBL_t *c, const char *key, Object *value)
{
	const knh_ClassTBL_t *tbl = c;
	int i = 0;
	int is32BIT = 4 / sizeof(void *);
	for (i = 0; i < tbl->fsize; i++) {
		knh_fields_t *field = tbl->fields + i;
		knh_String_t *fname = knh_getFieldName(ctx, field->fn);
		knh_type_t type = field->type;
		switch (type) {
		case CLASS_Boolean: {
			if (!strncmp(key, fname->str.text, fname->str.len) &&
				O_cTBL(value)->cid == CLASS_Boolean) {
				info->isFind = 1;
				info->flocate = i;
				goto L_END;
			}
			if (is32BIT) i++;
			break;
		}
		case CLASS_Int: {
			if (!strncmp(key, fname->str.text, fname->str.len) &&
				O_cTBL(value)->cid == CLASS_Int) {
				info->isFind = 1;
				info->flocate = i;
				goto L_END;
			}
			if (is32BIT) i++;
			break;
		}
		case CLASS_Float: {
			if (!strncmp(key, fname->str.text, fname->str.len) &&
				O_cTBL(value)->cid == CLASS_Float) {
				info->isFind = 1;
				info->flocate = i;
				goto L_END;
			}
			if (is32BIT) i++;
			break;
		}
		case CLASS_String: {
			if (!strncmp(key, fname->str.text, fname->str.len) &&
				O_cTBL(value)->cid == CLASS_String) {
				info->isFind = 1;
				info->flocate = i;
				goto L_END;
			}
			break;
		}
		case CLASS_Tdynamic: {
			if (!strncmp(key, fname->str.text, fname->str.len)) {
				info->isFind = 1;
				info->flocate = i;
				info->type = CLASS_Tdynamic;
				goto L_END;
			}
			break;
		}
		default:
			//Array or Object
			if (!strncmp(key, fname->str.text, fname->str.len) &&
				O_cTBL(value)->bcid == CLASS_Array) {
				info->isFind = 1;
				info->flocate = i;
			} else if (!strncmp(key, fname->str.text, fname->str.len) &&
					   O_cTBL(value)->p1 == c->cid) {
				info->isFind = 1;
				info->flocate = i;
			}
			break;
		}
	}
L_END:;
}

static Object *knh_msgpack_getClassObject(CTX ctx, knh_ClassTBL_t *c, msgpack_object_kv *map, const msgpack_object_kv *map_end)
{
	Object *o;
	msgpack_object _value = map->val;
	const char *class_name = _value.via.raw.ptr;
	if (!strncmp(class_name, (char *)c->sname->str.text, sizeof(class_name))) {
		o = (Object *)new_Object_init2(ctx, c);
		knh_FieldInfo_t info = {0, 0, 0};
		map++;
		for (; map < map_end; map++) {
			const char *var_name = map->key.via.raw.ptr;
			msgpack_object val = map->val;
			Object *target = knh_msgpack_getObject(ctx, c, (msgpack_unpacked *)&val, 1);
			if (target == NULL) {
				KNH_THROW(ctx, NULL, LOG_CRIT, "Message !!", "cannnot create object from : \"%s\"", var_name);
			}
			knh_msgpack_setFieldInfo(ctx, &info, c, var_name, target);
			if (!info.isFind) {
				KNH_THROW(ctx, NULL, LOG_CRIT, "Message !!", "cannnot find field variable name : \"%s\"", var_name);
				break;
			}
			Object **v = (Object **)o->ref;
			int cid = (info.type == CLASS_Tdynamic) ? CLASS_Tdynamic : O_cTBL(target)->cid;
			switch (cid) {
			case CLASS_Int: {
				intptr_t ivalue = (intptr_t)((knh_Int_t *)target)->n.ivalue;
				v[info.flocate] = (Object *)ivalue;
				break;
			}
			case CLASS_Float: {
				knh_float_t fvalue = ((knh_Float_t *)target)->n.fvalue;
				v[info.flocate] = (Object *)&fvalue;
				break;
			}
			case CLASS_Boolean: {
				knh_int_t bvalue = ((knh_Boolean_t *)target)->n.bvalue;
				v[info.flocate] = (Object *)&bvalue;
				break;
			}
			default:
				//Object
				KNH_SETv(ctx, v[info.flocate], target);
				break;
			}
		}
	} else {
		KNH_THROW(ctx, NULL, LOG_CRIT, "Message !!", "cannnot find class name : \"%s\"", class_name);
	}
	return o;
}

static Object *knh_msgpack_getObject(CTX ctx, knh_ClassTBL_t *c, msgpack_unpacked *result, bool isRecursive)
{
	msgpack_object obj = (isRecursive) ? *(msgpack_object *)result : result->data;
	Object *o;
	switch (obj.type) {
	case MSGPACK_OBJECT_POSITIVE_INTEGER:
		o = (Object *)new_Int(ctx, CLASS_Int, obj.via.u64);
		break;
	case MSGPACK_OBJECT_DOUBLE:
		o = (Object *)new_Float(ctx, CLASS_Float, obj.via.dec);
		break;
	case MSGPACK_OBJECT_BOOLEAN:
		if (obj.via.boolean) {
			o = (Object *)new_Boolean(ctx, KNH_TRUE);
			((knh_Boolean_t *)o)->n.bvalue = 1;
		} else {
			o = (Object *)new_Boolean(ctx, KNH_FALSE);
			((knh_Boolean_t *)o)->n.bvalue = 0;
		}
		break;
	case MSGPACK_OBJECT_RAW:
		o = (Object *)new_String(ctx, obj.via.raw.ptr);
		break;
	case MSGPACK_OBJECT_MAP:
		if (obj.via.map.size == 0) break;
		msgpack_object_kv *map = obj.via.map.ptr;
		msgpack_object_kv *map_end = obj.via.map.ptr + obj.via.map.size;
		msgpack_object _key = map->key;
		const char *key = _key.via.raw.ptr;
		if (!strncmp(key, "ks:atype", sizeof("ks:atype"))) {
			//Array
			o = (Object *)knh_msgpack_getArray(ctx, c, map);
		} else if (!strncmp(key, "ks:class", sizeof("ks:class"))) {
			//Object
			o = (Object *)knh_msgpack_getClassObject(ctx, c, map, map_end);
		}
		break;
	default:
		KNH_THROW(ctx, NULL, LOG_CRIT, "Message !!", "undefined msgpack object");
		break;
	}
	return o;
}

static knh_type_t knh_msgpack_unpack(CTX ctx, knh_ClassTBL_t *c, struct knh_InputStream_t *in, knh_sfp_t *sfp)
{
	char buf[MAX_MSG_BUFFER_SIZE] = {0};
	msgpack_unpacker upk;
	msgpack_unpacked result;
	in->dspi->fread(ctx, DP(in)->fio, buf, MAX_MSG_BUFFER_SIZE, NULL);
	knh_msgapck_setUnpacker(&upk, &result, buf);
	msgpack_unpacker_next(&upk, &result);
	Object *o = knh_msgpack_getObject(ctx, c, &result, 0);
	KNH_SETv(ctx, sfp[0].o, o);
	return O_cTBL(o)->cid;
}

static knh_PackSPI_t pack = {
	"msgpack",
	knh_msgpack_init,
	knh_msgpack_flushfree,
	knh_msgpack_null,
	knh_msgpack_bool,
	knh_msgpack_int,
	knh_msgpack_float,
	knh_msgpack_string,
	knh_msgpack_raw,
	knh_msgpack_putc,
	knh_msgpack_array,
	NULL,
	knh_msgpack_map,
	NULL,
	knh_msgpack_unpack,
};

static knh_PackSPI_t *knh_getPackSPI()
{
	return &pack;
}

/* ------------------------------------------------------------------------ */
//## method void OutputStream.writeObject(Object data, NameSpace ns);

METHOD OutputStream_writeObject(CTX ctx, knh_sfp_t *sfp _RIX)
{
	knh_OutputStream_t *w = sfp[0].w;
	Object *o = sfp[1].o;
	//knh_NameSpace_t *ns = sfp[2].ns; //needs getPackSPI
	knh_PackSPI_t *packspi = knh_getPackSPI();
	knh_packer_t packer = {w, NULL, NULL};
	knh_packer_t *pkr = packspi->pack_init(ctx, &packer);
	if (O_cTBL(o)->ospi->wdata != NULL) {
		O_cTBL(o)->ospi->wdata(ctx, pkr, o, packspi);
	} else if (O_cTBL(o)->bcid == CLASS_Array) {
		O_cTBL(o)->ospi->wdata(ctx, pkr, o, packspi);
	}
	packspi->pack_flushfree(ctx, pkr);
	fprintf(stderr, "\n");
	RETURNvoid_();
}

/* ------------------------------------------------------------------------ */
//## method dyn InputStream.readObject(Class c, NameSpace ns);

METHOD InputStream_readObject(CTX ctx, knh_sfp_t *sfp _RIX)
{
	BEGIN_LOCAL(ctx, lsfp, 1);
	//BEGIN_LOCAL(ctx, lsfp, 2);
	knh_InputStream_t *in = sfp[0].in;
	knh_Class_t *class = sfp[1].c;
	//knh_NameSpace_t *ns = sfp[2].ns; //needs getPackSPI
	knh_PackSPI_t *packspi = knh_getPackSPI();
	knh_type_t type = packspi->unpack(ctx, (knh_ClassTBL_t *)class->cTBL, in, lsfp);
	Object *o;
	if (type != TYPE_void) {
		KNH_SETv(ctx, o, lsfp[0].o);
	} else {
		KNH_SETv(ctx, o, KNH_NULL);
	}
	END_LOCAL(ctx, lsfp, o);
	RETURN_(o);
}
