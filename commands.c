// Copyright Mihai-Cosmin Nour & David-Cristian Bacalu 311CA 2022-2023

#include "commands.h"
#include "menu.h"

// Search for a given command inside a list of commands
int which_command(char *command, const char **command_list, int n_commands)
{
	for (int i = 0; i < n_commands; ++i)
		if (strcmp(command, command_list[i]) == 0)
			return i;
	return -1;
}

// Open a file and load the image
void load_cmd(image_t *image, selection_t *sel, char *file_name)
{
	system("clear");
	printf("----------------LOAD----------------\n\n");
	printf("Type the path to the wanted file: ");
	FILE *image_file = NULL;
	fgets(file_name, BUFFER_SIZE, stdin);
	remove_newline(file_name);

	// clear from memory the existing image
	if (image->gray_img) {
		free_matrix(image->row_num, image->gray_img);
		image->gray_img = NULL;
	}
	if (image->color_img) {
		free_color_matrix(image->row_num, image->color_img);
		image->color_img = NULL;
	}
	strcpy(image->header, "");

	// Open the file as "read text"
	image_file = fopen(file_name, "rt");
	if (!image_file) {
		printf("%s%s\n", LOAD_FAIL, file_name);
		strcpy(file_name, "");
		go_back();
		return;
	}

	// Obtain the type of image and, if it's binary, reopen the file as binary
	fgets(image->type, SMALL_SIZE + 1, image_file);
	if (image->type[1] == '5' || image->type[1] == '6') {
		fclose(image_file);
		image_file = fopen(file_name, "rb");
		if (!image_file) {
			printf("%s%s\n", LOAD_FAIL, file_name);
			strcpy(file_name, "");
			go_back();
			return;
		}
		fgets(image->type, SMALL_SIZE + 1, image_file);
	}
	read_header(image, &image_file);

	// Depending on the type of file and image, read the image
	switch (image->type[1]) {
	case '2':
		read_gray_img_text(image, &image_file);
		break;
	case '3':
		read_color_img_text(image, &image_file);
		break;
	case '5':
		read_gray_img_binary(image, &image_file);
		break;
	case '6':
		read_color_img_binary(image, &image_file);
		break;
	default:
		break;
	}

	// Select all and close the file
	select_all(image, sel);
	fclose(image_file);

	printf("%s%s\n", LOAD_SUCCES, file_name);

	go_back();
}

// Select
void select_cmd(image_t *image, selection_t *sel)
{
	system("clear");
	printf("----------------SELECT----------------\n\n");
	printf("Choose which part of the image you want to select ");
	printf("(\"ALL\" or <x1> <y1> <x2> <y2>): ");

	char content[BUFFER_SIZE] = "", command[BUFFER_SIZE] = "", *word;

	// Read the entire command
	fgets(content, BUFFER_SIZE, stdin);
	strcpy(command, content);
	remove_newline(command);
	remove_trailing_whitespace(command);

	if (!image->gray_img && !image->color_img) {
		printf(NO_IMG_LOADED);
		go_back();
		return;
	}
	if (strlen(command) == 0) {
		printf(INVALID_COMMAND);
		go_back();
		return;
	}

	if (strcmp(command, "ALL") == 0) {
		select_all(image, sel);
		printf("%s%s\n", SELECTED, command);
	} else {
		selection_t *temp_sel = malloc(sizeof(selection_t));

		// Get each coordinate from the command string
		word = strtok(command, " ");
		if (!word || !is_number(word)) {
			printf(INVALID_COMMAND);
			go_back();
			return;
		}
		temp_sel->y1 = atoi(word);
		word = strtok(NULL, " ");
		if (!word || !is_number(word)) {
			printf(INVALID_COMMAND);
			go_back();
			return;
		}
		temp_sel->x1 = atoi(word);
		word = strtok(NULL, " ");
		if (!word || !is_number(word)) {
			printf(INVALID_COMMAND);
			go_back();
			return;
		}
		temp_sel->y2 = atoi(word);
		word = strtok(NULL, " ");
		if (!word || !is_number(word)) {
			printf(INVALID_COMMAND);
			go_back();
			return;
		}
		temp_sel->x2 = atoi(word);
		word = strtok(NULL, " ");
		if (word) {
			printf(INVALID_COMMAND);
			go_back();
			return;
		}

		// Put the coordinates in the right place
		if (temp_sel->x1 > temp_sel->x2)
			swap_ints(&temp_sel->x1, &temp_sel->x2);
		if (temp_sel->y1 > temp_sel->y2)
			swap_ints(&temp_sel->y1, &temp_sel->y2);
		if (valid_coord(image, temp_sel) == 0) {
			printf(INVALID_COORD);
			go_back();
			return;
		}
		sel->x1 = temp_sel->x1;
		sel->y1 = temp_sel->y1;
		sel->x2 = temp_sel->x2;
		sel->y2 = temp_sel->y2;
		printf("%s%d %d %d %d\n", SELECTED, sel->y1, sel->x1, sel->y2, sel->x2);

		free(temp_sel);
	}

	go_back();
}

