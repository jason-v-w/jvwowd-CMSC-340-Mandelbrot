#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <unistd.h>

#define PIX_WIDTH 3840
#define PIX_HEIGHT 2160

// initialize globals
double min_x, max_x, min_y, max_y, dist_x;
int pix_width, pix_height;


/* Main method */
int main(int argc, char **argv) {
	// for non-interactive run need 4 arguments: program name and 3 parameters
	if (argc != 4) {
		// if the only paramter is the program name, then run interactively
		if (argc == 1) {
			// assign globals
			printf("\nWhat is the minimum x coordinate?\n>> ");
			scanf("%f", &min_x);
			printf("What is the minimum y coordinate?\n>> ");
			scanf("%f", &min_y);
			printf("What is the x distance covered?\n>> ");
			scanf("%f", &dist_x);
		} else {
			printf("Invalid number of arguments (%d).\
					Exiting with status 1.\n", argc);
			exit(1);
		}
	} else {
		// assign globals
		min_x = atof(argv[1]);
		min_y = atof(argv[2]);
		dist_x = atof(argv[3]);
	}

	


	max_x = min_x + dist_x;
	max_y = min_y + (dist_x*PIX_HEIGHT/PIX_WIDTH);

	
	char *max_x_str = (char*)malloc(30);
	char *max_y_str = (char*)malloc(30);
	char *pix_x_str = (char*)malloc(30);
	char *pix_y_str = (char*)malloc(30);
	sprintf(max_x_str, "%f", max_x);
	sprintf(max_y_str, "%f", max_y);
	sprintf(pix_x_str, "%d", PIX_WIDTH);
	sprintf(pix_y_str, "%d", PIX_HEIGHT);

	
	execl("./main.o", 
	      argv[0],
	      argv[1],
	      argv[2],
	      max_x_str,
	      max_y_str,
	      pix_x_str,
	      pix_y_str,
	      (char*)NULL);

	
    return 0;
}




