#ifndef CUT_H
#define CUT_H

#include <stdio.h>
#include <stdbool.h>

#ifndef CUT_ERROR_MESSAGE_BUFFER_SIZE
    #define CUT_ERROR_MESSAGE_BUFFER_SIZE 200
#endif

#ifndef CUT_LOG_ERROR
    #define CUT_LOG_ERROR(__ctx) CutLogError(__ctx)
#endif
#ifndef CUT_LOG_SUCCESS
    #define CUT_LOG_SUCCESS(__ctx) CutLogSuccess(__ctx)
#endif
#ifndef CUT_PRINTF
    #define CUT_PRINTF(__format, ...) CutPrintF(__format, ##__VA_ARGS__)
#endif
#ifndef CUT_PRINTF_NO_LOG
    #define CUT_PRINTF_NO_LOG(__format, ...) printf(__format, ##__VA_ARGS__)
#endif

#ifndef CUT_OPEN_TEST_LOG_FILE
    #define CUT_OPEN_TEST_LOG_FILE(__filename) CutOpenTestLogFile(__filename)
#endif
#ifndef CUT_CLOSE_TEST_LOG_FILE
    #define CUT_CLOSE_TEST_LOG_FILE() CutCloseTestLogFile()
#endif

typedef struct CutTestContext CutTestContext;
typedef struct CutTestCase CutTestCase;
typedef void (*CutTestFunction)(CutTestContext* ctx);
typedef void (*CutLogErrorFunction)(const CutTestContext* ctx);

struct CutTestContext {
    const CutTestCase* current_test;
    char error_message[CUT_ERROR_MESSAGE_BUFFER_SIZE];
    int error_present;
    const char* caller_file;
    int caller_line;
};
struct CutTestCase {
    const char* name;
    CutTestFunction function;
};

#define BEGIN_TEST_CASE_LIST(__module_name) const CutTestCase __module_name##_test_cases[] = {
#define DECLARE_TEST_CASE(__module_name, __test_name) ((CutTestCase){ #__test_name, &__module_name##_##__test_name##_test })
#define END_TEST_CASE_LIST() };

#define TEST_SETUP(__module_name)   void __module_name##_setup()
#define TEST_CLEANUP(__module_name) void __module_name##_cleanup()
#define TEST(__module_name, __test_name) void __module_name##_##__test_name##_test(CutTestContext* ctx)

