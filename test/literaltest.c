#include "literaltest.h"
#include "yats.h"

SETUP_YATS();

static void test_undef() {
    unsigned char expected[] = {
            0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
            0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
            NCONST,
            POP,
            HALT
    };
    ASSERT_GEN_BC_EQ(expected, "undef;");
}

static void test_true() {
    unsigned char expected[] = {
            0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
            0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
            BCONST_T,
            POP,
            HALT
    };
    ASSERT_GEN_BC_EQ(expected, "true;");
}

static void test_false() {
    unsigned char expected[] = {
            0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
            0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
            BCONST_F,
            POP,
            HALT
    };
    ASSERT_GEN_BC_EQ(expected, "false;");
}

static void test_bin() {
    unsigned char expected[] = {
            0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
            0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
            ICONST,
            0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
            POP,
            HALT
    };
    ASSERT_GEN_BC_EQ(expected, "0b11;");
}

static void test_oct() {
    unsigned char expected[] = {
            0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
            0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
            ICONST,
            0x0A, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
            POP,
            HALT
    };
    ASSERT_GEN_BC_EQ(expected, "0c12;");
}

static void test_dec() {
    unsigned char expected[]  = {
            0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
            0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
            ICONST,
            0x0A, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
            POP,
            HALT
    };
    ASSERT_GEN_BC_EQ(expected, "10;");
}

static void test_hex() {
    unsigned char expected[] = {
            0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
            0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
            ICONST,
            0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
            POP,
            HALT
    };
    ASSERT_GEN_BC_EQ(expected, "0x10;");
}

static void test_float() {
    unsigned char expected[] = {
            0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
            0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
            DCONST,
            0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xF0, 0x3F,
            POP,
            HALT
    };
    ASSERT_GEN_BC_EQ(expected, "1.0;");
}

static void test_string() {
    unsigned char expected[] = {
            0x1C, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
            0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
            0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
            'Y', 'A', 'S', 'L',
            NEWSTR,
            0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
            POP,
            HALT
    };
    ASSERT_GEN_BC_EQ(expected, "'YASL';");
}

static void test_list() {
    unsigned char expected[] = {
            0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
            0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
            END,
            ICONST,
            0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
            ICONST,
            0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
            ICONST,
            0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
            ICONST,
            0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
            ICONST,
            0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
            NEWLIST,
            POP,
            HALT
    };
    ASSERT_GEN_BC_EQ(expected, "[0, 1, 2, 3, 4];");
}

static void test_table() {
    unsigned char expected[] = {
            0x27, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
            0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
            0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
            'z',  'e',  'r',  'o',
            0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
            'o',  'n',  'e',
            END,
            ICONST,
            0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
            NEWSTR,
            0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
            ICONST,
            0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
            NEWSTR,
            0x1C, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
            NEWTABLE,
            POP,
            HALT
    };
    ASSERT_GEN_BC_EQ(expected, "[0:'zero', 1:'one'];");
}

int literaltest(void) {
    test_undef();
    test_true();
    test_false();
    test_bin();
    test_oct();
    test_dec();
    test_hex();
    test_float();
    test_string();
    test_list();
    test_table();

    return __YASL_TESTS_FAILED__;
}