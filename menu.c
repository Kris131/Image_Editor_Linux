// Copyright Mihai-Cosmin Nour & David-Cristian Bacalu 311CAb 2022-2023

#include <stdio.h>
#include <stdlib.h>

#include "my_defs.h"
#include "utils.h"

void start_info(char *file_name)
{
	system("clear");

	printf("----------------IMAGE-EDITOR----------------\n\n");

	if (!strlen(file_name))
		printf("Add an image to begin. Use the command \"LOAD\".\n");
	else
		printf("Current image: %s\n", file_name);

	printf("Type \"HELP\" for more information.\n\n");

	printf("Type your command: ");
}

void go_back(void)
{
	printf("\nPress enter to return to the menu.\n");

	getchar();
}

void next_command(void)
{
	system("clear");

	printf("----------------HELP----------------\n\n");

	printf("1. To load a new image: \"LOAD\".\n");

	printf("2. To select the whole image: \"SELECT ALL\".\n");

	printf("3. To select a part of the image: ");
	printf("\"SELECT <x1> <y1> <x2> <y2>\".\n");

	printf("4. To show the histogram (for grayscale images): \"HISTOGRAM\".\n");
	printf("\t\tThe HISTOGRAM command uses two parameters: a number of bins ");
	printf("and a number of stars for each bin.\n");

	printf("5. To equalize the image (for grayscale image): \"EQUALIZE\".\n");

	printf("6. To rotate the selected part of the image : \"ROTATE\".\n");

	printf("7. To crop the image to the selection: \"CROP\".\n");

	printf("8. To change the image: \"APPLY <TYPE>\".\n");
	printf("\t\t<TYPE> is: \"EDGE\", \"SHARPEN\", \"BLUR\", \"GAUSSIAN_BLUR\".\n");

	printf("9. To save the image: \"SAVE <file_name> [ascii]\".\n");
	
	printf("\t\tUse \"ascii\" for text file or nothing for binary file.\n");

	printf("10. To open the image: \"OPEN\"\n");
	printf("\t\tAfter opening the image, close the image to continue.\n");

	printf("11. To exit the program: \"EXIT\".\n\n");

	go_back();
}
