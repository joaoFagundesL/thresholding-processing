#include "threshold.h"
#include <assert.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static int check_image_format(FILE *file) {
  char format[3];
  fscanf(file, "%2s", format);

  if (format[0] != 'P' || format[1] != '2') {
    printf("Invalid PGM format: %s\n", format);
    fclose(file);
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}

static unsigned char **allocate_pixel(int width, int height) {
  unsigned char **pixels =
      (unsigned char **)malloc(height * sizeof(unsigned char *));
  assert(pixels != NULL);
  for (int i = 0; i < height; i++) {
    pixels[i] = (unsigned char *)malloc(width * sizeof(unsigned char));

    assert(pixels[i] != NULL);
  }

  return pixels;
}

static Pgm_image *set_image(int width, int height, int max_gray,
                            unsigned char **pixels) {
  Pgm_image *image = (Pgm_image *)malloc(sizeof(Pgm_image));

  assert(image != NULL);

  image->width = width;
  image->height = height;
  image->max_gray = max_gray;
  image->pixels = pixels;

  return image;
}

Pgm_image *read_pgm_image(const char *filename) {
  FILE *file = fopen(filename, "r");
  if (!file) {
    perror("error opening file");
    return NULL;
  }

  if (check_image_format(file) == EXIT_FAILURE)
    return NULL;

  int width, height, max_gray;
  fscanf(file, "%d %d\n%d\n", &width, &height, &max_gray);

  unsigned char **pixels = allocate_pixel(width, height);

  for (int i = 0; i < height; i++) {
    for (int j = 0; j < width; j++)
      fscanf(file, "%hhu", &pixels[i][j]);
  }

  fclose(file);

  Pgm_image *image = set_image(width, height, max_gray, pixels);
  return image;
}

void write_pgm_image(const char *filename, Pgm_image *image) {
  FILE *file = fopen(filename, "w");
  if (!file) {
    printf("Error opening file: %s\n", filename);
    return;
  }

  fprintf(file, "P2\n%d %d\n%d\n", image->width, image->height,
          image->max_gray);

  for (int i = 0; i < image->height; i++) {
    for (int j = 0; j < image->width; j++) {
      fprintf(file, "%hhu ", image->pixels[i][j]);
    }
    fprintf(file, "\n");
  }

  fclose(file);
}

void threshold_image(Pgm_image *image, int threshold) {
  for (int i = 0; i < image->height; i++) {
    for (int j = 0; j < image->width; j++) {

      image->pixels[i][j] = image->pixels[i][j] <= threshold ? BLACK : WHITE;
    }
  }

  printf("threshold used: %d\n", threshold);
}

static void fill_histogram(Pgm_image *image, int *histogram) {
  for (int i = 0; i < image->height; i++) {
    for (int j = 0; j < image->width; j++)
        histogram[image->pixels[i][j]]++;
  }
}

int threshold_generate(Pgm_image *image, int threshold) {
  int histogram[HISTOGRAM_LEN];
  memset(histogram, 0, sizeof(histogram));

  fill_histogram(image, histogram);

  const int total_pixels = image->height * image->width;
  double sum = 0.0;

  for (int i = 0; i < HISTOGRAM_LEN; i++)
    sum += i * histogram[i];

  double sum_b = 0.0;
  int background_pixel_count = 0;
  int foreground_pixel_count = 0;

  double var_max = 0.0;

  for (int i = 0; i < HISTOGRAM_LEN; i++) {

    UPDATE_BACKGROUND_COUNT(i);
    UPDATE_FOREGROUND_COUNT;

    if (foreground_pixel_count == 0)
      break;

    UPDATE_SUM_B(i);

    COMPUTE_MEANS;

    COMPUTE_VARIANCE;

    UPDATE_THRESHOLD;
  }

  return threshold;
}

void free_pgm_image(Pgm_image *image) {
  for (int i = 0; i < image->height; i++)
    free(image->pixels[i]);

  free(image->pixels);
  free(image);
}
