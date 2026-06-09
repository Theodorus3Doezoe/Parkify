#include <Arduino.h>
#include <unity.h>
#include "db_memory.h"

// Note: This test requires an ESP32 to run because it depends on LittleFS and Arduino framework.

void test_database_add_and_get() {
    Database test_db;
    // We might not be able to call begin() in a unit test easily if it formats flash
    // But we can test the in-memory logic
    
    uint32_t id = 123;
    Data data;
    data.payed = false;
    sprintf(data.licenceplate.data(), "ABC-123");
    
    test_db.add(id, data);
    
    auto retrieved = test_db.get(id);
    TEST_ASSERT_TRUE(retrieved.has_value());
    TEST_ASSERT_EQUAL_STRING("ABC-123", retrieved->licenceplate.data());
    TEST_ASSERT_FALSE(retrieved->payed);
}

void test_database_exists() {
    Database test_db;
    uint32_t id = 456;
    Data data;
    
    test_db.add(id, data);
    TEST_ASSERT_TRUE(test_db.exists(id));
    TEST_ASSERT_FALSE(test_db.exists(999));
}

void test_get_lowest_id() {
    Database test_db;
    
    Data data;
    test_db.add(0, data);
    test_db.add(1, data);
    test_db.add(3, data);
    
    auto lowest = test_db.getLowestId();
    TEST_ASSERT_TRUE(lowest.has_value());
    TEST_ASSERT_EQUAL_UINT32(2, *lowest);
}

void setup() {
    delay(2000); // Wait for serial
    UNITY_BEGIN();
    RUN_TEST(test_database_add_and_get);
    RUN_TEST(test_database_exists);
    RUN_TEST(test_get_lowest_id);
    UNITY_END();
}

void loop() {}
