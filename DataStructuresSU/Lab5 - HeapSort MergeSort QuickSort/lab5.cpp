// Lab 5, HeapSort, MergeSort, and Quick Sort of Pet pointers
// Todd Pieper
// CPSC 2430

#include <iostream>
#include <chrono>
using namespace std;
using namespace chrono;

struct Pet{

    // Member variables
    string name;
    int weight;

    // Constructor
    Pet(string name, int weight);
};

struct MaxHeap{

private:

    // Member variables, it is useful to have a count of size and copy of the array so we won't have to pass
    // it into each function call of percolate/sort, we can just copy the unsorted into a MaxHeap object, then
    // sort it within the object, and copy the values back after
    int numPets;
    Pet** petHeap;

    // Necessary functions to build and sort the MaxHeap
    void percolateUp(int index);
    void percolateDown(int index);
    char whichChildrenToCheck(int index);
    Pet* petMax(Pet* pet1, Pet* pet2);

public:

    // Public function to call in heap sort
    void maxSort(Pet*A[]);

    // Constructor and Destructor
    MaxHeap(Pet* A[], int n);
    ~MaxHeap();
};

// HeapSort
void heapSort(Pet* A[], int size);

// Quicksort and its helpers
void quickSort(Pet* A[], int size);
void quickHelp(Pet* A[], int i, int j);
int partition(Pet* A[], int i, int j);
int partitionRandom(Pet* A[], int i, int j);

// Merge Sort and it's helpers
void mergeSort(Pet* A[], int size);
void mergeHelp(Pet* A[], int i, int j);
void merge(Pet* A[], int p, int q, int r);

// General functions for all 3 sorts
void printArray(Pet* A[], int size);
void measureSort(void (*sortingFunction)(Pet*[], int));



int main() {

  // function pointer
  void (*sortingFunction)(Pet*[], int);

  cout << endl << "HEAP SORT" << endl;
  sortingFunction = &heapSort;
  measureSort(sortingFunction);

  cout << endl << "MERGE SORT" << endl;
  sortingFunction = &mergeSort;
  measureSort(sortingFunction);

  cout << endl << "QUICK SORT" << endl;
  sortingFunction = &quickSort;
  measureSort(sortingFunction);
}



// Pet object constructor
Pet::Pet(string petName, int petWeight) {

    name = petName;
    weight = petWeight;

}


// MEASURE SORT FUNCTION
// Takes in a function pointer to a sorting function that has a Pet* array and integer (size) as parameters
// and will generate 5 random arrays of increasing magnitude size starting at 10, up to 100000 and will test the sorting
// function to see if it worked by printing the sorted and unsorted arrays (of size 10, anything beyond would be too much)
// and test their speed by starting/ending a clock before/after the sort is called, NOT including the print.
void measureSort(void (*sortingFunction)(Pet*[], int)) {

    int sizeTracker; // will hold onto the size to save from calling the size array below many times
    int size[] = {10, 100, 1000, 10000, 100000}; // sizes to be used
    for (int i = 0; i < 5; i++) { // 5 arrays to test

        sizeTracker = size[i];
        //allocate memory for the array of size[i]
        Pet **array = new Pet *[sizeTracker];

        // fill in array with random numbers
        for (int j = 0; j < sizeTracker; j++) {

            string name = "J";
            int weight = rand() % 100000;
            array[j] = new Pet(name, weight);

        }

        // Print out size 10 random array
        if (sizeTracker == 10) {
            cout << "Random array size " << sizeTracker << ": " << endl;
            printArray(array, sizeTracker);
        }

        // Start a clock, call the sorting function, then end the clock and take the difference to get execution time
        auto start = high_resolution_clock::now();
        sortingFunction(array, sizeTracker);
        auto stop = high_resolution_clock::now();
        auto duration = stop - start;
        auto duration_ns = duration_cast<nanoseconds>(duration);

        // Print out size 10 sorted array
        if (sizeTracker == 10) {
            cout << "Sorted Array size " << sizeTracker << ": " << endl;
            printArray(array, sizeTracker);
        }

        // Print out the time it took to sort the array, this is not dependent on size, so we will see all 5 tests
        cout << "Array size " << sizeTracker << " duration: ";
        cout << duration_ns.count() << " ns." << endl; // match your time units

        // Finished with measure sort, time to free up the memory
        // Iterate over each row to delete the pets first
        for (int k =0; k < sizeTracker; k++) {
            delete array[k];

        }
        //And finally delete the pointer array
        delete[] array;

    }

}

