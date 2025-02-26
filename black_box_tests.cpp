//======= Copyright (c) 2025, FIT VUT Brno, All rights reserved. ============//
//
// Purpose:     Red-Black Tree - public interface tests
//
// $NoKeywords: $ivs_project_1 $black_box_tests.cpp
// $Author:     JMENO PRIJMENI <xlogin00@stud.fit.vutbr.cz>
// $Date:       $2025-02-19
//============================================================================//
/**
 * @file black_box_tests.cpp
 * @author JMENO PRIJMENI
 *
 * @brief Implementace testu binarniho stromu.
 */

#include <vector>

#include "gtest/gtest.h"

#include "red_black_tree.h"

//============================================================================//
// ** ZDE DOPLNTE TESTY **
//
// Zde doplnte testy Red-Black Tree, testujte nasledujici:
// 1. Verejne rozhrani stromu
//    - InsertNode/DeleteNode a FindNode
//    - Chovani techto metod testuje pro prazdny i neprazdny strom.
// 2. Axiomy (tedy vzdy platne vlastnosti) Red-Black Tree:
//    - Vsechny listove uzly stromu jsou *VZDY* cerne.
//    - Kazdy cerveny uzel muze mit *POUZE* cerne potomky.
//    - Vsechny cesty od kazdeho listoveho uzlu ke koreni stromu obsahuji
//      *STEJNY* pocet cernych uzlu.
//============================================================================//

//============================================================================//
// Empty Tree Tests
//============================================================================//

bool isBlack(BinaryTree::Node_t *node)
{
    return node == nullptr || node->color == BinaryTree::BLACK;
}

bool LeafNodesAreBlack(BinaryTree &tree) {
    std::vector<BinaryTree::Node_t *> leafNodes;
    tree.GetLeafNodes(leafNodes);

    for (auto node : leafNodes) {
        if (node != nullptr && !isBlack(node)) {
            return false;
        }
    }
    return true;
}

bool RedNodesHaveBlackChildren(BinaryTree &tree) {
    std::vector<BinaryTree::Node_t *> allNodes;
    tree.GetAllNodes(allNodes);

    for (auto node : allNodes) {
        if (node != nullptr && node->color == BinaryTree::RED) {
            if (!isBlack(node->pLeft) || !isBlack(node->pRight)) {
                return false;
            }
        }
    }
    return true;
}

int getBlackHeight(BinaryTree::Node_t *node)
{
    if (node == nullptr)
        return 1; // NIL nodes are considered black

    int leftHeight = getBlackHeight(node->pLeft);
    int rightHeight = getBlackHeight(node->pRight);

    // If either subtree has invalid black height, return -1
    if (leftHeight == -1 || rightHeight == -1)
        return -1;
    if (leftHeight != rightHeight)
        return -1;

    return leftHeight + (isBlack(node) ? 1 : 0);
}

bool BlackHeightConsistency(BinaryTree &tree) {
    int blackHeight = getBlackHeight(tree.GetRoot());
    return blackHeight != -1;
}

class EmptyTree : public ::testing::Test
{
protected:
    BinaryTree tree;
};

TEST_F(EmptyTree, InsertNode_Single)
{
    auto result = tree.InsertNode(10);

    EXPECT_TRUE(result.first);                           // Insertion should succeed
    EXPECT_NE(result.second, nullptr);                   // Node should be created
    EXPECT_EQ(result.second->key, 10);                   // Node should have correct key
    EXPECT_EQ(tree.GetRoot(), result.second);            // Should be root
    EXPECT_EQ(tree.GetRoot()->color, BinaryTree::BLACK); // Root should be black
}

TEST_F(EmptyTree, DeleteNode_NonExistent)
{
    EXPECT_FALSE(tree.DeleteNode(10)); // Deletion should fail on empty tree
}

TEST_F(EmptyTree, FindNode_NonExistent)
{
    EXPECT_EQ(tree.FindNode(10), nullptr); // Find should return nullptr on empty tree
}

//============================================================================//
// Non-Empty Tree Tests
//============================================================================//

class NonEmptyTree : public ::testing::Test
{
protected:
    BinaryTree tree;
    void SetUp() override {
        auto keys = {50, 30, 70, 20, 40, 60, 80};
        for (auto key : keys)
        {
            tree.InsertNode(key);
        }
    }
};

// insertion of a single node
TEST_F(NonEmptyTree, InsertNode_Single)
{
    auto result = tree.InsertNode(10);
    auto found = tree.FindNode(10);
    EXPECT_TRUE(result.first);                  
    EXPECT_NE(result.second, nullptr);
    EXPECT_EQ(result.second->key, 10);
    EXPECT_EQ(found, result.second);                      
    
}

// insertion of duplicate nodes
TEST_F(NonEmptyTree, InsertNode_Duplicate)
{
    auto firstInsert = tree.InsertNode(10);
    auto duplicateInsert = tree.InsertNode(10);

    EXPECT_FALSE(duplicateInsert.first);
    EXPECT_EQ(duplicateInsert.second, firstInsert.second);
}

