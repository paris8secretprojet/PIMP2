#include "functions.h"

FILE * ouvrir(char * nom){
  //Permet d'obtenir un FILE* de type lecture pour le fichier "nom".

  FILE * fd;
  fd=fopen(nom,"r");
  assert(fd);
  return fd;
}

FILE * ouvrirEcrire(char * nom){
  //Permet d'obtenir un FILE* de type ecriture(append) pour le fichier "nom".

  FILE * fd;
  fd=fopen(nom,"a");
  assert(fd);
  return fd;
}

void fermer(FILE * fd){
  //Permet de fermer un FILE*.
  fclose(fd);
}

void allocImage(image * pic){
  //Alloue la mémoire necessaire au champ "tabl" de pic.

  int i;

  pic->tabl = malloc( (pic->ny) * sizeof(*(pic->tabl)) );
  for (i=0;i<pic->ny;i++){
    (pic->tabl)[i] = malloc ( (pic->nx) * sizeof(triplet) );
  }
}

char * ajoutChar(char * info, char n){
  //Ajoute un "char" au vecteur de char "info".

  char * copy;
  info = realloc(info,sizeof(*info));
  copy = info;
  while(!(*info=='\0')){
    ++info;
  }
  *info=n;
  *(++info)='\0';
  return copy;
}

char * resetStr(char * info){
  //Reset d un vecteur de char.
  
  free(info);
  info=malloc(sizeof(*info));
  assert(info);
  *info='\0';
  return info;
}

image * lireExtraire(image * pic, char * nom){
  //QUICK AND VERY DIRTY
  //Lecture des donnees d une image et insertion de ces donnees dans une structure "image"

  int val;
  val=0;

  int ix, iy;
  FILE * fd;
  char n;
  char * info;
  unsigned char a;
  
  info = malloc (sizeof(*info));
  assert(info);
  *info='\0';
  fd=ouvrir(nom);
  fscanf(fd,"%c",&n);
  fscanf(fd,"%c",&n);
  info=ajoutChar(info,n);
  pic->entete=atoi(info);
  resetStr(info);
  fscanf(fd,"%c",&n);
  
  do{
    fscanf(fd,"%c",&n);
  }while(!(n=='\n'));
  
  //EXTRACTION NX:
  do{
    fscanf(fd,"%c",&n);
    val*=10;
    val+=atoi(&n);
  }while(!(n==' '));
  pic->nx=val/10;
  val=0;
  
  //EXTRACTION NY:
  do{
    fscanf(fd,"%c",&n);
    val*=10;
    val+=atoi(&n);
  }while(!(n=='\n'));

  pic->ny=val/10;
  val=0;


  //EXTRACTION VALMAX:
  do{
    fscanf(fd,"%c",&n);
    info=ajoutChar(info,n);
  }while(!(n=='\n'));
  pic->valmax=atoi(info);
  resetStr(info);

  allocImage(pic);

  //EXTRACTION DES PIXELS
  for(iy=0;iy<pic->ny;iy++){
    for(ix=0;ix<pic->nx;ix++){
      
      fscanf(fd,"%c",&a);
      pic->tabl[iy][ix].r=(int)a;

      fscanf(fd,"%c",&a);
      pic->tabl[iy][ix].g=(int)a;

      fscanf(fd,"%c",&a);
      pic->tabl[iy][ix].b=(int)a;
    }
  }
  fermer(fd);
  return pic;
}

triplet getPixel(image * pic, int x, int y){
  //Return un "triplet" de coordonnes (x,y) issue de "pic".
  
  return pic->tabl[y][x];
}

void setPixel(image * pic, int x, int y, triplet p){
  //Le "triplet" de "pic" de coordonnees (x,y) devient le triplet
  //pris en argument par la fonction.
  
  pic->tabl[y][x]=p;
}

