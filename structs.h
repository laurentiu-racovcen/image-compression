#ifndef _STRUCTS_H_
#define _STRUCTS_H_

#include <stdio.h>
#include <stdlib.h>

typedef struct {
	unsigned char red;
	unsigned char green;
	unsigned char blue;
} pixel;

typedef struct {
	char* img_format;
	unsigned int width;
	unsigned int height;
	unsigned int max_color_value;
	pixel** pixels_matrix;
} img_data;

typedef pixel* Item;

typedef struct node {
	pixel value;
	unsigned int x;
	unsigned int y;
	unsigned int size;
	struct node** descendants;
	unsigned char isLeaf;
} QuadTreeNode, *Tree;

#endif	// _STRUCTS_H_
