#include "filtrado.h"
#include <cstring>
#include <thread>
#include <mutex>
#include <vector>
#include <functional> // std::ref
#include <iostream>
// void aplicaFiltroBilinear(float* data, int numChannels, int w, int h, float* &dataOut){
//
//
//     float bilFilter[9]={1.0f/9.0f,1.0f/9.0f,1.0f/9.0f,
//                     1.0f/9.0f,1.0f/9.0f,1.0f/9.0f,
//                     1.0f/9.0f,1.0f/9.0f,1.0f/9.0f};
//
//     if ( numChannels == 3 ){
//       rgbColor_t* pixels= (rgbColor_t*) data;
//       rgbColor_t* pixelsOut= new rgbColor_t[w*h];
//       memset(pixelsOut,0,sizeof(rgbColor_t)*w*h);
//
//       for(int i = 1; i<(h-1);i++)
//       for (int j = 1; j<(w-1);j++){
//
//         rgbColor_t pixelAux;
//         pixelAux.r=pixelAux.g=pixelAux.b=0;
//         // La operacion start
//         for(int yf = -1; yf<2;yf++)
//         for (int xf = -1; xf<2;xf++){
//           pixelAux.r += pixels[(i+yf)*w+(j+xf)].r*bilFilter[(yf+1)*3+(xf+1)];
//           pixelAux.g += pixels[(i+yf)*w+(j+xf)].g*bilFilter[(yf+1)*3+(xf+1)];
//           pixelAux.b += pixels[(i+yf)*w+(j+xf)].b*bilFilter[(yf+1)*3+(xf+1)];
//         }
//         // La operacion fin de pizel i*w+j
//         pixelsOut[i*w+j] = pixelAux;
//       }
//
//       dataOut=(float*)pixelsOut;
//     }else if ( numChannels == 4 ){
//       rgbaColorf_t* pixels= (rgbaColorf_t*) data;
//       rgbaColorf_t* pixelsOut= new rgbaColorf_t[w*h];
//       memset(pixelsOut,0,sizeof(rgbaColorf_t)*w*h);
//
//       for(int i = 1; i<(h-1);i++)
//       for (int j = 1; j<(w-1);j++){
//
//         rgbaColorf_t pixelAux;
//         pixelAux.r=pixelAux.g=pixelAux.b=pixelAux.a=0;
//
//         for(int yf = -1; yf<2;yf++)
//         for (int xf = -1; xf<2;xf++){
//           pixelAux.r += pixels[(i+yf)*w+(j+xf)].r*bilFilter[(yf+1)*3+(xf+1)];
//           pixelAux.g += pixels[(i+yf)*w+(j+xf)].g*bilFilter[(yf+1)*3+(xf+1)];
//           pixelAux.b += pixels[(i+yf)*w+(j+xf)].b*bilFilter[(yf+1)*3+(xf+1)];
//           pixelAux.a += pixels[(i+yf)*w+(j+xf)].a*bilFilter[(yf+1)*3+(xf+1)];
//         }
//         pixelsOut[i*w+j] = pixelAux;
//       }
//       dataOut=(float*)pixelsOut;
//     }
//   }
  std::mutex cerrojo;

  void aplicaFiltroBilinear(float* data, int numChannels, int w, int h, float* &dataOut, int numThreads){
    std::vector<std::thread*> threads;
    // doing the division
    int salto = ((w)*(h))/numThreads;

    if (numChannels == 3){
      dataOut = new float[w*h*3];
      memset(dataOut,0,sizeof(rgbColor_t)*w*h);
    }else{
      dataOut = new float[w*h*4];
      memset(dataOut,0,sizeof(rgbaColorf_t)*w*h);
    }


    for (int i = 0; i < numThreads; i++) {
      // todo sumar el resto al ultimo thread

      int posInicio = (1+(i*salto));
      int posFinal = ((i+1)*salto);

      std::cout << posInicio << " : " << posFinal << '\n';
      if (numChannels == 3){
        threads.push_back(new std::thread(aplicaFiltroBilinearPartesChan3,data,w,h,posInicio,posFinal,&dataOut));
      }else{
        threads.push_back(new std::thread(aplicaFiltroBilinearPartesChan4,data,w,h,posInicio,posFinal,&dataOut));
      }
    }

    for(auto it=threads.begin(); it != threads.end(); it++){
        (*it)->join();
    }
  }

  void aplicaFiltroBilinearPartesChan3(float* data,int w,int h, int posInicio, int posFinal, float** dataOut){


    float bilFilter[9]={1.0f/9.0f,1.0f/9.0f,1.0f/9.0f,
                    1.0f/9.0f,1.0f/9.0f,1.0f/9.0f,
                    1.0f/9.0f,1.0f/9.0f,1.0f/9.0f};

    rgbColor_t* pixels= (rgbColor_t*) data;

    for (int pos = posInicio; pos <= posFinal; pos++) {
      int x = ((pos) % w);
      int y = ((pos - x)/w);

      if ( x > 0 && y > 0 && x < w-1 && y < h-1 ){

        //std::cout << "x=" << x << " y=" << y << '\n';

        rgbColor_t pixelAux;
        pixelAux.r=pixelAux.g=pixelAux.b=0;
        // La operacion start
        for(int yf = -1; yf<2;yf++)
        for (int xf = -1; xf<2;xf++){
          pixelAux.r += pixels[(y+yf)*w+(x+xf)].r*bilFilter[(yf+1)*3+(xf+1)];
          pixelAux.g += pixels[(y+yf)*w+(x+xf)].g*bilFilter[(yf+1)*3+(xf+1)];
          pixelAux.b += pixels[(y+yf)*w+(x+xf)].b*bilFilter[(yf+1)*3+(xf+1)];

        }
          // La operacion fin de pixel
          cerrojo.lock();
          ((rgbColor_t*)dataOut[0])[y*w+x] = pixelAux;
          cerrojo.unlock();
      }
    }

  }

    void aplicaFiltroBilinearPartesChan4(float* data,int w,int h, int posInicio, int posFinal, float** dataOut){


      float bilFilter[9]={1.0f/9.0f,1.0f/9.0f,1.0f/9.0f,
                      1.0f/9.0f,1.0f/9.0f,1.0f/9.0f,
                      1.0f/9.0f,1.0f/9.0f,1.0f/9.0f};

      rgbaColorf_t* pixels= (rgbaColorf_t*) data;


      for (int pos = posInicio; pos <= posFinal; pos++) {
        int x = (pos % w);
        int y = ((pos - x)/w);

        if ( x > 0 && y > 0 && x < w-1 && y < h-1 ){

          rgbaColorf_t pixelAux;
          pixelAux.r=pixelAux.g=pixelAux.b=pixelAux.a=0;
          // La operacion start
          for(int yf = -1; yf<2;yf++)
          for (int xf = -1; xf<2;xf++){
            pixelAux.r += pixels[(y+yf)*w+(x+xf)].r*bilFilter[(yf+1)*3+(xf+1)];
            pixelAux.g += pixels[(y+yf)*w+(x+xf)].g*bilFilter[(yf+1)*3+(xf+1)];
            pixelAux.b += pixels[(y+yf)*w+(x+xf)].b*bilFilter[(yf+1)*3+(xf+1)];
            pixelAux.a += pixels[(y+yf)*w+(x+xf)].a*bilFilter[(yf+1)*3+(xf+1)];
          }
            // La operacion fin de pixel
            cerrojo.lock();
            ((rgbaColorf_t*)dataOut[0])[pos] = pixelAux;
            cerrojo.unlock();
        }
      }

    }