// Determine the histogram of a gray image
void histogram_cmd(image_t *image)
{
	system("clear");
	printf("----------------HISTOGRAM----------------\n\n");
	printf("Type the number of bins and number of stars: ");

	char *word, content[BUFFER_SIZE] = "", command[BUFFER_SIZE] = "";
	int *histogram = NULL, freq[MAX_VALUE] = {0};
	int nr_stars = 0, nr_bins = 0, pixels_interval = 0, max_freq = 0;

	// Read the entire command
	fgets(content, BUFFER_SIZE, stdin);
	strcpy(command, content);
	remove_newline(command);
	remove_trailing_whitespace(command);

	if (!image->gray_img && !image->color_img) {
		printf(NO_IMG_LOADED);
		go_back();
		return;
	}

	// Get the number of stars and the number of bins from the command string
	word = strtok(command, " ");
	if (!word || !is_number(word)) {
		printf(INVALID_COMMAND);
		go_back();
		return;
	}
	nr_stars = atoi(word);
	word = strtok(NULL, " ");
	if (!word || !is_number(word)) {
		printf(INVALID_COMMAND);
		go_back();
		return;
	}
	nr_bins = atoi(word);
	word = strtok(NULL, " ");
	if (word) {
		printf(INVALID_COMMAND);
		go_back();
		return;
	}
	if (image->type[1] == '3' || image->type[1] == '6') {
		printf(GRAY_IMG);
		go_back();
		return;
	}
	if (nr_stars < 0 || nr_bins < 0 ||
		nr_bins > MAX_VALUE || !is_power_2(nr_bins)) {
		printf(INVALID_COORD);
		go_back();
		return;
	}

	// Determine the frequency of each pixel and add it in the right interval
	histogram = alloc_array(nr_bins);
	det_freq(freq, image);
	pixels_interval = MAX_VALUE / nr_bins;
	for (int i = 0; i < nr_bins; ++i) {
		histogram[i] = 0;
		for (int j = 0; j < pixels_interval; ++j)
			histogram[i] += freq[i * pixels_interval + j];
	}

	// Determine the number of stars of each line by using the formula
	max_freq = max_array(histogram, nr_bins);
	for (int i = 0; i < nr_bins; ++i)
		histogram[i] = histogram[i] * nr_stars / max_freq;

	// Print the histogram
	for (int i = 0; i < nr_bins; ++i) {
		printf("%d\t|\t", histogram[i]);
		for (int j = 0; j < histogram[i]; ++j)
			printf("*");
		printf("\n");
	}

	// Remove the histogram array from memory
	free(histogram);

	go_back();
}

// Equalize a gray image
void equalize_cmd(image_t *image)
{
	system("clear");
	printf("----------------EQUALIZE----------------\n\n");
	int freq[MAX_VALUE] = {0};
	if (!image->gray_img && !image->color_img) {
		printf(NO_IMG_LOADED);
		go_back();
		return;
	}
	if (image->type[1] == '3' || image->type[1] == '6') {
		printf(GRAY_IMG);
		go_back();
		return;
	}

	// Determine the frequency of each pixel and apply the formula
	det_freq(freq, image);
	for (int i = 0; i < image->row_num; ++i) {
		for (int j = 0; j < image->col_num; ++j) {
			int hist_sum = 0;

			// Sum of the frequencies from all pixels from 0 to current
			for (int k = 0; k <= image->gray_img[i][j]; ++k)
				hist_sum += freq[k];

			// Formula
			image->gray_img[i][j] = round((double)hist_sum * 255 /
									(image->row_num * image->col_num));
			clamp(&image->gray_img[i][j]);
		}
	}
	printf(EQUALIZE_SUCCES);

	go_back();
}

