#include "ShelterBST.h"
#include <cmath>

// Todd Pieper - CPSC2430 - Assignment 3
//ShelterBST.cpp , implementation of functions defined in ShelterBST.h and used in assignment3.cpp


// Constructor for class object
ShelterBST::ShelterBST() {
    root = nullptr;

}

// Default and regular constructor for pet objects
ShelterBST::Pet::Pet() {
    name = "NONAME";
    age = -1;

}
ShelterBST::Pet::Pet(string Name, int Age) {
    name = Name;
    age = Age;

}

// Constructor for our nodes, will call them when we have a pet to add
ShelterBST::TreeNode::TreeNode(Pet* pet1) {
    pet = pet1;
    left = nullptr;
    right = nullptr;

}

// CHECK IF THIS NODE IS A LEAF NODE FUNCTION:
// Method for a node to call, will return true if it has no children (is a leaf node) or false if not
bool ShelterBST::TreeNode::isLeaf() {

  // This will not check if the root itself is nullptr so before calling this function we must check that
  // If the node we are checking has no left or right child, then it is a leaf
  if (right == nullptr && left == nullptr) {
    return true;
    
  } else { // The node we are looking at has at least one child, it is not a leaf
    return false;
    
  }

}

// LEFT OR RIGHT CHILD CHECKING FUNCTION:
// When deleting a node we will need to reassign the parent node to the deleted nodes children if it has any
// Our findParent function defined later will retrieve the parent of a given node, but we won't know if it's
// child is left or right. This function is made so that when we have a know parent of an existing node, we can determine
// which child it is. We will NOT call this function unless we have a positive parent child match.
// Call by using (parentName)->(isThisMyLeftChild((childName))
ShelterBST::TreeNode* ShelterBST::TreeNode::isThisMyLeftChild(TreeNode* child) {

    if (this->left == child) {
        return this->left;

    } else {
        return this->right;
    }

}

// INSERT FUNCTION:
// For this lab, TreeNodes will be ordered by Pet age.
// If the current node's pet age is greater than the age of the pet we are adding,
// we need to continue the search left for a spot, if not we search right.
// Note: The potential for names to match exactly is part of this lab, if they match, return root and handle
// an error statement in the public method calling insert
ShelterBST::TreeNode* ShelterBST::insert(TreeNode* root, Pet* newPet) {

    // If the tree is empty, or we have reached a point where we need to make a new node, we make one
    // Although we are setting "root" = new node, in the case of recursively making it here
    // This won't change the root, but rather point the respective edge to this new node
    if (root == nullptr) {

        root = new TreeNode(newPet);
        return root;

    } else if (root->pet->age == newPet->age) { // BST value is the same, notify user with a statement and return root
        cout << "Sorry, we already have a pet of age: " << newPet->age << ", their name is " << root->pet->name
             << "." << endl;
        return root;

    } else if (root->pet->age > newPet->age) { // BST node value larger, so we search left

        root->left = insert(root->left, newPet);
        return root;

    } else { // BST value must be smaller, so we search right

        root->right = insert(root->right, newPet);
        return root;

    }

} // end of insert


// SEARCH FOR A NODE FUNCTION:
// Returns pointer to TreeNode that matches the given age
// Nullptr if no match is found
ShelterBST::TreeNode* ShelterBST::search(TreeNode *root, int petAge) {

    // We can check both conditions in one if since if root is null, it won't check the second
    // condition and cause an error. If no match is found (root == nullptr) or a match is found
    // (root->pet->age == petAge) we can return root in both cases
    if (root == nullptr || root->pet->age == petAge) {

        return root;

    } else {

        // If the current node's pet age is greater than the name we are looking for,
        // we need to continue the search left, if not we search right.
        // Note: This recursion will look similar to our insert function, the key difference
        // is we are not setting an edge equal to the return of the recursion, but rather we
        // are simply digging deeper until we can return the node or a nullptr and not a whole subtree
        if (root->pet->age > petAge) {

            return search(root->left, petAge);

        } else {

            return search(root->right, petAge);

        }

    }

} // end of search


// THREE TRAVERSALS, INORDER, PREORDER, POSTORDER
// The three traversals below will neatly print to screen
// the Pets’ names and ages in the respective order

