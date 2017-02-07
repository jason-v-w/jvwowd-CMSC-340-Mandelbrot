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
	// need 4 arguments: program name and 3 parameters
/*	if (argc != 4) {
		printf("Invalid number of arguments (%d). Exiting with status 1.\n", 
		       argc);
		exit(1);
	}*/

	// assign globals
    min_x = atof(argv[1]);
    min_y = atof(argv[2]);
    //max_x = atof(argv[3]);
    //max_y = atof(argv[4]);
    //pix_width = atoi(argv[5]);
    //pix_height = atoi(argv[6]);

	
    dist_x = atof(argv[3]);
	max_x = min_x + dist_x;
	max_y = min_y + (dist_x*PIX_HEIGHT/PIX_WIDTH);

	
/*	printf("min_x: %f\n", min_x);
	printf("min_y: %f\n", min_y);
	printf("dist_x: %f\n", dist_x);
	printf("max_x: %f\n", max_x);
	printf("max_y: %f\n", max_y);*/

	char *max_x_str = (char*)malloc(30);
	char *max_y_str = (char*)malloc(30);
	char *pix_x_str = (char*)malloc(30);
	char *pix_y_str = (char*)malloc(30);
	sprintf(max_x_str, "%f", max_x);
	sprintf(max_y_str, "%f", max_y);
	sprintf(pix_x_str, "%d", PIX_WIDTH);
	sprintf(pix_y_str, "%d", PIX_HEIGHT);
	

/*	execl("./main.o", argv[0], 
	      min_x, 
	      min_y, 
	      max_x, 
	      max_y, 
	      pix_width, 
	      pix_height,
	      (char*)NULL);*/

	printf("Arguments:\n");
	printf("%s\n",argv[0]);
	printf("%s\n",argv[1]);
	printf("%s\n",argv[2]);
	printf("%s\n",max_x_str);
	printf("%s\n",max_y_str);
	printf("%s\n",pix_x_str);
	printf("%s\n",pix_y_str);
	execl("./main.o", 
	      argv[0],
	      argv[1],
	      argv[2],
	      max_x_str,
	      max_y_str,
	      pix_x_str,
	      pix_y_str,
	      (char*)NULL);


/*	printf("Arguments:\n");
	printf("%s\n",argv[0]);
	printf("%s\n",argv[1]);
	printf("%s\n",argv[2]);
	printf("%s\n",argv[3]);
	printf("%s\n",argv[4]);
	printf("%s\n",argv[5]);
	printf("%s\n",argv[6]);
	execl("./main.o", 
	      argv[0],
	      argv[1],
	      argv[2],
	      argv[3],
	      argv[4],
	      argv[5],
	      argv[6],
	      (char*)NULL);*/

	//execv("./main.o", argv);
	
	
    return 0;
}