// PRINT ARRAY FUNCTION
// Take in an array and its size, print its contents seperated by a comma
void printArray(Pet* A[], int size) {

    for (int i = 0; i < size; i++) {
        cout << A[i]->weight << ", ";
    }

    // Formatting new line
    cout << endl;
}



// MERGE SORT AND HELPERS BELOW
// MERGE SORT AND HELPERS BELOW
// MERGE SORT AND HELPERS BELOW

// mergeSort is what will be used for our function pointer in main, like the others this won't be the recursive method
// since Merge Sort needs more arguments (a start and end index) so we use this function to call a recursive helper
// starting from 0 to size-1 to work with the indices of array A
void mergeSort(Pet* A[], int size) {

  mergeHelp(A, 0, size-1);

}

// mergeHelp will recursively work through the indices by taking the middle and calling itself on the left and right half
// of the array based on the middle index. The base case stops the array splitting when the array (per indices i through j)
// would only have 1 element. Then as those stack frames pop it'll return to the array with increasing range of indices and call
// merge on them.
void mergeHelp(Pet* A[], int i, int j) {

  if (i < j) {

    int mid = (i+j)/2;
    mergeHelp(A, i, mid);
    mergeHelp(A, mid+1, j);
    merge(A, i, mid, j);

  }

}

// merge is the main work of Merge Sort, it takes in a start (p), middle (q), and end index (r) of array A.
// It will create a temporary array to be able to hold onto the values between p and r inclusive.
// Since the idea is we are have split the arrays down to this point, we will only be taking in two sorted arrays
// (even if they are size 1 they are still sorted, anything beyond 1 will have been sorted by this function)
// Thus we will track indexes to the left and right arrays, compare the values, add then add them to the temp array
// eventually one side will run out of values to compare to than the other, so we will run two  additional copies of the
// first while loop , however with only one of each condition (left is not empty or right is not empty) and just add those
// values to the temp array since all leftover values will be greater (we are sorting in ascending order) than the already
// sorted values
void merge(Pet* A[], int p, int q, int r) {

  int size = (r-p) + 1;
  int index = 0;

  Pet** merged = new Pet*[size]{nullptr};

  int left = p;
  int right = q+1; // right starts at middle plus one

  // While left index tracker has not passed the middle, and right index tracker has not passed the end
  while (left <= q && right <= r) {

    if (A[left]->weight <= A[right]->weight) { // If left value <= right, add left to temp array and increment left

      merged[index] = A[left];
      left++;

    } else { // else add the right and increment right

      merged[index] = A[right];
      right++;

    }
    index++;



  } // end of first while

  while (left <= q) { // Left side remains unsorted, add to the end

    merged[index] = A[left];
    left++;
    index++;

  }
  while (right < r+1) { // Right side remains unsorted, add to the end

    merged[index] = A[right];
    right++;
    index++;

  }

  // Copy the values of our temp array into the actual array, Start at index p because as we split the array in our helper
  // we will end up at different start indexes for what we are merging here, all of which start at p and span size
  for (int k = 0; k < size; k++) {
    A[p] = merged[k];
    p++;

  }

  // We are done with the temp array, so we can free the memory, this Merge Sort as a whole still uses O(n) space as we
  // have to create a temporary array of size n when we are merging the final left and right half of original array size n
  delete[] merged;

}


