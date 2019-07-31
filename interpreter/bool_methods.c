#include "bool_methods.h"

#include "yasl_state.h"
#include "yasl_error.h"

int bool_tostr(struct YASL_State *S) {
	if (!YASL_ISBOOL(vm_peek((struct VM *)S))) {
		YASL_PRINT_ERROR_BAD_ARG_TYPE("bool.tostr", 0, Y_BOOL, vm_peek((struct VM *)S).type);
		return YASL_TYPE_ERROR;
	}
	bool val = (bool)YASL_GETBOOL(vm_pop((struct VM *) S));
	struct YASL_String *string;
	if (val == 0) {
		string = YASL_String_new_sized(strlen("false"), "false");
	} else {
		string = YASL_String_new_sized(strlen("true"), "true");
	}
	struct YASL_Object to = YASL_STR(string);
	vm_push((struct VM *) S, to);
	return 0;
}

int bool_tobool(struct YASL_State *S) {
	if (!YASL_ISBOOL(vm_peek((struct VM *)S))) {
		YASL_PRINT_ERROR_BAD_ARG_TYPE("bool.tobool", 0, Y_BOOL, vm_peek((struct VM *)S).type);
		return YASL_TYPE_ERROR;
	}
	return 0;
}