// Rotate the selection of the image
void rotate_cmd(image_t *image, selection_t *sel)
{
	system("clear");
	printf("----------------ROTATE----------------\n\n");
	printf("Type the angle wanted for rotation: ");

	char content[BUFFER_SIZE] = "", command[BUFFER_SIZE] = "";
	int angle = 0;

	// Read the entire command
	fgets(content, BUFFER_SIZE, stdin);
	strcpy(command, content);
	remove_newline(command);
	remove_trailing_whitespace(command);

	if (!image->gray_img && !image->color_img) {
		printf(NO_IMG_LOADED);
		go_back();
		return;
	}

	// Get the angle from the string
	angle = atoi(command);
	if (angle % SQUARE_ANGLE != 0) {
		printf(ANGLE_ERR);
		go_back();
		return;
	}
	if (!is_sel_square(sel) && !is_whole_img(image, sel)) {
		printf(NO_SQUARE);
		go_back();
		return;
	}

	angle %= CIRCLE_ANGLE;
	if (angle < 0)
		angle += CIRCLE_ANGLE;

	// Rotate the desired selection of the image
	if (is_sel_square(sel)) {
		for (int i = 0; i < angle; i += SQUARE_ANGLE)
			rotate_selection(image, sel);
	} else {
		for (int i = 0; i < angle; i += SQUARE_ANGLE) {
			rotate_image(image);
			select_all(image, sel);
		}
	}
	printf("%s%s\n", ROTATED, command);

	go_back();
}

// Crop the selection of the image
void crop_cmd(image_t *image, selection_t *sel)
{
	system("clear");
	printf("---------------CROP---------------\n\n");
	if (!image->gray_img && !image->color_img) {
		printf(NO_IMG_LOADED);
		go_back();
		return;
	}
	crop(image, sel);
	select_all(image, sel);
	printf(IMG_CROPPED);

	go_back();
}

// Apply kernel over the selection of the image
void apply_cmd(image_t *image, selection_t *sel, const char *apply_list[])
{
	system("clear");
	printf("---------------APPLY---------------\n\n");
	printf("Choose what to apply to the image ");
	printf("(EDGE, SHARPEN, BLUR, GAUSSIAN_BLUR): ");

	char content[BUFFER_SIZE] = "", command[BUFFER_SIZE] = "";

	// The kernels and div for each type of apply
	const int edge_det[SMALL_SIZE][SMALL_SIZE] = {{-1, -1, -1},
												  {-1, 8, -1},
												  {-1, -1, -1}};
	const int sharpen[SMALL_SIZE][SMALL_SIZE] = {{0, -1, 0},
												 {-1, 5, -1},
												 {0, -1, 0}};
	const int box_blur[SMALL_SIZE][SMALL_SIZE] = {{1, 1, 1},
												  {1, 1, 1},
												  {1, 1, 1}};
	const int gauss_blur[SMALL_SIZE][SMALL_SIZE] = {{1, 2, 1},
													{2, 4, 2},
													{1, 2, 1}};
	const int edge_det_div = 1, sharpen_div = 1,
	box_blur_div = 9, gauss_blur_div = 16;

	// Read the entire command
	fgets(content, BUFFER_SIZE, stdin);
	strcpy(command, content);
	remove_newline(command);
	remove_trailing_whitespace(command);

	if (!image->gray_img && !image->color_img) {
		printf(NO_IMG_LOADED);
		go_back();
		return;
	}
	if (strlen(command) == 0) {
		printf(INVALID_COMMAND);
		go_back();
		return;
	}
	if (image->type[1] == '2' || image->type[1] == '5') {
		printf(APPLY_GRAY);
		go_back();
		return;
	}

	// Apply the requested kernel
	switch (which_command(command, apply_list, N_APPLY)) {
	case 0:
		apply(image, edge_det, edge_det_div, sel);
		printf(APPLIED_EDGE);
		break;
	case 1:
		apply(image, sharpen, sharpen_div, sel);
		printf(APPLIED_SHARPEN);
		break;
	case 2:
		apply(image, box_blur, box_blur_div, sel);
		printf(APPLIED_BLUR);
		break;
	case 3:
		apply(image, gauss_blur, gauss_blur_div, sel);
		printf(APPLIED_GAUSSIAN_BLUR);
		break;
	default:
		printf(APPLY_PARAM_INVALID);
		break;
	}

	go_back();
}