// INORDER TRAVERSAL
// Inorder is middle left middle right, so we explore left, then print middle, and then explore right
void ShelterBST::inorder(TreeNode * root) {

    if (root == nullptr) {
        return;

    } else {

        inorder(root->left);
        cout << root->pet->name << ", " << root->pet->age << endl;
        inorder(root->right);

    }
}

// PREORDER TRAVERSAL
// Preorder is middle left right, so we print middle, then explore left, and then explore right
void ShelterBST::preorder(TreeNode * root) {

    if (root == nullptr) {
        return;
    } else {

        cout << root->pet->name << ", " << root->pet->age << endl;
        preorder(root->left);
        preorder(root->right);

    }
}

// POST ORDER TRAVERSAL
// Post order is left right middle, so we explore left, then right, then print middle
void ShelterBST::postorder(TreeNode * root) {

    if (root == nullptr) {
        return;
    } else {

        postorder(root->left);
        postorder(root->right);
        cout << root->pet->name << ", " << root->pet->age << endl;

    }
}

// PARENT OF NODE RETRIEVING FUNCTION:
// The below code is essentially a copy of our standard search except since we desire the parent, we add a parameter
// to keep track of the previous node and will instead return that once a match is found.
// Note: this assignment does not make us account for finding the parent of a non-existing node, I did anyway
// So I had to separate the base case so that if a match isn't found, we always return null, and if it is we return the
// previous (parent). Inside the public display function we will handle interpretation of result, as well as using it for
// other functions like deletion.
ShelterBST::TreeNode* ShelterBST::findParent(TreeNode * root, TreeNode * previous, int petAge) {


    if (root == nullptr) {
        return root;

    } else if (root->pet->age == petAge) { // Previous is initialized to nullptr, so if the root is requested we return nullptr
        return previous;

    } else {

        // We have entered this else block which means we haven't found a match
        // This will hold the current root before we recurse to track the previous, we will return it once the match is found
        previous = root;

        // The following is a copy of how we determine which subtree to search, see standard search function for notes
        if (root->pet->age > petAge) {

            return findParent(root->left, previous, petAge);

        } else {

            return findParent(root->right, previous, petAge);

        }

    }

} // end of findParent


// FIND INORDER SUCCESSOR FUNCTION:
// Finding the inorder successor for a node with < 2 children is not required, but I have implemented it as practice
// and I see why it was not required, this thought process was quite annoying, but I have a better understanding now
// and I hope this knowledge will serve me in the future.

// The inorder successor of any given node is found within these ways
// #1 if it has a right subtree, then the leftmost element of that subtree will be it's inorder successor
// #2 if it does not have a right subtree, but it is the left child of its parent, then it's parent is the inorder successor
// #3 if it does not have a right subtree and is the right child of it's parent, the inorder successor can be found
// by trailing back the list of parents until you find the nth parent that is a left child of its parent, that
// nth parent's parent will be the inorder successor of the given node
// #4 worst case, no right child nor is it a child of any node, then the given node is the root with no current in order successor
ShelterBST::TreeNode* ShelterBST::findSuccessor(int age) {

    // Retrieve the node we are trying to find the successor of
    TreeNode* Node = search(root, age);

    // Since my search function can return nullptr if the element doesn't exist in the BST, we must check
    // we can also check for case 4 right after since we want to return nullptr for that as well
    // Case #4, there is a root, there is no right subtree, and we are looking for the successor of the root
    if (Node == nullptr || (root != nullptr && root->right == nullptr && root->pet->age == age)) {

        return nullptr;
    }
    TreeNode* Successor = Node->right;


    // If there is no right subtree, we can check
    if (Successor == nullptr) {

        Successor = successorHelper(nullptr, Node);
        return Successor;

    } else {

        // We have checked our edge cases, so now we just need the left most element of the right subtree
        // Successor is already pointing to the right subtree, so we can quickly iterate though it
        // Case #1
        while (Successor->left != nullptr) {
            Successor = Successor->left;
        }
        return Successor;

    }

}

// FIND INORDER SUCCESSOR HELPER FUNCTION:
// This helper will be used for the edge cases where we need to find the inorder successor of a node with no right
// children , and it is either a right or left child of its parent, cases #2 and case #3
ShelterBST::TreeNode* ShelterBST::successorHelper(TreeNode* parent, TreeNode* node) {

    parent = findParent(root, nullptr, node->pet->age);
    if (node == parent->left) {
        return parent;

    } else if (parent == root) {
        return nullptr;

    } else {
        return successorHelper(node, parent); // Doesn't matter what we pass as first param since it gets overwritten

    }

}


