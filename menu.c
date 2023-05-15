// Copyright Mihai-Cosmin Nour & David-Cristian Bacalu 311CAb 2022-2023

#include <stdio.h>

void start_info(void)
{
	printf("----IMAGE-EDITOR----\n\n");

	printf("Add an image to begin. Use the command \"LOAD\"\n");
}

void next_command(void)
{
	printf("1. To load a new image: \"LOAD\"\n");

	printf("2. To select the whole image: \"SELECT ALL\"\n");

	printf("3. To select a part of the image: ");
	printf("\"SELECT <x1> <y1> <x2> <y2>\"\n");

	printf("4. To show the histogram (for grayscale images): \"HISTOGRAM\"\n");

	printf("5. To rotate the selected part of the image: \"ROTATE\"\n");

	printf("6. To crop the image to the selection: \"ROTATE\"\n");

	printf("7. To change the image: \"APPLY <TYPE>\"\n");
	printf("\t\tType is: \"EDGE\", \"SHARPEN\", \"BLUR\", \"GAUSSIAN_BLUR\"\n");

	printf("8. To save the image: \"SAVE <file_name> [ascii]\"\n");
	
	printf("\t\tUse \"ascii\" for text file or nothing for binary file\n");

	printf("9. To exit the program: \"EXIT\"\n");

	printf("Your command:");
}

