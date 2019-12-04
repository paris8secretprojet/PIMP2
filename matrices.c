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

matrix *create_matrix(matrix *m, int number_rows, int number_columns, float init){

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
  return m;
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

matrix * matrix_scalar_prod(matrix * m1, float k, matrix * m3){

  int i, j;

  m3->mat_h=m1->mat_h;
  m3->mat_w=m1->mat_w;

  alloc_matrix(m3);

  for(i=0;i<m3->mat_h;i++){
    for(j=0;j<m3->mat_w;j++){
      m3->mat[i][j]=m1->mat[i][j] * k;
    }
  }
  return m3;
}

matrix * matrix_sigmoid(matrix * m1, matrix * m3){

  int i, j;

  m3->mat_h=m1->mat_h;
  m3->mat_w=m1->mat_w;

  alloc_matrix(m3);

  for(i=0;i<m3->mat_h;i++){
    for(j=0;j<m3->mat_w;j++){
      m3->mat[i][j]=1/(1+exp(-(m1->mat[i][j])));
    }
  }
  return m3;
}

matrix * matrix_sigmoid_prime(matrix * m1, matrix * m3){

  int i, j;

  m3->mat_h=m1->mat_h;
  m3->mat_w=m1->mat_w;

  alloc_matrix(m3);

  for(i=0;i<m3->mat_h;i++){
    for(j=0;j<m3->mat_w;j++){
      m3->mat[i][j]=(1/(1+exp(-(m1->mat[i][j]))))*(1-(1/(1+exp(-(m1->mat[i][j])))));
    }
  }
  return m3;
}

matrix * matrix_tanh(matrix * m1, matrix * m3){

  int i, j;

  m3->mat_h=m1->mat_h;
  m3->mat_w=m1->mat_w;

  alloc_matrix(m3);

  for(i=0;i<m3->mat_h;i++){
    for(j=0;j<m3->mat_w;j++){
      m3->mat[i][j]=tanh(m1->mat[i][j]);
    }
  }
  return m3;
}

matrix * matrix_relu(matrix * m1, matrix * m3){

  int i, j;

  m3->mat_h=m1->mat_h;
  m3->mat_w=m1->mat_w;

  alloc_matrix(m3);

  for(i=0;i<m3->mat_h;i++){
    for(j=0;j<m3->mat_w;j++){
      if(m1->mat[i][j]<0){
	m3->mat[i][j]=0;
      }
      else{
	m3->mat[i][j]=m1->mat[i][j];
      }	
    }
  }
  return m3;
}


matrix * dot_product(matrix * m1, matrix * m2, matrix * m3){

  int i, j, k;
  float res;

  /*
  printf("m->mat_h : %d\n"
	 "m->mat_w : %d\n\n",
	 m->mat_h, m->mat_w);
  */

  if(m1->mat_w != m2->mat_h){
    printf("Produit des matrices impossible : Le nombre de ligne de la premiere matrice est different du nombre de colonne de la seconde\n\n");
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

int main(void){

  
  matrix *m1, *m2, *m3, *m4, *m5, *m6, *m7, *m8;
  matrix objm1, objm2, objm3, objm4, objm5, objm6, objm7, objm8;
  m1=&objm1, m2=&objm2, m3=&objm3, m4=&objm4, m5=&objm5, m6=&objm6, m7=&objm7, m8=&objm8;

  m1 = create_matrix(m1, 3, 8, 18);
  printMatrix(m1);

  m2 = create_matrix(m2, 8, 3, 10.0);
  printMatrix(m2);

  m3 = dot_product(m1, m2, m3);
  printMatrix(m3);

  m4 = create_matrix(m4, 3, 3, 25.5);
  printMatrix(m4);

  m5 = matrix_sum(m3,m4,m5);
  printMatrix(m5);

  m6 = matrix_add_column(m5,m6,2,1);
  printMatrix(m6);

  m7 = matrix_add_row(m6,m7,2,2);
  printMatrix(m7);

  //m8 = matrix_remove_row(m6,m8,0);
  //printMatrix(m8);
}