void charToFloat(unsigned char* data, rgbaByteColor_t* palette, int w, int h, float* &dataOut){
  if (palette == nullptr){
    dataOut = new float[w*h*3];
    for(int i = 0; i<w*h*3;i++)            pixelAux.b += pixels[(y+yf)*w+(x+xf)].b*bilFilter[(yf+1)*3+(xf+1)];

      dataOut[i]=((float)data[i])/256.0f;
  }else{
    dataOut = new float[w*h*4];
    for(int i = 0; i<w*h*4;i++){
      dataOut[i*4]=((float)palette[data[i]].r)/256.0f;
      dataOut[i*4+1]=((float)palette[data[i]].g)/256.0f;
      dataOut[i*4+2]=((float)palette[data[i]].b)/256.0f;
      dataOut[i*4+3]=((float)palette[data[i]].a)/256.0f;
    }
  }

}

void floatToChar(float* data, int w, int h, int numChannels, unsigned char* &dataOut){
  dataOut=new unsigned char[w*h*3];
  //por defecto guardamos a rgb
  int dataOutCount=0;
  for(int i = 0;i<w*h*numChannels;i+=numChannels){
    dataOut[dataOutCount++]=(unsigned char)(255.0f*data[i]);
    dataOut[dataOutCount++]=(unsigned char)(255.0f*data[i+1]);
    dataOut[dataOutCount++]=(unsigned char)(255.0f*data[i+2]);
  }
}
