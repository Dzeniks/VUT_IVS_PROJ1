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

TEST(EmptyTree, InsertNode_Basic) {
    BinaryTree tree;
    auto result = tree.InsertNode(10);
    
    EXPECT_TRUE(result.first);  // Insertion should succeed
    EXPECT_NE(result.second, nullptr);  // Node should be created
    EXPECT_EQ(result.second->key, 10);  // Node should have correct key
    EXPECT_EQ(tree.GetRoot(), result.second);  // Should be root
    EXPECT_EQ(tree.GetRoot()->color, BinaryTree::BLACK);  // Root should be black
}

TEST(EmptyTree, DeleteNode_NonExistent) {
    BinaryTree tree;
    EXPECT_FALSE(tree.DeleteNode(10));  // Deletion should fail on empty tree
}

TEST(EmptyTree, FindNode_NonExistent) {
    BinaryTree tree;
    EXPECT_EQ(tree.FindNode(10), nullptr);  // Find should return nullptr on empty tree
}


//============================================================================//
// Non-Empty Tree Tests
//============================================================================//


class BinaryTreeTest : public ::testing::Test {
protected:
    BinaryTree tree;
};

// insertion of a single node
TEST_F(BinaryTreeTest, InsertSingleNode) {
    auto result = tree.InsertNode(10);

    EXPECT_TRUE(result.first);
    EXPECT_NE(result.second, nullptr);
    EXPECT_EQ(result.second->key, 10);
    EXPECT_EQ(tree.GetRoot(), result.second);
}

// insertion of duplicate nodes
TEST_F(BinaryTreeTest, InsertDuplicateNode) {
    auto firstInsert = tree.InsertNode(10);
    auto duplicateInsert = tree.InsertNode(10);

    EXPECT_FALSE(duplicateInsert.first);
    EXPECT_EQ(duplicateInsert.second, firstInsert.second);
}

// Test insertion of multiple nodes
TEST_F(BinaryTreeTest, InsertMultipleNodes) {
    auto keys = {50, 30, 70, 20, 40, 60, 80};
    std::vector<std::pair<bool, BinaryTree::Node_t*>> results;

    tree.InsertNodes(keys, results);

    EXPECT_EQ(results.size(), keys.size());

    // All nodes should be newly inserted
    for (const auto& result : results) {
        EXPECT_TRUE(result.first);
        EXPECT_NE(result.second, nullptr);
    }

    // Verify all nodes can be found
    for (int key : keys) {
        BinaryTree::Node_t* node = tree.FindNode(key);
        EXPECT_NE(node, nullptr);
        EXPECT_EQ(node->key, key);
    }
}

// Test finding nodes
TEST_F(BinaryTreeTest, FindNode) {
    tree.InsertNode(10);
    tree.InsertNode(5);
    tree.InsertNode(15);

    BinaryTree::Node_t* node = tree.FindNode(5);
    EXPECT_NE(node, nullptr);
    EXPECT_EQ(node->key, 5);

    // Test finding non-existent node
    node = tree.FindNode(100);
    EXPECT_EQ(node, nullptr);
}

// Test deleting nodes
TEST_F(BinaryTreeTest, DeleteNode) {
    tree.InsertNode(10);
    tree.InsertNode(5);
    tree.InsertNode(15);

    // Delete existing node
    bool result = tree.DeleteNode(5);
    EXPECT_TRUE(result);
    EXPECT_EQ(tree.FindNode(5), nullptr);

    // Delete non-existent node
    result = tree.DeleteNode(100);
    EXPECT_FALSE(result);
}

// Test getting leaf nodes
TEST_F(BinaryTreeTest, GetLeafNodes) {
    // Create a simple tree
    //       50
    //      /  \
    //     30   70
    tree.InsertNode(50);
    tree.InsertNode(30);
    tree.InsertNode(70);

    std::vector<BinaryTree::Node_t*> leafNodes;
    tree.GetLeafNodes(leafNodes);

    // In a properly implemented Red-Black tree, we should have leaves beyond
    // the actual data nodes, which are NIL nodes (represented as nullptr in many implementations)
    EXPECT_FALSE(leafNodes.empty());
}

// Test getting all nodes
TEST_F(BinaryTreeTest, GetAllNodes) {
    auto keys = {50, 30, 70, 20, 40, 60, 80};
    for (int key : keys) {
        tree.InsertNode(key);
    }

    std::vector<BinaryTree::Node_t*> allNodes;
    tree.GetAllNodes(allNodes);

    // We should have at least as many nodes as keys
    EXPECT_GE(allNodes.size(), keys.size());

    // Verify all keys are present
    std::vector<int> foundKeys;
    for (auto node : allNodes) {
        if (node != nullptr) {
            foundKeys.push_back(node->key);
        }
    }

    for (int key : keys) {
        EXPECT_NE(std::find(foundKeys.begin(), foundKeys.end(), key), foundKeys.end());
    }
}

