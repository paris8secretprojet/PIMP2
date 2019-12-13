//TODO : mettre le batch_size dans info
#define STEPSIZE 0.001

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
  float * score;
  int * prediction;
};
typedef struct main_network main_network;



//[nbfeat,3,3,1]

void print_errors(main_network * main_ntw, int network_index){

  int i, j;

  for(i=main_ntw->ntw[network_index].nb_layer-1; i>=0 ;i--){
    //printf("\nLAYER [%d] :\n",i);
    for(j=0; j<main_ntw->ntw[network_index].lyr[i].nb_nrn; j++){
      //printf("\tERROR Neurone [%d] ---> %e\n",j,main_ntw->ntw[network_index].lyr[i].error->mat[j][0]);
    }
  }
}



void matrix_init_bias(matrix *m){
  int i;

  for(i=0;i<m->mat_h;i++){
    m->mat[i][(m->mat_w)-1]=1.0;
  }
}



float frand_a_b(float a, float b){
    return ( rand()/(float)RAND_MAX ) * (b-a) + a;
}

int rand_Belhadj(int n){

  return (int) (n*(rand()/RAND_MAX+1.0));
}

void alloc_network(network * ntw, information info, int batch_size){
  int i, j, k;

  ntw->info=info;
  ntw->nb_layer=(info.n)-1;
  ntw->lyr = malloc(ntw->nb_layer * sizeof(layer));

  // printf("D\n");

  for(i=0;i<info.n-1;i++){

    ntw->lyr[i].weights = malloc(sizeof(matrix));
    ntw->lyr[i].weights->mat_h = info.vect[i]+1;
    ntw->lyr[i].weights->mat_w = info.vect[i+1];
    alloc_matrix(ntw->lyr[i].weights);

    //  printf("E\n");

    ntw->lyr[i].outputs = malloc(sizeof(matrix));
    ntw->lyr[i].outputs->mat_h = batch_size;
    ntw->lyr[i].outputs->mat_w = info.vect[i+1]+1;
    alloc_matrix(ntw->lyr[i].outputs);

    //  printf("F\n");

    ntw->lyr[i].error = malloc(sizeof(matrix));
    ntw->lyr[i].error->mat_h = info.vect[i+1];
    ntw->lyr[i].error->mat_w = 1;
    alloc_matrix(ntw->lyr[i].error);

    //    printf("G\n");

    ntw->lyr[i].nb_nrn = info.vect[i+1];

    //    printf("H\n");

    for(j=0;j<batch_size;j++){
      ntw->lyr[i].outputs->mat[j][ntw->lyr[i].outputs->mat_w-1] = 1.0;
    }

    //    printf("I\n");

    for (j=0; j<ntw->lyr[i].weights->mat_h; j++){
      for (k=0; k<ntw->lyr[i].weights->mat_w; k++){
	ntw->lyr[i].weights->mat[j][k] = frand_a_b(-0.5,0.5);
      }
    }

    //   printf("J\n");
  }
}

void build_main_network(main_network * main_ntw, int nb_ntw, information info,int batch_size){
  int i;

  //  printf("100\n");

  main_ntw->nb_ntw=nb_ntw;
  main_ntw->batch_size=batch_size;
  main_ntw->info=info;

  //  printf("200\n");

  for(i=0 ; i<main_ntw->nb_ntw ; i++){
    alloc_network(&(main_ntw->ntw[i]),info,batch_size);
  }

  //  printf("300\n");

  main_ntw->current_batch_data = malloc(sizeof(matrix));
  main_ntw->current_batch_data->mat_h = batch_size;
  main_ntw->current_batch_data->mat_w = (info.vect[0])+1;
  alloc_matrix(main_ntw->current_batch_data);
  matrix_init_bias(main_ntw->current_batch_data);

  //  printf("400\n");
  main_ntw->current_batch_class = malloc(sizeof(matrix));
  main_ntw->current_batch_class->mat_h = batch_size;
  main_ntw->current_batch_class->mat_w = 1;

  alloc_matrix(main_ntw->current_batch_class);

//  printf("500\n");

}



