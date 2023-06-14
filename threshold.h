#include <stdlib.h>
#include <stdio.h>

#define ARGS_MAX_QUANTITY 4
#define ARGS_MIN_QUANTITY 3
#define HISTOGRAM_LEN 256
#define WHITE 255
#define BLACK 0

#define UPDATE_BACKGROUND_COUNT(index) \
    background_pixel_count += histogram[index];

#define UPDATE_FOREGROUND_COUNT \
    foreground_pixel_count = total_pixels - background_pixel_count;

#define UPDATE_SUM_B(index) \
    sum_b += (index) * histogram[index];

#define COMPUTE_MEANS \
    double m_b = sum_b / background_pixel_count; \
    double m_f = (sum - sum_b) / foreground_pixel_count;

#define COMPUTE_VARIANCE \
    double var_between = background_pixel_count * foreground_pixel_count * pow((m_b - m_f), 2);

#define UPDATE_THRESHOLD \
    if (var_between > var_max) { \
        var_max = var_between; \
        threshold = i; \
    }

typedef struct {
	int width;
	int height;
	int max_gray;
	unsigned char **pixels;
} Pgm_image;

Pgm_image *read_pgm_image(const char *filename);

void write_pgm_image(const char *filename, Pgm_image * image);
void threshold_image(Pgm_image * image, int threshold);
int threshold_generate(Pgm_image * image, int threshold);
void free_pgm_image(Pgm_image * image);