#define TEST_FAIL(__error_message, ...) \
    {\
        int size = snprintf((ctx)->error_message, CUT_ERROR_MESSAGE_BUFFER_SIZE, __error_message, ##__VA_ARGS__);\
        if (size >= 0) { (ctx)->error_message[size] = '\0'; }\
        (ctx)->error_present = 1;\
        (ctx)->caller_file = __FILE__;\
        (ctx)->caller_line = __LINE__;\
        return;\
    }

#define TEST_ASSERT_TRUE(__expression) if (!(__expression)) { TEST_FAIL("Expected expression '%s' to be 'true'", #__expression); }
#define TEST_ASSERT_TRUE_MESSAGE(__expression, __error_message, ...) if (!(__expression)) { TEST_FAIL(__error_message, ##__VA_ARGS__); }
#define TEST_ASSERT_FALSE(__expression) if ((__expression)) { TEST_FAIL("Expected expression '%s' to be 'false'", #__expression); }
#define TEST_ASSERT_FALSE_MESSAGE(__expression, __error_message, ...) if ((__expression)) { TEST_FAIL(__error_message, ##__VA_ARGS__); }
#define TEST_ASSERT_EQUAL(__expected, __actual) if ((__expected) != (__actual)) { TEST_FAIL("Expected '%s' to equal '%s'", #__expected, #__actual); }
#define TEST_ASSERT_EQUAL_MESSAGE(__expected, __actual, __error_message, ...) if ((__expected) != (__actual)) { TEST_FAIL(__error_message, ##__VA_ARGS__); }
#define TEST_ASSERT_NOT_EQUAL(__expected, __actual) if ((__expected) == (__actual)) { TEST_FAIL("Expected '%s' to not equal '%s'", #__expected, #__actual); }
#define TEST_ASSERT_NOT_EQUAL_MESSAGE(__expected, __actual, __error_message, ...) if ((__expected) == (__actual)) { TEST_FAIL(__error_message, ##__VA_ARGS__); }
#define TEST_ASSERT_STR_EQUAL(__expected, __actual) if (strcmp(__expected, __actual) != 0) { TEST_FAIL("Expected '%s' to equal '%s'", #__expected, #__actual); }
#define TEST_ASSERT_STR_EQUAL_MESSAGE(__expected, __actual, __error_message, ...) if (strcmp(__expected, __actual) != 0) { TEST_FAIL(__error_message, ##__VA_ARGS__); }
#define TEST_ASSERT_STR_NOT_EQUAL(__expected, __actual) if (strcmp(__expected, __actual) == 0) { TEST_FAIL("Expected '%s' to not equal '%s'", #__expected, #__actual); }
#define TEST_ASSERT_STR_NOT_EQUAL_MESSAGE(__expected, __actual, __error_message, ...) if (strcmp(__expected, __actual) == 0) { TEST_FAIL(__error_message, ##__VA_ARGS__); }
#define TEST_ASSERT_FLOAT_EQUAL(__expected, __actual) if (fabs(__expected - __actual) > 0.0001) { TEST_FAIL("Expected '%f' to equal '%f'", __expected, __actual); }
#define TEST_ASSERT_FLOAT_EQUAL_MESSAGE(__expected, __actual, __error_message, ...) if (fabs(__expected - __actual) > 0.0001) { TEST_FAIL(__error_message, ##__VA_ARGS__); }
#define TEST_ASSERT_FLOAT_EQUAL_EPSILON(__expected, __actual, __epsilon) if (fabs(__expected - __actual) > __epsilon) { TEST_FAIL("Expected '%f' to equal '%f'", __expected, __actual); }
#define TEST_ASSERT_FLOAT_EQUAL_EPSILON_MESSAGE(__expected, __actual, __epsilon, __error_message, ...) if (fabs(__expected - __actual) > __epsilon) { TEST_FAIL(__error_message, ##__VA_ARGS__); }
#define TEST_ASSERT_FLOAT_NOT_EQUAL(__expected, __actual) if (fabs(__expected - __actual) <= 0.0001) { TEST_FAIL("Expected '%f' to not equal '%f'", __expected, __actual); }
#define TEST_ASSERT_FLOAT_NOT_EQUAL_MESSAGE(__expected, __actual, __error_message, ...) if (fabs(__expected - __actual) <= 0.0001) { TEST_FAIL(__error_message, ##__VA_ARGS__); }
#define TEST_ASSERT_FLOAT_NOT_EQUAL_EPSILON(__expected, __actual, __epsilon) if (fabs(__expected - __actual) <= __epsilon) { TEST_FAIL("Expected '%f' to not equal '%f'", __expected, __actual); }
#define TEST_ASSERT_FLOAT_NOT_EQUAL_EPSILON_MESSAGE(__expected, __actual, __epsilon, __error_message, ...) if (fabs(__expected - __actual) <= __epsilon) { TEST_FAIL(__error_message, ##__VA_ARGS__); }
#define TEST_ASSERT_INT_IN_RANGE(__value, __min, __max) if (!((__value) >= (__min) && (__value) <= (__max))) { TEST_FAIL("Expected '%i' to be in range [%i, %i]", __value, __min, __max); }
#define TEST_ASSERT_INT_IN_RANGE_MESSAGE(__value, __min, __max, __error_message, ...) if (!((__value) >= (__min) && (__value) <= (__max))) { TEST_FAIL(__error_message, ##__VA_ARGS__); }
#define TEST_ASSERT_INT_OUT_OF_RANGE(__value, __min, __max) if (!((__value) < (__min) || (__value) > (__max))) { TEST_FAIL("Expected '%i' to be out of range [%i, %i]", __value, __min, __max); }
#define TEST_ASSERT_INT_OUT_OF_RANGE_MESSAGE(__value, __min, __max, __error_message, ...) if (!((__value) < (__min) || (__value) > (__max))) { TEST_FAIL(__error_message, ##__VA_ARGS__); }
#define TEST_ASSERT_FLOAT_IN_RANGE(__value, __min, __max) if (!((__value) >= (__min) && (__value) <= (__max))) { TEST_FAIL("Expected '%f' to be in range [%f, %f]", __value, __min, __max); }
#define TEST_ASSERT_FLOAT_IN_RANGE_MESSAGE(__value, __min, __max, __error_message, ...) if (!((__value) >= (__min) && (__value) <= (__max))) { TEST_FAIL(__error_message, ##__VA_ARGS__); }
#define TEST_ASSERT_FLOAT_OUT_OF_RANGE(__value, __min, __max) if (!((__value) < (__min) || (__value) > (__max))) { TEST_FAIL("Expected '%f' to be out of range [%f, %f]", __value, __min, __max); }
#define TEST_ASSERT_FLOAT_OUT_OF_RANGE_MESSAGE(__value, __min, __max, __error_message, ...) if (!((__value) < (__min) || (__value) > (__max))) { TEST_FAIL(__error_message, ##__VA_ARGS__); }
#define TEST_ASSERT_EQUAL_CUSTOM(__expected, __actual, __cmp) if (!(__cmp(__expected, __actual))) { TEST_FAIL("Expected '%s' to equal '%s'", #__expected, #__actual); }
#define TEST_ASSERT_EQUAL_CUSTOM_MESSAGE(__expected, __actual, __cmp, __error_message, ...) if (!(__cmp(__expected, __actual))) { TEST_FAIL(__error_message, ##__VA_ARGS__); }
#define TEST_ASSERT_NOT_EQUAL_CUSTOM(__expected, __actual, __cmp) if ((__cmp(__expected, __actual))) { TEST_FAIL("Expected '%s' to equal '%s'", #__expected, #__actual); }
#define TEST_ASSERT_NOT_EQUAL_CUSTOM_MESSAGE(__expected, __actual, __cmp, __error_message, ...) if ((__cmp(__expected, __actual))) { TEST_FAIL(__error_message, ##__VA_ARGS__); }
#define TEST_ASSERT_NOT_NULL(__ptr) if ((__ptr) == NULL) { TEST_FAIL("Expected pointer to be not NULL"); }
#define TEST_ASSERT_NOT_NULL_MESSAGE(__ptr, __error_message, ...) if ((__ptr) == NULL) { TEST_FAIL(__error_message, ##__VA_ARGS__); }
#define TEST_ASSERT_NULL(__ptr) if ((__ptr) != NULL) { TEST_FAIL("Expected pointer to be NULL"); }
#define TEST_ASSERT_NULL_MESSAGE(__ptr, __error_message, ...) if ((__ptr) != NULL) { TEST_FAIL(__error_message, ##__VA_ARGS__); }

#define DECLARE_UT_MAIN(__module_name) \
    int cut_main(int argc, char** argv) { \
        CutParseArgs(argc, argv);\
        __module_name##_setup();\
        CUT_OPEN_TEST_LOG_FILE(#__module_name "_test_logs.txt");\
        size_t success = 0, failure = 0;\
        const size_t testcase_count = sizeof(__module_name##_test_cases) / sizeof(__module_name##_test_cases[0]);\
        for (size_t i = 0; i < testcase_count; ++i) {\
            CutTestContext ctx = { 0 };\
            ctx.current_test = &__module_name##_test_cases[i];\
            ctx.current_test->function(&ctx);\
            CUT_PRINTF(" -- %s\n", ctx.current_test->name);\
            if (ctx.error_present) {\
                CUT_LOG_ERROR(&ctx);\
                ++failure;\
            } else {\
                CUT_LOG_SUCCESS(&ctx);\
                ++success;\
            }\
        }\
        CUT_PRINTF("Passed [%zu/%zu]\n", success, testcase_count);\
        CUT_PRINTF("Failed [%zu/%zu]\n", failure, testcase_count);\
        if (failure == 0) {\
            CutSetTextColorGreen();\
            CUT_PRINTF("UT passed!\n");\
        } else {\
            CutSetTextColorRed();\
            CUT_PRINTF("UT failed!\n");\
            CutResetTextColor();\
        }\
        __module_name##_cleanup();\
        CUT_CLOSE_TEST_LOG_FILE();\
        return (failure == 0) ? 0 : 1;\
    }

#define RUN_TESTS() cut_main(argc, argv)

extern void CutParseArgs(int argc, char** argv);
extern void CutSetTextColorRed();
extern void CutSetTextColorGreen();
extern void CutResetTextColor();
extern bool CutLogColorsEnabled();
extern void CutLogError(const CutTestContext* ctx);
extern void CutLogSuccess(const CutTestContext* ctx);
extern void CutPrintF(const char* format, ...);
extern void CutOpenTestLogFile(const char* filename);
extern void CutCloseTestLogFile();

#endif