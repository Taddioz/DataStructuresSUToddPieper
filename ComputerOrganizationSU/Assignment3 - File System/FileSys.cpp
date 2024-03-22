// Todd Pieper CPSC3500 Assignment 2
// CPSC 3500: File System
// Implements the file system commands that are available to the shell.

#include <cstring>
#include <iostream>
#include <cmath>
using namespace std;

#include "FileSys.h"
#include "BasicFileSys.h"
#include "Blocks.h"

// mounts the file system
void FileSys::mount() {
  bfs.mount();
  curr_dir = 1;
}

// unmounts the file system
void FileSys::unmount() {
  bfs.unmount();
}


// make a directory
void FileSys::mkdir(const char *name)
{

  // Check if the file name is too long
  if (strlen(name) > 9) {
    cout << "Error: File name is too long" << endl;
    return;
  }

  // Read the current directory
  dirblock_t currentDirBlock;
  bfs.read_block(curr_dir, (void *) &currentDirBlock);

  // Check if the current directory is full
  if (currentDirBlock.num_entries == MAX_DIR_ENTRIES) {
    cout << "Error: Directory is full" << endl;
    return;
  }

  // Check if the new directory name is unique to the cwd
  if (findFile((void*) &currentDirBlock, name) != -11) {
    cout << "Error: File already exists" << endl;
    return;
  }
  

  // Check if disk is full
  short freeBlock = bfs.get_free_block();
  if (freeBlock == 0) {
    cout << "Error: Disk is full" << endl;
    return;
  }

  // Passed all error checks, and we now have the next free block stored in variable "freeblock"

  // Create new directory
  dirblock_t newDirBlock;
  newDirBlock.magic = DIR_MAGIC_NUM;
  newDirBlock.num_entries = 0;

  // Add new directory to current directory and update the count
  strcpy(currentDirBlock.dir_entries[currentDirBlock.num_entries].name, name);
  currentDirBlock.dir_entries[currentDirBlock.num_entries].block_num = freeBlock;
  currentDirBlock.num_entries++;

  // Write both the current directory and new directory back to the disk
  bfs.write_block(curr_dir, (void *) &currentDirBlock);
  bfs.write_block(freeBlock, (void *) &newDirBlock);



}

// list the contents of current directory
void FileSys::ls()
{
  // Read the current directory
  dirblock_t currentDirBlock, dirEntry;
  inode_t test;
  bfs.read_block(curr_dir, (void *) &currentDirBlock);

  // Check if the new directory name is unique to the cwd
  for (int i = 0; i < currentDirBlock.num_entries; i++) {

    // Always print the name of the entry
    cout << currentDirBlock.dir_entries[i].name;

    // Read the block to check if it is a directory or inode, if directory, concatenate a forward slash
    if (isDirectory(currentDirBlock.dir_entries[i].block_num)) {
      cout << "/";
    }

    // Regardless, print a newline
    cout << endl;

  }

}

// switch to a directory
// all names are unique relative to a current directory, so if we  find a file with a name matching
// what we attempt to switch to, we don't have to search the rest of the cwd for a directory with the
// correct name
void FileSys::cd(const char *name)
{


  // Read the current directory
  dirblock_t currentDirBlock, dirEntry;
  bfs.read_block(curr_dir, (void *) &currentDirBlock);

  // Call helper function to find the index of the entry with the matching file name
  short index = findFile((void*) &currentDirBlock, name);

  // Index 0-9 means it's a directory, change global curr_dir to it's block number
  if (index >= 0) {

    curr_dir = currentDirBlock.dir_entries[index].block_num;
    return;
    
  } else if (index == -11) { // File with name DNE

    cout << "Error: File does not exist" << endl;
    return;

  } else { // File with matching name is an inode

    cout << "Error: File is not a directory" << endl;
    return;

  }
  
}// end of cd


// switch to home directory
void FileSys::home() {
  curr_dir = 1;
}


