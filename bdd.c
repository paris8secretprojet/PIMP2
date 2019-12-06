#include<stdio.h>
#include<stdlib.h>
#include<assert.h>

struct data_image{
  int n;
  int * pix;
  int classe;
};
typedef struct data_image data_image;

struct bdd{
  int n;
  data_image * im;
};
typedef struct bdd bdd;



FILE * ouvrir(char * nom){

  FILE * fd;
  fd=fopen(nom,"r");
  assert(fd);
  return fd;
}

int nb_row_file(char * char_file, int n){
  
  int res, i;
  res=1;

  for(i=0;i<n;i++){
    //printf("III\n");
    if(char_file[i]=='\n'){
      ++res;
    }
  }
  return res;
}

int nb_pixel_file(char * char_file){
  
  int n;
  n=1;

  do{
    if(*(char_file)==','){
      ++n;
    }
    ++char_file;
  }while(*(char_file)!='\n');
  return n;
}


void alloc_bdd(bdd * basedd, int n){
  basedd->im = malloc(n*sizeof(data_image));
}

void load_data_image(data_image * im, int n, int * pix){
  int i;
  
  im->n=n-1;
  im->classe=pix[0];

  for(i=1;i<n;i++){
    im->pix[i-1]=pix[i];
    /*
    if(pix[i]==0){
      im->pix[i-1]=0.001;
    }
    else{
      im->pix[i-1]=(float)pix[i]/(float)255;
    }
    */
  }
}

int nb_char_file(FILE * fd){

  char c;
  int n;
  n=0;

  printf("B3\n");

  do{
    fscanf(fd,"%c",&c);
    //printf("%c",c);
    n++;
  }while(!feof(fd));
  return n;
}

void extract_char_file(FILE * fd, int n, char * char_file){

  int i;

  for(i=0;i<n;i++){
    fscanf(fd,"%c",&(char_file[i]));
    //printf("%c",char_file[i]);
  }
}

void load_bdd(bdd * basedd, char * nom){

  int n, i, j, z;
  FILE * fd;
  char * char_file;
  int nb_row;
  int nb_pixel;
  char * nombre3;
  char * nombre2;
  char * nombre1;
  int nb_chiffre;
  int val_pixel;
  int indice_pix;
  int indice_image;
  int * integer_data;
  z=0;

  nombre3 = malloc(3*sizeof(char));
  nombre2 = malloc(2*sizeof(char));
  nombre1 = malloc(1*sizeof(char));

  //printf("B1\n");
  
  fd = ouvrir(nom);


  //printf("B2\n");
  
  n = nb_char_file(fd);

  //printf("N = %d\n",n);

  //printf("B9\n");
  
  char_file = malloc(n * sizeof(char));
  fd = ouvrir(nom);
  extract_char_file(fd,n,char_file);


  //printf("C\n");
  
  nb_row = nb_row_file(char_file,n);
  nb_pixel = nb_pixel_file(char_file);

  //printf("D\n");

  basedd->n = nb_row;
  basedd->im = malloc(nb_row * sizeof(data_image));
  for(i=0;i<nb_row;i++){
    basedd->im[i].pix = malloc((nb_pixel-1) * sizeof(int));
    basedd->im[i].n = nb_pixel-1;
  }
  integer_data = malloc(nb_pixel * sizeof(int));

  indice_image=0;

  //printf("E\n");
  
  do{

    indice_pix=0;  
    do{

      nb_chiffre=0; 
      do{
	nombre3[nb_chiffre]=*char_file;
	//printf("%c,",*char_file);
	++nb_chiffre;
	++char_file;
	++z;
      
      }while((*char_file != ',') && (*char_file !='\n') && (z<n) );

      if(nb_chiffre==1){
	nombre1[0]=nombre3[0];
	integer_data[indice_pix] = atoi(nombre1);
      }
      else if(nb_chiffre==2){
	nombre2[0]=nombre3[0];
	nombre2[1]=nombre3[1];
	integer_data[indice_pix] = atoi(nombre2);
      }
      else if(nb_chiffre==3){
	integer_data[indice_pix] = atoi(nombre3);
      }
      //printf(",,%d\t",integer_data[indice_pix]);

      ++indice_pix;
      if(*char_file=='\n'){
	++char_file;
	++z;
	//printf("RETOUR CHARIOT\n");
	break;
      }
      if(z>n){
	break;
      }
      ++char_file;
      ++z;
    
      
    }while(*char_file != '\n');

    //printf("%d : ",n);
    //printf("%d\t",z);
    //printf("W\n");
    //printf("COUCOU\n");
    load_data_image(&(basedd->im[indice_image]),nb_pixel,integer_data);
    ++indice_image;
    //printf("IND IM = %d\n",indice_image);
    //printf("NB PIXEL = %d\n",nb_pixel);
    //printf("NB ROW = %d\n",nb_row);

  }while(z<n);

}

/*
void load_bdd(bdd * basedd, char * nom){
  
  int i, j;
  FILE * fd;
  int nb_row;
  int nb_pixel;
  int * data_tmp;
  char extract[3];

  fd = ouvrir(nom);
  nb_row = nb_row_file(fd);
  nb_pixel = nb_pixel_file(fd);

  basebdd->im = malloc(nb_row*sizeof(data_image));
  data_tmp = malloc(nb_pixel*sizeof(int));
  
  for(i=0;i<nb_row;i++){
    for(j=0;j<nb_pixel;j++){
      fscanf(fd,
      
    }
    load_data_image(data_image * im, int n, float * pix, int classe);
  }
*/

void print_bdd(bdd * basedd){

  int i, j;

  for(i=0;i<basedd->n;i++){
    //printf("I : %d\n",i);
    for(j=0;j<basedd->im[i].n;j++){
      printf("%d,",basedd->im[i].pix[j]);
    }
    //printf("\n\n\n");
  }
  //printf("\n\n");
}


int main(){

  bdd * basedd;
  basedd = malloc(sizeof(bdd));

  //printf("A\n");
  load_bdd(basedd,"emnist-balanced-test.txt");


  //printf("%d\n",basedd->n);
  print_bdd(basedd);




}
    
  