// NODE COUNTING FUNCTIONS:
// numNodes, number of nodes in the entire tree
// numLeafs, number of leaf nodes in the entire tree
// numNodesInLevel, number of nodes at a given level of the tree, level indexing starts at 0 for the root

// NUM NODES IN TREE
int ShelterBST::numNodes(TreeNode* root) {

  if (root != nullptr) { //If the node exists, count it and check its children
      return 1 + numNodes(root->left) + numNodes(root->right);

  } else { // Otherwise, the node is null, and we won't count it, add 0
      return 0;

  }

}

// NUMBER OF LEAF NODES
int ShelterBST::numLeafs(TreeNode* root) {

  if (root == nullptr) { // If the node is null, it is not a leaf so we return 0
      return 0;

  } else if (root->isLeaf()) { // If the node is a leaf we count it with just a 1, no recursion since it has no children
      return 1;

  } else { // Else the node is not null and is not a leaf, so we return zero and continue to search its children
      return 0 + numLeafs(root->left) + numLeafs(root->right);

  }
}

// NUM NODES IN LEVEL
// Current depth will always be initialized to zero when called by our public method numNodesLevelDisplay
// Our recursion will stem from iterating the current depth until we have reached our desired level (else if - level !> currentDepth)
// or all nodes/children we check are  (if). If level == currentDepth and the node is != nullptr, we count it and return 1
// No need to recurse deeper since we are at the desired level (else)
int ShelterBST::numNodesInLevel(TreeNode * root, int level, int currentDepth) {

    if (root == nullptr) { // If the node is null, it is not in the level so we return 0
        return 0;

    } else if (level > currentDepth) {

        return 0 + numNodesInLevel(root->left, level, currentDepth + 1)
        + numNodesInLevel(root->right, level, currentDepth + 1);

    } else {
        return 1;
    }

}

// TREE HEIGHT CALCULATOR
// Height indexing starts at 0, so a tree or subtree consisting of a singular node, from the perspective of that node,
// the height is zero. A completely empty tree has a height of -1 which will be our base case. If we are looking at
// an empty node, its height is -1. We can achieve the zero of a singular node by adding a 1 to the height of its empty children
// Height is counting the maximum depth between a nodes' subtrees such that if a root has a height of 5 in its left tree
// but no nodes in the right, the height of the tree is 6 (counting the root). So recursively, we can use cmath library
// function max(a,b) to return the largest value between examining the height of a nodes left and right subtrees
int ShelterBST::heightTree(TreeNode* root) {

    if (root == nullptr) {
        return -1;

    } else {
        return 1 + max(heightTree(root->left), heightTree(root->right));

    }

}

// BALANCED TREE CHECK
// A balanced tree requires that the height difference between any two nodes subtrees is > 1. We can use our height
// function above and simply check if the difference between the height of every subtree set within the tree is <= 1
// and break otherwise. We can use cmath function abs (absolute value) to resolve the non-commutative nature of subtraction
// and be able to check the absolute difference between them.
// This is a checking function, will return true if the tree is balanced, and false if it is not
bool ShelterBST::balanced(TreeNode* root) {

  if (root == nullptr) {
    return true;
  }
  if ((abs(heightTree(root->left) - heightTree(root->right)) > 1)) {
    return false;

  } else {
      return balanced(root->left) && balanced(root->right);

    }
}

// DEALLOCATE NODE
// We are meant to free up dynamically allocated memory for this assignment so this is a small function to take in a node
// reset its pointers to null, and subsequently delete the pet within it, and then the node itself. This function is used
// in our delete specific node function (reconstruct) and when we delete the entire tree (deleteTree)
void ShelterBST::deallocateNode(TreeNode* node) {

    if (node != nullptr) {
        node->left = nullptr;
        node->right = nullptr;
        delete node->pet;
        delete node;
    }

}

//DELETE SPECIFIC NODE