void print_network(network * ntw){
  int i, j, k;

  //printf("\nNombre de features : %d\n",ntw->info.vect[0]);
  //printf("Nombre de layer : %d\n\n",(ntw->info.n)-1);

  for(i=0;i<ntw->info.n-1;i++){
    for(j=0;j<ntw->info.vect[i+1];j++){
      for(k=0;k<ntw->info.vect[i]+1;k++){

	//	if(i==0){
	//printf("LAYER%d, NEUR%d, WEIGHT%d  --->\t",i,j,k);
	//printf("%f\n",ntw->lyr[i].weights->mat[k][j]);
      }
    }
    //printf("\n");
  }
}


void layer_output(matrix * previous_layer_output, layer * lyr){

  int i,j;

  matrix * preactivation_matrix;
  matrix obj1;
  preactivation_matrix = &obj1;

  //printf("<\n");

  preactivation_matrix = matrix_dot_product(previous_layer_output, lyr->weights, preactivation_matrix);

  //printf("<<\n");
  
  matrix_sigmoid(preactivation_matrix);

  //printf("<<<\n");
  printf("PRE W : %d\n",preactivation_matrix->mat_w);
  printf("PRE H : %d\n",preactivation_matrix->mat_h);

  printf("OUT W : %d\n",lyr->outputs->mat_w);
  printf("OUT H : %d\n",lyr->outputs->mat_h);
  
  for(i=0; i<preactivation_matrix->mat_w; i++){
    for(j=0; j<preactivation_matrix->mat_h; j++){
      //printf("I = %d, J = %d\n",i,j);
      lyr->outputs->mat[j][i] = preactivation_matrix->mat[j][i];
    }
  }

  //printf("<<<<\n");
  
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
      m->mat[row][col] = frand_a_b(-0.5,0.5);
    }
  }
}



void print_layer_output(main_network * main_ntw, int indice_network, layer *lyr){
  int i, j;

  for(i=0;i<lyr->outputs->mat_h;i++){
    printf("\nData %d :\n",i);
    printf("\t NEURONE CLASS -> %d\n",main_ntw->ntw[indice_network].class);
    printf("\t Class ---> %f ",main_ntw->current_batch_class->mat[i][0]);
    for(j=0;j<lyr->outputs->mat_w;j++){
      printf("\tOutput %d --->   %e\n",j,lyr->outputs->mat[i][j]);
    }
  }
}

/*
float calculate_error(float predict, float target){
  return (target-predict)*(target-predict)/2;
}
*/

void forward(main_network * main_ntw, int indice_network){
  int i;

  matrix ** pt_data;
  pt_data = malloc(sizeof(matrix*));
  pt_data=&(main_ntw->current_batch_data);

  //printf("INSIDE 1\n");

  for(i=0;i<main_ntw->ntw[indice_network].nb_layer;i++){
    //printf("INSIDE 2\n");
    layer_output(*pt_data,&(main_ntw->ntw[indice_network].lyr[i]));
    pt_data=&(main_ntw->ntw[indice_network].lyr[i].outputs);
    //print_layer_output(&(main_ntw->ntw[indice_network].lyr[i]));
  }
  //printf("\n\n\nPredict :\n");
  //print_layer_output(main_ntw, indice_network, &(main_ntw->ntw[indice_network].lyr[(main_ntw->ntw[indice_network].nb_layer)-1]));

}

/*
void global_forward(main_network * main_ntw, matrix * data){
  int i;

  for(i=0;i<main_ntw->nb_ntw;i++){
    forward(&(main_ntw->ntw[i]),data);
  }
}
*/

