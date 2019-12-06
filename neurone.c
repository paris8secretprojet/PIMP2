#include <stdio.h>
#include <stdlib.h>
#include "matrices.c"
#include "bdd.c"

struct information{
  int n;
  int * vect;
};
typedef struct information information;


struct layer{
  int nb_nrn;
  matrix * weights;
  matrix * outputs;
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
  network ntw[26];
  bdd * training;
  bdd * test;
  data_image * current;
};
typedef struct main_network main_network;



//[nbfeat,3,3,1]

float frand_a_b(float a, float b){
    return ( rand()/(float)RAND_MAX ) * (b-a) + a;
}

int rand_Belhadj(int n){

  return (int) (n*(rand()/RAND_MAX+1.0));
}

void alloc_network(network * ntw, information info){
  int i, j, k;

  ntw->info=info;
  ntw->nb_layer=(info.n)-1;
  ntw->lyr = malloc(ntw->nb_layer * sizeof(layer));

  
  for(i=0;i<info.n-1;i++){

    ntw->lyr[i].weights = malloc(sizeof(matrix));
    ntw->lyr[i].weights->mat_h = info.vect[i]+1;
    ntw->lyr[i].weights->mat_w = info.vect[i+1];
    alloc_matrix(ntw->lyr[i].weights);

    ntw->lyr[i].outputs = malloc(sizeof(matrix));
    ntw->lyr[i].outputs->mat_h = 1;
    ntw->lyr[i].outputs->mat_w = info.vect[i+1]+1;
    alloc_matrix(ntw->lyr[i].outputs);

    ntw->lyr[i].nb_nrn = info.vect[i+1];

    ntw->lyr[i].outputs->mat[0][ntw->lyr[i].outputs->mat_w-1] = 1.0;
    
    for (j=0; j<ntw->lyr[i].weights->mat_h; j++){
      for (k=0; k<ntw->lyr[i].weights->mat_w; k++){
	ntw->lyr[i].weights->mat[j][k] = frand_a_b(-1.0,1);
      }
    }
  }
}

void build_main_network(main_network * main_ntw, int nb_ntw, information info){
  int i;

  main_ntw->nb_ntw=nb_ntw;

  for(i=0;i<nb_ntw;i++){
    alloc_network(&(main_ntw->ntw[i]),info);
  }
}


 

void print_network(network * ntw){
  int i, j, k;

  printf("\nNombre de features : %d\n",ntw->info.vect[0]);
  printf("Nombre de layer : %d\n\n",(ntw->info.n)-1);

  for(i=0;i<ntw->info.n-1;i++){
    for(j=0;j<ntw->info.vect[i+1];j++){
      for(k=0;k<ntw->info.vect[i]+1;k++){

	//	if(i==0){
	  printf("LAYER%d, NEUR%d, WEIGHT%d  --->\t",i,j,k);
	  printf("%f\n",ntw->lyr[i].weights->mat[k][j]);
      }
    }
    printf("\n");
  }
}


void layer_output(matrix * previous_layer_output, layer * lyr){

  int i;
  
  matrix * preactivation_matrix;
  matrix obj1;
  preactivation_matrix = &obj1;

  preactivation_matrix = matrix_dot_product(previous_layer_output, lyr->weights, preactivation_matrix);
  matrix_sigmoid(preactivation_matrix);

  for(i=0; i<preactivation_matrix->mat_w; i++){
    lyr->outputs->mat[0][i] = preactivation_matrix->mat[0][i];
  }
  lyr->outputs->mat[0][preactivation_matrix->mat_w] = 1.0;

  //free(preactivation_matrix);
}



/*
float preactivation(input in, neurone * nrn){
  int i;
  float output;
  output=0;
  for(i=0;i<in.n;i++){
    output += in.vect[i] * nrn->weights[i];
  }
  output += nrn->weights[nrn->n];
  
  return output;
}
float activation(float preact){
  return 1/(1+exp(-preact));
}
*/

/*
matrix * weight_matrix(layer * lyr, matrix * m){
  int i, j;
  m->mat_h = lyr->nrn.n;
  m->mat_w = lyr->nb_neur;
  for(i=0;i<m->mat_h;i++){
    for(j=0;j<m->mat_w;j++){
      m->mat[j][i]=lyr->nrn[i]->weights[j];
    }
  }
  return m;
}
matrix * bias_vector(layer * lyr, matrix * m){
  int i;
  m->mat_h=1;
  m->mat_w=lyr->nb_neur;
  for(i=0;i<m->mat_w;i++){
    m->mat[0][i]=lyr->nrn[i].weights[lyr->nrn[i].n];
  }
  return m;
}
*/

void create_matrix_random(matrix *m, int number_rows, int number_columns){

  int row;
  int col;

  m->mat_h = number_rows;
  m->mat_w = number_columns;
  
  alloc_matrix(m);
  
  for(row=0; row<number_rows; row++){
    for(col=0; col<number_columns; col++){
      m->mat[row][col] = frand_a_b(-1.0,1);
    }
  }
}

void matrix_init_bias(matrix *m){
  m->mat[0][(m->mat_w)-1]=1.0;
}

void print_layer_output(layer *lyr){
  int i;
  
  for(i=0;i<lyr->outputs->mat_w;i++){
    printf("Output %d --->   %f\n",i,lyr->outputs->mat[0][i]);
  }
}

float calculate_error(float predict, float target){
  return (target-predict)*(target-predict)/2;
}

void foreward(network * ntw, matrix * data){
  int i;

  matrix ** pt_data;
  pt_data = malloc(sizeof(matrix*));
  pt_data=&data;

  for(i=0;i<ntw->nb_layer;i++){
    layer_output(*pt_data,&(ntw->lyr[i]));
    pt_data=&(ntw->lyr[i].outputs);
    print_layer_output(&(ntw->lyr[i]));
  }
  printf("Predict :\n");
  print_layer_output(&(ntw->lyr[(ntw->nb_layer)-1]));
}

void global_foreward(main_network * main_ntw, matrix * data){
  int i;

  for(i=0;i<main_ntw->nb_ntw;i++){
    foreward(&(main_ntw->ntw[i]),data);
  }
}
 
    
      
int main(){

  information info;
  main_network * main_ntw;
  //network * ntw;
  bdd * basedd_train;
  bdd * basedd_test;
  basedd_train = malloc(sizeof(bdd));
  basedd_test = malloc(sizeof(bdd));
  main_ntw = malloc(sizeof(main_network));
  //ntw = malloc(sizeof(network));

  //[2,3,3,1] : Deux features, premiere couche(3 neurones), deuxieme couche(3 neurones), derniere couche(1 neurone)
  int t[]={784,30,10,5,1};
  info.vect=t;
  info.n=sizeof(t)/sizeof(*t);
  
  //alloc_network(ntw,info);
  //print_network(ntw);

  //matrix * test;
  //test = malloc(sizeof(matrix));
  //create_matrix_random(test,1,785);
  //matrix_init_bias(test);
  //printMatrix(test);

  //layer_output(test,&(ntw->lyr[0]));
  //print_layer_output(&(ntw->lyr[0]));

  //foreward(ntw,test);

  build_main_network(main_ntw,26,info);
  main_ntw->training = malloc(sizeof(bdd));
  main_ntw->test = malloc(sizeof(bdd));
  load_bdd(main_ntw->training,"emnist-balanced-train.txt");
  load_bdd(main_ntw->test,"emnist-balanced-test.txt");
}
