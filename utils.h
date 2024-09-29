#ifndef _UTILS_H_
#define _UTILS_H

#include <stdio.h>
#include <stdlib.h>

#include "QuadTree.h"

/* lungimile string-urilor corespunzatoare valorilor maxime
 * pe care le pot lua variabilele din structura "img_data" */
#define IMAGE_FORMAT_SIZE 2
#define IMAGE_WIDTH_SIZE 10
#define IMAGE_HEIGHT_SIZE 10
#define MAX_COLOR_VALUE_SIZE 7
#define MAX_IMAGE_LENGTH 4

/* valoarea maxima a unui canal de culoare */
#define MAX_COLOR_VAL 255

img_data* read_image_data(FILE* fin) {
	// se aloca memorie pentru o structura de tip "img_data"
	img_data* image_data = calloc(1, sizeof(img_data));
	// se aloca memorie pentru tipul fisierului (P6) + null terminator
	image_data->img_format = malloc((IMAGE_FORMAT_SIZE + 1) * sizeof(char));

	// citirea tipului fisierului (P6)
	// last_char este pentru setarea null terminator-ului la final de string
	int last_char = fread(image_data->img_format, sizeof(char), 2, fin);
	// adaugare null terminator la sfarsitul string-ului citit
	image_data->img_format[last_char] = '\0';

	// initializare read_newline pentru a citi caracterul newline
	char read_newline = '-';
	fread(&read_newline, sizeof(char), 1, fin);

	// citire width sub forma de sir de caractere
	char* width = malloc((IMAGE_WIDTH_SIZE) * sizeof(char));
	// "temp_char" se va utiliza pentru a determina caracterul curent
	// din string-ul care este citit
	char temp_char = '-';
	int char_pos = 0;
	while (temp_char != ' ') {
		// se modifica pozitia caracterului curent din string
		// si se copiaza caracterul curent in temp_char
		fread(&temp_char, sizeof(char), 1, fin);
		// temp_char este adaugat in string-ul width
		width[char_pos] = temp_char;
		// se incrementeaza pozitia curenta din string
		char_pos++;
	}
	// adaugare null terminator la sfarsitul string-ului citit
	width[char_pos - 1] = '\0';
	image_data->width = atoi(width);
	free(width);

	// citire height sub forma de sir de caractere
	char* height = malloc((IMAGE_HEIGHT_SIZE) * sizeof(char));
	temp_char = '-';
	char_pos = 0;
	while (temp_char != '\n') {
		fread(&temp_char, sizeof(char), 1, fin);
		// temp_char este adaugat in string-ul height
		height[char_pos] = temp_char;
		// se incrementeaza pozitia curenta din string
		char_pos++;
	}
	// adaugare null terminator la sfarsitul string-ului citit
	height[char_pos - 1] = '\0';
	image_data->height = atoi(height);
	free(height);

	// citirea valorii maxime a unei culori sub forma de sir de caractere
	char* max_val = malloc((MAX_COLOR_VALUE_SIZE) * sizeof(char));
	temp_char = '-';
	char_pos = 0;
	while (temp_char != '\n') {
		fread(&temp_char, sizeof(char), 1, fin);
		// temp_char este adaugat in string-ul max_val
		max_val[char_pos] = temp_char;
		// se incrementeaza pozitia curenta din string
		char_pos++;
	}
	// adaugare null terminator la sfarsitul string-ului citit
	max_val[char_pos - 1] = '\0';
	image_data->max_color_value = atoi(max_val);
	free(max_val);

	/* citirea tuturor pixelilor din imagine */

	// alocarea memoriei pentru o "coloana" de pointeri
	// catre "randuri" de pixeli
	image_data->pixels_matrix = calloc(image_data->height, sizeof(pixel*));
	for (int i = 0; i < image_data->height; i++) {
		// alocarea memoriei pentru toti pixelii din imagine
		image_data->pixels_matrix[i] = calloc(image_data->width, sizeof(pixel));
	}

	// citirea pixelilor
	for (int i = 0; i < image_data->height; i++) {
		for (int j = 0; j < image_data->width; j++) {
			/* copierea in matricea de pixeli din structura
			 * a valorilor canalelor corespunzatoare pixelilor imaginii */
			fread(&(image_data->pixels_matrix[i][j].red), sizeof(char), 1, fin);
			fread(&(image_data->pixels_matrix[i][j].green), sizeof(char), 1,
				  fin);
			fread(&(image_data->pixels_matrix[i][j].blue), sizeof(char), 1,
				  fin);
		}
	}

	return image_data;
}

