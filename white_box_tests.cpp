//======= Copyright (c) 2025, FIT VUT Brno, All rights reserved. ============//
//
// Purpose:     White Box - test suite
//
// $NoKeywords: $ivs_project_1 $white_box_tests.cpp
// $Author:     JMENO PRIJMENI <xlogin00@stud.fit.vutbr.cz>
// $Date:       $2025-02-19
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

class HashMapTest : public ::testing::Test {
public:
    hash_map_t* map = hash_map_ctor();
};


//============================================================================//
// hash_map_reserve
//============================================================================//

TEST_F(HashMapTest, reserve_small) {
    EXPECT_EQ(hash_map_reserve(map, 1), OK);
    EXPECT_EQ(hash_map_reserve(map, map->used-1), MEMORY_ERROR);
}


TEST_F(HashMapTest, reserve_eq) {
    // Put
    EXPECT_EQ(hash_map_reserve(map, map->allocated), OK);
}

TEST_F(HashMapTest, reserve_bigger) {
    EXPECT_EQ(hash_map_reserve(map, map->allocated + map->allocated), OK);
}

//============================================================================//
// hash_map_contains
//============================================================================//

TEST_F(HashMapTest, contains_empty) {
    EXPECT_EQ(hash_map_contains(map, "10"), false);
}

TEST_F(HashMapTest, contains_single) {
    EXPECT_EQ(hash_map_put(map, "10", 10), OK);
    EXPECT_EQ(hash_map_contains(map, "10"), true);
}

//============================================================================//
// hash_map_put
//============================================================================//

// put to hash map
TEST_F(HashMapTest, put_Single) {
    // Put
    hash_map_state_code_t result = hash_map_put(map, "10", 10);
    EXPECT_EQ(result, OK);
}

TEST_F(HashMapTest, put_SingleEmptyStr) {
    // Put
    hash_map_state_code_t result = hash_map_put(map, "", 10);
    EXPECT_EQ(result, OK);
}

// Reallocate
TEST_F(HashMapTest, put_Multiple) {
    EXPECT_EQ(hash_map_put(map, "10", 10), OK);
    EXPECT_EQ(hash_map_put(map, "20", 20), OK);
    EXPECT_EQ(hash_map_put(map, "30", 30), OK);
    EXPECT_EQ(hash_map_put(map, "40", 40), OK);
    EXPECT_EQ(hash_map_put(map, "50", 50), OK);
    EXPECT_EQ(hash_map_put(map, "60", 60), OK);
    EXPECT_EQ(hash_map_put(map, "70", 70), OK);
}

// Kolize klíčů
TEST_F(HashMapTest, put_DuplicateKey) {
    EXPECT_EQ(hash_map_put(map, "10", 10), OK);
    EXPECT_EQ(hash_map_put(map, "10", 10), KEY_ALREADY_EXISTS);
}

// Kolize hashů
TEST_F(HashMapTest, put_DuplicateHash) {
    EXPECT_EQ(hash_map_put(map, "ab", 10), OK);
    EXPECT_EQ(hash_map_put(map, "ba", 10), OK);

}

TEST_F(HashMapTest, put_DuplicateValue) {
    EXPECT_EQ(hash_map_put(map, "10", 10), OK);
    EXPECT_EQ(hash_map_put(map, "20", 10), OK);
}

//============================================================================//
// hash_map_get
//============================================================================//

TEST_F(HashMapTest, get_empty) {
    int value;
    EXPECT_EQ(hash_map_get(map, "10", &value), KEY_ERROR);
}

TEST_F(HashMapTest, get_single) {
    EXPECT_EQ(hash_map_put(map, "10", 10), OK);
    int value;
    EXPECT_EQ(hash_map_get(map, "10", &value), OK);
    EXPECT_EQ(value, 10);
}

//============================================================================//
// hash_map_remove
//============================================================================//

TEST_F(HashMapTest, remove_empty) {
    EXPECT_EQ(hash_map_remove(map, "10"), KEY_ERROR);
}

TEST_F(HashMapTest, remove_single) {
    EXPECT_EQ(hash_map_put(map, "10", 10), OK);
    int value;
    EXPECT_EQ(hash_map_remove(map, "10"), OK);
    EXPECT_EQ(hash_map_get(map, "10", &value), KEY_ERROR);
}

//============================================================================//
// hash_map_pop
//============================================================================//

TEST_F(HashMapTest, pop_empty) {
    int value;
    EXPECT_EQ(hash_map_pop(map, "10", &value), KEY_ERROR);
}


//============================================================================//
// hash_map_clear
//============================================================================//

TEST_F(HashMapTest, clear_empty) {
    hash_map_clear(map);
    EXPECT_EQ(map->used, 0);
}

TEST_F(HashMapTest, clear_single) {
    EXPECT_EQ(hash_map_put(map, "10", 10), OK);
    hash_map_clear(map);
    EXPECT_EQ(map->used, 0);
}

TEST_F(HashMapTest,clear_multiple) {
    EXPECT_EQ(hash_map_put(map, "10", 10), OK);
    EXPECT_EQ(hash_map_put(map, "20", 20), OK);
    EXPECT_EQ(hash_map_put(map, "30", 30), OK);
    hash_map_clear(map);
    EXPECT_EQ(map->used, 0);
}

//============================================================================//
// hash_map_capacity
//============================================================================//

TEST_F(HashMapTest, capacity) {
    EXPECT_EQ(hash_map_capacity(map), 8);
}
/*** Konec souboru white_box_tests.cpp ***/
