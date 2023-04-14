
// neural net
// there is a grid of neurons representing an img
// every neuron(cell in the grid) represents a pixel 
// every cell has a weight associated with it
// the answer is sumation(neurons*resp_weights)
// if the sumation is more than the bias of the neuron
// it lights up 
// the bias after training enough number of times converges 
/* 
   training:
   initially all the neurons are 0 weighted
   based on the answer if it is wrong
   if we wanted a 1 and it is 0
   we add the cell values to the weight: exciting
   if we wanted a 0 and it is 1
   we sub the cell values from the weight: suppressing
*/
// 0 for rectangle and 1 for circle


#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <errno.h>
#include <math.h>
#define WIDTH 100
#define HEIGHT 100
#define PPM_SCALER 50 
typedef float Layer[HEIGHT][WIDTH];  // a 2 dimensional array of floats type of size height*width 

static inline int clampi(int x, int low, int high) {
	if (x < low)  x = low;
	if (x > high) x = high;
	return x;
}

void layer_fill_rect(Layer layer, int x, int y, int w, int h, float value) {
	assert(w>0);
	assert(h>0);
	int x0 = clampi(x, 0, WIDTH-1); 
	int y0 = clampi(y, 0, HEIGHT-1); 
	int x1 = clampi(x0 + w-1, 0, WIDTH-1);
	int y1 = clampi(y0 + h-1, 0, HEIGHT-1);

	for (int y=y0; y<=y1; ++y) {
		for (int x=x0; x<=x1; ++x) {
			layer[y][x] = value;
		}
	}
}

void layer_fill_circle(Layer layer, int cx, int cy, int r, float value) {
	assert(r>0);
	int x0 = clampi(cx - r, 0, WIDTH-1);
	int y0 = clampi(cy - r, 0, HEIGHT-1);
	int x1 = clampi(cx + r, 0, WIDTH-1);
	int y1 = clampi(cy + r, 0, HEIGHT-1);
	
	for (int y=y0; y<=y1; ++y) {
		for (int x=x0; x<=x1; ++x) {
			int dx = x-cx;
			int dy = y-cy;
			if (dx*dx + dy*dy <= r*r) {
				layer[y][x] = value;
			}
		}
	}
}

void layer_save_as_ppm(Layer layer, const char* filepath) {
	FILE* f = fopen(filepath, "wb");
	if (f == NULL) {
		fprintf(stderr, "ERROR: Invalid file path %s: %m\n", filepath);
		exit(1);
	}
	fprintf(f, "P6\n%d %d 255\n", WIDTH*PPM_SCALER, HEIGHT*PPM_SCALER);
	for (int y=0;y<HEIGHT*PPM_SCALER; ++y) {
		for (int x=0;x<WIDTH*PPM_SCALER; ++x) {
			float s = layer[y/PPM_SCALER][x/PPM_SCALER];
			char pixel[3] = {
				(char) floor(255*s), 0, 0
			};
			fwrite(pixel, sizeof(pixel), 1, f);
		}
	}

	fclose(f);
}

float feed_forward(float inputs[HEIGHT][WIDTH], float weights[WIDTH][HEIGHT]) {
	
	float output = 0.0f;
	for (int x=0; x<HEIGHT; ++x) { 
		for (int y=0; y<WIDTH; ++y) {
			output += inputs[y][x] * weights[y][x];
		}
	}
	return output;
}

static Layer inputs;
static Layer weights;

int main() {
	//printf("Hello, subroza!\n");
	
	layer_fill_circle(inputs, WIDTH/2, HEIGHT/2, WIDTH/2, 1.0f);
	layer_save_as_ppm(inputs, "inputs.ppm");
	//float output = feed_forward(inputs, weights);
	//printf("output = %f\n", output);
	return 0;
}