void build_batch(main_network * main_ntw, int n, int choice_dataset){
  //Le n sert a choisir le fait de construire à partir de la bdd training ou la bdd test (0->training / 1->test).
  int i, j;

  for(i=0;i<main_ntw->batch_size;i++){
    for(j=0;j<main_ntw->info.vect[0];j++){
      main_ntw->current_batch_data->mat[i][j]=main_ntw->char_training[choice_dataset].im[n*(main_ntw->batch_size)+i].pix[j];
    }
    main_ntw->current_batch_class->mat[i][0]=main_ntw->char_training[choice_dataset].im[n*(main_ntw->batch_size)+i].class;
  }
  

  /*
  if(train_or_test==1){
    for(i=0;i<main_ntw->batch_size;i++){
      for(j=0;j<main_ntw->info.vect[0];j++){
	main_ntw->current_batch_data->mat[i][j]=main_ntw->test->im[n*(main_ntw->batch_size)+i].pix[j];
      }
      main_ntw->current_batch_class->mat[i][0]=main_ntw->test->im[n*(main_ntw->batch_size)+i].class;
    }
  }
  */
}

void whatdiduexpected(matrix * current_batch_class, int expected, matrix * matrix_expected){
  int i;

  //printf("\nEXPECTED 1\n");
  for(i=0;i<matrix_expected->mat_h;i++){
    if(((int)current_batch_class->mat[i][0])==expected){
      matrix_expected->mat[i][0]=1;
      //printf("\t1\n");
    }
    else{
      matrix_expected->mat[i][0]=0;
      //printf("\t0\n");
    }
  }
}

void total_error(main_network * main_ntw, layer * last_lyr, int nrn_class){
  int i;
  float error;
  float negative_target;
  error=0;
  negative_target=0;

  matrix * matrix_expected;
  matrix_expected = malloc(sizeof(matrix));
  matrix_expected->mat_h = main_ntw->batch_size;
  matrix_expected->mat_w = 1;
  alloc_matrix(matrix_expected);
  whatdiduexpected(main_ntw->current_batch_class, nrn_class, matrix_expected);

  for(i=0;i<main_ntw->batch_size;i++){
    error += last_lyr->outputs->mat[i][0] - matrix_expected->mat[i][0];
    negative_target += ((matrix_expected->mat[i][0])-(last_lyr->outputs->mat[i][0]));
  }

  last_lyr->error->mat[0][0] = error;
  last_lyr->negative_target = negative_target;
  free(matrix_expected);
}

void layer_error(main_network * main_ntw, network * ntw, int layer_index){
  int i, j;
  float error;

  //printf("\nLAYER_ERROR : nb_neurone for layer_index=%d   ---> %d\n",layer_index,ntw->lyr[layer_index].nb_nrn);
  //printf("LAYER_ERROR : nb_neurone for layer_index=%d   ---> %d\n",layer_index+1,ntw->lyr[layer_index+1].nb_nrn);

  for(i=0;i<ntw->lyr[layer_index].nb_nrn;i++){
    error = 0;
    for(j=0;j<ntw->lyr[layer_index+1].nb_nrn;j++){
      //printf("A %f\t",ntw->lyr[layer_index+1].error->mat[j][0]);
      //printf("B %f\t",ntw->lyr[layer_index+1].weights->mat[i][j]);
      error += (ntw->lyr[layer_index+1].error->mat[j][0]) * (ntw->lyr[layer_index+1].weights->mat[i][j]);
    }
    //printf("C %f\n",ntw->lyr[layer_index].error->mat[i][0]);
    error *= matrix_mean_column(ntw->lyr[layer_index].outputs,0) * (1 - (matrix_mean_column(ntw->lyr[layer_index].outputs,0)));
    ntw->lyr[layer_index].error->mat[i][0] = error;
  }
}

void new_final_layer_weights(main_network * main_ntw, layer * last_lyr, layer * second_to_last, int nb_layer){

  int i, j;
  float delta;

  for(i=0; i<last_lyr->weights->mat_h; i++){

    delta = 0;

    if(nb_layer>1){
      for(j=0;j<main_ntw->batch_size;j++){
	delta += last_lyr->outputs->mat[j][0] * ( 1 - last_lyr->outputs->mat[j][0]) * second_to_last->outputs->mat[j][i];
      }
    }

    else{
      for(j=0;j<main_ntw->batch_size;j++){
	delta += last_lyr->outputs->mat[j][0] * ( 1 - last_lyr->outputs->mat[j][0]) * main_ntw->current_batch_data->mat[j][i];
      }
    }

    /*
    printf("DELTA : %f\n", delta );
    printf("ERROR : %f\n", last_lyr->error->mat[0][0] );
    */
    

    last_lyr->weights->mat[i][0] -= STEPSIZE * delta * last_lyr->error->mat[0][0];
  }
}