void afficher(image * pic){
  //AFFICHE LES DONNEES ET META-DONNEES
  
  int ix, iy;

  printf("entete : %d\n",pic->entete);
  printf("NX : %d\n",pic->nx);
  printf("NY : %d\n",pic->ny);
  printf("valmax : %d\n\n",pic->valmax);
  
  for(iy=0;iy<10;iy++){
    for(ix=0;ix<10;ix++){
      printf("|%d|%d|%d|\t",pic->tabl[iy][ix].r,pic->tabl[iy][ix].g,pic->tabl[iy][ix].b);
    }
  }
  printf("\n");
}

void rotation90anti(image * pic){
  //A partir d une image 1, creation d une image 2 qui est la
  //transformee de l image 1 par une rotation 90 antihoraire
  
  int ix, iy;
  FILE * fd;
  fd=ouvrirEcrire("Rotation.512.ppm");
  fprintf(fd,"P%d\n%d %d\n%d\n",pic->entete,pic->nx,pic->ny,pic->valmax);

  for(ix=0;ix<pic->nx;ix++){
    for(iy=0;iy<pic->ny;iy++){
      fprintf(fd,"%c%c%c",(unsigned int)pic->tabl[iy][ix].r,(unsigned int)pic->tabl[iy][ix].g,(unsigned int)pic->tabl[iy][ix].b);
    }
  }
  fermer(fd);
}

void dupliquer(image * pic, char * nom){
  //Creation d une image a partir des donnees de "pic"
  
  int ix, iy;
  FILE * fd;
  fd=ouvrirEcrire(nom);

  fprintf(fd,"P%d\n%d %d\n%d\n",pic->entete,pic->nx,pic->ny,pic->valmax);

  for(iy=0;iy<pic->ny;iy++){
    for(ix=0;ix<pic->nx;ix++){
      fprintf(fd,"%c%c%c",(unsigned char)pic->tabl[iy][ix].r,(unsigned char)pic->tabl[iy][ix].g,(unsigned char)pic->tabl[iy][ix].b);
    }
  }
  fermer(fd);
}

int contrasteVoisin(triplet p1, triplet p2){
  //Return la distance de manhattan entre deux pixels.
  
  return abs(p1.r-p2.r)+abs(p1.g-p2.g)+abs(p1.b-p2.b);
}