// QUICK SORT AND HELPERS BELOW
// QUICK SORT AND HELPERS BELOW
// QUICK SORT AND HELPERS BELOW

// quickSort is what will be used for our function pointer in main, like the others this won't be the recursive method
// since Quick Sort needs more arguments (a start and end index) so we use this function to call a recursive helper
// starting from 0 to size-1 to work with the indices of array A
void quickSort(Pet* A[], int size) {

  quickHelp(A, 0, size-1);

}

// Quick Sort as a whole works by partitioning an array, this means picking an element in an array, putting it at the beginning
// of the array, and moving the other elements into two regions of <= to the pivot and > the pivot (for ascending order sort)
// doing so means you can then move the pivot in between those sections, and you have its final position.
// At first glance this seems like bubble sort, comparing one element to every other element in the array to find that
// one element's position, and it essentially is. Where it differs from bubble sort is it sort of takes a merge sort approach
// in then dividing the array into two sub parts (reducing comparisons) and in this case, partitioning again.
// The important part is picking a good pivot element ideally the middle value so that the array gets perfectly split
// in two each time, this of course cannot be guaranteed so the best thing to do to consider average case is to pick a
// random pivot element and then sort. Since we will be finding each element's exact position in the array, we can sort in
// place and not use extra space for a temporary array
void quickHelp(Pet* A[], int i, int j) {

  if (i < j) {

    int pivotIndex = partitionRandom(A, i, j);
    quickHelp(A, i, pivotIndex-1);
    quickHelp(A, pivotIndex + 1, j);

  }

}

// partitionRandom generates a random number between i and j inclusive, swaps that value in the array to the front, then
// calls our partition method
int partitionRandom(Pet* A[], int i, int j) {

  int randomPosition = i + rand() % (j - i + 1);
  swap(A[i], A[randomPosition]);
  return partition(A, i, j);

}

// partition will check the elements within the rang of i through j and increment an s1 counter that starts at the first
// index i whenever there is a value found <= to the pivot, then place the value in the rang of i to s1. Thus by the end of the loop
// s1 will be the index of the end of the s1 region and we can swap our pivot into it's position
int partition(Pet* A[], int i, int j) {

  int pivotElement = A[i]->weight;
  int s1 = i; // s1 tracks the end index of the <= pivot region

  for (int k = i+1; k <= j; k++) { // pivot is at i, so start comparing at i+1

    if (pivotElement < A[k]->weight) {
      continue;

    } else {

      s1++;
      swap(A[s1], A[k]);

    }

  }

  swap(A[s1], A[i]);
  return s1;
}



// Heap sort doesn't necessarily need a helper, however when building a heap it is FAR easier to have it inside a class
// object so that when building/sorting, you always have access to the array without having to pass it into calls like
// percolate that are going to execute thousands of times.
// Create a heap based on the random array A of size n, and call a object member function to sort the array and copy values
// into A
void heapSort(Pet* A[], int n) {

  MaxHeap heap(A, n);
  heap.maxSort(A);

}

// Use extra space to create a MaxHeap of size n and fill ti with the contents of A
MaxHeap::MaxHeap(Pet* A[], int n) {  

  petHeap = new Pet*[n];
  numPets = n;
  for (int i = 0; i < n; i++) {
    petHeap[i] = A[i];
    percolateUp(i);
  }

}

// Destructor to free memory once we are done with the object
MaxHeap::~MaxHeap() {
    delete[] petHeap;

}

// maxSort, sort our heap and copy the value over to A
void MaxHeap::maxSort(Pet* A[]) {

    // hold onto numPets for copying values back into A after sorting
    int size = numPets;

    // We intentionally iterate through and decrement the member variable tracking size so that we don't touch the max
    // value we swapped to the end after percolating down (since percolate uses numPets after decrementing it will assume
    // the swapped value is out of bounds)
    while (numPets > 0) {

        numPets--;
        swap(petHeap[0], petHeap[numPets]);
        if (numPets != 0) { // make sure not to call percolate down on the final value when we are done, avoid ruining heap
            percolateDown(0);
        }

    }

    // Copy values back into A
    for (int i = 0; i < size; i++) {
        A[i] = petHeap[i];

    }
}