void layer_new_weights(main_network * main_ntw, int network_index, int layer_index){
  int i,j;
  //printf("START LAYER_NEW_WEIGHTS\n");

  //printf("NB_NEURONE DU LAYER [%d] = %d\n",layer_index,main_ntw->ntw[network_index].lyr[layer_index].nb_nrn);

  for (i=0; i<main_ntw->ntw[network_index].lyr[layer_index].nb_nrn; i++){
    //printf("\nITERATION = %d   (LAYER_NEW_WEIGHTS)\n",i);
    //printf("WEIGHT --> %f\n",main_ntw->ntw[network_index].lyr[layer_index].weights->mat[i][0]);
    //printf("STEP_SIZE --> %f\n",STEPSIZE);
    //printf("ERROR --> %f\n",main_ntw->ntw[network_index].lyr[layer_index].error->mat[i][0]);
    for (j=0 ;j<main_ntw->ntw[network_index].lyr[layer_index].weights->mat_h; j++){
      //printf("WEIGHT --> %f\n",main_ntw->ntw[network_index].lyr[layer_index].weights->mat[i][0]);
      main_ntw->ntw[network_index].lyr[layer_index].weights->mat[j][i] -= STEPSIZE * (main_ntw->ntw[network_index].lyr[layer_index].error->mat[i][0]);
      //printf("NEW WEIGHT --> %f\n",main_ntw->ntw[network_index].lyr[layer_index].weights->mat[i][0]);
    }
  }
    //printf("END LAYER_NEW_WEIGHTS\n");
}

void backpropagation(main_network * main_ntw, int network_index){
  int i;

  int nb_layer = main_ntw->ntw[network_index].nb_layer;
  layer * last_lyr = &(main_ntw->ntw[network_index].lyr[(main_ntw->ntw[network_index].nb_layer)-1]);
  layer * second_to_last;
  if(nb_layer>1){
    second_to_last = &(main_ntw->ntw[network_index].lyr[(main_ntw->ntw[network_index].nb_layer)-1]);
  }
  else{
    second_to_last = NULL;
  }
  //printf("STARTING BP, CALCULATING TOTAL ERROR\n\n");
  total_error(main_ntw, last_lyr, main_ntw->ntw[network_index].class);
  //printf("CONTINUING BP, CALCULATING LAYER ERROR\n\n");
  //for(i=(main_ntw->ntw[network_index].nb_layer)-2; i>=0; i--){
  //layer_error(main_ntw, network_index, i);
  //}

  //print_errors(main_ntw, network_index);

  //printf("CONTINUING BP, CALCULATING NEW LAYER WEIGHTS\n\n");
  //printf("%d\n",main_ntw->ntw[network_index].nb_layer);
  //for(i=(main_ntw->ntw[network_index].nb_layer)-1; i>=0; i--){
    //printf("TEST\n");
    //layer_new_weights(main_ntw, network_index, i);
  //}
  //printf("FINISHED BP, CALCULATING LAYER ERROR\n\n");

  new_final_layer_weights(main_ntw, last_lyr, second_to_last, nb_layer);
  //print_layer_output(main_ntw, network_index, &(main_ntw->ntw[network_index].lyr[(main_ntw->ntw[network_index].nb_layer)-1]));



}

