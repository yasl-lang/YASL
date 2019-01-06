#include "comprehensiontest.h"
#include "yats.h"

SETUP_YATS();

static void test_tablecomp_noif() {
    unsigned char expected[] = {
            0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
            0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
            END,
            ICONST_1,
            ICONST_2,
            ICONST_3,
            NEWLIST,
            INITFOR,
            END,
            ITER_1,
            BRF_8,
            0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
            GSTORE_1, 0x00,
            GLOAD_1, 0x00,
            GLOAD_1, 0x00,
            NEG,
            BR_8,
            0xE6, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
            NEWTABLE,
            ENDCOMP,
            PRINT,
            HALT
    };
    ASSERT_GEN_BC_EQ(expected,"echo {i:-i for i <- [1,2,3]};");
}

static void test_tablecomp() {
    unsigned char expected[] = {
            0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
            0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
            END,
            ICONST_1,
            ICONST_2,
            ICONST_3,
            NEWLIST,
            INITFOR,
            END,
            ITER_1,
            BRF_8,
            0x20, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
            GSTORE_1, 0x00,
            GLOAD_1, 0x00,
            ICONST_2,
            MOD,
            ICONST_0,
            EQ,
            NOT,
            BRF_8,
            0x05, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
            GLOAD_1, 0x00,
            GLOAD_1, 0x00,
            NEG,
            BR_8,
            0xD6, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
            NEWTABLE,
            ENDCOMP,
            PRINT,
            HALT
    };
    ASSERT_GEN_BC_EQ(expected,"echo {i:-i for i <- [1,2,3] if i % 2 != 0};");
}

static void test_listcomp_noif() {
    unsigned char expected[] = {
            0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
            0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
            END,
            ICONST_1,
            ICONST_2,
            ICONST_3,
            NEWLIST,
            INITFOR,
            END,
            ITER_1,
            BRF_8,
            0x0E, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
            GSTORE_1, 0x00,
            GLOAD_1, 0x00,
            NEG,
            BR_8,
            0xE8, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
            NEWLIST,
            ENDCOMP,
            PRINT,
            HALT
    };
    ASSERT_GEN_BC_EQ(expected,"echo [-i for i <- [1,2,3]];");
}

static void test_listcomp() {
    unsigned char expected[] = {
            0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
            0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
            END,
            ICONST_1,
            ICONST_2,
            ICONST_3,
            NEWLIST,
            INITFOR,
            END,
            ITER_1,
            BRF_8,
            0x1E, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
            GSTORE_1, 0x00,
            GLOAD_1, 0x00,
            ICONST_2,
            MOD,
            ICONST_0,
            EQ,
            NOT,
            BRF_8,
            0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
            GLOAD_1, 0x00,
            NEG,
            BR_8,
            0xD8, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
            NEWLIST,
            ENDCOMP,
            PRINT,
            HALT
    };
    ASSERT_GEN_BC_EQ(expected,"echo [-i for i <- [1,2,3] if i % 2 != 0];");
}

int comprehensiontest(void) {
    test_listcomp();
    test_listcomp_noif();
    test_tablecomp();
    test_tablecomp_noif();
    return __YASL_TESTS_FAILED__;
}