ShelterBST::TreeNode* ShelterBST::reconstruct(TreeNode* node, int age) {

    if (node == nullptr) {
        return node;

    } else if (node->pet->age > age) { // BST node value larger, so we search left

        node->left = reconstruct(node->left, age);
        return node;

    } else if (node->pet->age < age) { // BST value must be smaller, so we search right

        node->right = reconstruct(node->right, age);
        return node;

    }

    // If the node is a leaf, we can just deallocate it and return a nullptr to sever the connection
    if (node->isLeaf()) {

        deallocateNode(node);
        return nullptr;

    }

    // Essentially a temporary pointer to hold onto the pointer from the parent to the child (node we are deleting)
    TreeNode* parentChildConnection = nullptr;

    if (node->left == nullptr) { // We passed the leaf node check, so if the left child is null, it only has a right
        parentChildConnection = node->right;
        deallocateNode(node);

    } else if (node->right == nullptr) { // Like above but if the right child is null, it only has a left
        parentChildConnection = node->left;
        deallocateNode(node);

    } else { // Else, the node has two children

        TreeNode* parent = findParent(root, nullptr, age);
        if (parent == nullptr) {
            parent = root;

        }
        parentChildConnection = parent->isThisMyLeftChild(node);

        // If the node has two children, successor is guaranteed to not be null
        TreeNode* successor = findSuccessor(age);

        // The successor is also guaranteed to have a parent
        TreeNode* successorParent = findParent(root, nullptr, successor->pet->age);

        // The successor is guaranteed to not have a left child, since it will be the leftmost child of the noe-to-deletes'
        // subtree, so no matter what we will need to set the successors left pointer to the nodes left subtree,
        // even if it is null
        successor->left = node->left;

        // If the parent of the successor is the node we are trying to delete/replace, we don't need to reassign
        // the right pointer of the successor since whatever is in its right subtree will just come with it when we
        // return it.
        if (successorParent == node) {

            deallocateNode(node);
            return successor;

        }

        // If the parent of the successor is NOT the node we are trying to delete/replace, we must store the right
        // subtree of the successor into the left child of its parent whether its nullptr or not. If it does have values
        // They will still all be less than the successor parent since we know successor was a left child of its parent
        // Then we point the successor to the node's right tree to preserve the values before deallocation.
        successorParent->left = successor->right;
        successor->right = node->right;
        deallocateNode(node);
        return successor;

    } // end of two children else block

    // Across the two cases of just left child, just right child, we want to return this temporary
    // pointer after reassigning it, this will get returned to the subtree that called this pointer and finish our
    // reconstruction.
    return parentChildConnection;

} // end of reconstruct

// DELETE TREE FUNCTION
// Similar to counting nodes, we need to just visit all the nodes that exist, but we will deallocate them after
// our recursion so that we delete the tree from the bottom up because going top down would risk losing access to nodes
ShelterBST::TreeNode* ShelterBST::deleteTree(TreeNode* root) {

    if (root == nullptr) {
        return nullptr;
    } else {
        deleteTree(root->left);
        deleteTree(root->right);
    }
    deallocateNode(root);
    return nullptr; // We are to return a nullptr once our tree is finished deleting
}

// PUBLIC METHODS BELOW
// PUBLIC METHODS BELOW
// PUBLIC METHODS BELOW
// PUBLIC METHODS BELOW
// PUBLIC METHODS BELOW
// PUBLIC METHODS BELOW


// PUBLIC SEARCH DISPLAY
void ShelterBST::searchPet(int age) {

    // Call our private recursive method to search for the name in our BST and print the general
    // statement
    TreeNode* result = search(root, age);
    cout << "Searching for pet with age " << age << ": ";

    // If we return nullptr, the pet was not found, otherwise we can print the information
    // Note: Since we did not print a new line in the first general statement, whichever print follows
    // will be on the same line and form on sentence as desired.
    if (result == nullptr) {
        cout << "no matching pet found" << endl;

    } else {
        cout << "found " << result->pet->name << " age " << age << "!" << endl;
    }

}

// PUBLIC INSERT DISPLAY
void ShelterBST::insertPet(string name, int age) {

    // Print the adding statement and then call our private function
    cout << "Adding pet, name: " << name << ", age: " << age << endl;
    root = insert(root, new Pet(name, age));

}



// Order traversal display calls

// PUBLIC INORDER DISPLAY
void ShelterBST::inorderDisplay() {

    cout << "In order traversal:" << endl;
    inorder(root);

}

// PUBLIC PREORDER DISPLAY
void ShelterBST::preorderDisplay() {

    cout << "Pre order traversal:" << endl;
    preorder(root);

}

