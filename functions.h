#include <stdio.h>
#include <stdlib.h>
#include<assert.h>
#include<math.h>

#define STEPSIZE 0.001

//STRUCTURES:

struct rect{
  int x1;
  int x2;
  int y1;
  int y2;
};
typedef struct rect rect;

struct triplet{
  int r;
  int g;
  int b;
};
typedef struct triplet triplet;
  
struct image{
  int entete;
  int nx;
  int ny;
  int valmax;
  triplet ** tabl;
};
typedef struct image image;

struct data_image{
  int n;
  float * pix;
  int class;
};
typedef struct data_image data_image;

struct bdd{
  int n;
  data_image * im;
};
typedef struct bdd bdd;

struct matrix{
  int mat_h;
  int mat_w;
  float ** mat;
};
typedef struct matrix matrix;

struct information{
  int n;
  int * vect;
};
typedef struct information information;

struct layer{
  int nb_nrn;
  matrix * weights;
  matrix * outputs;
  matrix * error;
  float negative_target;
};
typedef struct layer layer;

struct network{
  int nb_layer;
  layer * lyr;
  information info;
  int class;
};
typedef struct network network;

struct main_network{
  int nb_ntw;
  information info;
  float step_size;
  network ntw[10];
  bdd char_training[10];
  bdd * test;
  int batch_size;
  matrix * current_batch_data;
  matrix * current_batch_class;
};
typedef struct main_network main_network;




//FICHIER.C
FILE * ouvrir(char * nom);
FILE * ouvrirEcrire(char * nom);
void fermer(FILE * fd);
void allocImage(image * pic);
char * ajoutChar(char * info, char n);
char * resetStr(char * info);
image * lireExtraire(image * pic, char * nom);
triplet getPixel(image * pic, int x, int y);
void setPixel(image * pic, int x, int y, triplet p);
void afficher(image * pic);
void rotation90anti(image * pic);
void dupliquer(image * pic, char * nom);
int contrasteVoisin(triplet p1, triplet p2);
int contraste(image * pic, int x, int y);
void imageContrast(image * pic, int delta);
int moyenneRGB(image * pic, int x, int y);
void gris(image * pic);
int isBlack(image * pic, int x, int y);
int isWhite(image * pic, int x, int y);
int isBelow(image * pic, int x, int y, int epsi);
int isAbove(image * pic, int x, int y, int epsi);
rect delimit(image * pic);
image * loadChar(image * pic, image * c, rect rectangle);
image * initWhite(image * pic);
image * initBlack(image * pic);
image * resize(image * pic, image * new);


//BDD.C
int nb_row_file(char * char_file, int n);
int nb_pixel_file(char * char_file);
void load_data_image(data_image * im, int n, int * pix);
int nb_char_file(FILE * fd);
void extract_char_file(FILE * fd, int n, char * char_file);
void load_bdd(bdd * basedd, char * nom);
void print_bdd(bdd * basedd);

//MATRICES.C
void alloc_matrix(matrix *m);
void create_matrix(matrix *m, int number_rows, int number_columns, float init);
void printMatrix(matrix *m);
matrix * matrix_sum(matrix * m1, matrix * m2, matrix * m3);
matrix * matrix_elmt_prod(matrix * m1, matrix * m2, matrix * m3);
matrix * matrix_scalar_add(matrix * m1, float k, matrix * m3);
void matrix_scalar_prod(matrix * m1, float k);
void matrix_sigmoid(matrix * m);
void matrix_sigmoid_prime(matrix * m);
void matrix_tanh(matrix * m);
void matrix_relu(matrix * m);
matrix * matrix_dot_product(matrix * m1, matrix * m2, matrix * m3);
matrix * matrix_transposition(matrix * m1, matrix * m3);
matrix * matrix_add_column(matrix *m1, matrix *m3, int indice, float val);
matrix * matrix_add_row(matrix *m1, matrix *m3, int indice, float val);
matrix * matrix_remove_row(matrix *m1, matrix *m3, int indice);
float matrix_mean_column(matrix * m, int column_index);
void matrix_init_bias(matrix *m);
void create_matrix_random(matrix *m, int number_rows, int number_columns);

//NEURONE.C
void print_errors(main_network * main_ntw, int network_index);
float frand_a_b(float a, float b);
void alloc_network(network * ntw, information info, int batch_size);
void build_main_network(main_network * main_ntw, int nb_ntw, information info,int batch_size);
void print_network(network * ntw);
void layer_output(matrix * previous_layer_output, layer * lyr);
void print_layer_output(main_network * main_ntw, int indice_network, layer *lyr);
void forward(main_network * main_ntw, int indice_network);
void build_batch(main_network * main_ntw, int n, int choice_dataset);
void whatdiduexpect(matrix * current_batch_class, int expected, matrix * matrix_expected);
void total_error(main_network * main_ntw, layer * last_lyr, int nrn_class);
void layer_error(main_network * main_ntw, network * ntw, int layer_index);
void new_final_layer_weights(main_network * main_ntw, layer * last_lyr, layer * second_to_last, int nb_layer);
void backpropagation(main_network * main_ntw, int network_index);
void training(main_network * main_ntw, int network_index);
void build_batch_test(main_network * main_ntw);
void conversion(main_network * main_ntw, image * newtest);
void prediction(main_network * main_ntw);

