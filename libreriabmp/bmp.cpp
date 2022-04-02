#include "bmp.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h> // para el memset

fileBMP_t* loadBMP(const char* fileName){
  fileBMP_t* bmp = new fileBMP_t;
  memset(bmp,0,sizeof(fileBMP_t));
  // read cabecera1
  FILE* f = fopen(fileName, "rb");
  if (f==nullptr){
    // error
    printf("no existe el fuchero");
    free(bmp);
    return nullptr;
  }
  fread(&bmp->header,sizeof(bmp_header_t),1,f);
  // read tipo fichero
  int headerSize=0;
  fread(&headerSize,sizeof(int),1,f);
  if(headerSize != 40){
    // si no es 40bytes error ( diferencia para solo coger versiones modernas)
    printf("no es el formato correcto");
    free(bmp);
    fclose(f);
    return nullptr;
  }
  //read cabecera 2
  fseek(f,-4,SEEK_CUR);

  fread(&bmp->attributes,sizeof(dibHeader_t),1,f);

  if (bmp->attributes.bpp <=8) {
    // si paleta leer paleta
    bmp->palette = new rgbaColor_t[bmp->attributes.numColorsPalette];
    fread(bmp->palette, sizeof(rgbaColor_t),bmp->attributes.numColorsPalette, f);
  }

  //leer datos
  unsigned int tamanioDeImagen = ((bmp->attributes.height) * (bmp->attributes.width) * (bmp->attributes.bpp) )/ 8; // arreglar que se rompe con imagenes de un pixel
  bmp->data = new unsigned char[tamanioDeImagen];
  fread(bmp->data,1,tamanioDeImagen,f);
  fclose(f);

  return bmp;
}

void freeBMP(fileBMP_t* bmp){
  if(bmp->palette)
    delete[] bmp->palette;
  if(bmp->data)
    delete[] bmp->data;
  delete bmp;
}


int writeBMP(fileBMP_t* bmp, const char* fileName){
  // write cabecera1
  FILE* f = fopen(fileName, "wb");
  if (f==nullptr){
    // error
    printf("no existe el fuchero");
    return -1;
  }
  fwrite(&bmp->header,sizeof(bmp_header_t),1,f);
  // write cabecera 2
  fwrite(&bmp->attributes,sizeof(dibHeader_t),1,f);

  if (bmp->attributes.bpp <=8) {
    // si paleta leer paleta
    bmp->palette = new rgbaColor_t[bmp->attributes.numColorsPalette];
    fwrite(bmp->palette, sizeof(rgbaColor_t),bmp->attributes.numColorsPalette, f);
  }

  //write datos
  unsigned int tamanioDeImagen = ((bmp->attributes.height) * (bmp->attributes.width) * (bmp->attributes.bpp) )/ 8; // arreglar que se rompe con imagenes de un pixel
  fwrite(bmp->data,1,tamanioDeImagen,f);
  fclose(f);

  return 1;
}

fileBMP_t* createBMP(int w, int h, int bpp){
  fileBMP_t* bmp = new fileBMP_t;
  memset(bmp, 0, sizeof(fileBMP_t));
  bmp->attributes.width = w;
  bmp->attributes.height = h;
  bmp->attributes.bpp = bpp;

  return bmp;
}
