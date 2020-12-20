#include "table_methods.h"

#include "yasl.h"
#include "yasl_aux.h"
#include "yasl_error.h"
#include "yasl_state.h"

static struct YASL_Table *YASLX_checktable(struct YASL_State *S, const char *name, int pos) {
	if (!YASL_istable(S)) {
		vm_print_err_type(&S->vm, "%s expected arg in position %d to be of type table, got arg of type %s.",
				  name, pos, YASL_peektypestr(S));
		YASL_throw_err(S, YASL_TYPE_ERROR);
	}
	return (struct YASL_Table *)YASL_popuserdata(S);
}

static struct YASL_Table *YASLX_checkntable(struct YASL_State *S, const char *name, unsigned pos) {
	if (!YASL_isntable(S, pos)) {
		vm_print_err_type(&S->vm, "%s expected arg in position %d to be of type table, got arg of type %s.",
				  name, pos, YASL_peektypestr(S));
		YASL_throw_err(S, YASL_TYPE_ERROR);
	}
	return (struct YASL_Table *)YASL_peeknuserdata(S, pos);
}

void table___len(struct YASL_State *S) {
	struct YASL_Table *ht = YASLX_checkntable(S, "table.__get", 0);
	YASL_pushint(S, YASL_Table_length(ht));
}

void table___get(struct YASL_State *S) {
	struct YASL_Object key = vm_pop((struct VM *) S);
	struct YASL_Table *ht = YASLX_checktable(S, "table.__get", 0);
	struct YASL_Object result = YASL_Table_search(ht, key);
	if (result.type == Y_END) {
		vm_pushundef(&S->vm);
	} else {
		vm_push((struct VM *) S, result);
	}
}

void table___set(struct YASL_State *S) {
	struct YASL_Object val = vm_pop((struct VM *) S);
	struct YASL_Object key = vm_pop((struct VM *) S);
	struct YASL_Table *ht = YASLX_checktable(S, "table.__set", 0);
	if (obj_isundef(&val)) {
		YASL_Table_rm(ht, key);
		return;
	}

	if (!YASL_Table_insert(ht, key, val)) {
		vm_print_err_type(&S->vm, "unable to use mutable object of type %s as key.", YASL_TYPE_NAMES[key.type]);
		YASL_throw_err(S, YASL_TYPE_ERROR);
	}
}

void table___bor(struct YASL_State *S) {
	struct YASL_Table *right = YASLX_checkntable(S, "table.__bor", 1);
	struct YASL_Table *left = YASLX_checkntable(S, "table.__bor", 0);

	YASL_pushtable(S);

	struct RC_UserData *new_ht = rcht_new_sized(left->base_size);

	FOR_TABLE(i, litem, left) {
		YASL_Table_insert_fast((struct YASL_Table *) new_ht->data, litem->key, litem->value);
	}

	FOR_TABLE(i, ritem, right) {
		YASL_Table_insert_fast((struct YASL_Table *) new_ht->data, ritem->key, ritem->value);
	}

	vm_push((struct VM *) S, YASL_TABLE(new_ht));
}

void table___eq(struct YASL_State *S) {
	struct YASL_Table *right = YASLX_checkntable(S, "table.__eq", 1);
	struct YASL_Table *left = YASLX_checkntable(S, "table.__eq", 0);

	if (left->count != right->count) {
		YASL_pushbool(S, false);
		return;
	}

	FOR_TABLE(i, item, left) {
		struct YASL_Object search = YASL_Table_search(right, item->key);
		if (search.type == Y_END) {
			YASL_pushbool(S, false);
			return;
		}
		vm_push((struct VM *) S, item->value);
		vm_push((struct VM *) S, search);
		vm_EQ((struct VM *) S);
		if (!YASL_popbool(S)) {
			YASL_pushbool(S, false);
			return;
		}
	}

	YASL_pushbool(S, true);
}

void object_tostr(struct YASL_State *S) {
	enum YASL_Types index = vm_peek((struct VM *) S, S->vm.sp).type;
	struct YASL_Object key = YASL_STR(YASL_String_new_sized(strlen("tostr"), "tostr"));
	struct YASL_Object result = YASL_Table_search((struct YASL_Table *)S->vm.builtins_htable[index]->data, key);
	str_del(obj_getstr(&key));
	YASL_GETCFN(result)->value(S);
}

int list_tostr_helper(struct YASL_State *S, void **buffer, size_t buffer_size, size_t buffer_count);

#define FOUND_LIST "[...], "
#define FOUND_TABLE "{...}, "

bool buffer_contains(void **buffer, size_t buffer_count, void *val) {
	for (size_t j = 0; j < buffer_count; j++) {
		if (buffer[j] == val) {
			return true;
		}
	}
	return false;
}

void rec_call(struct YASL_State *S, void **buffer, const size_t buffer_count, const size_t buffer_size, int (*f)(struct YASL_State *, void **, size_t, size_t)) {
	size_t tmp_buffer_size =
		buffer_count == buffer_size ? buffer_size * 2 : buffer_size;
	void **tmp_buffer = (void **) malloc(tmp_buffer_size * sizeof(void *));
	memcpy(tmp_buffer, buffer, sizeof(void *) * buffer_count);
	tmp_buffer[buffer_count] = vm_peeklist((struct VM *) S);
	f(S, tmp_buffer, tmp_buffer_size, buffer_count + 1);
	free(tmp_buffer);
}