void training(main_network * main_ntw, int network_index){

  int i, j, k;
  int epoch;

  for(i=0 ;i<100 ;i++){
    for(epoch=0 ; epoch < 50 ; epoch++){
      for(j=0 ; j<10 ; j++){

	//printf("C\n");

	if(j != (main_ntw->ntw[network_index].class)){
	  
	  build_batch(main_ntw, epoch, j);
	  forward(main_ntw, network_index);
	  backpropagation(main_ntw, network_index);

	  //printf("\n\n*****************************************************************\n\n");
	  
	  build_batch(main_ntw, epoch, main_ntw->ntw[network_index].class);
	  forward(main_ntw, network_index);
	  backpropagation(main_ntw, network_index);
	}
      }

      /*
	printf("\n\nBATCH\n\n");
	for(j=0 ; j<main_ntw->current_batch_data->mat_h ; j++){
	for(k=0 ; k<main_ntw->current_batch_data->mat_w ; k++){
	printf("%f\t",main_ntw->current_batch_data->mat[j][k]);
	}
	printf("\n");
	}
      */
    }
  }
}


/*

void download_weights(main_network * main_ntw, FILE * fd){

  




}

void global_download_weights(main_netwok * main_ntw, int class){

  FILE * fd;

  switch(class){

  case 0 :
    fd = ouvrir("./SAVE_WEIGHTS/save_weights_10.txt");
    
    


}


void global_training(main_network * main_ntw){

  int i;

  for(i=0;i<26;i++){
    main_ntw->ntw[0].class = i+10;
    training(main_ntw, 0);
  }




}

*/


void build_batch_test(main_network * main_ntw){
  //Le n sert a choisir le fait de construire à partir de la bdd training ou la bdd test (0->training / 1->test).
  int i, j;

  printf("AAA\n");
  
  main_ntw->current_batch_data = malloc(sizeof(matrix));
  printf("BBB\n");
  main_ntw->current_batch_data->mat_h = main_ntw->test->n;
  printf("CCC\n");
  main_ntw->current_batch_data->mat_w = (main_ntw->info.vect[0])+1;
  printf("DDD\n");
  alloc_matrix(main_ntw->current_batch_data);
  printf("EEE\n");
  matrix_init_bias(main_ntw->current_batch_data);

  main_ntw->current_batch_class = malloc(sizeof(matrix));
  printf("BBB\n");
  main_ntw->current_batch_class->mat_h = main_ntw->test->n;
  printf("CCC\n");
  main_ntw->current_batch_class->mat_w = (main_ntw->info.vect[0])+1;
  printf("DDD\n");
  alloc_matrix(main_ntw->current_batch_class);
  printf("EEE\n");
  //matrix_init_bias(main_ntw->current_batch_data);

  printf("FFF\n");

  for(i=0;i<main_ntw->test->n;i++){
    //printf("HELLO\n");
    for(j=0;j<main_ntw->info.vect[0];j++){
      main_ntw->current_batch_data->mat[i][j]=main_ntw->test->im[i].pix[j];
    }
    //printf("COUCOU1\n");
    main_ntw->current_batch_class->mat[i][0]=main_ntw->test->im[i].class;
    //printf("COUCOU2\n");
    
  }
  
  printf("GGG\n");
  
  /*
  if(train_or_test==1){
    for(i=0;i<main_ntw->batch_size;i++){
      for(j=0;j<main_ntw->info.vect[0];j++){
	main_ntw->current_batch_data->mat[i][j]=main_ntw->test->im[n*(main_ntw->batch_size)+i].pix[j];
      }
      main_ntw->current_batch_class->mat[i][0]=main_ntw->test->im[n*(main_ntw->batch_size)+i].class;
    }
  }
  */
}