int contraste(image * pic, int x, int y){
  //Return la valeur de contraste d un triplet de coordonnees (x,y)
  //contenu dans pic.

  if(x<0 || x>pic->nx || y<0 || y>pic->ny){
    return -1;
  }
  
  if(x==0 && y==0){
    return contrasteVoisin(pic->tabl[y][x],pic->tabl[y][x+1]) + contrasteVoisin(pic->tabl[y][x],pic->tabl[y+1][x+1]) + contrasteVoisin(pic->tabl[y][x],pic->tabl[y+1][x]);
  }//COEFFICIENTS ???
  
  if(x==pic->nx-1 && y==0){
    return contrasteVoisin(pic->tabl[y][x],pic->tabl[y][x-1]) + contrasteVoisin(pic->tabl[y][x],pic->tabl[y+1][x-1]) + contrasteVoisin(pic->tabl[y][x],pic->tabl[y+1][x]);
  }//COEFFICIENTS ???

  if(x==0 && y==pic->ny-1){
    return contrasteVoisin(pic->tabl[y][x],pic->tabl[y-1][x]) + contrasteVoisin(pic->tabl[y][x],pic->tabl[y-1][x+1]) + contrasteVoisin(pic->tabl[y][x],pic->tabl[y][x+1]);
  }//COEFFICIENTS ???

  if(x==pic->nx-1 && y==pic->ny-1){
    return contrasteVoisin(pic->tabl[y][x],pic->tabl[y-1][x]) + contrasteVoisin(pic->tabl[y][x],pic->tabl[y-1][x-1]) + contrasteVoisin(pic->tabl[y][x],pic->tabl[y][x-1]);
  }//COEFFICIENTS ???

  if(y==0){
    return contrasteVoisin(pic->tabl[y][x],pic->tabl[y][x-1]) + contrasteVoisin(pic->tabl[y][x],pic->tabl[y][x+1]) + contrasteVoisin(pic->tabl[y][x],pic->tabl[y+1][x-1]) + contrasteVoisin(pic->tabl[y][x],pic->tabl[y+1][x]) + contrasteVoisin(pic->tabl[y][x],pic->tabl[y+1][x+1]);
  }

  if(x==pic->nx-1){
    return contrasteVoisin(pic->tabl[y][x],pic->tabl[y-1][x]) + contrasteVoisin(pic->tabl[y][x],pic->tabl[y-1][x-1]) + contrasteVoisin(pic->tabl[y][x],pic->tabl[y][x-1]) + contrasteVoisin(pic->tabl[y][x],pic->tabl[y+1][x-1]) + contrasteVoisin(pic->tabl[y][x],pic->tabl[y+1][x]);
  }

  if(y==pic->ny-1){
    return contrasteVoisin(pic->tabl[y][x],pic->tabl[y][x-1]) + contrasteVoisin(pic->tabl[y][x],pic->tabl[y-1][x-1]) + contrasteVoisin(pic->tabl[y][x],pic->tabl[y-1][x]) + contrasteVoisin(pic->tabl[y][x],pic->tabl[y-1][x+1]) + contrasteVoisin(pic->tabl[y][x],pic->tabl[y][x+1]);
  }

  if(x==0){
    return contrasteVoisin(pic->tabl[y][x],pic->tabl[y-1][x]) + contrasteVoisin(pic->tabl[y][x],pic->tabl[y-1][x+1]) + contrasteVoisin(pic->tabl[y][x],pic->tabl[y][x+1]) + contrasteVoisin(pic->tabl[y][x],pic->tabl[y+1][x+1]) + contrasteVoisin(pic->tabl[y][x],pic->tabl[y+1][x]);
  }
  
  return contrasteVoisin(pic->tabl[y][x],pic->tabl[y-1][x-1]) + contrasteVoisin(pic->tabl[y][x],pic->tabl[y-1][x]) + contrasteVoisin(pic->tabl[y][x],pic->tabl[y-1][x+1]) + contrasteVoisin(pic->tabl[y][x],pic->tabl[y][x-1]) + contrasteVoisin(pic->tabl[y][x],pic->tabl[y][x+1]) + contrasteVoisin(pic->tabl[y][x],pic->tabl[y+1][x-1]) + contrasteVoisin(pic->tabl[y][x],pic->tabl[y+1][x]) + contrasteVoisin(pic->tabl[y][x],pic->tabl[y+1][x+1]);

}

void imageContrast(image * pic, int delta){
  //A partir dune image 1 : creation d une image 2 contenant
  //des pixels blancs relatifs aux pixels a fort
  //contrastes de l image 1. Les autres pixels sont noirs

  int ix, iy;
  FILE * fd;
  fd=ouvrirEcrire("Contrast.512.ppm");
  fprintf(fd,"P%d\n%d %d\n%d\n",pic->entete,pic->nx,pic->ny,pic->valmax);

  for(iy=0;iy<pic->ny;iy++){
    for(ix=0;ix<pic->nx;ix++){
      /*
	if(contraste(pic,ix,iy)>delta){
	fprintf(fd,"%c%c%c",(unsigned char)255,(unsigned char)255,(unsigned char)255);
	}
	else{
	fprintf(fd,"%c%c%c",(unsigned char)0,(unsigned char)0,(unsigned char)0);
	}*/
      if(contraste(pic,ix,iy)>delta){
	fprintf(fd,"%c%c%c",(unsigned char)0,(unsigned char)0,(unsigned char)0);
      }
      else{
	fprintf(fd,"%c%c%c",(unsigned char)255,(unsigned char)255,(unsigned char)255);
      }
    }
  }
  fermer(fd);
}


int moyenneRGB(image * pic, int x, int y){
  //Return la moyenne des valeurs "RGB" d un pixel.

  int val=0;
  val+=pic->tabl[y][x].r;
  val+=pic->tabl[y][x].g;
  val+=pic->tabl[y][x].b;
  return val/3;
}
    

