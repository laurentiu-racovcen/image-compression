# **Image Compression using Quadtrees**

>This project consists in the implementation of an algorithm for the compression of `PPM` format images. Using quaternary trees, compression and decompression of images can be performed in order to represent them using as little information as possible.

In order to realize the given requirements some structures have been created and declared in the `"structs.h"` file. The structure `"img_data"` contains the given image dimensions, the file format, the maximum intensity of a color and the matrix formed by all pixels of the image. The `"QuadTreeNode"` structure allows the creation of a quaternary tree containing the following data: the RGB channel values as `"pixel"` structure, the `x` and `y` coordinates corresponding to the first pixel of the node's submatrix, the size of the submatrix represented by the given node, a vector of descendant nodes and whether the given node is leaf or not.

The functions needed to perform all the requirements were written in the header files `"utils.h"`, `"QuadTree.h"` and `"Queue.h"`.

## **Table of contents**

1. ["main" function](#main-function)
2. [Functions in the "utils.h" file](#functions-in-the-utilsh-file)
3. [Quaternary tree functions](#quaternary-tree-functions)
4. [Queue functions](#queue-functions)
5. [Functions needed to achieve the 1st requirement](#functions-needed-to-achieve-the-1st-requirement)
6. [Functions needed to achieve the 2nd requirement](#functions-needed-to-achieve-the-2nd-requirement)
7. [Functions needed to achieve the 3rd requirement](#functions-needed-to-achieve-the-3rd-requirement)

## **"main" function**

**1.** For each requirement, 2 variables of type `FILE*` are defined (fin, fout) to access the files:
- `fin` corresponds to the input file, from which the required information to be processed is read;
- `fout` corresponds to the output file, where the results of the given requirement are written;

**2.** For 1st and 2nd requirement, read and store the required information in the `image_data` and `factor` variables,
and for the 3rd requirement, use only the `image_data` variable to store the information;

**3.** Depending on the first argument of the executable on the command line, call the functions:
- `"task1"` - function that realizes 1st requirement;
- `"task2"` - function that realizes 2nd requirement;
- `"task3"` - function that realizes 3rd requirement;

**4.** Frees the dynamically allocated memory for image data

**5.** Closes the "fin" and "fout" files.

## **Functions in the "utils.h" file**

- `"read_image_data"` - function that processes the contents of an image and returns a pointer to a structure containing the image data;
- `"read_compressed_image_data"` - a function that processes the contents of a binary file containing information about an image and returns a pointer to a structure containing the image data;
- `"write_image_data"` - function that writes the binary file based on the quaternary tree created by reading the compressed file;
- `"mean_pixel"` - calculates the average color of a subarray and returns a `pixel` structure containing this color;
- `"colors_mean"` - computes the average RGB color of a submatrix according to the formula given in the requirement;
- `"free_memory"` - function that frees the dynamically allocated memory for image data.

## **Quaternary tree functions**

- `"createTree"` - creates a tree;
- `"insert_node"` - inserts a new node into the tree;
- `"level_order_insert"` - inserts a new node into the tree, in the order of traversal per level;
- `"insert_node_details"` - inserts the details of a node according to its type (whether it is leaf or not);
- `"height"` - returns the height of a tree;
- `"write_current_level"` - writes the node data on a given level to the binary file;
- `"write_all_levels"` - writes to the binary file the data of all nodes in a quaternary tree in order by level;
- `"leaves_number"` - returns the number of leaves of a tree;
- `"get_leaves"` - function that saves the leaf node pointers into a vector;
- `"largest_undivided_area"` - function that returns the size of the largest undivided area (an undivided area is represented by a leaf of the tree);
- `"freeTree"` - function that frees dynamically allocated memory for the quaternary tree;

## **Queue functions**

- `"createQueue"` - creates a queue;
- `"isQueueEmpty"` - determines whether the queue is empty or not;
- `"enqueue"` - inserts a new element into the queue;
- `"dequeue"` - removes an element from the queue;
- `"front"` - returns the value of the first element in the queue;
- `"destroyQueue"` - frees the memory occupied by the dynamically allocated queue;

## **Functions needed to achieve the 1st requirement**

- **`"read_image_data"`**
- **`"insert_node"`**
- **`"height"`**
- **`"leaves_number"`**
- **`"largest_undivided_area"`**
- **`"freeTree"`**
- **`"free_memory"`**

## **Functions needed to achieve the 2nd requirement**

- **`"read_image_data"`**
- **`"insert_node"`**
- **`"write_all_levels"`**
- **`"freeTree"`**
- **`"free_memory"`**

## **Functions needed to achieve the 3rd requirement**

- **`"read_compressed_image_data"`**
- **`"write_image_data"`**
- **`"free_memory"`**