void prediction(main_network * main_ntw){
  int i, j;
  float accuracy;
  accuracy = 0;

  main_ntw->prediction = malloc(main_ntw->test->n * sizeof(int));
  main_ntw->score = malloc(main_ntw->test->n * sizeof(float));

  printf("00\n");

  for(i=0 ; i<main_ntw->test->n ; i++){
    main_ntw->prediction[i]=0;
    main_ntw->score[i]=0.0;
  }

  for(i=0 ; i<10 ; i++){
    for(j=0;j<main_ntw->info.n-1;j++){
      main_ntw->ntw[i].lyr[j].outputs = malloc(sizeof(matrix));
      main_ntw->ntw[i].lyr[j].outputs->mat_h = main_ntw->test->n;
      main_ntw->ntw[i].lyr[j].outputs->mat_w = main_ntw->info.vect[j+1]+1;
      alloc_matrix(main_ntw->ntw[i].lyr[j].outputs);
    }
  }

  printf("11\n");
  
  build_batch_test(main_ntw);

  printf("22\n");
  
  for(i=0 ; i<10 ; i++){

    printf("FORWARD %d\n",i); 
    
    forward(main_ntw, i);

    printf("ALPHA\n");

    for(j=0 ; j<main_ntw->test->n ; j++){
      printf("BETA : %d\n",j);
      if(main_ntw->ntw[i].lyr[(main_ntw->ntw[i].nb_layer)-1].outputs->mat[j][0] > main_ntw->score[j]){
	main_ntw->score[j] = main_ntw->ntw[i].lyr[(main_ntw->ntw[i].nb_layer)-1].outputs->mat[j][0];
	main_ntw->prediction[j] = main_ntw->ntw[i].class;
      }
    }
  }

  printf("33\n");

  for(i=0 ; i<main_ntw->test->n ; i++){
    printf("EXCEPTED : %d --->\t\t PREDICT : %d\n",(int)main_ntw->current_batch_class->mat[i][0],main_ntw->prediction[i]);
    if((int)main_ntw->current_batch_class->mat[i][0]==main_ntw->prediction[i]){
      accuracy++;
    }
  }
  printf("ACCURACY = %f\n",accuracy/main_ntw->test->n);
}
      

  
  
int main(){

  int i;
  information info;
  main_network * main_ntw;
  //network * ntw;
  //bdd * basedd_train;
  //bdd * basedd_test;
  //basedd_train = malloc(sizeof(bdd));
  //basedd_test = malloc(sizeof(bdd));
  main_ntw = malloc(sizeof(main_network));
  //ntw = malloc(sizeof(network));

  //[2,3,3,1] : Deux features, premiere couche(3 neurones), deuxieme couche(3 neurones), derniere couche(1 neurone)
  int t[]={784,1};
  info.vect=t;
  info.n=sizeof(t)/sizeof(*t);

  //printf("B\n");

  //alloc_network(ntw,info,3);
  //printf("L\n");
  //print_network(ntw);

  //printf("M\n");

  //matrix * test;
  //test = malloc(sizeof(matrix));
  //create_matrix_random(test,3,3);
  //matrix_init_bias(test);
  //printMatrix(test);

  //layer_output(test,&(ntw->lyr[0]));
  //print_layer_output(&(ntw->lyr[0]));

  //forward(ntw,test);

  build_main_network(main_ntw,10,info,5);
  //main_ntw->training = malloc(sizeof(bdd));

  printf("A\n");

  load_bdd(&(main_ntw->char_training[0]),"./DATA/0.txt");
  load_bdd(&(main_ntw->char_training[1]),"./DATA/1.txt");
  load_bdd(&(main_ntw->char_training[2]),"./DATA/2.txt");
  load_bdd(&(main_ntw->char_training[3]),"./DATA/3.txt");
  load_bdd(&(main_ntw->char_training[4]),"./DATA/4.txt");
  load_bdd(&(main_ntw->char_training[5]),"./DATA/5.txt");
  load_bdd(&(main_ntw->char_training[6]),"./DATA/6.txt");
  load_bdd(&(main_ntw->char_training[7]),"./DATA/7.txt");
  load_bdd(&(main_ntw->char_training[8]),"./DATA/8.txt");
  load_bdd(&(main_ntw->char_training[9]),"./DATA/9.txt");
  printf("N9 = %d\n",main_ntw->char_training[9].n);
  
  printf("B\n");

  for(i=0 ; i<10 ;i++){
    printf("%d\n",main_ntw->char_training[i].n);
  }
 
  for(i=0;i<10;i++){
    main_ntw->ntw[i].class=i;
    training(main_ntw, i);
    printf("I = %d\n",i);
  }
  
  main_ntw->test = malloc(sizeof(bdd));
  load_bdd(main_ntw->test,"numbers_test.txt");
  printf("N = %d\n",main_ntw->test->n);
  main_ntw->test->n=3000;

  printf("Y\n");

  prediction(main_ntw);

  
  
  
}