img_data* read_compressed_image_data(FILE* fin) {
	// se aloca memorie pentru o structura de tip "img_data"
	img_data* image_data = calloc(1, sizeof(img_data));
	// citire size din fisierul de intrare
	unsigned int size = 0;
	fread(&size, sizeof(unsigned int), 1, fin);
	image_data->width = size;
	image_data->height = size;

	// deplasare la sfarsitul fisierului pentru a afla dimensiunea acestuia
	fseek(fin, 0, SEEK_END);
	long file_size = ftell(fin);

	// intoarcere la inceputul fisierului pentru a-i citi continutul
	fseek(fin, MAX_IMAGE_LENGTH, SEEK_SET);
	unsigned char* file_content = malloc(file_size * sizeof(unsigned char));
	// citirea continutului fisierului in vectorul "file_content"
	fread(file_content, file_size, sizeof(unsigned char), fin);
	// initializare variabile temporare
	unsigned char temp = '\0';
	unsigned int i = 1;
	unsigned int x = 0;
	unsigned int y = 0;
	unsigned int nodes_number = 0;
	for (int i = 0; i < file_size - MAX_IMAGE_LENGTH; i++) {
		if (file_content[i] == 1) {
			// daca nodul are culoare, se sare peste datele culorii
			i = i + 3;
		}
		nodes_number++;
	}
	// se aloca memorie pentru nodurile din fisier
	QuadTreeNode* node_data = calloc(nodes_number, sizeof(QuadTreeNode));
	// initializare indice pentru vectorul "node_data"
	unsigned int j = 0;
	for (int i = 0; i < file_size - MAX_IMAGE_LENGTH; i++) {
		// se initializeaza campurile nodului radacina
		node_data[j].descendants = NULL;
		node_data[j].value.red = 0;
		node_data[j].value.green = 0;
		node_data[j].value.blue = 0;
		node_data[j].size = 0;
		node_data[j].x = 0;
		node_data[j].y = 0;
		node_data[j].isLeaf = 0;

		if (file_content[i] == 0) {
			node_data[j].size = size / 2;
		}

		// daca nodul este de tip 1,
		// in vectorul "node_data" se pastreaza datele acestuia
		else if (file_content[i] == 1) {
			node_data[j].isLeaf = 1;
			node_data[j].value.red = file_content[i + 1];
			node_data[j].value.green = file_content[i + 2];
			node_data[j].value.blue = file_content[i + 3];
			// se sare peste valorile culorilor (se trece la urmatorul nod)
			i = i + 3;
		}
		j++;
	}
	free(file_content);

	Tree qtree = NULL;
	unsigned int idx = 0;

	/* se construieste un arbore utilizand un vector de noduri
	 * obtinuti din imaginea compresata */

	// se creeaza nodul radacina al arborelui qtree
	qtree = createTree(&node_data[0]);

	// se creeaza o coada pentru retinerea nodurilor arborelui pe nivel
	Queue* q = createQueue();

	// in coada se adauga radacina arborelui
	if (isQueueEmpty(q) == 1) {
		enqueue(q, qtree);
	}

	// se face insertia tuturor nodurilor arborelui (cate 4)
	for (int j = 1; j < nodes_number - 1; j += 4) {
		level_order_insert(&node_data[j], &q);
	}
	destroyQueue(q);

	// alocarea memoriei pentru o coloana de pointeri catre randuri de pixeli
	image_data->pixels_matrix = calloc(image_data->height, sizeof(pixel*));
	for (int i = 0; i < image_data->height; i++) {
		// alocarea memoriei pentru toti pixelii din imagine
		image_data->pixels_matrix[i] = calloc(image_data->width, sizeof(pixel));
	}

	// in fiecare nod din arbore se adauga detele "size" si coordonate "(x,y)"
	// si totodata se umple matricea de pixeli
	qtree = insert_node_details(qtree, image_data, size, 0, 0);

	// eliberare memorie alocata dinamic
	free(node_data);
	freeTree(&qtree);

	// initializare valoarea maxima a unei culori
	image_data->max_color_value = MAX_COLOR_VAL;

	free(qtree);
	return image_data;
}