// Test getting non-leaf nodes
TEST_F(BinaryTreeTest, GetNonLeafNodes) {
    // Create a tree
    //       50
    //      /  \
    //     30   70
    //    / \   / \
    //   20 40 60 80
    auto keys = {50, 30, 70, 20, 40, 60, 80};
    for (int key : keys) {
        tree.InsertNode(key);
    }

    std::vector<BinaryTree::Node_t*> nonLeafNodes;
    tree.GetNonLeafNodes(nonLeafNodes);

    // We should have non-leaf nodes
    EXPECT_FALSE(nonLeafNodes.empty());

    // Check that every non-leaf node has at least one child
    for (auto node : nonLeafNodes) {
        EXPECT_TRUE(node->pLeft != nullptr || node->pRight != nullptr);
    }
}

// Test tree properties after complex operations
TEST_F(BinaryTreeTest, TreePropertiesAfterOperations) {
    // Insert a sequence of nodes
    for (int i = 1; i <= 100; i++) {
        tree.InsertNode(i);
    }

    // Delete some nodes
    for (int i = 10; i <= 90; i += 10) {
        tree.DeleteNode(i);
    }

    // Insert some nodes again
    for (int i = 200; i <= 250; i += 10) {
        tree.InsertNode(i);
    }

    // Get all nodes and verify tree structure
    std::vector<BinaryTree::Node_t*> allNodes;
    tree.GetAllNodes(allNodes);

    // Verify root is not null
    EXPECT_NE(tree.GetRoot(), nullptr);

    // Verify each node maintains parent-child relationship
    for (auto node : allNodes) {
        if (node != nullptr && node != tree.GetRoot()) {
            // Each non-root node should have a valid parent
            EXPECT_NE(node->pParent, nullptr);

            // The node should be a child of its parent
            bool isLeftChild = (node->pParent->pLeft == node);
            bool isRightChild = (node->pParent->pRight == node);
            EXPECT_TRUE(isLeftChild || isRightChild);
        }
    }
}

// Test Red-Black properties (simplified)
TEST_F(BinaryTreeTest, RedBlackProperties) {
    // Insert a sequence of nodes that would trigger rotations
    auto keys = {10, 20, 30, 40, 50, 25, 15, 5};
    for (int key : keys) {
        tree.InsertNode(key);
    }

    std::vector<BinaryTree::Node_t*> allNodes;
    tree.GetAllNodes(allNodes);

    // Property 1: Every node is either red or black (ensured by enum)

    // Property 2: The root is black
    EXPECT_EQ(tree.GetRoot()->color, BinaryTree::BLACK);

    // Property 4: If a node is red, both its children must be black
    for (auto node : allNodes) {
        if (node != nullptr && node->color == BinaryTree::RED) {
            if (node->pLeft != nullptr) {
                EXPECT_EQ(node->pLeft->color, BinaryTree::BLACK);
            }
            if (node->pRight != nullptr) {
                EXPECT_EQ(node->pRight->color, BinaryTree::BLACK);
            }
        }
    }
}

//============================================================================//
// Red-Black Tree Axiom Tests
//============================================================================//

class TreeAxioms : public ::testing::Test {
    protected:
        void SetUp() override {
            // Insert various nodes to create a complex tree structure
            std::vector<int> values = {50, 30, 70, 20, 40, 60, 80, 35, 45, 65, 75};
            for (int val : values) {
                tree.InsertNode(val);
            }
        }
    
        bool isBlack(BinaryTree::Node_t* node) {
            return node == nullptr || node->color == BinaryTree::BLACK;
        }
    
        int getBlackHeight(BinaryTree::Node_t* node) {
            if (node == nullptr) return 1;  // NIL nodes are considered black
            
            int leftHeight = getBlackHeight(node->pLeft);
            int rightHeight = getBlackHeight(node->pRight);
            
            // If either subtree has invalid black height, return -1
            if (leftHeight == -1 || rightHeight == -1) return -1;
            if (leftHeight != rightHeight) return -1;
            
            return leftHeight + (isBlack(node) ? 1 : 0);
        }
    
        BinaryTree tree;
    };
    
    TEST_F(TreeAxioms, Axiom1_LeafNodesAreBlack) {
        std::vector<BinaryTree::Node_t*> leafNodes;
        tree.GetLeafNodes(leafNodes);
        
        for (auto node : leafNodes) {
            if (node != nullptr) {  // Some implementations might return nullptr for NIL nodes
                EXPECT_TRUE(isBlack(node)) << "Leaf node with key " << node->key << " is not black";
            }
        }
    }
    
    TEST_F(TreeAxioms, Axiom2_RedNodesHaveBlackChildren) {
        std::vector<BinaryTree::Node_t*> allNodes;
        tree.GetAllNodes(allNodes);
        
        for (auto node : allNodes) {
            if (node != nullptr && node->color == BinaryTree::RED) {
                EXPECT_TRUE(isBlack(node->pLeft)) 
                    << "Left child of red node " << node->key << " is not black";
                EXPECT_TRUE(isBlack(node->pRight))
                    << "Right child of red node " << node->key << " is not black";
            }
        }
    }
    
    TEST_F(TreeAxioms, Axiom3_BlackHeightConsistency) {
        // Check if black height is consistent for all paths
        int blackHeight = getBlackHeight(tree.GetRoot());
        EXPECT_NE(blackHeight, -1) << "Black height is not consistent across all paths";
    }

/*** Konec souboru black_box_tests.cpp ***/