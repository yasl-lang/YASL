#include <interpreter/YASL_string/YASL_string.h>
#include <string.h>
#include <color.h>
#include <interpreter/float/float64_methods.h>
#include "interpreter/float/float64.h"
#include "YASL_Object.h"


// Keep up to date with the YASL_Types
const char *YASL_TYPE_NAMES[] = {
    "undef",    // Y_UNDEF,
    "float64",  // Y_FLOAT64,
    "int64",    // Y_INT64,
    "bool",     // Y_BOOL,
    "str",      // Y_STR,
    "str",      // Y_STR_W,
    "list",     // Y_LIST,
    "list",     // Y_LIST_W,
    "table",    // Y_TABLE,
    "table",    // Y_TABLE_W,
    "file",     // Y_FILE,
    "fn",       // Y_FN
    "mn"        // Y_BFN
};

YASL_Object YASL_Undef(void) {
    return (YASL_Object) { .type = Y_UNDEF, .value.ival = 0 };
}
YASL_Object YASL_Float(double value) {
    return (YASL_Object) { .type = Y_FLOAT64, .value.dval = value };
}
YASL_Object YASL_Integer(int64_t value) {
    return (YASL_Object) { .type = Y_INT64, .value.ival = value };
}
YASL_Object YASL_Boolean(int value) {
    return (YASL_Object) { .type = Y_BOOL, .value.ival = value };
}

YASL_Object YASL_String(String_t *str) {
    return (YASL_Object) { .type = Y_STR, .value.sval = str };
}

YASL_Object YASL_Table(struct Hash_s *ht) {
    return (YASL_Object) { .type = Y_TABLE, .value.mval = ht };
}

//YASL_Object YASL_List();
//YASL_Object YASL_Table();

int yasl_type_equals(YASL_Types a, YASL_Types b) {
    if (a == Y_STR_W || a == Y_LIST_W || a == Y_TABLE_W) a -= 1;
    if (b == Y_STR_W || b == Y_LIST_W || b == Y_TABLE_W) b -= 1;
    return a == b;
}

int isfalsey(YASL_Object v) {
    // TODO: add NaN as falsey
    return (
            yasl_type_equals(v.type, Y_UNDEF) ||
            (yasl_type_equals(v.type, Y_BOOL) && v.value.ival == 0) ||
            (yasl_type_equals(v.type, Y_STR) && yasl_string_len(v.value.sval) == 0) ||
            (yasl_type_equals(v.type, Y_FLOAT64) && v.value.dval != v.value.dval)
    );
}

YASL_Object isequal(YASL_Object a, YASL_Object b) {
        if (yasl_type_equals(a.type, Y_UNDEF) || yasl_type_equals(b.type, Y_UNDEF)) {
            return UNDEF_C;
        }
        switch(a.type) {
        case Y_BOOL:
            if (yasl_type_equals(b.type, Y_BOOL)) {
                if (a.value.ival == b.value.ival) {
                    return TRUE_C;
                } else {
                    return FALSE_C;
                }
            } else {
                return FALSE_C;
            }
        case Y_TABLE:
        case Y_TABLE_W:
            if (yasl_type_equals(b.type, Y_TABLE)) {
                puts("Warning: comparison of hashes currently is not implemented.");
                return UNDEF_C;
            }
            return FALSE_C;
        case Y_LIST:
        case Y_LIST_W:
            if (yasl_type_equals(b.type, Y_LIST)) {
                puts("Warning: comparison of lists currently is not implemented.");
                return UNDEF_C;
            }
            return FALSE_C;
        case Y_STR:
        case Y_STR_W:
            if (yasl_type_equals(b.type, Y_STR)) {
                if (yasl_string_len(a.value.sval) != yasl_string_len(b.value.sval)) {
                    return FALSE_C;
                } else {
                    int i = 0;
                    while (i < yasl_string_len(a.value.sval)) {
                        if ((a.value.sval)->str[i+a.value.sval->start] != (b.value.sval)->str[i+b.value.sval->start]) {
                            return FALSE_C;
                        }
                        i++;
                    }
                    return TRUE_C;
                }
            }
            return FALSE_C;
        default:
            if (yasl_type_equals(b.type, Y_BOOL) || yasl_type_equals(b.type, Y_TABLE)) {
                return FALSE_C;
            }
            int c;
            if (yasl_type_equals(a.type, Y_INT64) && yasl_type_equals(b.type, Y_INT64)) {
                c = a.value.ival == b.value.ival;
            } else if (yasl_type_equals(a.type, Y_FLOAT64) && yasl_type_equals(b.type, Y_INT64)) {
                c = a.value.dval == (double)b.value.ival;
            } else if (yasl_type_equals(a.type, Y_INT64) && yasl_type_equals(b.type, Y_FLOAT64)) {
                c = (double)a.value.ival == b.value.dval;
            } else if (yasl_type_equals(a.type, Y_FLOAT64) && yasl_type_equals(b.type, Y_FLOAT64)) {
                c = a.value.dval == b.value.dval;
            } else {
                printf("== and != not supported for operands of types %x and %x.\n", a.type, b.type);
                return UNDEF_C;
            }
            return (YASL_Object) {Y_BOOL, c};
        }
}

int print(YASL_Object v) {
    int64_t i;
    switch (v.type) {
        case Y_INT64:
            printf("%" PRId64 "", v.value.ival);
            //printf("int64: %" PRId64 "\n", v.value);
            break;
        case Y_FLOAT64:
            printf("%s", float64_to_str(v.value.dval));
            //printf("%f", *((double*)&v.value));
            //printf("float64: %f\n", *((double*)&v.value));
            break;
        case Y_BOOL:
            if (v.value.ival == 0) printf("false");
            else printf("true");
            break;
        case Y_UNDEF:
            printf("undef");
            break;
        case Y_STR:
            //printf("str (before print): %s\n", v.value.sval->str);
            //printf("vm->stack[vm->sp]: %x (%d, %d)\n", v.value.ival, v.value.sval->rc->refs, v.value.sval->rc->weak_refs);
            for (i = 0; i < yasl_string_len(v.value.sval); i++) {
                printf("%c", (v.value.sval)->str[i + v.value.sval->start]);
            }
            break;
        /* case Y_TABLE:
            printf("<hash %" PRIx64 ">", v.value);
            break; */
        /* case Y_LIST:
            //ls_print((List_t*)v.value);
            // printf("<list %" PRIx64 ">", v.value);
            break; */
        case Y_FILE:
            if (v.value.fval == stdin) {
                printf("stdin");
            } else if (v.value.fval == stdout) {
                printf("stdout");
            } else if (v.value.fval == stderr) {
                printf("stderr");
            } else {
                printf("<file %" PRIx64 ">", v.value.ival);
            }
            break;
        case Y_FN:
            printf("<fn: %" PRIx64 ">", v.value.ival);
            break;
        case Y_BFN:
            printf("<mn: %" PRIx64 ">", v.value.ival);
            break;
        default:
            printf("Error, unknown type: %x", v.type);
            return -1;
    }
    return 0;
}
