//sigmoid(17) != 1 ; Par contre sigmoid(18)==1;

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <assert.h>
#include <math.h>


struct matrix{
  
  int mat_h;
  int mat_w;
  float ** mat;
};

typedef struct matrix matrix;

void alloc_matrix(matrix *m){

  int i;

  m->mat = malloc( (m->mat_h) * sizeof(float *) );
  for (i=0;i<m->mat_h;i++){
    (m->mat)[i] = malloc ((m->mat_w) * sizeof(float));
  }
}

void create_matrix(matrix *m, int number_rows, int number_columns, float init){

  int row;
  int col;

  m->mat_h = number_rows;
  m->mat_w = number_columns;
  
  alloc_matrix(m);
  
  for(row=0; row<number_rows; row++){
    for(col=0; col<number_columns; col++){
      m->mat[row][col] = init;
    }
  }
}



void printMatrix(matrix *m){

  int row;
  int col;

  printf("mat_h : %d\n"
	 "mat_w : %d\n\n",
	 m->mat_h, m->mat_w);

  for(row=0; row<m->mat_h; row++){
    for(col=0; col<m->mat_w; col++){
      printf("%f ",(m->mat)[row][col]);
    }
    printf("\n");
  }
  printf("\n");
}

matrix * matrix_sum(matrix * m1, matrix * m2, matrix * m3){

  int i, j;

  if((m1->mat_h != m2->mat_h)||(m1->mat_w != m2->mat_w)){
    printf("Somme des matrices impossible : Les deux matrices n ont pas la meme shape\n\n");
  }
  assert((m1->mat_h == m2->mat_h)&&(m1->mat_w == m2->mat_w));

  m3->mat_h=m1->mat_h;
  m3->mat_w=m1->mat_w;

  alloc_matrix(m3);

  for(i=0;i<m3->mat_h;i++){
    for(j=0;j<m3->mat_w;j++){
      m3->mat[i][j]=m1->mat[i][j]+m2->mat[i][j];
    }
  }
  return m3;
}

matrix * matrix_elmt_prod(matrix * m1, matrix * m2, matrix * m3){

  int i, j;

  if((m1->mat_h != m2->mat_h)||(m1->mat_w != m2->mat_w)){
    printf("Operation impossible : Les deux matrices n ont pas la meme shape\n\n");
  }
  assert((m1->mat_h == m2->mat_h)&&(m1->mat_w == m2->mat_w));

  m3->mat_h=m1->mat_h;
  m3->mat_w=m1->mat_w;

  alloc_matrix(m3);

  for(i=0;i<m3->mat_h;i++){
    for(j=0;j<m3->mat_w;j++){
      m3->mat[i][j]=m1->mat[i][j] * m2->mat[i][j];
    }
  }
  return m3;
}

matrix * matrix_scalar_add(matrix * m1, float k, matrix * m3){

  int i, j;

  m3->mat_h=m1->mat_h;
  m3->mat_w=m1->mat_w;

  alloc_matrix(m3);

  for(i=0;i<m3->mat_h;i++){
    for(j=0;j<m3->mat_w;j++){
      m3->mat[i][j]=m1->mat[i][j] + k;
    }
  }
  return m3;
}

void matrix_scalar_prod(matrix * m1, float k){

  int i, j;

  for(i=0;i<m1->mat_h;i++){
    for(j=0;j<m1->mat_w;j++){
      m1->mat[i][j]=m1->mat[i][j] * k;
    }
  }
}

void matrix_sigmoid(matrix * m){

  int i, j;

  for(i=0;i<m->mat_h;i++){
    for(j=0;j<m->mat_w;j++){
      m->mat[i][j]=1/(1+exp(-(m->mat[i][j])));
    }
  }
}

void matrix_sigmoid_prime(matrix * m){

  int i, j;

  for(i=0;i<m->mat_h;i++){
    for(j=0;j<m->mat_w;j++){
      m->mat[i][j]=(1/(1+exp(-(m->mat[i][j]))))*(1-(1/(1+exp(-(m->mat[i][j])))));
    }
  }
}

void matrix_tanh(matrix * m){
  int i, j;
  
  for(i=0;i<m->mat_h;i++){
    for(j=0;j<m->mat_w;j++){
      m->mat[i][j]=tanh(m->mat[i][j]);
    }
  }
}

void matrix_relu(matrix * m){
  int i, j;

  for(i=0;i<m->mat_h;i++){
    for(j=0;j<m->mat_w;j++){
      if(m->mat[i][j]<0){
	m->mat[i][j]=0;
      }
      else{
	m->mat[i][j]=m->mat[i][j];
      }	
    }
  }
}


