#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "structs.h"
#include "utils.h"
#include "QuadTree.h"
#include "Queue.h"

void task1(img_data* image_data, unsigned int factor, FILE* fout) {
	// initializare pointer qtree
	Tree qtree = NULL;

	qtree = insert_node(qtree, image_data, image_data->width, 0, 0, factor);
	// scrierea in fisier a inaltimii arborelui
	fprintf(fout, "%d\n", height(qtree) + 1);
	// scrierea in fisier a numarului de noduri pentru care mean <= factor
	fprintf(fout, "%d\n", leaves_number(qtree));
	// scrierea in fisier a celui mai mare patrat nedivizat
	fprintf(fout, "%d\n", largest_undivided_area(qtree));

	// eliberarea memoriei ocupate de arbore
	freeTree(&qtree);
	free(qtree);
}

void task2(img_data* image_data, unsigned int factor, FILE* fout) {
	unsigned int dimensiune_imagine = image_data->width;

	// scrierea in fisier a dimensiunii de tip unsigned int
	fwrite(&dimensiune_imagine, 1, sizeof(unsigned int), fout);

	char* output = calloc(20, sizeof(char));
	free(output);
	Tree qtree = NULL;
	qtree = insert_node(qtree, image_data, image_data->width, 0, 0, factor);

	// scrierea in fisier a nodurilor pe nivele
	write_all_levels(qtree, fout);

	// eliberarea memoriei ocupate de arbore
	freeTree(&qtree);
	free(qtree);
}

void task3(FILE* fin, FILE* fout) {
	img_data* image_data = read_compressed_image_data(fin);
	write_image_data(image_data, fout);
	free_memory(&image_data);
}

int main(int argc, char const* argv[]) {
	/* daca primul argument este "-c1", se executa cerinta 1 */
	if (strcmp(argv[1], "-c1") == 0) {
		/* se deschide fisierul de intrare pentru cerinta 1*/
		FILE* fin = fopen(argv[3], "rb");
		if (fin == NULL) {
			printf("Fisierul de intrare nu poate fi deschis\n");
			return -1;
		}
		/* se deschide fisierul de iesire */
		FILE* fout = fopen(argv[4], "w");
		if (fout == NULL) {
			printf("Fisierul de iesire nu poate fi deschis\n");
			return -1;
		}
		// in factor se stocheaza valoarea factorului ca numar natural
		unsigned int factor = atoi(argv[2]);
		// se citesc datele din fisierul binar de intrare
		// si acestea se salveaza in structura de tip "img_data"
		img_data* image_data = read_image_data(fin);
		// se efectueaza cerinta 1
		task1(image_data, factor, fout);
		// eliberarea memoriei alocate dinamic
		free_memory(&image_data);
		fclose(fin);
		fclose(fout);
		/* daca primul argument este "-c2", se executa cerinta 2 */
	} else if (strcmp(argv[1], "-c2") == 0) {
		/* se deschide fisierul de intrare pentru cerinta 2*/
		FILE* fin = fopen(argv[3], "rb");
		if (fin == NULL) {
			printf("Fisierul de intrare nu poate fi deschis\n");
			return -1;
		}
		/* se deschide fisierul de iesire (binar) */
		FILE* fout = fopen(argv[4], "wb");
		if (fout == NULL) {
			printf("Fisierul de iesire nu poate fi deschis\n");
			return -1;
		}
		// in factor se stocheaza valoarea factorului
		unsigned int factor = atoi(argv[2]);
		// se citesc datele din fisierul binar de intrare
		img_data* image_data = read_image_data(fin);
		// se efectueaza cerinta 2
		task2(image_data, factor, fout);
		// eliberarea memoriei alocate dinamic
		free_memory(&image_data);
		fclose(fin);
		fclose(fout);
		/* daca primul argument este "-d", se executa cerinta 3 */
	} else if (strcmp(argv[1], "-d") == 0) {
		/* se deschide fisierul de intrare pentru cerinta 3*/
		FILE* fin = fopen(argv[2], "rb");
		if (fin == NULL) {
			printf("Fisierul de intrare nu poate fi deschis\n");
			return -1;
		}
		/* se deschide fisierul de iesire (binar) */
		FILE* fout = fopen(argv[3], "wb");
		if (fout == NULL) {
			printf("Fisierul de iesire nu poate fi deschis\n");
			return -1;
		}
		// se efectueaza cerinta 3
		task3(fin, fout);
		fclose(fin);
		fclose(fout);
	}
	return 0;
}