void gris(image * pic){
  //Construit une image en nuances de gris a partir
  //des donnees contenus dans "pic".

  int gris;
  int ix, iy;
  FILE * fd;
  fd=ouvrirEcrire("gris.ppm");
  fprintf(fd,"P%d\n%d %d\n%d\n",pic->entete,pic->nx,pic->ny,pic->valmax);

  for(iy=0;iy<pic->ny;iy++){
    for(ix=0;ix<pic->nx;ix++){
      /*
	if(contraste(pic,ix,iy)>delta){
	fprintf(fd,"%c%c%c",(unsigned char)255,(unsigned char)255,(unsigned char)255);
	}
	else{
	fprintf(fd,"%c%c%c",(unsigned char)0,(unsigned char)0,(unsigned char)0);
	}*/
      gris = moyenneRGB(pic,ix,iy);
      fprintf(fd,"%c%c%c",(unsigned char)gris,(unsigned char)gris,(unsigned char)gris);
    }
  }
  fermer(fd);
}

int isBlack(image * pic, int x, int y){
  //Return "1" si le pixel de coordonnees (x,y) contenu
  //dans "pic" est noir, sinon return "0".

  if(pic->tabl[y][x].r==0 && pic->tabl[y][x].g==0 && pic->tabl[y][x].b==0){
    return 1;
  }
  return 0;
}

int isWhite(image * pic, int x, int y){
  //Return "1" si le pixel de coordonnees (x,y) contenu
  //dans "pic" est blanc, sinon return "0".

  if(pic->tabl[y][x].r==255 && pic->tabl[y][x].g==255 && pic->tabl[y][x].b==255){
    return 1;
  }
  return 0;
}


int isBelow(image * pic, int x, int y, int epsi){
   //Return "1" si la moyenne des valeurs "RGB" du pixel de coordonnees (x,y)
  //est en dessous de "epsi", sinon return "0".

  float val;
  val = pic->tabl[y][x].r + pic->tabl[y][x].g + pic->tabl[y][x].b;
  val /= 3;

  if(val < epsi){
    return 1;
  }
  return 0;
}

int isAbove(image * pic, int x, int y, int epsi){
  //Return "1" si la moyenne des valeurs "RGB" du pixel de coordonnees (x,y)
  //est au dessus de "epsi", sinon return "0".

  float val;
  val = pic->tabl[y][x].r + pic->tabl[y][x].g + pic->tabl[y][x].b;
  val /= 3;

  if(val > epsi){
    return 1;
  }
  return 0;
}



rect delimit(image * pic){
  //Delimite le plus petit rectangle contenant une forme blanche
  //sur fond noir a partir des donnees de "pic".
  
  rect rectangle;

  //Coordonnees que l on veut trouver:
  int x1=-1, x2=-1, y1=-1, y2=-1;
  //iterateurs:
  int i,j;
  //
  int a;
  int b;
  a=0;
  b=0;


  //On parcours les colonnes de l image de haut en bas.
  //Au premier pixel noir qu on trouve on connait x1.
  for(i=0;i<pic->nx;i++){ // X
    for(j=0;j<pic->ny;j++){ // Y
      if(isWhite(pic,i,j)){
	  x1=i;
	  b=1;
	  break;
	}
    }
    if(b==1){
      break;
    }
  }
  
  b=0;
  //On parcours les colonnes a droite de x1 de haut en bas.
  //A la premiere colonne entierement blanche on connait x2.
  for(i=x1+1;i<pic->nx;i++){ // Y
    a=0;
    for(j=0;j<pic->ny;j++){ // X
      if(isWhite(pic,i,j)){
	a=1;
      }
      if(a==0 && j==(pic->ny)-1){
	x2=i-1;
	b=1;
	break;
      }
    }
    if(b==1){
      break;
    }
  }

  b=0;
  //On parcours les lignes (de gauche a droite) dans lintervalle [x1;x2] pour trouver y1.
  //Au premier pixel noir quon trouve on connait y1;
  for(j=0;j<pic->ny;j++){
    for(i=x1;i<=x2;i++){
      if(isWhite(pic,i,j)){
	y1=j;
	b=1;
	break;
      }
    }
    if(b==1){
      break;
    }
  }

  b=0;
  //On parcours les lignes en dessous de y1 dans lintervalle [x1;x2] pour trouver y2.
  //A la premiere ligne entierement blanche on connait y2.
  for(j=y1+1;j<pic->ny;j++){
    a=0;
    for(i=x1;i<=x2;i++){
      if(isWhite(pic,i,j)){
	a=1;
      }
      if(a==0 && i==x2){
	y2=j-1;
	b=1;
	break;
      }
    }
    if(b==1){
      break;
    }
  }
  
  rectangle.x1=x1;
  rectangle.x2=x2;
  rectangle.y1=y1;
  rectangle.y2=y2;

  return rectangle;
}