// remove a directory
void FileSys::rmdir(const char *name)
{
  // Read the current directory
  dirblock_t currentDirBlock;
  bfs.read_block(curr_dir, (void *) &currentDirBlock);

  // Call helper function to find the index of the entry with the matching file name
  short index = findFile((void*) &currentDirBlock, name);

  if (index == -11) { // File DNE
    cout << "Error: File does not exist" << endl;
    return;

  } else if (index < 0) { // < 0 means it's an inode
    cout << "Error: File is not a directory" << endl;
    return;

  }

  // Else it is a directory, need to check if its empty
  dirblock_t toBeRemoved;
  short toBeRemoved_blockNum = currentDirBlock.dir_entries[index].block_num;
  bfs.read_block(toBeRemoved_blockNum, (void*) &toBeRemoved);

  if (toBeRemoved.num_entries > 0) {
    cout << "Error: Directory is not empty" << endl;
    return;

  }

  // Else directory is empty
  // Heap style deletion of decrementing the entry count and swapping the directory to the end
  // So that it will not be read through during prints and overridden if new entries are added
  // Must also reclaim the block number of the deleted directory, and write the current back to disk
  currentDirBlock.num_entries--;
  swap(currentDirBlock.dir_entries[index],
       currentDirBlock.dir_entries[currentDirBlock.num_entries]);
  bfs.reclaim_block(toBeRemoved_blockNum);
  bfs.write_block(curr_dir, &currentDirBlock);
  return;

}



// create an empty data file
void FileSys::create(const char *name)
{

  // Check if the file name is too long                                                           
  if (strlen(name) > 9) {

    cout << "Error: File name is too long" << endl;
    return;

  }

  // Read the current directory                                                                   
  dirblock_t currentDirBlock;
  bfs.read_block(curr_dir, (void *) &currentDirBlock);

  // Check if the current directory is full                                                       
  if (currentDirBlock.num_entries == MAX_DIR_ENTRIES) {

    cout << "Error: Directory is full" << endl;
    return;

  }

  // Check if the new directory name is unique to the cwd                                         
  if (findFile((void*) &currentDirBlock, name) != -11) {

    cout << "Error: File already exists" << endl;
    return;

  }

  // Check if disk is full, if not, we still hold the value of the freeblock
  short freeBlock = bfs.get_free_block();
  if (freeBlock == 0) {

    cout << "Error: Disk is full" << endl;
    return;

  }

  // Create an inode and initialize fields
  inode_t fileNode;
  fileNode.magic = INODE_MAGIC_NUM;
  fileNode.size = 0;

  // Set the block/name in the current directory entry to the block/name obtained for the node
  // Increment the current directory entry count
  // Then write both the inode and current directory back to disk
  currentDirBlock.dir_entries[currentDirBlock.num_entries].block_num = freeBlock;
  strcpy(currentDirBlock.dir_entries[currentDirBlock.num_entries].name, name);
  currentDirBlock.num_entries++;
  bfs.write_block(freeBlock, (void*) &fileNode);
  bfs.write_block(curr_dir, (void*) &currentDirBlock);
}

