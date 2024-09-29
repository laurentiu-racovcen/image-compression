#ifndef _QUAD_TREE_
#define _QUAD_TREE_

#include "Queue.h"

double colors_mean();
pixel mean_pixel();

/* Functie care creeaza un arbore cu doar un nod */
QuadTreeNode* createTree(QuadTreeNode* node_data) {
	QuadTreeNode* root = (QuadTreeNode*)calloc(1, sizeof(QuadTreeNode));
	root->isLeaf = node_data->isLeaf;
	root->x = 0;
	root->y = 0;
	root->size = 0;
	root->descendants = NULL;
	/* daca nodul este de tip 0, se aloca memorie
	 * pentru cele 4 noduri descendente */
	if (node_data->isLeaf == 0) {
		root->descendants = calloc(4, sizeof(QuadTreeNode*));
		root->descendants[0] = NULL;
		root->descendants[1] = NULL;
		root->descendants[2] = NULL;
		root->descendants[3] = NULL;
	}
	// daca nodul este de tip 1 (este frunza)
	if (node_data->isLeaf == 1) {
		root->value.red = node_data->value.red;
		root->value.green = node_data->value.green;
		root->value.blue = node_data->value.blue;
	}
	return root;
}

/* functie care insereaza un nou nod in arbore
 * si primeste valori in dependenta de media culorilor obtinuta */
Tree insert_node(Tree root, img_data* image_data, unsigned int size,
				 unsigned int x, unsigned int y, unsigned int factor) {
	// se initializeaza un nou nod
	root = (Tree)calloc(1, sizeof(QuadTreeNode));

	// se introduc detaliile nodului
	root->x = x;
	root->y = y;
	root->size = size;

	if ((unsigned int)colors_mean(image_data, size, x, y) <= factor) {
		// se calculeaza culoarea medie si se salveaza in valoarea nodului
		root->value = mean_pixel(image_data, size, x, y);
		root->descendants = NULL;
	} else {
		// vor fi creati cei 4 descendenti ai nodului curent
		// se aloca memorie pentru pointerii catre cele 4 noduri descendente
		root->descendants = (Tree*)malloc(4 * sizeof(Tree));

		root->descendants[0] = insert_node(root->descendants[0], image_data, size / 2, x, y, factor);
		root->descendants[1] = insert_node(root->descendants[1], image_data, size / 2, x, y + (size / 2), factor);
		root->descendants[2] = insert_node(root->descendants[2], image_data, size / 2, x + (size / 2), y + (size / 2), factor);
		root->descendants[3] = insert_node(root->descendants[3], image_data, size / 2, x + (size / 2), y, factor);
	}
	return root;
}

/* functie care insereaza un nou nod in arbore
 * in ordinea parcurgerii pe nivel */
void level_order_insert(QuadTreeNode* node_data, Queue** q) {
	while (!isQueueEmpty((*q))) {
		QuadTreeNode* aux = front((*q));
		dequeue((*q));

		if (aux->isLeaf == 0) {
			if (aux->descendants[0] == NULL) {
				aux->descendants[0] = createTree(node_data);
				node_data++;
				aux->descendants[1] = createTree(node_data);
				node_data++;
				aux->descendants[2] = createTree(node_data);
				node_data++;
				aux->descendants[3] = createTree(node_data);
				// se retine in coada ultimul nod analizat
				enqueue((*q), aux);
				return;
			} else {
				for (int i = 0; i < 4; i++) {
					enqueue((*q), aux->descendants[i]);
				}
			}
		}
	}
}

Tree insert_node_details(Tree root, img_data* image_data, unsigned int size,
						 unsigned int x, unsigned int y) {
	// se introduc detaliile nodului dat
	root->x = x;
	root->y = y;
	root->size = size;
	if (root->isLeaf == 1) {
		for (int j = x; j < x + root->size; j++) {
			for (int k = y; k < y + root->size; k++) {
				image_data->pixels_matrix[j][k].red = root->value.red;
				image_data->pixels_matrix[j][k].green = root->value.green;
				image_data->pixels_matrix[j][k].blue = root->value.blue;
			}
		}
	}

	if (root->isLeaf == 0) {
		// se initializeaza datele celor 4 noduri descendente
		root->descendants[0] = insert_node_details(root->descendants[0], image_data, size / 2, x, y);
		root->descendants[1] = insert_node_details(root->descendants[1], image_data, size / 2, x, y + (size / 2));
		root->descendants[2] = insert_node_details(root->descendants[2], image_data, size / 2, x + (size / 2), y + (size / 2));
		root->descendants[3] = insert_node_details(root->descendants[3], image_data, size / 2, x + (size / 2), y);
	}
	return root;
}

