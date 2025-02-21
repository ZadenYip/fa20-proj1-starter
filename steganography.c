/************************************************************************
**
** NAME:        steganography.c
**
** DESCRIPTION: CS61C Fall 2020 Project 1
**
** AUTHOR:      Dan Garcia  -  University of California at Berkeley
**              Copyright (C) Dan Garcia, 2020. All rights reserved.
**				Justin Yokota - Starter Code
**				YOUR NAME HERE
**
** DATE:        2020-08-23
**
**************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include "imageloader.h"

//Determines what color the cell at the given row/col should be. This should not affect Image, and should allocate space for a new Color.
Color *evaluateOnePixel(Image *image, int row, int col)
{
	uint8_t blue = image->image[row][col].B;
	uint8_t check_bit = 0x1; // 0000 0001
	Color *new_color = malloc(sizeof(Color));
	if (blue & check_bit) {
		// White
		new_color->R = 255;
		new_color->G = 255;
		new_color->B = 255;
		return new_color;
	} else {
		// Black
		new_color->R = 0;
		new_color->G = 0;
		new_color->B = 0;
		return new_color;
	}
}

//Given an image, creates a new image extracting the LSB of the B channel.
Image *steganography(Image *image)
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
			Color* decode_color = evaluateOnePixel(image, row, col);
			new_image->image[row][col] = *decode_color;
			free(decode_color);
		}
	}
	return new_image;
}

/*
Loads a file of ppm P3 format from a file, and prints to stdout (e.g. with printf) a new image, 
where each pixel is black if the LSB of the B channel is 0, 
and white if the LSB of the B channel is 1.

argc stores the number of arguments.
argv stores a list of arguments. Here is the expected input:
argv[0] will store the name of the program (this happens automatically).
argv[1] should contain a filename, containing a file of ppm P3 format (not necessarily with .ppm file extension).
If the input is not correct, a malloc fails, or any other error occurs, you should exit with code -1.
Otherwise, you should return from main with code 0.
Make sure to free all memory before returning!
*/
int main(int argc, char **argv)
{
	if (argc != 2) {
		exit(-1);
	}
	Image *image = readData(argv[1]);
	
	if (image == NULL) {
		exit(-1);
	}
	Image *new_image = steganography(image);
	writeData(new_image);
	freeImage(image);
	freeImage(new_image);
	return 0;
}