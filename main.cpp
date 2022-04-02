#include "libreriabmp/bmp.h"
#include "filtrolineal/filtrado.h"
#include <iostream>

int main(int argc, char** argv){
  fileBMP_t* bmp = loadBMP("FLAG_B24.BMP");

  std::cout << "Width: " << bmp->attributes.width << "\n";
  std::cout << "Height: " << bmp->attributes.height << "\n";

  writeBMP(bmp,"imagen22.bmp");

  fileBMP_t* bmp2 = createBMP(bmp->attributes.width,bmp->attributes.height,bmp->attributes.bpp);
  bmp2->header=bmp->header;
  bmp2->attributes=bmp->attributes;

  float *data;
  float *data2;
  charToFloat(bmp->data, nullptr,bmp->attributes.width,bmp->attributes.height,data);

  aplicaFiltroBilinear(data, 3, bmp->attributes.width, bmp->attributes.height, data2,1);

  floatToChar(data2,bmp2->attributes.width,bmp2->attributes.height,3,bmp2->data);

  writeBMP(bmp2,"imagen33.bmp");

  return 0;
}