int max_of(int x, int y) {
	if (x > y) return x;
	return y;
}

int height(Tree root) {
	if (root == NULL) {
		return 0;
	}
	if (root->descendants != NULL) {
		int h0 = height(root->descendants[0]);
		int h1 = height(root->descendants[1]);
		int h2 = height(root->descendants[2]);
		int h3 = height(root->descendants[3]);
		// se returneaza maximul dintre inaltimile celor 4 descendenti
		return max_of(max_of(h0, h1), max_of(h1, h2)) + 1;
	}
}

void write_current_level(Tree root, int current_level, FILE* fout) {
	if (root == NULL) {
		return;
	}
	if (current_level == 1) {
		unsigned char node_type = 0;
		// daca nodul este frunza, se scriu detaliile nodului curent in fisier
		if (root->descendants == NULL) {
			node_type = 1;
			fwrite(&node_type, 1, sizeof(unsigned char), fout);
			fwrite(&(root->value), 1, sizeof(pixel), fout);
			// daca nodul NU este frunza, se scrie doar caracterul '0' in fisier
		} else {
			node_type = 0;
			fwrite(&node_type, 1, sizeof(unsigned char), fout);
		}
	} else if (current_level > 1) {
		if (root->descendants != NULL) {
			write_current_level(root->descendants[0], current_level - 1, fout);
			write_current_level(root->descendants[1], current_level - 1, fout);
			write_current_level(root->descendants[2], current_level - 1, fout);
			write_current_level(root->descendants[3], current_level - 1, fout);
		}
	}
}

void write_all_levels(Tree root, FILE* fout) {
	int h = height(root);
	for (int i = 0; i <= h; i++) {
		write_current_level(root, i + 1, fout);
	}
}

unsigned int leaves_number(Tree root) {
	if (root == NULL) {
		return 0;
	}
	if (root->descendants == NULL) {
		return 1;
	}
	if (root->descendants != NULL) {
		int k = 0;
		for (int i = 0; i < 4; i++) {
			// in "k" se incrementeaza numarul de frunze ale descendentilor
			k += leaves_number(root->descendants[i]);
		}
		// returneaza numarul de frunze ale arborelui
		return k;
	}
}

// functie care salveaza pointerii nodurilor frunza in vectorul "leaves"
void get_leaves(Tree root, Tree* leaves, int* k) {
	if (root == NULL) {
		return;
	}
	if (root->descendants == NULL) {
		leaves[(*k)] = root;
		(*k)++;
	}
	if (root->descendants != NULL) {
		for (int i = 0; i < 4; i++) {
			get_leaves(root->descendants[i], leaves, k);
		}
	}
}

int largest_undivided_area(Tree root) {
	int k = leaves_number(root);
	Tree* leaves = malloc(k * sizeof(Tree));
	int i = 0;
	// toate frunzele arborelui se salveaza in vectorul "leaves"
	get_leaves(root, leaves, &i);
	int max_size_leaf = 0;
	// frunza cu cel mai mare "size" este stocata in max_leaf
	for (i = 0; i < k; i++) {
		if (leaves[i]->size > max_size_leaf) {
			max_size_leaf = leaves[i]->size;
		}
	}
	free(leaves);
	return max_size_leaf;
}

void freeTree(Tree* root) {
	if ((*root) == NULL) {
		return;
	}

	// se elibereaza memoria alocata pentru descendenti
	// (daca nodul are descendenti)
	if ((*root)->descendants != NULL) {
		for (int i = 0; i < 4; i++) {
			freeTree(&((*root)->descendants[i]));
		}
		free((*root)->descendants);
	}

	// se elibereaza memoria alocata pentru fiecare nod
	free(*root);

	// arborele devine vid
	*root = NULL;
}

#endif	// _QUAD_TREE_
