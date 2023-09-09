#include "CppUTest/TestHarness.h"

extern "C" {
    #include "mytimeloop.h"
}

// Create a test group
TEST_GROUP(average_test_group)
{
    void setup()
    {
        // Initialize before each test
    }
    
    void teardown()
    {
        // Deinitialize after each test
    }
};

// Test null array
TEST(average_test_group, no_init_test)
{
    TimeLoop mytimeloop;
//   MYTIMELOOP_Init(&timeloop, 500, 200);
    CHECK_EQUAL(MYTIMELOOP_Run(&mytimeloop, 0), 0);
}

// Test the average function
TEST(average_test_group, simple_test)
{
    TimeLoop mytimeloop;
    MYTIMELOOP_Init(&mytimeloop, 500, 200);
    CHECK_EQUAL(MYTIMELOOP_Run(&mytimeloop, 200 + 501), 1);
}

// Test null array
TEST(average_test_group, null_test)
{
    TimeLoop mytimeloop;
    MYTIMELOOP_Init(&mytimeloop, 500, 200);
    CHECK_EQUAL(MYTIMELOOP_Run(&mytimeloop, 200 + 301), 1);
}
