
#define STEPSIZE 0.001

#include<math.h>
#include "matrices.c"
#include "bdd.c"
#include "fichier.c"

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


void print_errors(main_network * main_ntw, int network_index){

  int i, j;

  for(i=main_ntw->ntw[network_index].nb_layer-1; i>=0 ;i--){
    printf("\nLAYER [%d] :\n",i);
    for(j=0; j<main_ntw->ntw[network_index].lyr[i].nb_nrn; j++){
      printf("\tERROR Neurone [%d] ---> %e\n",j,main_ntw->ntw[network_index].lyr[i].error->mat[j][0]);
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

  for(i=0;i<info.n-1;i++){

    ntw->lyr[i].weights = malloc(sizeof(matrix));
    ntw->lyr[i].weights->mat_h = info.vect[i]+1;
    ntw->lyr[i].weights->mat_w = info.vect[i+1];
    alloc_matrix(ntw->lyr[i].weights);

    ntw->lyr[i].outputs = malloc(sizeof(matrix));
    ntw->lyr[i].outputs->mat_h = batch_size;
    ntw->lyr[i].outputs->mat_w = info.vect[i+1]+1;
    alloc_matrix(ntw->lyr[i].outputs);

    ntw->lyr[i].error = malloc(sizeof(matrix));
    ntw->lyr[i].error->mat_h = info.vect[i+1];
    ntw->lyr[i].error->mat_w = 1;
    alloc_matrix(ntw->lyr[i].error);

    ntw->lyr[i].nb_nrn = info.vect[i+1];

    for(j=0;j<batch_size;j++){
      ntw->lyr[i].outputs->mat[j][ntw->lyr[i].outputs->mat_w-1] = 1.0;
    }

    for (j=0; j<ntw->lyr[i].weights->mat_h; j++){
      for (k=0; k<ntw->lyr[i].weights->mat_w; k++){
	ntw->lyr[i].weights->mat[j][k] = frand_a_b(-0.5,0.5);
      }
    }
  }
}

void build_main_network(main_network * main_ntw, int nb_ntw, information info,int batch_size){
  int i;
  
  main_ntw->nb_ntw=nb_ntw;
  main_ntw->batch_size=batch_size;
  main_ntw->info=info;

  for(i=0 ; i<main_ntw->nb_ntw ; i++){
    alloc_network(&(main_ntw->ntw[i]),info,batch_size);
  }

  main_ntw->current_batch_data = malloc(sizeof(matrix));
  main_ntw->current_batch_data->mat_h = batch_size;
  main_ntw->current_batch_data->mat_w = (info.vect[0])+1;
  alloc_matrix(main_ntw->current_batch_data);
  matrix_init_bias(main_ntw->current_batch_data);

  main_ntw->current_batch_class = malloc(sizeof(matrix));
  main_ntw->current_batch_class->mat_h = batch_size;
  main_ntw->current_batch_class->mat_w = 1;

  alloc_matrix(main_ntw->current_batch_class);

}



void print_network(network * ntw){
  int i, j, k;

  printf("\nNombre de features : %d\n",ntw->info.vect[0]);
  printf("Nombre de layer : %d\n\n",(ntw->info.n)-1);

  for(i=0;i<ntw->info.n-1;i++){
    for(j=0;j<ntw->info.vect[i+1];j++){
      for(k=0;k<ntw->info.vect[i]+1;k++){

	printf("LAYER%d, NEUR%d, WEIGHT%d  --->\t",i,j,k);
	printf("%f\n",ntw->lyr[i].weights->mat[k][j]);
      }
    }
    printf("\n");
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

void forward(main_network * main_ntw, int indice_network){
  int i;

  matrix ** pt_data;
  pt_data = malloc(sizeof(matrix*));
  pt_data=&(main_ntw->current_batch_data);

  for(i=0;i<main_ntw->ntw[indice_network].nb_layer;i++){

    layer_output(*pt_data,&(main_ntw->ntw[indice_network].lyr[i]));
    pt_data=&(main_ntw->ntw[indice_network].lyr[i].outputs);
  }
}


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

  for(i=0;i<matrix_expected->mat_h;i++){
    if(((int)current_batch_class->mat[i][0])==expected){
      matrix_expected->mat[i][0]=1;
    }
    else{
      matrix_expected->mat[i][0]=0;
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

  for(i=0;i<ntw->lyr[layer_index].nb_nrn;i++){
    error = 0;
    for(j=0;j<ntw->lyr[layer_index+1].nb_nrn;j++){
      
      error += (ntw->lyr[layer_index+1].error->mat[j][0]) * (ntw->lyr[layer_index+1].weights->mat[i][j]);
    }

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

  int nb_layer = main_ntw->ntw[network_index].nb_layer;
  layer * last_lyr = &(main_ntw->ntw[network_index].lyr[(main_ntw->ntw[network_index].nb_layer)-1]);
  layer * second_to_last;
  if(nb_layer>1){
    second_to_last = &(main_ntw->ntw[network_index].lyr[(main_ntw->ntw[network_index].nb_layer)-2]);
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

  int i, j;
  int epoch;

  for(i=0 ;i<10 ;i++){
    for(epoch=0 ; epoch < 100 ; epoch++){
      for(j=0 ; j<10 ; j++){

	if(j != (main_ntw->ntw[network_index].class)){
	  
	  build_batch(main_ntw, epoch, j);
	  forward(main_ntw, network_index);
	  backpropagation(main_ntw, network_index);
	  
	  build_batch(main_ntw, epoch, main_ntw->ntw[network_index].class);
	  forward(main_ntw, network_index);
	  backpropagation(main_ntw, network_index);
	}
      }
    }
  }
}




void build_batch_test(main_network * main_ntw){

  int i, j;
  
  main_ntw->current_batch_data = malloc(sizeof(matrix));
  main_ntw->current_batch_data->mat_h = main_ntw->test->n;
  main_ntw->current_batch_data->mat_w = (main_ntw->info.vect[0])+1;
  alloc_matrix(main_ntw->current_batch_data);
  matrix_init_bias(main_ntw->current_batch_data);

  main_ntw->current_batch_class = malloc(sizeof(matrix));
  main_ntw->current_batch_class->mat_h = main_ntw->test->n;
  main_ntw->current_batch_class->mat_w = (main_ntw->info.vect[0])+1;
  alloc_matrix(main_ntw->current_batch_class);

  for(i=0;i<main_ntw->test->n;i++){
    for(j=0;j<main_ntw->info.vect[0];j++){
      main_ntw->current_batch_data->mat[i][j]=main_ntw->test->im[i].pix[j];
    }
    main_ntw->current_batch_class->mat[i][0]=main_ntw->test->im[i].class;
  }
}



void prediction(main_network * main_ntw){
  int i, j;
  float * score;
  int * predi;

  score = malloc(main_ntw->test->n * sizeof(float));
  predi = malloc(main_ntw->test->n * sizeof(int));

  for(i=0 ; i<main_ntw->test->n ; i++){
    *score=-1.0;
    score++;
  }

  for(i=0 ; i<10 ; i++){
    for(j=0;j<main_ntw->info.n-1;j++){
      main_ntw->ntw[i].lyr[j].outputs = malloc(sizeof(matrix));
      main_ntw->ntw[i].lyr[j].outputs->mat_h = main_ntw->test->n;
      main_ntw->ntw[i].lyr[j].outputs->mat_w = main_ntw->info.vect[j+1]+1;
      alloc_matrix(main_ntw->ntw[i].lyr[j].outputs);
    }
  }
  
  build_batch_test(main_ntw);

  for(i=0 ; i<10 ; i++){
    forward(main_ntw, i);
  }

  for(i=0 ; i<main_ntw->test->n ; i++){
    for(j=0 ; j<10 ; j++){
      if(main_ntw->ntw[j].lyr[0].outputs->mat[i][0] > score[i]){
	score[i] =  main_ntw->ntw[j].lyr[0].outputs->mat[i][0];
	predi[i] = main_ntw->ntw[j].class;
      }
    }
  }

  for(i=0 ; i<main_ntw->test->n ; i++){
    printf("Prediction de l'image %d --->\t%d\n",i,predi[i]);
  }
}
      
void conversion(main_network * main_ntw, image * newtest){

  int i, j;
  int val;

  for(i=0 ; i<newtest->ny ; i++){
    for(j=0 ; j<newtest->nx ; j++){
      val=0;
      val+=newtest->tabl[j][i].r;
      val+=newtest->tabl[j][i].g;
      val+=newtest->tabl[j][i].b;
      val/=3;
      main_ntw->test->im[main_ntw->test->n].pix[i*newtest->nx+j]=val/255*2-1;
    }
  }
  main_ntw->test->n+=1;
}


  
int main(int argc, char ** argv){


  int i;
  information info;
  main_network * main_ntw;
  image * pic;
  image * c;
  image * formatted;

  main_ntw = malloc(sizeof(main_network));
  pic = malloc(sizeof(*pic));
  c = malloc(sizeof(*c));
  formatted = malloc(sizeof(*formatted));

  printf("Lancement du programme\n\n");

  int t[]={784,1};
  info.vect=t;
  info.n=sizeof(t)/sizeof(*t);


  build_main_network(main_ntw,10,info,24);

  printf("Chargement des bases de données training...\n");
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
  printf("Done\n\n");
 
  for(i=0;i<10;i++){
    printf("Training du caractere \"%d\" en cours...\n",i);
    main_ntw->ntw[i].class=i;
    training(main_ntw, i);
    printf("Done\n");
  }

  
  /*
  main_ntw->test = malloc(sizeof(bdd));
  main_ntw->test->n=0;
  load_bdd(main_ntw->test,"numbers_test.txt");
  //printf("N = %d\n",main_ntw->test->n);
  main_ntw->test->n=3000;

  prediction(main_ntw);
  */


  main_ntw->test = malloc(sizeof(bdd));
  main_ntw->test->n = 0;
  main_ntw->test->im = malloc(32 * sizeof(data_image));
  //le 32 est temporaire, il doit etre le nombre de caractere qu on doit predire
  for(i=0;i<32;i++){
    main_ntw->test->im[i].pix = malloc(784 * sizeof(float));
    main_ntw->test->im[i].n = 784;
  }


   /*
  main_ntw->test = malloc(sizeof(bdd));
  main_ntw->test->n=0;
  load_bdd(main_ntw->test,"numbers_test.txt");
  //printf("N = %d\n",main_ntw->test->n);
  main_ntw->test->n=3000;

  prediction(main_ntw);
  */

  if(argc<2){

  pic=lireExtraire(pic,"00.ppm");
  c=loadChar(pic,c,delimit(pic));
  formatted=resize(c,formatted);
  conversion(main_ntw,formatted);

  pic=lireExtraire(pic,"11.ppm");
  c=loadChar(pic,c,delimit(pic));
  formatted=resize(c,formatted);
  conversion(main_ntw,formatted);

  pic=lireExtraire(pic,"22.ppm");
  c=loadChar(pic,c,delimit(pic));
  formatted=resize(c,formatted);
  conversion(main_ntw,formatted);
  
  pic=lireExtraire(pic,"33.ppm");
  c=loadChar(pic,c,delimit(pic));
  formatted=resize(c,formatted);
  conversion(main_ntw,formatted);

  pic=lireExtraire(pic,"44.ppm");
  c=loadChar(pic,c,delimit(pic));
  formatted=resize(c,formatted);
  conversion(main_ntw,formatted);

  pic=lireExtraire(pic,"55.ppm");
  c=loadChar(pic,c,delimit(pic));
  formatted=resize(c,formatted);
  conversion(main_ntw,formatted);

  pic=lireExtraire(pic,"66.ppm");
  c=loadChar(pic,c,delimit(pic));
  formatted=resize(c,formatted);
  conversion(main_ntw,formatted);
  
  pic=lireExtraire(pic,"77.ppm");
  c=loadChar(pic,c,delimit(pic));
  formatted=resize(c,formatted);
  conversion(main_ntw,formatted);
  
  pic=lireExtraire(pic,"88.ppm");
  c=loadChar(pic,c,delimit(pic));
  formatted=resize(c,formatted);
  conversion(main_ntw,formatted);
  
  pic=lireExtraire(pic,"99.ppm");
  c=loadChar(pic,c,delimit(pic));
  formatted=resize(c,formatted);
  conversion(main_ntw,formatted);

  }

  else{
    for(i=1 ; i<argc ; i++){
      pic=lireExtraire(pic,argv[i]);
      c=loadChar(pic,c,delimit(pic));
      formatted=resize(c,formatted);
      conversion(main_ntw,formatted);
    }
  }
  

  //dupliquer(formatted,"copiedimanche.ppm");

  printf("\n\nPREDICTION :\n\n");

  prediction(main_ntw);

  
  
  
}