matrix * matrix_dot_product(matrix * m1, matrix * m2, matrix * m3){

  int i, j, k;
  float res;

  /*
  printf("m->mat_h : %d\n"
	 "m->mat_w : %d\n\n",
	 m->mat_h, m->mat_w);
  */

  if(m1->mat_w != m2->mat_h){
    printf("Produit des matrices impossible : Le nombre de ligne de la premiere matrice est different du nombre de colonne de la seconde\n\n");
    printf("m1->mat_h : %d\n"
	 "m1->mat_w : %d\n\n"
	   "m2->mat_h : %d\n"
	   "m2->mat_w : %d\n\n",
	   m1->mat_h, m1->mat_w, m2->mat_h, m2->mat_w);
  }
  assert(m1->mat_w == m2->mat_h);

  m3->mat_h=m1->mat_h;
  m3->mat_w=m2->mat_w;

  alloc_matrix(m3);
  
  for(i=0;i<m1->mat_h;i++){
    for(j=0;j<m2->mat_w;j++){
      res=0.0;
      for(k=0;k<m1->mat_w;k++){
	res+= m1->mat[i][k] * m2->mat[k][j];
      }
      m3->mat[i][j]=res;
    }
  }
  printf("\n");
  
  return m3;
}

matrix * matrix_transposition(matrix * m1, matrix * m3){
  int i, j;

  m3->mat_w=m1->mat_h;
  m3->mat_h=m1->mat_w;

  alloc_matrix(m3);

  for(i=0;i<m3->mat_h;i++){
    for(j=0;j<m3->mat_w;j++){
      m3->mat[i][j]=m1->mat[j][i];
    }
  }
  return m3;
}

matrix * matrix_add_column(matrix *m1, matrix *m3, int indice, float val){
  //"indice" donne l indice auquel on veut ajouter une colonne et "val" les valeurs de cette colonne.
  int i, j;

  m3->mat_w=(m1->mat_w)+1;
  m3->mat_h=m1->mat_h;

  alloc_matrix(m3);

  for(i=0;i<m3->mat_h;i++){
    m3->mat[i][indice]=val;
  }

  for(i=0;i<m3->mat_h;i++){
    for(j=0;j<indice;j++){
      m3->mat[i][j]=m1->mat[i][j];
    }
  }

  for(i=0;i<m3->mat_h;i++){
    for(j=indice;j<m3->mat_w;j++){
      m3->mat[i][j+1]=m1->mat[i][j];
    }
  }
  return m3;
}

matrix * matrix_add_row(matrix *m1, matrix *m3, int indice, float val){
  //"indice" donne l indice auquel on veut ajouter une ligne et "val" les valeurs de cette ligne.
  int i, j;

  if((indice < 0)||(indice > (m1->mat_h))){
    printf("\"matrix_add_row\" ERREUR : indice trop faible ou trop grand\n\n");
  }
  assert((indice>-1)&&(indice<(m1->mat_h)+1));
  

  m3->mat_h=(m1->mat_h)+1;
  m3->mat_w=m1->mat_w;

  alloc_matrix(m3);
  
  for(i=0;i<m3->mat_w;i++){
    m3->mat[indice][i]=val;
  }
  for(i=0;i<indice;i++){
    for(j=0;j<m3->mat_w;j++){
      m3->mat[i][j]=m1->mat[i][j];
    }
  }
  for(i=indice;i<m1->mat_h;i++){
    for(j=0;j<m3->mat_w;j++){
      m3->mat[i+1][j]=m1->mat[i][j];
    }
  }
  return m3;
}

matrix * matrix_remove_row(matrix *m1, matrix *m3, int indice){
  int i, j;

  if((indice < 0)||(indice > (m1->mat_h)-1)){
    printf("\"matrix_remove_row\" ERREUR : indice trop faible ou trop grand\n\n");
  }
  assert((indice>-1)&&(indice<(m1->mat_h)));

  printf("A\n");
  m3->mat_h = (m1->mat_h)-1;
  m3->mat_w = m1->mat_w;

  alloc_matrix(m3);

  printf("B\n");
  for(i=0;i<indice;i++){
    for(j=0;j<m3->mat_w;j++){
      m3->mat[i][j]=m1->mat[i][j];
    }
  }

  printf("C\n");
  for(i=indice;i<m3->mat_h;i++){
    for(j=0;j<m3->mat_w;j++){
      m3->mat[indice][j]=m1->mat[indice+1][j];
      //printf("M1 %f\n",m1->mat[indice+1][j]);
      //printf("M3 %f\n",m3->mat[indice][j]);
    }
  }
  printf("D\n");
  printMatrix(m3);
  return m3;
}


/*
int main(void){
  
  matrix *m1, *m2, *m3;
  m1 = malloc(sizeof(matrix));

  create_matrix(m1,3,4,2.5);
  printMatrix(m1);
  
}
*/
