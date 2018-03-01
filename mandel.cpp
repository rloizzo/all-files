// Ryan Loizzo (rloizzo)
// Nathan Rao (nrao)
// mandel.c updated for threads

#include "bitmap.h"

#include <getopt.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <errno.h>
#include <string.h>
#include <pthread.h>
#include <vector>
using namespace std;

int iteration_to_color( int i, int max );
int iterations_at_point( double x, double y, int max );
void * thread_compute_image(void * pArgs);

typedef struct _thread_args {
	struct bitmap *bm;
	double xmin;
	double xmax;
	double ymin;
	double ymax;
	int max;
	int h_start;
	int h_end; 
} thread_args;

void show_help()
{
	printf("Use: mandel [options]\n");
	printf("Where options are:\n");
	printf("-n threads  The number of threads to use (default=1)\n");
	printf("-m <max>    The maximum number of iterations per point. (default=1000)\n");
	printf("-x <coord>  X coordinate of image center point. (default=0)\n");
	printf("-y <coord>  Y coordinate of image center point. (default=0)\n");
	printf("-s <scale>  Scale of the image in Mandlebrot coordinates. (default=4)\n");
	printf("-W <pixels> Width of the image in pixels. (default=500)\n");
	printf("-H <pixels> Height of the image in pixels. (default=500)\n");
	printf("-o <file>   Set output file. (default=mandel.bmp)\n");
	printf("-h          Show this help text.\n");
	printf("\nSome examples are:\n");
	printf("mandel -x -0.5 -y -0.5 -s 0.2\n");
	printf("mandel -x -.38 -y -.665 -s .05 -m 100\n");
	printf("mandel -x 0.286932 -y 0.014287 -s .0005 -m 1000\n\n");
}

int main( int argc, char *argv[] )
{
	char c;

	// These are the default configuration values used
	// if no command line arguments are given.

	const char *outfile = "mandel.bmp";
	double xcenter = 0;
	double ycenter = 0;
	double scale = 4;
	int    image_width = 500;
	int    image_height = 500;
	int    max = 1000;
	int num_threads = 1;

	// For each command line argument given,
	// override the appropriate configuration value.

	while((c = getopt(argc,argv,"x:y:s:W:H:m:o:h"))!=-1) {
		switch(c) {
			case 'n':
				num_threads = atoi(optarg);
				break;
			case 'x':
				xcenter = atof(optarg);
				break;
			case 'y':
				ycenter = atof(optarg);
				break;
			case 's':
				scale = atof(optarg);
				break;
			case 'W':
				image_width = atoi(optarg);
				break;
			case 'H':
				image_height = atoi(optarg);
				break;
			case 'm':
				max = atoi(optarg);
				break;
			case 'o':
				outfile = optarg;
				break;
			case 'h':
				show_help();
				exit(1);
				break;
		}
	}

	// Display the configuration of the image.
	printf("mandel: x=%lf y=%lf scale=%lf max=%d outfile=%s\n",xcenter,ycenter,scale,max,outfile);

	// Create a bitmap of the appropriate size.
	struct bitmap *bm = bitmap_create(image_width,image_height);

	// Fill it with a dark blue, for debugging
	bitmap_reset(bm,MAKE_RGBA(0,0,255,0));

	//Create threads and split up work
	vector<pthread_t> threads;
	for (int i = 0; i < num_threads; i++) {
		pthread_t p;
		thread_args *pArgs = (thread_args*)malloc(sizeof(thread_args));
		pArgs->bm = bm;
		pArgs->xmin = xcenter-scale;
		pArgs->xmax = xcenter+scale;
		pArgs->ymin = ycenter-scale;
		pArgs->ymax = ycenter-scale;
		pArgs->max = max;
		pArgs->h_start = i * bitmap_height(bm) / num_threads;
		pArgs->h_end = (i+1) * bitmap_height(bm) / num_threads;
		if (pthread_create(&p, NULL, thread_compute_image, (void*)pArgs)) {
			fprintf(stderr, "Error creating thread\n");
		}
		threads.push_back(p);
	}
	
	// wait for all threads to finish
	for (auto p : threads) {
		pthread_join(p, NULL);
	}
	
	for (auto p : threads) {
		free(p);
	}

	// Save the image in the stated file.
	if(!bitmap_save(bm,outfile)) {
		fprintf(stderr,"mandel: couldn't write to %s: %s\n",outfile,strerror(errno));
		return 1;
	}

	return 0;
}

/*
Compute an entire Mandelbrot image, writing each point to the given bitmap.
Scale the image to the range (xmin-xmax,ymin-ymax), limiting iterations to "max"
*/

void * thread_compute_image (void * pArgs) {
	thread_args *a = (thread_args*) pArgs;
	struct bitmap *bm = a->bm;
	double xmin = a->xmin;
	double xmax = a->ymax;
	double ymin = a->ymin;
	double ymax = a->ymax;
	int max = a->max;
	int h_start = a->h_start;
	int h_end = a->h_end;

	int i,j;

	int width = bitmap_width(bm);
	int height = bitmap_height(bm);

	// For every pixel in the image...

	for(j=h_start;j<h_end;j++) {

		for(i=0;i<width;i++) {

			// Determine the point in x,y space for that pixel.
			double x = xmin + i*(xmax-xmin)/width;
			double y = ymin + j*(ymax-ymin)/height;

			// Compute the iterations at that point.
			int iters = iterations_at_point(x,y,max);

			// Set the pixel in the bitmap.
			bitmap_set(bm,i,j,iters);
		}
	}
	return NULL;
}

/*
Return the number of iterations at point x, y
in the Mandelbrot space, up to a maximum of max.
*/

int iterations_at_point( double x, double y, int max )
{
	double x0 = x;
	double y0 = y;

	int iter = 0;

	while( (x*x + y*y <= 4) && iter < max ) {

		double xt = x*x - y*y + x0;
		double yt = 2*x*y + y0;

		x = xt;
		y = yt;

		iter++;
	}

	return iteration_to_color(iter,max);
}

/*
Convert a iteration number to an RGBA color.
Here, we just scale to gray with a maximum of imax.
Modify this function to make more interesting colors.
*/

int iteration_to_color( int i, int max )
{
	int gray = 255*i/max;
	return MAKE_RGBA(gray,gray,gray,0);
}




