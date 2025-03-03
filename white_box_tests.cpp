//======= Copyright (c) 2025, FIT VUT Brno, All rights reserved. ============//
//
// Purpose:     White Box - test suite
//
// $NoKeywords: $ivs_project_1 $white_box_tests.cpp
// $Author:     Jan Soukeník <xsoukej00@stud.fit.vutbr.cz>
// $Date:       $2025-02-24
//============================================================================//
/**
 * @file white_box_tests.cpp
 * @author JMENO PRIJMENI
 * 
 * @brief Implementace testu hasovaci tabulky.
 */

#include <vector>
#include <gmock/gmock-matchers.h>

#include "gtest/gtest.h"

#include "white_box_code.h"

//============================================================================//
// ** ZDE DOPLNTE TESTY **
//
// Zde doplnte testy hasovaci tabulky, testujte nasledujici:
// 1. Verejne rozhrani hasovaci tabulky
//     - Vsechny funkce z white_box_code.h
//     - Chovani techto metod testuje pro prazdnou i neprazdnou tabulku.
// 2. Chovani tabulky v hranicnich pripadech
//     - Otestujte chovani pri kolizich ruznych klicu se stejnym hashem 
//     - Otestujte chovani pri kolizich hashu namapovane na stejne misto v 
//       indexu
//============================================================================//

// Test suite for testing empty hash map
class EmptyMapTest : public ::testing::Test {
protected:
    hash_map_t* map;

    void SetUp() override {
        map = hash_map_ctor();
    }

    void TearDown() override {
        hash_map_dtor(map);
    }
};

// Test suite for testing non-empty hash map
class NonEmptyMapTest : public ::testing::Test {
protected:
    hash_map_t* map;

    void SetUp() override {
        map = hash_map_ctor();
        // Initialize map with some data
        hash_map_put(map, "10", 10);
        hash_map_put(map, "20", 20);
        hash_map_put(map, "30", 30);
    }

    void TearDown() override {
        hash_map_dtor(map);
    }
};

//============================================================================//
// EmptyMapTest - hash_map_size
//============================================================================//

TEST_F(EmptyMapTest, size) {
    EXPECT_EQ(hash_map_size(map), 0);
}

//============================================================================//
// EmptyMapTest - hash_map_reserve
//============================================================================//

TEST_F(EmptyMapTest, reserve_Small) {
    EXPECT_EQ(hash_map_reserve(map, 1), OK);
    EXPECT_EQ(hash_map_reserve(map, map->used - 1), MEMORY_ERROR);
}

TEST_F(EmptyMapTest, reserve_Zero) {
    EXPECT_EQ(hash_map_reserve(map, 0), OK);
}

TEST_F(EmptyMapTest, reserve_Equal) {
    EXPECT_EQ(hash_map_reserve(map, map->allocated), OK);
}

TEST_F(EmptyMapTest, reserve_bigger) {
    EXPECT_EQ(hash_map_reserve(map, map->allocated + map->allocated), OK);
}

//============================================================================//
// EmptyMapTest - hash_map_contains
//============================================================================//

TEST_F(EmptyMapTest, contains_Empty) {
    EXPECT_EQ(hash_map_contains(map, "10"), false);
}

//============================================================================//
// EmptyMapTest - hash_map_put
//============================================================================//

TEST_F(EmptyMapTest, put_Single) {
    EXPECT_EQ(hash_map_put(map, "10", 10), OK);
}

TEST_F(EmptyMapTest, put_SingleEmptyStr) {
    EXPECT_EQ(hash_map_put(map, "", 10), OK);
}

TEST_F(EmptyMapTest, put_Multiple) {
    EXPECT_EQ(hash_map_put(map, "10", 10), OK);
    EXPECT_EQ(hash_map_put(map, "20", 20), OK);
    EXPECT_EQ(hash_map_put(map, "30", 30), OK);
    EXPECT_EQ(hash_map_put(map, "40", 40), OK);
    EXPECT_EQ(hash_map_put(map, "50", 50), OK);
    EXPECT_EQ(hash_map_put(map, "60", 60), OK);
    EXPECT_EQ(hash_map_put(map, "70", 70), OK);
}

