#include <stdio.h>
#include <stdlib.h>

#define PIXEL_SIZE 12
#define CHANNEL_SIZE 4

// define headers
void init_storage(void);
void basic_compute(void);

// initialize globals
double min_x, max_x, min_y, max_y;
int pix_width, pix_height;
void *storage;

typedef struct {
   double real;
   double imag;
} complex;

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

	basic_compute();

//	*(char*)(storage) = "255 255 000";
	(char*)(storage) = '6';
	printf("%c", storage);
	
    // spawn thread
	
    // block on thread completion
	
    // get saved string
	
    // write string to file
	
    // open file

	// free the memory that was allocated
	free(storage);
    return 0;
}

int compute_iterations(double real, double imag, int max_interations) {
    // TODO
}

void set_color(int iterations) {
    // TODO
	//set the correct location in memory with the necessary
	//color based on the iterations
}

complex pixel_to_complex(int pix_x, int pix_y) {
    // TODO
    // return the complex number associated with the pixel
}


/*
 * This function will add spaces, newlines, and null
 * characters in the appropriate locations withint
 * the storage memory block
 */
void init_storage(void) {
	int row_size = pix_width*PIXEL_SIZE;

	*(char*)(storage+pix_width*pix_height) = '\0';

	for (int loc=1; loc<=pix_width*pix_height; ++loc) {
		*(char*)(storage+loc*CHANNEL_SIZE - 1) = ' ';
	}

	for (int row=1; row<=pix_height; ++row) {
		*(char*)(storage+row*row_size - 1) = '\n';
	}
}

void basic_compute(void) {
	int r = 255;
	int g = 255;
	int b = 0;

	int row_size = pix_width*PIXEL_SIZE;
	
	for (int x=0; x<pix_width; ++x) {
		for (int y=0; y<pix_height; ++y) {
			*(char**)(storage + x*PIXEL_SIZE + y*row_size) = "255 255 000";
		}   
	}
}





