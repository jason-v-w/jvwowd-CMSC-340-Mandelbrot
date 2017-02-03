#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define PIXEL_SIZE 12
#define CHANNEL_SIZE 4
#define MAX_ITER 100


typedef struct complex {
   double real;
   double imag;
} complex;


// define headers
int compute_iterations(complex c);
complex f(complex z, complex c);
void set_color(int x, int y, int iterations);
complex pixel_to_complex(int pix_x, int pix_y);
void init_storage(void);
char *strcpy_no_nul(char *dest, const char *src);
void write_data_s(const int x, const int y, char *rgb);
void write_data_d(const int x, const int y, int r, int g, int b);
void save_file(char *name);


// initialize globals
double min_x, max_x, min_y, max_y;
int pix_width, pix_height;
void *storage;


int main(int argc, char **argv) {

	// need 7 arguments: program name and 6 parameters
	if (argc != 7) {
		printf("Invalid number of arguments. Exiting with status 1.\n");
		exit(1);
	}

	// assign globals
    min_x = atof(argv[1]);
    min_y = atof(argv[2]);
    max_x = atof(argv[3]);
    max_y = atof(argv[4]);
    pix_width = atoi(argv[5]);
    pix_height = atoi(argv[6]);

	// image data will be stored directly into memory
	// each pixel will have 3 channels (RGB) each 
	// requiring 4 ascii characters (000-255) and a space/newline
	// so we need 12 bytes/characters for each pixel plus 1 extra
	// byte to place a \0 null character to terminate the string
	// char is 1 byte in size
	// storage is a void* pointer to the memory block
	storage = malloc(PIXEL_SIZE*pix_width*pix_height + 1);
	
	init_storage();

	for (int x=0; x<pix_width; ++x) {
		for (int y=0; y<pix_height; ++y) {
			complex c = pixel_to_complex(x,y);
			int iter = compute_iterations(c);
			//printf("x: %d\ny: %d\ncomplex: %f+%fi\niterations: %d\n",
			//     x, y, c.real, c.imag, iter);  
			set_color(x, y, iter);
		}   
	}

//	basic_compute();
	
//	printf("STORAGE:\n%s", storage);
//	complex c = {.real=-1, .imag=0.5};
//	printf("%d\n", compute_iterations(c));
	
    // spawn thread
	
    // block on thread completion
	
	save_file("mandelbrot.pbm");

	// free the memory that was allocated
	free(storage);
	
    return 0;
}

int compute_iterations(complex c) {
	complex z = {0,0};
	
	int iter = 0;
	
	while (iter < MAX_ITER && z.real*z.real + z.imag*z.imag < 4) {
		z = f(z, c);
		++iter;
	}

	return iter;
}

complex f(complex z, complex c) {
	complex res;
	res.real = z.real*z.real - z.imag*z.imag + c.real;
	res.imag = 2*z.real*z.imag + c.imag;
	return res;
}

void set_color(int x, int y, int iterations) {
	int r, g, b;
	if (iterations == MAX_ITER) {
		r = 0;
		g = 0;
		b = 0;
	} else {
		int base = (int)(255*sqrt(1-pow((((double)iterations)-MAX_ITER)/MAX_ITER,2)));
		r = base;
		g = base;
		b = 255;
	}
    int gray = 255 - 255.0/MAX_ITER*iterations;
	
	write_data_d(x,y,r,g,b);
}

complex pixel_to_complex(int pix_x, int pix_y) {
    // TODO check this function for correctness

	double num_width = max_x - min_x;
	double num_height = max_y - min_y;
	complex c;

	c.real = min_x + num_width/pix_width*(pix_x+0.5);
	c.imag = max_y - num_height/pix_height*(pix_y+0.5);
	return c;
}


/*
 * This function will add spaces, newlines, and null
 * characters in the appropriate locations withint
 * the storage memory block
 */
void init_storage(void) {
	int row_size = pix_width*PIXEL_SIZE;

	for (int loc=0; loc<=pix_width*pix_height*PIXEL_SIZE; ++loc) {
		*(char*)(storage + loc) = ' ';
	}
	
	for (int loc=1; loc<=pix_width*pix_height*PIXEL_SIZE/CHANNEL_SIZE; ++loc) {
		*(char*)(storage + loc*CHANNEL_SIZE - 1) = '\t';
	}

	for (int row=1; row<=pix_height; ++row) {
		*(char*)(storage+row*row_size - 1) = '\n';
	}

	*(char*)(storage + pix_width*pix_height*PIXEL_SIZE) = '\0';
}


/*
 * This will copy the string from src to dest, but
 * unlike the standard strcpy, the ending null character
 * will not be copied.
 */
char *strcpy_no_nul(char *dest, const char *src)
{
  unsigned i;
  for (i=0; src[i] != '\0'; ++i)
    dest[i] = src[i];
  return dest;
}


/*
 * This will write the provided rgb color string to storage.
 * It is the callers responsibility to ensure that the string
 * is no more than 11 characters in length and that no rgb value
 * is less than 0 or greater than 255.
 * 
 * Example: write_data(1, 5, "100 008 89")
 */
void write_data_s(const int x, const int y, char *rgb) {
	int row_size = pix_width*PIXEL_SIZE;
	void *addr = storage + x*PIXEL_SIZE + y*row_size;
	strcpy_no_nul(addr, rgb);
}


/*
 * This will write the provided rgb data to storage.
 * It is the callers responsibility to ensure that no rgb value
 * is less than 0 or greater than 255.
 * 
 * Example: write_data(1, 5, 100, 8, 89)
 */
void write_data_d(const int x, const int y, int r, int g, int b) {
	char str[11];
	sprintf(str, "%d %d %d", r,g,b);
	write_data_s(x,y,str);
}


/*
 * This will save the storage to the file with the name provided.
 * The resulting file will be formatted as a netpbm (pbm) file.
 */
void save_file(char *name) {
	FILE *file = fopen(name, "w");
	fprintf(file, "%s\n%d %d\n%d\n%s", 
	        "P3", pix_width, pix_height, 255, storage);
	fclose(file);
}