// Save the image in a file
void save_cmd(image_t *image)
{
	system("clear");
	printf("---------------SAVE---------------\n\n");
	printf("Type the file name (+ ascii for text file): ");

	FILE *save_file = NULL;
	char file_name[BUFFER_SIZE] = "", command[BUFFER_SIZE] = "", *word;

	// Read the entire command
	fgets(command, BUFFER_SIZE, stdin);
	if (!image->gray_img && !image->color_img) {
		printf(NO_IMG_LOADED);
		go_back();
		return;
	}
	remove_newline(command);
	word = strtok(command, " ");
	strcpy(file_name, word);
	word = strtok(NULL, " ");

	// Save as binary
	if (!word) {
		if (image->type[1] < '4') {
			image->type[1] += 3;
			image->header[1] += 3;
		}
		generate_header(image);

		// Open the save file as "write binary"
		save_file = fopen(file_name, "wb");
		fwrite(image->header, sizeof(char), strlen(image->header), save_file);
		if (image->type[1] == '5')
			print_gray_img_binary(image, &save_file);
		else
			print_color_img_binary(image, &save_file);
		fclose(save_file);
	} else {
		// Save as text
		if (strcmp(word, "ascii") == 0) {
			if (image->type[1] > '4') {
				image->type[1] -= 3;
				image->header[1] -= 3;
			}
			generate_header(image);
			remove_newline(file_name);
			remove_trailing_whitespace(file_name);

			// Open the save file as "write text"
			save_file = fopen(file_name, "wt");
			fprintf(save_file, "%s", image->header);
			if (image->type[1] == '2')
				print_gray_img_text(image, &save_file);
			else
				print_color_img_text(image, &save_file);
			fclose(save_file);
		} else {
			printf(INVALID_COMMAND);
			go_back();
			return;
		}
	}
	printf("%s%s\n", SAVED, file_name);

	go_back();
}

// Exit the program
void exit_cmd(image_t *image, selection_t *sel)
{
	if (!image->gray_img && !image->color_img) {
		printf(NO_IMG_LOADED);
		return;
	}

	// Free the memory
	if (image->gray_img)
		free_matrix(image->row_num, image->gray_img);
	if (image->color_img)
		free_color_matrix(image->row_num, image->color_img);

	free(image);
	free(sel);
}

void open_cmd(char *file_name)
{
	system("clear");
	printf("---------------OPEN---------------\n\n");

	char sys_command[BUFFER_SIZE] = "eog ";
	strcat(sys_command, file_name);
    system(sys_command);

	go_back();
}

void convert_cmd(image_t *image)
{
	system("clear");
	printf("---------------CONVERT---------------\n\n");

	if (!image->color_img) {
		printf(CONVERTED);

		go_back();
		return;
	}

	image->type[1]--;
	image->gray_img = alloc_matrix(image->row_num, image->col_num);
	for (int i = 0; i < image->row_num; ++i) {
		for (int j = 0; j < image->col_num; ++j) {
			int avg = round((float)(image->color_img[i][j].red +
					image->color_img[i][j].green +
					image->color_img[i][j].blue) / 3);
			image->gray_img[i][j] = avg;
		}
	}
	free_color_matrix(image->row_num, image->color_img);
	image->color_img = NULL;

	printf(CONVERTED);

	go_back();
}

void mirror_cmd(image_t *image)
{
	system("clear");
	printf("---------------MIRROR---------------\n\n");

	for (int i = 0; i < image->row_num; ++i) {
		for (int j = 0; j < image->col_num / 2; ++j) {
			swap_ints(&image->color_img[i][j].red, &image->color_img[i][image->col_num - j - 1].red);
			swap_ints(&image->color_img[i][j].green, &image->color_img[i][image->col_num - j - 1].green);
			swap_ints(&image->color_img[i][j].blue, &image->color_img[i][image->col_num - j - 1].blue);
		}
	}

	printf(MIRRORED);

	go_back();
}

void filter_cmd(image_t *image)
{
	system("clear");
	printf("---------------FILTER (SEPIA)---------------\n\n");

	if (!image->color_img) {
		printf(COLOR_IMG);

		go_back();
		return;
	}

	for (int i = 0; i < image->row_num; ++i) {
		for (int j = 0; j < image->col_num; ++j) {
			color_t sepia;

			sepia.red = 0.393 * image->color_img[i][j].red +
						0.769 * image->color_img[i][j].green +
						0.189 * image->color_img[i][j].blue;
			sepia.green = 0.349 * image->color_img[i][j].red +
						0.686 * image->color_img[i][j].green +
						0.168 * image->color_img[i][j].blue;
			sepia.blue = 0.272 * image->color_img[i][j].red +
						0.534 * image->color_img[i][j].green +
						0.131 * image->color_img[i][j].blue;

			clamp(&sepia.red);
			clamp(&sepia.green);
			clamp(&sepia.blue);

			image->color_img[i][j] = sepia;
		}
	}

	printf(SEPIA_FILTER);

	go_back();
}
