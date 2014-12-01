#ifndef __TEXTURE__
#define __TEXTURE__

unsigned char* bmp_data(const char * imagepath);
GLuint load_bmp(const char * imagepath, int width, int height);
GLuint loadDDS(const char * imagepath);

#endif
