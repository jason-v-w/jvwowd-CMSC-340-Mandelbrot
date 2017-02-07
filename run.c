#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <pthread.h>
#include <unistd.h>

#define PIXEL_SIZE 12
#define CHANNEL_SIZE 4
#define MAX_ITER 1000
#define NUM_THREADS 40


// define a struct used to hold complex numbers
typedef struct complex {
   double real;
   double imag;
} complex;


// define headers
void *thread_run();
void compute_pixel(int x, int y);
int compute_iterations(complex c);
complex f(complex z, complex c);
void set_color(int x, int y, int iterations);
complex pixel_to_complex(int pix_x, int pix_y);
void init_storage(void);
char *strcpy_no_nul(char *dest, const char *src);
void write_data_s(const int x, const int y, char *rgb);
void write_data_d(const int x, const int y, int r, int g, int b);
void save_file();
void display_status(double completed, double total, int length, int flush);


// initialize globals
double min_x, max_x, min_y, max_y;
int pix_width, pix_height;
void *storage;
pthread_mutex_t mutex;
int current_row = 0;


/* Main method */
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

	// initialize mutex
	pthread_mutex_init(&mutex, NULL);

	// image data will be stored directly into memory
	// each pixel will have 3 channels (RGB) each 
	// requiring 4 ascii characters (000-255) and a space/newline
	// so we need 12 bytes/characters for each pixel plus 1 extra
	// byte to place a \0 null character to terminate the string
	// char is 1 byte in size
	// storage is a void* pointer to the memory block
	storage = malloc(PIXEL_SIZE*pix_width*pix_height + 1);

	// initialize the storage space to have necessary tabs, newlines,
	// spaces, and null character
	init_storage();

	// initialize all threads
	pthread_t threads[NUM_THREADS];
	for (int t=0; t<NUM_THREADS; ++t) {
		pthread_create(&(threads[t]), NULL, thread_run, NULL);
	}

	// display status bar
	int status_length = 50;
	printf("\nComputing image...\n");
	while (current_row < pix_height) {
		display_status(current_row, pix_height, status_length, 0);
		usleep(10000);
	}
	display_status(current_row, pix_height, status_length, 1);

    // block on thread completion
	for (int t=0; t<NUM_THREADS; ++t) {
		pthread_join(threads[t], NULL);
	}

	// save file
	printf("Saving image...\n");
	save_file();
	display_status(1, 1, status_length, 1);

	// free the memory that was allocated
	free(storage);
	printf("Finished.\n\n");
	
    return 0;
}




