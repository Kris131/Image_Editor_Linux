// Copyright Mihai-Cosmin Nour & David-Cristian Bacalu 311CA 2022-2023

#include <stdio.h>
#include <string.h>
#include "commands.h"
#include "menu.h"

int main(void)
{

	char command[BUFFER_SIZE] = "", file_name[BUFFER_SIZE] = "";
	const char *command_list[N_COMMANDS] = {"LOAD", "SELECT", "HISTOGRAM",
											"EQUALIZE", "ROTATE", "CROP",
											"APPLY", "SAVE", "EXIT", "HELP",
											"OPEN", "CONVERT", "MIRROR",
											"FILTER"};
	const char *apply_list[N_APPLY] = {"EDGE", "SHARPEN", "BLUR",
									   "GAUSSIAN_BLUR"};
	image_t *image = malloc(sizeof(image_t));
	DIE(!image, "Malloc failed.\n");
	selection_t *sel = malloc(sizeof(selection_t));
	DIE(!sel, "Malloc failed.\n");
	image->row_num = 0;
	image->col_num = 0;
	image->max_value = 0;
	image->gray_img = NULL;
	image->color_img = NULL;
	strcpy(image->header, "");
	while (1) {
		start_info(file_name);

		fgets(command, BUFFER_SIZE, stdin);
		remove_newline(command);
		switch (which_command(command, command_list, N_COMMANDS)) {
		case 0:
			load_cmd(image, sel, file_name);
			break;
		case 1:
			select_cmd(image, sel);
			break;
		case 2:
			histogram_cmd(image);
			break;
		case 3:
			equalize_cmd(image);
			break;
		case 4:
			rotate_cmd(image, sel);
			break;
		case 5:
			crop_cmd(image, sel);
			break;
		case 6:
			apply_cmd(image, sel, apply_list);
			break;
		case 7:
			save_cmd(image);
			break;
		case 8:
			exit_cmd(image, sel);
			return 0;
		case 9:
			next_command();
			break;
		case 10:
			open_cmd(file_name);
			break;
		case 11:
			convert_cmd(image);
			break;
		case 12:
			mirror_cmd(image);
			break;
		case 13:
			filter_cmd(image);
			break;
		default:
			printf(INVALID_COMMAND);
			break;
		}
	}
	return 0;
}
