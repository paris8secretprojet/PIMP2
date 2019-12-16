#include "functions.h"

void print_errors(main_network * main_ntw, int network_index){
  //Affiche dans le terminal les valeur "error" associees a
  //chaque neurones de "main_ntw".

  int i, j;

  for(i=main_ntw->ntw[network_index].nb_layer-1; i>=0 ;i--){
    printf("\nLAYER [%d] :\n",i);
    for(j=0; j<main_ntw->ntw[network_index].lyr[i].nb_nrn; j++){
      printf("\tERROR Neurone [%d] ---> %e\n",j,main_ntw->ntw[network_index].lyr[i].error->mat[j][0]);
    }
  }
}




float frand_a_b(float a, float b){
  //Return un float aleatoire compris entre "a" et "b".
  
    return ( rand()/(float)RAND_MAX ) * (b-a) + a;
}

/*
int rand_Belhadj(int n){

  return (int) (n*(rand()/RAND_MAX+1.0));
  }
*/

void alloc_network(network * ntw, information info, int batch_size){
  //Alloue la memoire necessaire au differents champs de "ntw" et
  //initialise certaines valeurs de "ntw".
  
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
  //Calcul des outputs du layer "lyr" a partir de ses poids et
  //des outputs du layer precedent.

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




void print_layer_output(main_network * main_ntw, int indice_network, layer *lyr){
  //Affiche les outputs d un layer.
  
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
  //Effectue le "forward" d un certain reseau. A partir des donnees
  //d une image prises en entree on obtient un "output" final et
  //donc une prediction.
  
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
  //Construit un lot de donnees pour entrainer les reseaux.
  //La variable "choice_dataset" permet de choisir le caractere du lot d'apprentissage.

  int i, j;

  for(i=0;i<main_ntw->batch_size;i++){
    for(j=0;j<main_ntw->info.vect[0];j++){
      main_ntw->current_batch_data->mat[i][j]=main_ntw->char_training[choice_dataset].im[n*(main_ntw->batch_size)+i].pix[j];
    }
    main_ntw->current_batch_class->mat[i][0]=main_ntw->char_training[choice_dataset].im[n*(main_ntw->batch_size)+i].class;
  }
}

void whatdiduexpect(matrix * current_batch_class, int expected, matrix * matrix_expected){
  //Les valeurs de "matrix_expected" deviennent "1" si la donnee correspondante de
  //"current_batch_class" equivaut a la classe du reseau, sinon elles deviennent "0".
  
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
  //Calcul de l erreur du dernier neurone.
  
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
  whatdiduexpect(main_ntw->current_batch_class, nrn_class, matrix_expected);

  for(i=0;i<main_ntw->batch_size;i++){
    error += last_lyr->outputs->mat[i][0] - matrix_expected->mat[i][0];
    negative_target += ((matrix_expected->mat[i][0])-(last_lyr->outputs->mat[i][0]));
  }

  last_lyr->error->mat[0][0] = error;
  last_lyr->negative_target = negative_target;
  free(matrix_expected);
}

void layer_error(main_network * main_ntw, network * ntw, int layer_index){
  //Calcul de l erreur d un certain layer.
  
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
  //Modifie les poids du dernier neurone.

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
  //Retropropage la modification de poid pour tout les neurones du reseau.

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
  //Entraine tout les reseaux de neurones.

  int n, j;
  int epoch;

  //ITERATION GLOBALE:
  for(epoch=0 ;epoch<10 ;epoch++){

    //LE "n" PERMET DE DECALER LA SOURCE DE CONSTRUCTION DES LOTS D APPRENTISSAGE:
    for(n=0 ; n < 100 ; n++){

      //CHOIX DE LA SOURCE DE CONSTRUCTION DU LOT D APPRENTISSAGE:
      for(j=0 ; j<10 ; j++){

	if(j != (main_ntw->ntw[network_index].class)){
	  
	  build_batch(main_ntw, n, j);
	  forward(main_ntw, network_index);
	  backpropagation(main_ntw, network_index);
	  
	  build_batch(main_ntw, n, main_ntw->ntw[network_index].class);
	  forward(main_ntw, network_index);
	  backpropagation(main_ntw, network_index);

	  //Ci-dessus on alterne un apprentissage sur des caracteres autres que celui dont reseau est specialise avec un apprentissage sur un caractere pour lequel le reseau est specialiste.
	  //Exemple : si le reseau doit predire si un caractere est un "5" ou un "non-5" les lots d'apprentissage vont etre successivement : 1,5,2,5,3,5,4,5,6,5,7,5,8,5,9,5.
	  
	}
      }
    }
  }
}




void build_batch_test(main_network * main_ntw){
  //Construit un lot d image qu on veut predire a partir des donnees contenues
  //dans "main->test".

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


void conversion(main_network * main_ntw, image * newtest){
  //A partir des donnees d une structure image on remplit le
  //champ "main_ntw->test" (correspondant aux donnees des images
  //que l on veut predire).

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


void prediction(main_network * main_ntw){
  //Lance les forward sur tout les reseaux a partir du lot qu on veut
  //predire. Ensuite affiche les predictions.
  
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
      



  
