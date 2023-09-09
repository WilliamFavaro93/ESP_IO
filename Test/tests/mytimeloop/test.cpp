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
    MyTimeLoop mytimeloop;
//   MYTIMELOOP_Init(&timeloop, 500, 200);
    mytimeloop._LastRunTime += 500;
    CHECK_EQUAL(MYTIMELOOP_Run(), 0);
}

// Test the average function
TEST(average_test_group, simple_test)
{
    MyTimeLoop mytimeloop;
    MYTIMELOOP_Init(&mytimeloop, 500, 200);
    mytimeloop._LastRunTime += 500;
    CHECK_EQUAL(MYTIMELOOP_Run(), 1);
}

// Test null array
TEST(average_test_group, null_test)
{
    MyTimeLoop mytimeloop;
    MYTIMELOOP_Init(&mytimeloop, 500, 200);
    mytimeloop._LastRunTime += 300;
    CHECK_EQUAL(MYTIMELOOP_Run(), 0);
}