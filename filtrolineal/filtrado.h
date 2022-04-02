#pragma

typedef struct rgbaColorf_t{
  float r;
  float g;
  float b;
  float a;
}rgbaColorf_t;

typedef struct rgbColor_t{
  float r;
  float g;
  float b;
}rgbColor_t;

typedef struct rgbaByteColor_t{
  unsigned char r;
  unsigned char g;
  unsigned char b;
  unsigned char a;
}rgbaByteColor_t;

void aplicaFiltroBilinear(float* data, int numChannels, int w, int h, float* &dataOut, int numThreads);
void charToFloat(unsigned char* data, rgbaByteColor_t* palette, int w, int h, float* &dataOut);
void floatToChar(float* data, int w, int h, int numChannels, unsigned char* &dataOut);

void aplicaFiltroBilinearPartesChan3(float* data,int w,int h, int posInicio, int posFinal, float** dataOut);
void aplicaFiltroBilinearPartesChan4(float* data,int w,int h, int posInicio, int posFinal, float** dataOut);