// Test insertion of multiple nodes
TEST_F(NonEmptyTree, InsertNode_Multiple)
{
    auto keys = {110, 230, 500};
    std::vector<std::pair<bool, BinaryTree::Node_t *>> results;

    tree.InsertNodes(keys, results);

    EXPECT_EQ(results.size(), keys.size());

    // All nodes should be newly inserted
    for (const auto &result : results)
    {
        EXPECT_TRUE(result.first);
        EXPECT_NE(result.second, nullptr);
    }

    // Verify all nodes can be found
    for (int key : keys)
    {
        BinaryTree::Node_t *node = tree.FindNode(key);
        EXPECT_NE(node, nullptr);
        EXPECT_EQ(node->key, key);
    }
}

// Test finding nodes
TEST_F(NonEmptyTree, FindNode_Existent)
{
    BinaryTree::Node_t *node = tree.FindNode(50);
    EXPECT_NE(node, nullptr);
    EXPECT_EQ(node->key, 50);
}

TEST_F(NonEmptyTree, FindNode_NonExistent)
{
    BinaryTree::Node_t *node = tree.FindNode(999999);
    EXPECT_EQ(node, nullptr);
}

TEST_F(NonEmptyTree, DeleteNode_Root)
{
    // Get root node
    BinaryTree::Node_t *root = tree.GetRoot();
    EXPECT_NE(root, nullptr);

    // Delete root node
    bool result = tree.DeleteNode(root->key);
    EXPECT_TRUE(result);

    // Check tree is reorganized
    EXPECT_NE(tree.GetRoot(), nullptr);
    EXPECT_EQ(tree.GetRoot()->key, 40);
    EXPECT_EQ(tree.GetRoot()->color, BinaryTree::BLACK);
    
    // Check Red-Black Tree axioms
    EXPECT_EQ(BlackHeightConsistency(tree), true);
    EXPECT_EQ(LeafNodesAreBlack(tree), true);
    EXPECT_EQ(RedNodesHaveBlackChildren(tree), true);
}

TEST_F(NonEmptyTree, DeleteNode_Leaf)
{
    bool result = tree.DeleteNode(20);
    EXPECT_TRUE(result);
    EXPECT_EQ(tree.FindNode(20), nullptr);
}

TEST_F(NonEmptyTree, DeleteNode_Parent){
    // Delete root of the right subtree
    bool result = tree.DeleteNode(70);
    EXPECT_TRUE(result);
    EXPECT_EQ(tree.FindNode(70), nullptr);

    // Check right subtree is reorganized
    BinaryTree::Node_t *root = tree.GetRoot();
    EXPECT_NE(root, nullptr);
    EXPECT_NE(root->pRight, nullptr);
    EXPECT_EQ(root->pRight->key, 60);
    EXPECT_EQ(root->pRight->color, BinaryTree::BLACK);
    

    // Check Red-Black Tree axioms
    EXPECT_EQ(BlackHeightConsistency(tree), true);
    EXPECT_EQ(LeafNodesAreBlack(tree), true);
    EXPECT_EQ(RedNodesHaveBlackChildren(tree), true);
}



TEST_F(NonEmptyTree, DeleteNode_NonExistent)
{;
    // Delete non-existent node
    bool result = tree.DeleteNode(999999);
    EXPECT_FALSE(result);
}



//============================================================================//
// Red-Black Tree Axiom Tests
//============================================================================//

class TreeAxioms : public ::testing::Test
{
protected:
    BinaryTree tree;    
    void SetUp() override
    {
        // Insert various nodes to create a complex tree structure
        std::vector<int> values = {50, 30, 70, 20, 40, 60, 80, 35, 45, 65, 75};
        for (int val : values)
        {
            tree.InsertNode(val);
        }
    }
};

// Všechny listové uzly (tedy uzly bez potomků) jsou „černé”.
TEST_F(TreeAxioms, TreeAxioms_LeafNodesAreBlack)
{
    std::vector<BinaryTree::Node_t *> leafNodes;
    tree.GetLeafNodes(leafNodes);

    for (auto node : leafNodes)
    {
        if (node != nullptr)
        {
            EXPECT_TRUE(isBlack(node));
        }
    }
}

// Pokud je uzel „červený”, pak jsou jeho oba potomci „černé”.
TEST_F(TreeAxioms, TreeAxioms_RedNodesHaveBlackChildren)
{
    RedNodesHaveBlackChildren(tree);
}

// Každá cesta od každého listového uzlu ke kořeni obsahuje stejný počet „černých” uzlů (vizte oranžové cesty v obrázku).
TEST_F(TreeAxioms, TreeAxioms_BlackHeightConsistency)
{
    // Check if black height is consistent for all paths
    int blackHeight = getBlackHeight(tree.GetRoot());
    EXPECT_NE(blackHeight, -1);
}

/*** Konec souboru black_box_tests.cpp ***/