// PUBLIC POSTORDER DISPLAY
void ShelterBST::postorderDisplay() {

    cout << "Post order traversal:" << endl;
    postorder(root);

}

//PUBLIC PARENT DISPLAY
void ShelterBST::parentDisplay(int petAge) {

    cout << "Finding the parent of the pet age " << petAge << ": ";

    // Before even calling our search for parent, we can quickly check the trivial case of it matching the root value
    // Check if the tree is empty before checking if we matched the root
    if (root != nullptr && root->pet->age == petAge) {

        cout << "This is the root, it has no parent :(" << endl;
        return;

    }

    // Call our private method to search for the parent
    TreeNode* parent = findParent(root, nullptr, petAge);

    if (parent != nullptr) { // We found a parent!
        cout << "Found " << parent->pet->name << ", age " << parent->pet->age << "!" << endl;

    } else { // parent == nullptr and != root, must have looked for a nonexistent node
        cout << "There is no pet with age " << petAge << ", it can't have a parent!" << endl;

    }

}

// PUBLIC SUCCESSOR DISPLAY
void ShelterBST::successorDisplay(int age) {

    cout << "Attempting to find the inorder successor of a pet with age " << age << ": ";
    TreeNode* success = findSuccessor(age);

    if (success == nullptr) { // We did not find a successor

        cout << "There is no successor of a pet with age " << age;

        if (root != nullptr && root->right == nullptr) { // We didn't find it because we are looking at a root with no right subtree
            cout << endl << "because this is the root and the tree currently has no pets older than the root, sorry!" << endl;

        } else if (search(root, age) == nullptr){ // We didn't find a successor because the element does not exist in our BST
            cout << endl << "because a pet with this age does not exist in the tree, sorry!" << endl;

        } else { // The element doesn't have a successor since it is the biggest
            cout << endl << "because this is the largest element in the tree!" << endl;
        }

    } else { // We did find a successor
        cout << "Found! Pet " << success->pet->name << " age " << success->pet->age << endl;

    }


}

// PUBLIC NUMBER OF NODES DISPLAY
void ShelterBST::numNodesDisplay() {
  cout << "The number of nodes in out tree is: " << numNodes(root) << endl;

}

// PUBLIC NUMBER OF LEAF NODES DISPLAY
void ShelterBST::numLeafDisplay() {
  cout << "The number of leaf nodes in our tree is: " << numLeafs(root) << endl;

}

// PUBLIC NUMBER OF NODES IN A LEVEL DISPLAY
void ShelterBST::numNodesLevelDisplay(int level) {
    cout << "The number of nodes in our tree at level " << level << " is: " << numNodesInLevel(root, level, 0) << endl;

}

// PUBLIC TREE HEIGHT DISPLAY
void ShelterBST::heightDisplay() {
    cout << "The height of our tree is: " << heightTree(root) << endl;

}

// PUBLIC BALANCE CHECK DISPLAY
void ShelterBST::balancedDisplay() {

    cout << "Checking if tree is balanced: ";
    if (balanced(root)) {
        cout << "Tree is balanced!" << endl;

    } else if (root == nullptr){
        cout << "Tree is not balanced because the tree does not exist!" << endl;

    } else {
        cout << "Tree is not balanced!" << endl;

    }
}

// PUBLIC DELETE NODE DISPLAY
void ShelterBST::removeNode(int age) {

    cout << "Attempting to delete a pet with age " << age << ": ";

    if (root == nullptr) { // Can't delete a node from an empty tree
        cout << "Error, cannot delete a pet from a tree that doesn't exist!" << endl;
        return;
    }

    TreeNode* node = search(root, age);

    if (node == nullptr) { // Can't delete a non-existent node, this also helps ensure node != for our private call
        cout << "Error, cannot delete a pet that doesn't exist!" << endl;
        return;

    } else { // Node to delete exists
        root = reconstruct(root, age);

    }
    // If we made it here, we succeeded in deleting a node and reconstructing the tree
    cout << "Success! Pet has been deleted" << endl;

}

// PUBLIC DESTROY TREE DISPLAY
void ShelterBST::destroyTree() {

    cout << "Attempting to delete tree: ";

    root = deleteTree(root);
    if (root == nullptr) { // This message will print if the deletion was successful, or if the tree was empty to begin with
        cout << "Success! Tree deleted" << endl;

    } else {
        cout << "Error, something went wrong, tree was not fully deleted!" << endl;

    }
}
