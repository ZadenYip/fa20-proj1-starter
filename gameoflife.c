/************************************************************************
**
** NAME:        gameoflife.c
**
** DESCRIPTION: CS61C Fall 2020 Project 1
**
** AUTHOR:      Justin Yokota - Starter Code
**				YOUR NAME HERE
**
**
** DATE:        2020-08-23
**
**************************************************************************/

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <sys/types.h>
#include "imageloader.h"

uint8_t transform_negative_index(int index, int size);
uint8_t get_bit(uint8_t byte, uint8_t low2high);

#define CHANNEL_BYTE(INDENTIFY, RECEIVE_CHANNEL_BYTE) \
for (uint8_t bit_index = 0; bit_index < 8; bit_index++) { \
	uint8_t current_state = get_bit(image->image[row][col].INDENTIFY, bit_index); \
	uint8_t live_neighbors = 0; \
	\
	/* Count the number of live neighbors */ \
	for (uint8_t i = 0; i < 8; i++) { \
		uint8_t row_index = direction[i][0]; \
		uint8_t col_index = direction[i][1]; \
		uint8_t bit = get_bit(image->image[row_index][col_index].INDENTIFY, bit_index); \
		live_neighbors += bit; \
	} \
	uint8_t next_state = state_mapping[current_state * 9 + live_neighbors]; \
	*RECEIVE_CHANNEL_BYTE |= next_state << bit_index; \
}



//Determines what color the cell at the given row/col should be. This function allocates space for a new Color.
//Note that you will need to read the eight neighbors of the cell in question. The grid "wraps", so we treat the top row as adjacent to the bottom row
//and the left column as adjacent to the right column.
Color *evaluateOneCell(Image *image, int row, int col, uint32_t rule)
{
	unsigned int state_mapping[18];
	uint32_t mask = 0x1;
	for (int i = 0; i < 18; i++)
	{
		state_mapping[i] = rule & (mask << i) ? 1 : 0;
	}
	// Surrounding cells in 24-bit plane
	uint8_t row_up = transform_negative_index(row - 1, image->rows);
	uint8_t row_down = transform_negative_index(row + 1, image->rows);
	uint8_t col_left = transform_negative_index(col - 1, image->cols);
	uint8_t col_right = transform_negative_index(col + 1, image->cols);

	uint8_t direction[8][2] = {
		{row_up, col_left}, {row_up, col}, {row_up, col_right},
		{row, col_left}, {row, col_right},
		{row_down, col_left}, {row_down, col}, {row_down, col_right}
	};

	uint8_t red = 0;
	uint8_t green = 0;
	uint8_t blue = 0;
	// For each bit in the 8-bit plane
	CHANNEL_BYTE(R, &red);
	CHANNEL_BYTE(G, &green);
	CHANNEL_BYTE(B, &blue);
	Color *new_color = malloc(sizeof(Color));
	*new_color = (Color){red, green, blue};
	return new_color;
}

//The main body of Life; given an image and a rule, computes one iteration of the Game of Life.
//You should be able to copy most of this from steganography.c
Image *life(Image *image, uint32_t rule)
{
	Image *new_image = malloc(sizeof(Image));
	/* Init */
	new_image->cols = image->cols;
	new_image->rows = image->rows;
	new_image->image = malloc(sizeof(Color *) * new_image->rows);
	for (int i = 0; i < new_image->rows; i++) {
		new_image->image[i] = malloc(sizeof(Color) * new_image->cols);
	}

	for (int row = 0; row < new_image->rows; row++) {
		for (int col = 0; col < new_image->cols; col++) {
			Color* color = evaluateOneCell(image, row, col, rule);
			new_image->image[row][col] = *color;
			free(color);
		}
	}
	return new_image;
}

/*
Loads a .ppm from a file, computes the next iteration of the game of life, then prints to stdout the new image.

argc stores the number of arguments.
argv stores a list of arguments. Here is the expected input:
argv[0] will store the name of the program (this happens automatically).
argv[1] should contain a filename, containing a .ppm.
argv[2] should contain a hexadecimal number (such as 0x1808). Note that this will be a string.
You may find the function strtol useful for this conversion.
If the input is not correct, a malloc fails, or any other error occurs, you should exit with code -1.
Otherwise, you should return from main with code 0.
Make sure to free all memory before returning!

You may find it useful to copy the code from steganography.c, to start.
*/
int main(int argc, char **argv)
{
	if (argc != 3) {
		printf("Error: invalid number of arguments\n");
		exit(-1);
	}
	char *filename = argv[1];
	char *rule_str = argv[2];
	uint32_t rule = strtol(rule_str, NULL, 16);
	Image *image = readData(filename);
	if (image == NULL) {
		printf("Error: readData failed\n");
		exit(-1);
	}
	Image *new_image = life(image, rule);
	writeData(new_image);
	freeImage(image);
	freeImage(new_image);
	return 0;
}

uint8_t transform_negative_index(int index, int size) {
  if (index < 0) {
    return size + index;
  } else if (index >= size) {
    return index % size;
  } else {
    return index;
  }
}

uint8_t get_bit(uint8_t byte, uint8_t low2high) {
	uint8_t mask = 0x1;
	return (byte & (mask << low2high)) != 0;
}