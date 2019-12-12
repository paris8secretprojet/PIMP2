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
  network ntw[26];
  bdd char_training[26];
  bdd * test;
  int batch_size;
  matrix * current_batch_data;
  matrix * current_batch_class;
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

  for(i=0;i<nb_ntw;i++){
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

  preactivation_matrix = matrix_dot_product(previous_layer_output, lyr->weights, preactivation_matrix);
  matrix_sigmoid(preactivation_matrix);

  for(i=0; i<preactivation_matrix->mat_w; i++){
    for(j=0; j<preactivation_matrix->mat_h; j++){
      lyr->outputs->mat[j][i] = preactivation_matrix->mat[j][i];
    }
  }
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

  for(i=0;i<main_ntw->ntw[indice_network].nb_layer;i++){
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
      for(j=0 ; j<26 ; j++){

	if(j+10 != (main_ntw->ntw[network_index].class)){
	  
	  build_batch(main_ntw, epoch, j);
	  forward(main_ntw, network_index);
	  backpropagation(main_ntw, network_index);

	  //printf("\n\n*****************************************************************\n\n");
	  
	  build_batch(main_ntw, epoch, (main_ntw->ntw[network_index].class)-10);
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

   build_main_network(main_ntw,26,info,5);
  //main_ntw->training = malloc(sizeof(bdd));

  load_bdd(&(main_ntw->char_training[0]),"./DATA/data10.txt");
  load_bdd(&(main_ntw->char_training[1]),"./DATA/data11.txt");
  load_bdd(&(main_ntw->char_training[2]),"./DATA/data12.txt");
  load_bdd(&(main_ntw->char_training[3]),"./DATA/data13.txt");
  load_bdd(&(main_ntw->char_training[4]),"./DATA/data14.txt");
  load_bdd(&(main_ntw->char_training[5]),"./DATA/data15.txt");
  load_bdd(&(main_ntw->char_training[6]),"./DATA/data16.txt");
  load_bdd(&(main_ntw->char_training[7]),"./DATA/data17.txt");
  load_bdd(&(main_ntw->char_training[8]),"./DATA/data18.txt");
  load_bdd(&(main_ntw->char_training[9]),"./DATA/data19.txt");
  load_bdd(&(main_ntw->char_training[10]),"./DATA/data20.txt");
  load_bdd(&(main_ntw->char_training[11]),"./DATA/data21.txt");
  load_bdd(&(main_ntw->char_training[12]),"./DATA/data22.txt");
  load_bdd(&(main_ntw->char_training[13]),"./DATA/data23.txt");
  load_bdd(&(main_ntw->char_training[14]),"./DATA/data24.txt");
  load_bdd(&(main_ntw->char_training[15]),"./DATA/data25.txt");
  load_bdd(&(main_ntw->char_training[16]),"./DATA/data26.txt");
  load_bdd(&(main_ntw->char_training[17]),"./DATA/data27.txt");
  load_bdd(&(main_ntw->char_training[18]),"./DATA/data28.txt");
  load_bdd(&(main_ntw->char_training[19]),"./DATA/data29.txt");
  load_bdd(&(main_ntw->char_training[20]),"./DATA/data30.txt");
  load_bdd(&(main_ntw->char_training[21]),"./DATA/data31.txt");
  load_bdd(&(main_ntw->char_training[22]),"./DATA/data32.txt");
  load_bdd(&(main_ntw->char_training[23]),"./DATA/data33.txt");
  load_bdd(&(main_ntw->char_training[24]),"./DATA/data34.txt");
  load_bdd(&(main_ntw->char_training[25]),"./DATA/data35.txt");
  

  //main_ntw->test = malloc(sizeof(bdd));
  //load_bdd(main_ntw->test,"emnist-balanced-test.txt");

  for(i=0;i<26;i++){
    main_ntw->ntw[i].class = i+10;
  }

  
  
  for(i=0;i<26;i++){
    training(main_ntw, i);
  }
  
  

  //OK : 0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25
  //training(main_ntw, 25);
  
}
