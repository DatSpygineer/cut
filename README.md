# C.U.T. Unit Test framework
CUT (C Unit Test) framework is a lightweight library for unit testing, written in pure C.

## How to use
### Setting up the project
Clone the repository into your project and add it as subdirectory in your CMake project.
```cmake
add_subdirectory(cut)
```

To create a test case, you can use the `add_unit_test` macro
```cmake
add_unit_test(MODULE_NAME SOURCES [source1.c source2.c] INCLUDES [include_dir1 include_dir2] LIBRARIES [lib1 lib2])
```
Example:
```cmake
add_unit_test(ModuleName
    SOURCES
        test_module.c
        mocks/test_mock.c
    INCLUDES
        mocks/
)
```

This macro will set up the dependencies and test automatically.
To set up manually via CMake, you can use the following:
```cmake
add_executable(test_module test_module.c)
target_link_libraries(test_module PRIVATE cut)
target_include_directories(test_module PRIVATE mocks)

add_test(test_module_test test_module)
```

### Implementing tests
Tests must implement a setup and cleanup function, for that you can use the following macros:
```c
TEST_SETUP(module_name)
{
    // This function will run before running the test cases
}

TEST_CLEANUP(module_name)
{
    // This function will run after the tests are finished
}
```

After these two, you can implement your test cases.

```c
TEST(module_name, test_case_name)
{
    // Test case implementation
}
```

After the test cases are added, you need to create a test case list and declare the UT main function and call the test runner.
```c
BEGIN_TEST_CASE_LIST(module_name)
    DECLARE_TEST_CASE(module_name, test_case_name),
END_TEST_CASE_LIST()

DECLARE_UT_MAIN(module_name)

int main()
{
    return RUN_TESTS();
}
```

### Assert functions
There are many different assert functions available:

> #### TEST_ASSERT_TRUE
> `TEST_ASSERT_TRUE(<expression>)`
> 
> Asserts that the expression is true.

> #### TEST_ASSERT_FALSE
> `TEST_ASSERT_FALSE(<expression>)`
> 
> Asserts that the expression is false.

> #### TEST_ASSERT_EQUAL
> `TEST_ASSERT_EQUAL(<expected>, <actual>)`
> 
> Asserts that the expected and actual values are equal.

> #### TEST_ASSERT_NOT_EQUAL
> `TEST_ASSERT_NOT_EQUAL(<expected>, <actual>)`
> 
> Asserts that the expected and actual values are not equal.

> #### TEST_ASSERT_STR_EQUAL
> `TEST_ASSERT_STR_EQUAL(<expected>, <actual>)`
> 
> Asserts that the expected and actual values are equal, using strcmp.

> #### TEST_ASSERT_NOT_EQUAL
> `TEST_ASSERT_STR_NOT_EQUAL(<expected>, <actual>)`
> 
> Asserts that the expected and actual values are not equal, using strcmp.

> #### TEST_ASSERT_FLOAT_EQUAL
> `TEST_ASSERT_FLOAT_EQUAL(<expected>, <actual>)`
> 
> Asserts that the expected and actual values are equal, using a tolerance of 0.0001.

> #### TEST_ASSERT_FLOAT_EQUAL_EPSILON
> `TEST_ASSERT_FLOAT_EQUAL_EPSILON(<expected>, <actual>, <epsilon>)`
> 
> Asserts that the expected and actual values are equal, using a specific tolerance.

> #### TEST_ASSERT_FLOAT_NOT_EQUAL
> `TEST_ASSERT_FLOAT_NOT_EQUAL(<expected>, <actual>)`
> 
> Asserts that the expected and actual values are not equal, using a tolerance of 0.0001.

> #### TEST_ASSERT_FLOAT_NOT_EQUAL_EPSILON
> `TEST_ASSERT_FLOAT_NOT_EQUAL_EPSILON(<expected>, <actual>)`
> 
> Asserts that the expected and actual values are not equal, using a specific tolerance.

> #### TEST_ASSERT_INT_IN_RANGE
> `TEST_ASSERT_INT_IN_RANGE(<value>, <min>, <max>)`
> 
> Asserts that the integer value is in the range of min and max.

> #### TEST_ASSERT_INT_OUT_OF_RANGE
> `TEST_ASSERT_INT_OUT_OF_RANGE(<value>, <min>, <max>)`
> 
> Asserts that the integer value is not in the range of min and max.

> #### TEST_ASSERT_FLOAT_IN_RANGE
> `TEST_ASSERT_FLOAT_IN_RANGE(<value>, <min>, <max>)`
> 
> Asserts that the float value is in the range of min and max.

> #### TEST_ASSERT_FLOAT_OUT_OF_RANGE
> `TEST_ASSERT_FLOAT_OUT_OF_RANGE(<value>, <min>, <max>)`
> 
> Asserts that the float value is not in the range of min and max.

> #### TEST_ASSERT_EQUAL_CUSTOM
> `TEST_ASSERT_EQUAL_CUSTOM(<expected>, <actual>, <compare function>)`
> 
> Asserts that the expected and actual values are equal, using a custom comparison function.

> #### TEST_ASSERT_NOT_EQUAL_CUSTOM
> `TEST_ASSERT_NOT_EQUAL_CUSTOM(<expected>, <actual>, <compare function>)`
> 
> Asserts that the expected and actual values are not equal, using a custom comparison function.

> #### TEST_ASSERT_NULL
> `TEST_ASSERT_NULL(<pointer>)`
> 
> Asserts that the pointer is null.

> #### TEST_ASSERT_NOT_NULL
> `TEST_ASSERT_NOT_NULL(<pointer>)`
> 
> Asserts that the pointer is not null.

You can add a custom error message by adding `_MESSAGE` suffix to the assertion macro.

For example: `TEST_ASSERT_EQUAL_MESSAGE(<expected>, <actual>, <message format>, <message arguments>...)`

Example for test asserts:
```c
TEST(module_name, test_case_name)
{
    // Pointer "p" is allocated in the test setup function
    TEST_ASSERT_NOT_NULL_MESSAGE(p, "Pointer 'p' must be allocated!");
    TEST_ASSERT_EQUAL(1, 1);
    TEST_ASSERT_NOT_EQUAL(1, 2);
    TEST_ASSERT_TRUE(1);
    TEST_ASSERT_FALSE(0);
    TEST_ASSERT_STR_EQUAL("hello", "hello");
    TEST_ASSERT_STR_NOT_EQUAL("hello", "world");
}
```