TEST_F(EmptyMapTest, put_WithHashCollision) {
    EXPECT_EQ(hash_map_put(map, "ab", 10), OK);
    EXPECT_EQ(hash_map_put(map, "ba", 20), OK);  // "ab" and "ba" have same hash
}

//============================================================================//
// EmptyMapTest - hash_map_get
//============================================================================//

TEST_F(EmptyMapTest, get_Empty) {
    int value;
    EXPECT_EQ(hash_map_get(map, "10", &value), KEY_ERROR);
}

//============================================================================//
// EmptyMapTest - hash_map_remove
//============================================================================//

TEST_F(EmptyMapTest, remove_Empty) {
    EXPECT_EQ(hash_map_remove(map, "10"), KEY_ERROR);
}

TEST_F(EmptyMapTest, remove_SizeCheck) {
    size_t size_before = map->used;
    EXPECT_EQ(hash_map_remove(map, "10"), KEY_ERROR);
    EXPECT_EQ(map->used, size_before);  // Should remain unchanged
}

//============================================================================//
// EmptyMapTest - hash_map_pop
//============================================================================//

TEST_F(EmptyMapTest, pop_Empty) {
    int value;
    EXPECT_EQ(hash_map_pop(map, "10", &value), KEY_ERROR);
}

//============================================================================//
// EmptyMapTest - hash_map_clear
//============================================================================//

TEST_F(EmptyMapTest, clear_Empty) {
    hash_map_clear(map);
    EXPECT_EQ(map->used, 0);
}

//============================================================================//
// EmptyMapTest - hash_map_capacity
//============================================================================//

TEST_F(EmptyMapTest, capacity) {
    EXPECT_EQ(hash_map_capacity(map), 8);  // Default capacity
}

//============================================================================//
// NonEmptyMapTest - hash_map_reserve
//============================================================================//

TEST_F(NonEmptyMapTest, reserve_Small) {
    EXPECT_EQ(hash_map_reserve(map, 1), MEMORY_ERROR);  // Cannot reserve less than used
}

TEST_F(NonEmptyMapTest, reserve_Equal) {
    EXPECT_EQ(hash_map_reserve(map, map->allocated), OK);
}

TEST_F(NonEmptyMapTest, reserve_Bigger) {
    EXPECT_EQ(hash_map_reserve(map, map->allocated * 2), OK);
}

//============================================================================//
// NonEmptyMapTest - hash_map_contains
//============================================================================//

TEST_F(NonEmptyMapTest, contains_Existing) {
    EXPECT_EQ(hash_map_contains(map, "10"), true);
    EXPECT_EQ(hash_map_contains(map, "20"), true);
    EXPECT_EQ(hash_map_contains(map, "30"), true);
}

TEST_F(NonEmptyMapTest, contains_NonExisting) {
    EXPECT_EQ(hash_map_contains(map, "40"), false);
    EXPECT_EQ(hash_map_contains(map, ""), false);
}

//============================================================================//
// NonEmptyMapTest - hash_map_put
//============================================================================//

TEST_F(NonEmptyMapTest, put_DuplicateKey) {
    EXPECT_EQ(hash_map_put(map, "10", 100), KEY_ALREADY_EXISTS);
}

TEST_F(NonEmptyMapTest, put_DuplicateValue) {
    EXPECT_EQ(hash_map_put(map, "40", 10), OK);  // Same value as key "10" but different key
}

TEST_F(NonEmptyMapTest, put_Additional) {
    EXPECT_EQ(hash_map_put(map, "40", 40), OK);
    EXPECT_EQ(hash_map_contains(map, "40"), true);
}

//============================================================================//
// NonEmptyMapTest - hash_map_get
//============================================================================//