int table_tostr_helper(struct YASL_State *S, void **buffer, size_t buffer_size, size_t buffer_count) {
	struct YASL_ByteBuffer bb = NEW_BB(8);

	YASL_ByteBuffer_add_byte(&bb, '{');
	struct YASL_Table *table = vm_peektable((struct VM *) S);
	if (table->count == 0) {
		vm_pop((struct VM *) S);
		YASL_ByteBuffer_add_byte(&bb, '}');
		vm_push((struct VM *) S, YASL_STR(YASL_String_new_sized_heap(0, bb.count, (char *)bb.bytes)));
		return YASL_SUCCESS;
	}

	FOR_TABLE(i, item, table) {
		vm_push((struct VM *) S, item->key);

		object_tostr(S);

		struct YASL_String *str = vm_popstr((struct VM *) S);
		YASL_ByteBuffer_extend(&bb, (unsigned char *)str->str + str->start, YASL_String_len(str));
		YASL_ByteBuffer_extend(&bb, (unsigned char *)": ", strlen(": "));

		vm_push((struct VM *) S, item->value);

		if (vm_islist((struct VM *) S)) {
			bool found = buffer_contains(buffer, buffer_count, vm_peeklist((struct VM *) S));
			if (found) {
				YASL_ByteBuffer_extend(&bb, (unsigned char *)FOUND_LIST, strlen(FOUND_LIST));
				vm_pop((struct VM *) S);
				continue;
			} else {
				rec_call(S, buffer, buffer_count, buffer_size, &list_tostr_helper);
			}
		} else if (vm_istable((struct VM *) S, S->vm.sp)) {
			bool found = buffer_contains(buffer, buffer_count, vm_peeklist((struct VM *) S));
			if (found) {
				YASL_ByteBuffer_extend(&bb, (unsigned char *)FOUND_TABLE, strlen(FOUND_TABLE));
				vm_pop((struct VM *) S);
				continue;
			} else {
				rec_call(S, buffer, buffer_count, buffer_size, &table_tostr_helper);
			}
		} else {
			vm_stringify_top((struct VM *) S);
		}

		str = vm_popstr((struct VM *) S);
		YASL_ByteBuffer_extend(&bb, (unsigned char *)str->str + str->start, YASL_String_len(str));
		YASL_ByteBuffer_extend(&bb, (unsigned char *)", ", strlen(", "));
	}

	vm_pop((struct VM *) S);

	bb.count -= 2;
	YASL_ByteBuffer_add_byte(&bb, '}');

	vm_push((struct VM *) S, YASL_STR(YASL_String_new_sized_heap(0, bb.count, (char *)bb.bytes)));

	return YASL_SUCCESS;
}

void table_tostr(struct YASL_State *S) {
	if (!YASL_istable(S)) {
		YASLX_print_err_bad_arg_type(S, "table.tostr", 0, "table", YASL_peektypestr(S));
		YASL_throw_err(S, YASL_TYPE_ERROR);
	}

	void **buffer = (void **) malloc(8 * sizeof(void *));
	buffer[0] = vm_peektable((struct VM *) S, S->vm.sp);
	table_tostr_helper(S, buffer, 8, 1);
	free(buffer);
}

void table_keys(struct YASL_State *S) {
	struct YASL_Table *ht = YASLX_checkntable(S, "table.keys", 0);
	struct RC_UserData *ls = rcls_new();
	FOR_TABLE(i, item, ht) {
			YASL_List_append((struct YASL_List *) ls->data, (item->key));
	}

	vm_push((struct VM *) S, YASL_LIST(ls));
}

void table_values(struct YASL_State *S) {
	struct YASL_Table *ht = YASLX_checkntable(S, "table.values", 0);
	struct RC_UserData *ls = rcls_new();
	FOR_TABLE(i, item, ht) {
			YASL_List_append((struct YASL_List *) ls->data, (item->value));
	}
	vm_push((struct VM *) S, YASL_LIST(ls));
}

void table_remove(struct YASL_State *S) {
	struct YASL_Object key = vm_pop((struct VM *) S);
	if (!YASL_istable(S)) {
		YASLX_print_err_bad_arg_type(S, "table.remove", 0, "table", YASL_peektypestr(S));
		YASL_throw_err(S, YASL_TYPE_ERROR);
	}
	struct YASL_Table *ht = YASL_GETTABLE(vm_peek((struct VM *) S));

	YASL_Table_rm(ht, key);
}

void table_clone(struct YASL_State *S) {
	struct YASL_Table *ht = YASLX_checkntable(S, "table.copy", 0);
	struct RC_UserData *new_ht = rcht_new_sized(ht->base_size);

	FOR_TABLE(i, item, ht) {
		YASL_Table_insert_fast((struct YASL_Table *) new_ht->data, item->key, item->value);
	}

	vm_push((struct VM *) S, YASL_TABLE(new_ht));
}

void table_clear(struct YASL_State *S) {
	if (!YASL_istable(S)) {
		YASLX_print_err_bad_arg_type(S, "table.clear", 0, "table", YASL_peektypestr(S));
		YASL_throw_err(S, YASL_TYPE_ERROR);
	}
	struct YASL_Table *ht = YASL_GETTABLE(vm_peek((struct VM *) S));
	inc_ref(&vm_peek((struct VM *) S));
	FOR_TABLE(i, item, ht) {
		del_item(item);
	}

	ht->count = 0;
	ht->size = TABLE_BASESIZE;
	free(ht->items);
	ht->items = (struct YASL_Table_Item *) calloc((size_t) ht->size, sizeof(struct YASL_Table_Item));
	dec_ref(&vm_peek((struct VM *) S));
	vm_pop((struct VM *) S);
	YASL_pushundef(S);
}