void write_image_data(img_data* image_data, FILE* fout) {
	unsigned char temp = 'P';
	fwrite(&temp, 1, sizeof(unsigned char), fout);
	temp = '6';
	fwrite(&temp, 1, sizeof(unsigned char), fout);

	temp = '\n';
	fwrite(&temp, 1, sizeof(unsigned char), fout);

	// in str se stocheaza dimensiunile imaginii sub forma de string
	char* str = malloc(20 * sizeof(unsigned char));

	sprintf(str, "%u", image_data->height);

	for (int j = 0; j < strlen(str); j++) {
		fwrite(&str[j], 1, sizeof(unsigned char), fout);
	}

	temp = ' ';
	fwrite(&temp, 1, sizeof(unsigned char), fout);

	sprintf(str, "%u", image_data->width);

	for (int j = 0; j < strlen(str); j++) {
		fwrite(&str[j], 1, sizeof(unsigned char), fout);
	}

	temp = '\n';
	fwrite(&temp, 1, sizeof(unsigned char), fout);

	sprintf(str, "%u", image_data->max_color_value);

	for (int j = 0; j < strlen(str); j++) {
		fwrite(&str[j], 1, sizeof(unsigned char), fout);
	}

	temp = '\n';
	fwrite(&temp, 1, sizeof(unsigned char), fout);

	for (int j = 0; j < image_data->height; j++) {
		for (int k = 0; k < image_data->height; k++) {
			fwrite(&image_data->pixels_matrix[j][k].red, 1,
				   sizeof(unsigned char), fout);
			fwrite(&image_data->pixels_matrix[j][k].green, 1,
				   sizeof(unsigned char), fout);
			fwrite(&image_data->pixels_matrix[j][k].blue, 1,
				   sizeof(unsigned char), fout);
		}
	}
	free(str);
}

// functie pentru ridicarea la putere a oricarei baze cu orice exponent
unsigned int pw(unsigned int x, unsigned int exp) {
	if (exp != 0) {
		return x * pw(x, exp - 1);
	}
	return 1;
}

pixel mean_pixel(img_data* image_data, unsigned int size, unsigned int x,
				 unsigned int y) {
	// initializare sume ale culorilor
	unsigned long long red_sum = 0;
	unsigned long long green_sum = 0;
	unsigned long long blue_sum = 0;

	// se calculeaza sumele culorilor
	for (int i = x; i < x + size; i++) {
		for (int j = y; j < y + size; j++) {
			red_sum += image_data->pixels_matrix[i][j].red;
			green_sum += image_data->pixels_matrix[i][j].green;
			blue_sum += image_data->pixels_matrix[i][j].blue;
		}
	}

	// initializare medii ale culorilor
	double red_mean = 0;
	double green_mean = 0;
	double blue_mean = 0;

	// media pentru red
	red_mean = red_sum / (size * size);

	// media pentru green
	green_mean = green_sum / (size * size);

	// media pentru blue
	blue_mean = blue_sum / (size * size);

	pixel mean_color;
	mean_color.red = red_mean;
	mean_color.green = green_mean;
	mean_color.blue = blue_mean;

	// returneaza culoarea medie a zonei date
	return mean_color;
}

double colors_mean(img_data* image_data, unsigned int size, unsigned int x,
				   unsigned int y) {
	unsigned long long sum = 0;
	double mean = 0;

	// calcularea mediei celor 3 culori
	pixel mean_color = mean_pixel(image_data, size, x, y);
	double red_mean = mean_color.red;
	double green_mean = mean_color.green;
	double blue_mean = mean_color.blue;

	// se calculeaza suma pentru media celor 3 culori
	for (int i = x; i < x + size; i++) {
		for (int j = y; j < y + size; j++) {
			/* valorile culorilor pixelului curent se stocheaza
			 * in variabilele "r","g" si "b" */
			unsigned int r = image_data->pixels_matrix[i][j].red;
			unsigned int g = image_data->pixels_matrix[i][j].green;
			unsigned int b = image_data->pixels_matrix[i][j].blue;
			sum += pw(red_mean - r, 2) + pw(green_mean - g, 2) +
				   pw(blue_mean - b, 2);
		}
	}

	// se calculeaza media celor 3 culori
	mean = (1.0 * sum) / (3 * size * size);
	// se returneaza media
	return mean;
}

// functie care elibereaza memoria alocata dinamic pentru detaliile imaginii
void free_memory(img_data** image_data) {
	free((*image_data)->img_format);
	if ((*image_data)->pixels_matrix != NULL) {
		for (int i = 0; i < (*image_data)->height; i++) {
			free((*image_data)->pixels_matrix[i]);
		}
	}
	free((*image_data)->pixels_matrix);
	free(*image_data);
}

#endif	// _UTILS_H_
