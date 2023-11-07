// Todd Pieper
// Binary Search Tree Basic Operations
// Lab 3, CPSC 2430

// Lab 3 - Animal Shelter BST
// Create a binary search tree full of pet objects with different names and ages read from a file
// Also read from the file will be the option to search our BST for a specific node given a pet name
// As well as the option to print out the in order, pre-order and post order traversals of our BST

#include <iostream>
#include <string>
#include <fstream>

using namespace std;

class ShelterBST {
private:
    struct Pet {
        string name;
        int age;

        // Default constructor in case of an error
        Pet() {
            name = "NONAME";
            age = -1;
        }

        // Actual constructor to create pet objects
        Pet(string Name, int Age) {
            name = Name;
            age = Age;
        }

    };
    struct TreeNode {
        Pet* pet; // you MUST use a Pet pointer
        TreeNode* left;
        TreeNode* right;

        // Optional constructor to clean up the code for creation of new nodes
        TreeNode(Pet* pet1) {
            pet = pet1;
            left = nullptr;
            right = nullptr;
        }
    };

    TreeNode* root;

// PRIVATE RECURSIVE FUNCTIONS
// For this lab, TreeNodes will be ordered by Pet name alphabetically.
    TreeNode* insert(TreeNode* root, Pet* newPet) {

        // If the tree is empty, or we have reached a point where we need to make a new node, we make one
        // Although we are setting "root" = new node, in the case of recursively making it here
        // This won't change the root, but rather point the respective edge to this new node
        if (root == nullptr) {

            root = new TreeNode(newPet);
            return root;

        } else {

            // If the current node's pet name is greater than the name of the pet we are adding,
            // we need to continue the search left for a spot, if not we search right.
            // Note: The potential for names to match exactly is not part of this lab
            if ((root->pet->name).compare(newPet->name) > 0) {

                root->left = insert(root->left, newPet);
                return root;

            } else {

                root->right = insert(root->right, newPet);
                return root;

            }

        }


    }

// Returns pointer to TreeNode that matches the given name
// Nullptr if no match is found
    TreeNode* search(TreeNode *root, string petName) {

        // We can check both conditions in one if since if root is null, it won't check the second
        // condition and cause an error. If no match is found (root == nullptr) or a match is found
        // (root->pet->name == petName) we can return root in both cases
        if (root == nullptr || root->pet->name == petName) {

            return root;

        } else {

            // If the current node's pet name is greater than the name we are looking for,
            // we need to continue the search left, if not we search right.
            // Note: This recursion will look similar to our insert function, the key difference
            // is we are not setting an edge equal to the return of the recursion, but rather we
            // are simply digging deeper until we can return the node or a nullptr and not a whole subtree
            if ((root->pet->name).compare(petName) > 0) {

                return search(root->left, petName);

            } else {

                return search(root->right, petName);

            }

        }

    }
// The three traversals below will neatly print to screen
// the Pets’ names and ages in the respective order

// Inorder is middle left middle right, so we explore left, then print middle, and then explore right
    void inorder(TreeNode * root) {

        if (root == nullptr) {
            return;

        } else {

            inorder(root->left);
            cout << root->pet->name << ", " << root->pet->age << endl;
            inorder(root->right);

        }
    }

    // Pre order is middle left right, so we print middle, then explore left, and then explore right
    void preorder(TreeNode * root) {

        if (root == nullptr) {
            return;
        } else {

            cout << root->pet->name << ", " << root->pet->age << endl;
            preorder(root->left);
            preorder(root->right);

        }
    }

    // Post order is left right middle, so we explore left, then right, then print middle
    void postorder(TreeNode * root) {

        if (root == nullptr) {
            return;
        } else {

            postorder(root->left);
            postorder(root->right);
            cout << root->pet->name << ", " << root->pet->age << endl;

        }
    }

// We have a variety of public methods to call our private recursive methods so we can protect
// the private data/methods in our class
public:

    ShelterBST() {
        root = nullptr;
    }

    void insertPet(string name, int age){

        // Print the adding statement and then call our private function
        cout << "Adding pet, name: " << name << ", age: " << age << endl;
        root = insert(root, new Pet(name, age));
    }

    void searchPet(string name){

        // Call our private recursive method to search for the name in our BST and print the general
        // statement
        TreeNode* result = search(root, name);
        cout << "Searching for pet with name: " << name << ": ";

        // If we return nullptr, the pet was not found, otherwise we can print the information
        // Note: Since we did not print a new line in the first general statement, whichever print follows
        // will be on the same line and form on sentence as desired.
        if (result == nullptr) {
            cout << "no matching pet found" << endl;

        } else {
            cout << "found " << name << " age " << result->pet->age << "!" << endl;
        }

    }

    void inorderDisplay(){

        cout << "In order traversal:" << endl;
        inorder(root);

    }

    void preorderDisplay(){

        cout << "Pre order traversal:" << endl;
        preorder(root);

    }

    void postorderDisplay(){

        cout << "Post order traversal:" << endl;
        postorder(root);

    }

}; // end of class

// Define our class with our Pet struct to hold information and Tree struct to
// be a binary tree holding Pet pointers

// We have a switch control structure to handle options read from input file
// Option i: Insert pet (will be followed by Pet Name and Pet Age)
// Option s: Search pet (will be followed by Pet Name to be searched)
// Option n: Display in order traversal
// Option e: Display pre order traversal
// Option o: Display post order traversal


int main(int argc, char* argv[])
{

   // Error handle input and remind user of proper usage
   if(argc != 2) {

     cout << "Usage: assignment1 <testFileName>" << endl;
     return 1;
    
   }

  // Create a file reading object
  string filename = argv[1];
  ifstream infile(filename);

  // Error handle in case our file won't open
  if(!infile.is_open()) {

    cout << "Unable to open file: " << filename << endl;
    return 1;

  }

  // Create a Shelter object and define a string we will take as our choice input
  ShelterBST Shelter;
  string line;

  // "while we are reading choices from input file"
  while(getline(infile, line)) {

    // Set define variables to be used to store input from the file
    // We don't initialize petName or petAge as those are only guaranteed to follow two of 5 cases, and
    // so we only initialize when we enter those cases
    char choice = line[0];
    string petName;
    int petAge;

    switch(choice) { // Note above main goes over what each choice is

        case 'i':

            infile >> petName;
            infile >> petAge;
            Shelter.insertPet(petName, petAge);
            break;

        case 's':

              infile >> petName;
              Shelter.searchPet(petName);
              break;

        case 'n':

              Shelter.inorderDisplay();
              break;

        case 'e':

              Shelter.preorderDisplay();
              break;

        case 'o':

              Shelter.postorderDisplay();
              break;

    }// end of switch

   }// end of file reading while
  
  return 0;
} // end of main

