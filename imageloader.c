/************************************************************************
**
** NAME:        imageloader.c
**
** DESCRIPTION: CS61C Fall 2020 Project 1
**
** AUTHOR:      Dan Garcia  -  University of California at Berkeley
**              Copyright (C) Dan Garcia, 2020. All rights reserved.
**              Justin Yokota - Starter Code
**				YOUR NAME HERE
**
**
** DATE:        2020-08-15
**
**************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <string.h>
#include "imageloader.h"

void read_row(FILE *file, Image *image, unsigned int current_row, unsigned int max_color);

//Opens a .ppm P3 image file, and constructs an Image object. 
//You may find the function fscanf useful.
//Make sure that you close the file with fclose before returning.
Image *readData(char *filename) 
{
	Image* image = malloc(sizeof(Image));
	FILE *file = fopen(filename, "r");
	char header[3];
	fscanf(file, "%s", header);
	if (strcmp(header, "P3") != 0) {
		printf("Not a P3 PPM file.\n");
		return NULL;
	}

	unsigned int cols = 0;
	unsigned int rows = 0;
	fscanf(file, "%u %u", &cols, &rows);
	image->cols = (uint32_t) cols;
	image->rows = (uint32_t) rows;
	image->image = malloc(sizeof(Color *) * rows);
	for (int i = 0; i < cols; i++) {
		image->image[i] = malloc(sizeof(Color) * cols);
	}

	unsigned int max_color = 0;
	fscanf(file, "%u", &max_color);

	for (int row = 0; row < rows; row++) {
		read_row(file, image, row, max_color);

		char lf = fgetc(file);
		if (lf != '\n') {
		  printf("Error in file format.\n");
		  exit(1);
		}
	}

	fclose(file);
	return image;
}

//Given an image, prints to stdout (e.g. with printf) a .ppm P3 file with the image's data.
void writeData(Image *image)
{
	printf("P3\n");
	printf("%d %d\n", image->cols, image->rows);
	printf("255\n");
	for (int i = 0; i < image->rows; i++) {
		Color* color_row = image->image[i];
		for (int j = 0; j < image->cols - 1; j++) {
			printf("%3u %3u %3u   ", color_row[j].R, color_row[j].G, color_row[j].B);
		}
		if (image->cols > 0) {
			printf("%3u %3u %3u", color_row[image->cols - 1].R, color_row[image->cols - 1].G, color_row[image->cols - 1].B);
		}
		printf("\n");
	}
}

//Frees an image
void freeImage(Image *image)
{
	for (int i = 0; i < image->rows; i++) {
		free(image->image[i]);
	}
	free(image->image);
	free(image);
}

void read_row(FILE *file, Image *image, unsigned int current_row, unsigned int max_color) {
  unsigned int r = 0;
  unsigned int g = 0;
  unsigned int b = 0;
  for (int col = 0; col < image->cols; col++) {
    fscanf(file, "%u %u %u", &r, &g, &b);

	if (r > max_color || g > max_color || b > max_color) {
	  printf("Error: Color value exceeds max color value.\n");
	  exit(1);
	}

    image->image[current_row][col].R = (uint8_t)r;
    image->image[current_row][col].G = (uint8_t)g;
    image->image[current_row][col].B = (uint8_t)b;
  }
}