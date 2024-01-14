/* ShelterBST.h Class declarations */
// Todd Pieper - CPSC2430 - Assignment 3
//ShelterBST.h , prototypes of functions defined in ShelterBST.cpp and used in assignment3.cpp

#ifndef SHELTERBST_H
#define SHELTERBST_H

#include <iostream>
#include <string>
using namespace std;

class ShelterBST {
private:

    // Pet object
    struct Pet {

        string name;
        int age;

        // Default constructor in case of an error
        Pet();

        // Actual constructor to create pet objects
        Pet(string Name, int Age);

    };

    // Node object
    struct TreeNode {
        Pet* pet;
        TreeNode* left;
        TreeNode* right;

        // Constructor to clean up the code for creation of new nodes
        TreeNode(Pet* pet1);

        // Will help for counting leaf nodes  
        bool isLeaf();

        // Will be helpful for deletion in knowing which pointer of the parent needs to be altered
        TreeNode* isThisMyLeftChild(TreeNode* child);
    };

    // Private root object so we can protect it
    TreeNode* root;

    // Searches and insert
    TreeNode* insert(TreeNode* root, Pet* pet);
    TreeNode* search(TreeNode* root, int petAge);
    TreeNode* findParent(TreeNode* root, TreeNode* previous, int age);
    TreeNode* findSuccessor(int age);
    TreeNode* successorHelper(TreeNode* parent, TreeNode* node);

    // Traversal calls
    void preorder(TreeNode * root);
    void postorder(TreeNode * root);
    void inorder(TreeNode * root);

    // Counts
    int numNodes(TreeNode * root);
    int numLeafs(TreeNode * root);
    int numNodesInLevel(TreeNode * root, int level, int currentDepth);
    int heightTree(TreeNode * root);

    // Check
    bool balanced(TreeNode* root);

    // Deletion and deletion helpers
    TreeNode* reconstruct(TreeNode* node, int age);
    void deallocateNode(TreeNode* node);
    TreeNode* deleteTree(TreeNode * root);

// Define public prototypes for a ShelterBST object to call during execution
public:

    // Class Object Constructor
    ShelterBST();

    // Traversal calls
    void postorderDisplay();
    void preorderDisplay();
    void inorderDisplay();

    // Searches and insert
    void successorDisplay(int age);
    void parentDisplay(int age);
    void searchPet(int age);
    void insertPet(string name, int age);

    // Counts
    void numNodesDisplay();
    void numLeafDisplay();
    void numNodesLevelDisplay(int level);
    void heightDisplay();

    // Check
    void balancedDisplay();

    // Deletion
    void removeNode(int age);
    void destroyTree();

};
#endif