image * loadChar(image * pic, image * c, rect rectangle){
  //Remplit une nouvelle structure "c" a partir des donnees
  //de "pic" contenues dans un certains rectangle.

  //Iterateurs :
  //i1, j1 pour la sous-image.
  //i2, j2 pour l image de base.
  int i1, i2, j1, j2;
  
  c->nx=(rectangle.x2-rectangle.x1)+1;
  c->ny=(rectangle.y2-rectangle.y1)+1;
  c->entete=6;
  c->valmax=255;

  allocImage(c);

  for(j1=0,j2=rectangle.y1;j1<c->ny;j1++,j2++){
    for(i1=0,i2=rectangle.x1;i1<c->nx;i1++,i2++){
      
      c->tabl[j1][i1].r=pic->tabl[j2][i2].r;
      c->tabl[j1][i1].g=pic->tabl[j2][i2].g;
      c->tabl[j1][i1].b=pic->tabl[j2][i2].b;
    }
  }
  return c;
}

image * initWhite(image * pic){
  //Initialise tout les pixels de "pic" en blanc.

  int i, j;
  for(j=0;j<pic->ny;j++){
    for(i=0;i<pic->nx;i++){
      pic->tabl[j][i].r=255;
      pic->tabl[j][i].g=255;
      pic->tabl[j][i].b=255;
    }
  }
  return pic;
}

image * initBlack(image * pic){
  //Initialise tout les pixels de "pic" en noir.

  int i, j;
  for(j=0;j<pic->ny;j++){
    for(i=0;i<pic->nx;i++){
      pic->tabl[j][i].r=0;
      pic->tabl[j][i].g=0;
      pic->tabl[j][i].b=0;
    }
  }
  return pic;
}
   


image * resize(image * pic, image * new){
  //Creation des donnees d une nouvelle image 28*28 pixels
  //a partir des donnees d une autre.

  new->ny=28;
  new->nx=28;
  new->entete=6;
  new->valmax=255;
  allocImage(new);
  new=initBlack(new);

  //Iterateurs:
  int k, h, i, j;
  //Nombre de pixel en hauteur du nouveau caractere:
  //int newheight = 24;
  //Nombre de pixel de l image de base pour un pixel du nouveau caractere:
  int tilesize;
  //Nombre de pixel en largeur du nouveau caractere:
  int newwidth;
  //Moyenne d intensite pour un bloc de pixels de base (donc valeur du pixel du nouveau caractere)
  int mean;
  int sideborder;

  
  tilesize = pic->ny / 24;
  newwidth = (pic->nx * 24)/pic->ny;
  sideborder = (28-newwidth)/2;
  
  for(k=0;k<pic->ny-tilesize;k+=tilesize){
    for(h=0;h<pic->nx-tilesize;h+=tilesize){
      mean=0;
      for(j=0;j<tilesize;j++){
	for(i=0;i<tilesize;i++){
	  mean+=pic->tabl[k+j][h+i].r;
	}
      }
      mean/=(tilesize*tilesize);
      new->tabl[k/tilesize+2][h/tilesize+sideborder].r=mean;
      new->tabl[k/tilesize+2][h/tilesize+sideborder].g=mean;
      new->tabl[k/tilesize+2][h/tilesize+sideborder].b=mean;
    }
  }
  return new;
}