// append data to a data file
void FileSys::append(const char *name, const char *data)
{

  // Read the current directory
  dirblock_t currentDirBlock;
  bfs.read_block(curr_dir, (void*) &currentDirBlock);

  // Call helper function to find the index of the entry with the matching file name
  short index = findFile((void*) &currentDirBlock, name);

  if (index == -11) { // File DNE

    cout << "Error: File does not exist" << endl;
    return;

  } else if (index >=0) { // 0-9 File is a directory

    cout << "Error: File is a directory" << endl;
    return;

  } else { // File is an inode

    // convert the mapped index back to 0-9 range
    index *= -1;
    index--;

    // Read the inode
    inode_t iNode;
    short iNode_blockNum = currentDirBlock.dir_entries[index].block_num;
    bfs.read_block(iNode_blockNum, (void *) &iNode);

    unsigned int fileSize = iNode.size;
    unsigned int dataLength = strlen(data);

    // Check if the file is full or if the added data will overfill the file
    if ((fileSize + dataLength) > (MAX_FILE_SIZE)) {

      cout << "Error: Append exceeds maximum file size" << endl;
      return;

    }

    datablock_t dataBlock;
    short indexer = 0, offset, currentDB, freeBlock;
    bool needMoreBlocks = false;

    offset = fileSize % BLOCK_SIZE;
    currentDB = fileSize / BLOCK_SIZE;

    // Offset tells where the next write operation goes, if it is zero, then it is the beginning of a new block
    // if not, we have room in a block to append data
    if (offset != 0) {

      // Read the current block
      bfs.read_block(iNode.blocks[currentDB], (void *) &dataBlock);

      // Attempt to append all the data, break if we reach the end of the current block, and signal
      // the need to create more blocks
      for (short i = 0; i < dataLength; i++) {

        if (offset == BLOCK_SIZE) {
          needMoreBlocks = true;
          indexer = i;
          break;

        }

        dataBlock.data[offset] = data[i];
        offset++;
        fileSize++;

      }

      // Update the inode file size, write the data block and inode back to the disk
      iNode.size = fileSize;
      bfs.write_block(iNode.blocks[currentDB], (void *) &dataBlock);
      bfs.write_block(iNode_blockNum, (void *) &iNode);

      // Early exit if we do not need to create more blocks
      if (!needMoreBlocks) {
        return;
      }

    }


    // Number of new block will find out how much data we have to append and calculate the number of blocks needed
    // to store it. Make sure to subtract the amount of data already appended (indexer, 0 if none appended)
    // before dividing
    short numNewBlocks = ceil((double) (dataLength - indexer) / BLOCK_SIZE);
    datablock_t newDataBlock;

    for (short j = 0; j < numNewBlocks; j++) {

      // Check if disk is full
      freeBlock = bfs.get_free_block();
      if (freeBlock == 0) {

        cout << "Error: Disk is full" << endl;
        return;

      }

      currentDB = fileSize / BLOCK_SIZE; // Index in inode for new data block
      iNode.blocks[currentDB] = freeBlock;
      offset = 0; // Set offset to zero as we begin to append data to a new block

      // start character loop at indexer where we left off appending
      for (short k = indexer; k < dataLength; k++) {

        // If block is full, write it to disk and break appending loop but not block creation loop
        // in case there is more to append
        if (offset == BLOCK_SIZE) {
          bfs.write_block(freeBlock, (void *) &newDataBlock);
          indexer = k;
          break;

        }

        newDataBlock.data[offset] = data[k];
        offset++;
        fileSize++;

      }

    } // end of block making loop

    // Set the size of file, and write it and the data block to the disk
    iNode.size = fileSize;
    bfs.write_block(iNode_blockNum, (void *) &iNode);
    bfs.write_block(freeBlock, (void *) &newDataBlock);
    return;

  } // end of iNode else block

} // end of append

// display the contents of a data file
void FileSys::cat(const char *name) {

  // Read the current directory
  dirblock_t currentDirBlock;
  bfs.read_block(curr_dir, &currentDirBlock);

  // Call helper function to find the index of the entry with the matching file name
  short index = findFile((void *) &currentDirBlock, name);

  if (index == -11) { // File DNE

    cout << "Error: File does not exist" << endl;
    return;

  } else if (index >= 0) { // File is a directory

    cout << "Error: File is a directory" << endl;
    return;

  } else { // File is inode

    // Convert the mapped index back to 0-9
    index *= -1;
    index--;

    // Read the inode
    inode_t iNode;
    bfs.read_block(currentDirBlock.dir_entries[index].block_num, (void *) &iNode);

    short numBlocksInFile = ceil( (double) iNode.size / BLOCK_SIZE);
    short fileSize = iNode.size;
    short byteCount;
    datablock_t dataBlock;

    for (short i = 0; i < numBlocksInFile; i++) { // loop through blocks in file

      // Read the block at index i
      bfs.read_block(iNode.blocks[i], (void*) &dataBlock);

      // Set byte count to file size, since filesize is cumulative of all blocks, check if this block is full
      // by subtracting the max block size from file size. If the result is greater than zero, then the current block
      // is full so we only want to loop up to the max block size and not the full file. Loop will repeat process
      // until we reach the last block in the file
      byteCount = fileSize;
      fileSize -= BLOCK_SIZE;
      if(fileSize > 0) {
        byteCount = BLOCK_SIZE;

      }

      for (short j = 0; j < byteCount; j++) { // print out data character by character
        cout << dataBlock.data[j];

      }

      // Formatting newline to print after each block
      cout << endl;

    } // end of block loop

  } // end of inode else

  return;

} // end of cat

