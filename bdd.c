#include"functions.h"

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



int nb_row_file(char * char_file, int n){
  
  int res, i;
  res=1;

  for(i=0;i<n;i++){
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


void load_data_image(data_image * im, int n, int * pix){
  int i;
  
  im->n=n-1;
  im->class=pix[0];

  for(i=1;i<n;i++){
    im->pix[i-1]=((float)pix[i])/255*2-1;
  }
}

int nb_char_file(FILE * fd){

  char c;
  int n;
  n=0;

  do{
    fscanf(fd,"%c",&c);
    n++;
  }while(!feof(fd));
  return n;
}

void extract_char_file(FILE * fd, int n, char * char_file){

  int i;

  for(i=0;i<n;i++){
    fscanf(fd,"%c",&(char_file[i]));
  }
}

void load_bdd(bdd * basedd, char * nom){
  //QUICK AND VERY DIRTY

  int n, i, z;
  FILE * fd;
  char * char_file;
  int nb_row;
  int nb_pixel;
  char * nombre3;
  char * nombre2;
  char * nombre1;
  int nb_chiffre;
  int indice_pix;
  int indice_image;
  int * integer_data;
  z=0;

  nombre3 = malloc(3*sizeof(char));
  nombre2 = malloc(2*sizeof(char));
  nombre1 = malloc(1*sizeof(char));
  
  fd = ouvrir(nom);
  
  n = nb_char_file(fd);
  
  char_file = malloc(n * sizeof(char));
  fd = ouvrir(nom);
  extract_char_file(fd,n,char_file);
  
  nb_row = nb_row_file(char_file,n);
  nb_pixel = nb_pixel_file(char_file);

  basedd->n = nb_row;
  basedd->im = malloc(nb_row * sizeof(data_image));
  for(i=0;i<nb_row;i++){
    basedd->im[i].pix = malloc((nb_pixel-1) * sizeof(float));
    basedd->im[i].n = nb_pixel-1;
  }
  integer_data = malloc(nb_pixel * sizeof(int));

  indice_image=0;
  
  do{
    indice_pix=0;  
    do{

      nb_chiffre=0; 
      do{
	nombre3[nb_chiffre]=*char_file;
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

      ++indice_pix;
      if(*char_file=='\n'){
	++char_file;
	++z;
	break;
      }
      if(z>n){
	break;
      }
      ++char_file;
      ++z;
      
    }while(*char_file != '\n');

    load_data_image(&(basedd->im[indice_image]),nb_pixel,integer_data);
    ++indice_image;
  }while(z<n);
}

void print_bdd(bdd * basedd){

  int i, j;

  for(i=0;i<basedd->n;i++){
    for(j=0;j<basedd->im[i].n;j++){
      printf("%f,",basedd->im[i].pix[j]);
    }
  }
}