// HEAP BUILDING METHODS FROM LAB4 BELOW

// PERCOLATE UP FUNCTION
// Will be called as part of our insert function, since we are building a MaxHeap we calculate
// the parent index of the child we have added at index and compare the ages, if the parent is
// younger, we will use our helper function and swap the two pets and continue to call percolate
// up at the parent index (since we swapped the values we will keep following the child) until
// we reach a parent that is older than the child we are moving up, or we have made it all the
// way to the root, index 0
void MaxHeap::percolateUp(int index) {

    if (index == 0) {
        return;

    }

    int parent = (index-1) / 2;

    if (petHeap[parent]->weight <  petHeap[index]->weight) {

        swap(petHeap[parent], petHeap[index]);
        percolateUp(parent);

    }
    return;

}

// PERCOLATE DOWN FUNCTION
// Will be called as part of our delete function, since we are building a MaxHeap we calculate
// the index of our nodes children and
void MaxHeap::percolateDown(int index) {

  Pet* leftChild;
    int leftChildIndex;

    switch (whichChildrenToCheck(index)) { // Call helper function to know which children to check for

        case 'n': // case 'n', node at index has no children, so we can't move it down any more than it's current position
            break;


        // Case 'f' Index is out of bounds, this is an error case that should never happen, we will print a message
        // to terminal for the event it gets called
        case 'f':
            cout << endl << "Error, and index outside of the heaps size was called to percolate down!" << endl;
            break;

        // Cae 'l' Node only has a left child
        case 'l':
            leftChildIndex = 2*index + 1;
            if (petHeap[leftChildIndex]->weight > petHeap[index]->weight) {
                swap(petHeap[index], petHeap[leftChildIndex]);
                percolateDown(leftChildIndex);
                
            }
            break;

        case 'b': // case both children exist in our heap

            int rightChildIndex = 2*index + 2;
            leftChildIndex = 2*index + 1;
            Pet* rightChild = petHeap[rightChildIndex];
            leftChild = petHeap[leftChildIndex];
            Pet* largest = petMax(leftChild, rightChild);

            if (largest->weight > petHeap[index]->weight) {

              if (leftChild == largest) {
                swap(petHeap[index], petHeap[leftChildIndex]);
                percolateDown(leftChildIndex);

              } else {
                swap(petHeap[index], petHeap[rightChildIndex]);
                percolateDown(rightChildIndex);

            }

            }
            break;



    } //end of switch

} // end of percolate down

// WHICH CHILDREN TO CHECK HELPER FUNCTION
// Helper function to feed into a switch choice for percolate down since we need to do different comparisons based on
// whether a not a node has kids and which ones if it does have some.
// The switch choice will act accordingly to the char returned by this function
char MaxHeap::whichChildrenToCheck(int index) {

    int indexLC = 2*index + 1;
    int indexRC = 2*index + 2;

    if (index > numPets) { //Index is out of bounds, return 'f' for failed index
        return 'f';

        // If the right child is within the array bounds, since this is a complete tree, there will be a left child guaranteed
        // return 'b' for both children
    } else if (indexRC < numPets) {
        return 'b';

        // Similar to logic above, if the left child is out of bounds of our array,  we don't have to check the right
        // since we know it doesn't exist return 'n' for none
    } else if ( indexLC >= numPets) {
        return 'n';

    } else { // Final case is just having a left child since our nodes will never have just a right, return 'l' for left
        return 'l';

    }
}

// Takes in two pets and returns the one with the larger weight, useful for percolate down and checking two children
Pet* MaxHeap::petMax(Pet* pet1, Pet* pet2) {

  if (pet1->weight >= pet2->weight) {
      return pet1;

  } else {
      return pet2;

  }
  return nullptr;
}