// delete a data file
void FileSys::rm(const char *name)
{

  // Read the current directory
  dirblock_t currentDirBlock;
  bfs.read_block(curr_dir, &currentDirBlock);

  // Call helper function to find the index of the entry with the matching file name
  short index = findFile((void *) &currentDirBlock, name);

  if (index == -11) { // File DNE

    cout << "Error: File does not exist" << endl;
    return;

  } else if (index >= 0) { // File is a directory

    cout << "Error: File is a directory" << endl;
    return;

  } else { // File is an inode

    // Convert the mapped index back to 0-9
    index *= -1;
    index--;

    // Read the inode
    inode_t iNode;
    short toBeRemoved_blockNum = currentDirBlock.dir_entries[index].block_num;
    bfs.read_block(toBeRemoved_blockNum, (void *) &iNode);

    // Calculate the number of data blocks it has we have to reclaim
    short numBlocksInFile = ceil( (double) iNode.size / BLOCK_SIZE);

    for (short i = 0; i < numBlocksInFile; i++) {
      bfs.reclaim_block(iNode.blocks[i]);

    }

    // Data blocks have been reclaimed, now we reclaim the inode itself
    // Heap style deletion of decrementing the entry count and swapping the directory to the end
    // So that it will not be read through during prints and overridden if new entries are added
    // After reclaiming, write the current directory back to the disk
    currentDirBlock.num_entries--;
    swap(currentDirBlock.dir_entries[index],
         currentDirBlock.dir_entries[currentDirBlock.num_entries]);
    bfs.reclaim_block(toBeRemoved_blockNum);
    bfs.write_block(curr_dir, &currentDirBlock);
    return;
  }
}

// display stats about file or directory
void FileSys::stat(const char *name)
{

  // Read the current directory
  dirblock_t currentDirBlock;
  bfs.read_block(curr_dir, &currentDirBlock);

  // Call helper function to find the index of the entry with the matching file name
  short index = findFile((void*) &currentDirBlock, name);

  if (index == -11) { // File DNE

    cout << "Error: File does not exist" << endl;
    return;

  } else if (index < 0) { // File is an inode

    // Convert mapped index of -10 to -1 back to 0-9
    index *= -1;
    index--;

    // Read the inode and print out its block number, total size, and number of datablocks
    inode_t iNode;
    bfs.read_block(currentDirBlock.dir_entries[index].block_num, (void*) &iNode);
    unsigned int size = iNode.size;
    cout << "Inode block: "
         << currentDirBlock.dir_entries[index].block_num << endl
         << "Bytes in file: "
         << size << endl
         << "Number of blocks: "
         << ceil( (double) size / BLOCK_SIZE) + 1 << endl;
    return;

  } else { // File is a directory, print out its block number

    cout << "Directory block: "
         << currentDirBlock.dir_entries[index].block_num
         << endl;
  }

  // ICOE return
  return;
  
}

// HELPER FUNCTIONS (optional)
bool FileSys::isDirectory(short blockNum) {

  // Read the block at the block number, even though we don't know if it is an inode or directory
  // storing it as a directory block is okay as they both have an unsigned int field called magic
  dirblock_t entry;
  bfs.read_block(blockNum, (void*) &entry);

  // read the magic field, if it matches the directory number, return true, else it's an inode, return false
  if (entry.magic == DIR_MAGIC_NUM) {
    return true;
  }

  return false;
}

// Helper function to take in a directory and name, and return the index of file
// with the matching name 3 types of function returns:
// -11  ----  If no matching file is found
// 0-9  ----  If a matching directory is found
// -1-(-10) - If a matching inode is found
// Indices in the directory use 0-9 regardless of being an inode or a directory, to help discern
// the return to the calling function Indices 0-9 are mapped to -1-(-10) and converted back as needed
short FileSys::findFile(void* directory, const char *name) {

  short blockNumber, index = -11;
  dirblock_t currentDirBlock = *(dirblock_t *) directory; 
  for (short i = 0; i < currentDirBlock.num_entries; i++) {

    // Check if the entry has a matching name
    if (strcmp(currentDirBlock.dir_entries[i].name, name) == 0) {

      blockNumber = currentDirBlock.dir_entries[i].block_num;

      // Check if entry is a directory, if it is, return the index i
      if (isDirectory(blockNumber)) {
        index = i;

      } else {

      // Not a directory but matching name means it's an inode
      // to differentiate the return we can add 1 and multiply by
      // negative one to change the 0-9 into -1 to -10
      index = ((i+1) * -1);
      }
      
      break;

    }

  }

  return index;
}