TEST_F(NonEmptyMapTest, get_Existing) {
    int value;
    EXPECT_EQ(hash_map_get(map, "10", &value), OK);
    EXPECT_EQ(value, 10);

    EXPECT_EQ(hash_map_get(map, "20", &value), OK);
    EXPECT_EQ(value, 20);

    EXPECT_EQ(hash_map_get(map, "30", &value), OK);
    EXPECT_EQ(value, 30);
}

TEST_F(NonEmptyMapTest, get_NonExisting) {
    int value;
    EXPECT_EQ(hash_map_get(map, "40", &value), KEY_ERROR);
}

//============================================================================//
// NonEmptyMapTest - hash_map_remove
//============================================================================//

TEST_F(NonEmptyMapTest, remove_Existing) {
    EXPECT_EQ(hash_map_remove(map, "10"), OK);
    EXPECT_EQ(hash_map_contains(map, "10"), false);

    // Check other keys still exist
    EXPECT_EQ(hash_map_contains(map, "20"), true);
    EXPECT_EQ(hash_map_contains(map, "30"), true);
}

TEST_F(NonEmptyMapTest, remove_NonExisting) {
    EXPECT_EQ(hash_map_remove(map, "40"), KEY_ERROR);
}

TEST_F(NonEmptyMapTest, remove_SizeCheck) {
    size_t size_before = map->used;
    EXPECT_EQ(hash_map_remove(map, "10"), OK);
    EXPECT_EQ(map->used, size_before - 1);
}

//============================================================================//
// NonEmptyMapTest - hash_map_pop
//============================================================================//

TEST_F(NonEmptyMapTest, pop_Existing) {
    int value;
    EXPECT_EQ(hash_map_pop(map, "10", &value), OK);
    EXPECT_EQ(value, 10);
    EXPECT_EQ(hash_map_contains(map, "10"), false);
}

TEST_F(NonEmptyMapTest, pop_NonExisting) {
    int value;
    EXPECT_EQ(hash_map_pop(map, "40", &value), KEY_ERROR);
}

//============================================================================//
// NonEmptyMapTest - hash_map_clear
//============================================================================//

TEST_F(NonEmptyMapTest, clear) {
    hash_map_clear(map);
    EXPECT_EQ(map->used, 0);
    EXPECT_EQ(hash_map_contains(map, "10"), false);
    EXPECT_EQ(hash_map_contains(map, "20"), false);
    EXPECT_EQ(hash_map_contains(map, "30"), false);
}

//============================================================================//
// NonEmptyMapTest - hash_map_size
//============================================================================//

TEST_F(NonEmptyMapTest, size) {
    EXPECT_EQ(hash_map_size(map), 3);
}

//============================================================================//
// NonEmptyMapTest - hash_map_capacity
//============================================================================//

TEST_F(NonEmptyMapTest, Capacity) {
    EXPECT_EQ(hash_map_capacity(map), 8);  // Default capacity unless it was reallocated
}

//============================================================================//
// Test for hash collisions
//============================================================================//

TEST_F(NonEmptyMapTest, HashCollisions) {
    // First, add keys with same hash but different values
    EXPECT_EQ(hash_map_put(map, "ab", 100), OK);
    EXPECT_EQ(hash_map_put(map, "ba", 200), OK);  // "ab" and "ba" have same hash

    // Check both values can be retrieved correctly
    int value;
    EXPECT_EQ(hash_map_get(map, "ab", &value), OK);
    EXPECT_EQ(value, 100);

    EXPECT_EQ(hash_map_get(map, "ba", &value), OK);
    EXPECT_EQ(value, 200);

    // Remove one and check the other still exists
    EXPECT_EQ(hash_map_remove(map, "ab"), OK);
    EXPECT_EQ(hash_map_contains(map, "ab"), false);
    EXPECT_EQ(hash_map_contains(map, "ba"), true);
}

/*** Konec souboru white_box_tests.cpp ***/