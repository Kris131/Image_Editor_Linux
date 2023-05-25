// Copyright Mihai-Cosmin Nour & David-Cristian Bacalu 311CAb 2022-2023

#include "my_alloc.h"

int **alloc_matrix(int n, int m)
{
	int **a = (int **)malloc(n * sizeof(int *));
	DIE(!a, "Malloc failed.\n");

	for (int i = 0; i < n; ++i) {
		*(a + i) = (int *)malloc(m * sizeof(int));
		DIE(!*(a + i), "Malloc failed.\n");
	}
	return a;
}

color_t **alloc_color_matrix(int n, int m)
{
	color_t **a = (color_t **)malloc(n * sizeof(color_t *));
	DIE(!a, "Malloc failed.\n");


	for (int i = 0; i < n; ++i) {
		*(a + i) = (color_t *)malloc(m * sizeof(color_t));
		DIE(!*(a + i), "Malloc failed.\n");

	}
	return a;
}

int *alloc_array(int n)
{
	int *v = (int *)malloc(n * sizeof(int));
	DIE(!v, "Malloc failed.\n");
	return v;
}

void free_matrix(int n, int **a)
{
	for (int i = 0; i < n; ++i)
		free(*(a + i));
	free(a);
}

void free_color_matrix(int n, color_t **a)
{
	for (int i = 0; i < n; ++i)
		free(*(a + i));
	free(a);
}
