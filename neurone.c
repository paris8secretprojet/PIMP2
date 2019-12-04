#include <stdio.h>
#include <stdlib.h>
#include "matrices.c"

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
};
typedef struct network network;


//[nbfeat,3,3,1]

float frand_a_b(float a, float b){
    return ( rand()/(float)RAND_MAX ) * (b-a) + a;
}

int rand_Belhadj(int n){

  return (int) (n*(rand()/RAND_MAX+1.0));
}

network * alloc_network(network * ntw, information info){
  int i, j, k;
  matrix * weights;
  matrix obj1;
  weights = &obj1;

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
  ntw->info=info;
  return ntw;
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

  int i, j;
  
  matrix * preactivation_matrix;
  matrix obj1;
  preactivation_matrix = &obj1;

  preactivation_matrix = matrix_dot_product(previous_layer_output, lyr->weights, preactivation_matrix);
  matrix_sigmoid(preactivation_matrix);

  for(i=0; i<preactivation_matrix->mat_w; i++){
    lyr->outputs.mat[0][i] = preactivation_matrix->mat[0][i];
  }
  lyr->outputs.mat[0][preactivation_matrix->mat_w] = 1.0;

  free(preactivation_matrix);
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
 
    
      
int main(){

  information info;
  network objntw;
  network * ntw;
  ntw=&objntw;

  //[2,3,3,1] : Deux features, premiere couche(3 neurones), deuxieme couche(3 neurones), derniere couche(1 neurone)
  int t[]={2,3,3,1};
  info.vect=t;
  info.n=sizeof(t)/sizeof(*t);
  
  ntw = alloc_network(ntw,info);
  print_network(ntw);
  
}
