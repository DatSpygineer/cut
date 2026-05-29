#include "cut.h"

#include <stdlib.h>
#include <string.h>

int* p = NULL;

TEST_SETUP(cut) {
    p = malloc(sizeof(int));
}
TEST_CLEANUP(cut) {
    free(p);
    p = NULL;
}

TEST(cut, example) {
    TEST_ASSERT_NOT_NULL(p);
    TEST_ASSERT_EQUAL(1, 1);
    TEST_ASSERT_NOT_EQUAL(1, 2);
    TEST_ASSERT_TRUE(1);
    TEST_ASSERT_FALSE(0);
    TEST_ASSERT_STR_EQUAL("hello", "hello");
    TEST_ASSERT_STR_NOT_EQUAL("hello", "world");
}
TEST(cut, fail_example) {
    TEST_ASSERT_NOT_NULL(p);
    TEST_ASSERT_EQUAL(1, 2);
}

BEGIN_TEST_CASE_LIST(cut)
    DECLARE_TEST_CASE(cut, example),
    DECLARE_TEST_CASE(cut, fail_example)
END_TEST_CASE_LIST()

DECLARE_UT_MAIN(cut)

int main(int argc, char** argv) {
    int result = RUN_TESTS();
    if (p != NULL) {
        puts("Cleanup failed");
        result = 1;
    }
    return result;

}