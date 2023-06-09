#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/***********************************************************************
 *
  This code is based on GNU CODING STYLE
  Reference: https://www.gnu.org/prep/standards/standards.html
   ___                    ______                          _          
  |_  |                   |  ___|                        | |         
    | | ___   __ _  ___   | |_ __ _  __ _ _   _ _ __   __| | ___ ___ 
    | |/ _ \ / _` |/ _ \  |  _/ _` |/ _` | | | | '_ \ / _` |/ _ / __|
/\__/ | (_) | (_| | (_) | | || (_| | (_| | |_| | | | | (_| |  __\__ \
\____/ \___/ \__,_|\___/  \_| \__,_|\__, |\__,_|_| |_|\__,_|\___|___/
                                     __/ |                           
                                    |___/                            
 *
 ************************************************************************/

#define HISTOGRAM_LENGTH 256

typedef struct
{
  int row;
  int col;
  int value;
} Pixel;

typedef struct
{
  int rows;
  int cols;
  int maxGray;
  Pixel *pixels;
  int numPixels;
} Image;

Image *
createImage (int rows, int cols, int maxGray)
{
  Image *image = (Image *) malloc (sizeof (Image));

  assert (image != NULL);
  image->rows = rows;
  image->cols = cols;
  image->maxGray = maxGray;
  image->pixels = NULL;
  image->numPixels = 0;
  return image;
}

void
addPixel (Image *image, int row, int col, int value)
{

  image->pixels =
    (Pixel *) realloc (image->pixels,
		       (image->numPixels + 1) * sizeof (Pixel));

  assert (image->pixels != NULL);
  image->pixels[image->numPixels].row = row;
  image->pixels[image->numPixels].col = col;
  image->pixels[image->numPixels].value = value;
  image->numPixels++;
}

void
freeImage (Image *image)
{
  free (image->pixels);
  free (image);
}

Image *
loadImage (const char *filename)
{
  FILE *file = fopen (filename, "r");
  if (file == NULL)
    {
      perror ("file not found");
      return NULL;
    }

  char format[3];
  fscanf (file, "%2s", format);

  if (strcmp (format, "P2") != 0)
    {
      printf ("Formato de arquivo inválido. Esperado: P2\n");
      fclose (file);
      return NULL;
    }

  int cols, rows;
  fscanf (file, "%d %d", &cols, &rows);

  printf ("Numero de linhas = %d\nNumero de colunas = %d\n", rows, cols);

  int maxGray;
  fscanf (file, "%d", &maxGray);

  if (maxGray < 0 || maxGray > 255)
    {
      printf ("Valor inválido para maxGray. Deve estar entre 0 e 255.\n");
      fclose (file);
      return NULL;
    }

  printf ("MAX GRAY = %d\n", maxGray);

  Image *image = createImage (rows, cols, maxGray);

  int value;
  for (int i = 0; i < image->rows; i++)
    {
      for (int j = 0; j < image->cols; j++)
	{
	  fscanf (file, "%d", &value);
	  addPixel (image, i, j, value);
	}
    }

  fclose (file);
  return image;
}

void
computeHistogram (Image *image, int *histogram)
{
  int maxGray = image->maxGray;

  memset (histogram, 0, (maxGray + 1) * sizeof (int));

  for (int i = 0; i < image->numPixels; i++)
    {
      int value = image->pixels[i].value;

      if (value >= 0 && value <= maxGray)
	{
	  histogram[value]++;
	}
    }
}

void
thresholdImage (Image *image, int threshold)
{
  for (int i = 0; i < image->numPixels; i++)
    image->pixels[i].value = image->pixels[i].value >= threshold ? 255 : 0;
}

void
saveImage (const Image *image, const char *filename)
{
  FILE *file = fopen (filename, "w");
  if (file == NULL)
    {
      printf ("Erro ao criar o arquivo de saída.\n");
      return;
    }

  fprintf (file, "P2\n");

  fprintf (file, "%d %d\n", image->cols, image->rows);

  fprintf (file, "%d\n", image->maxGray);

  for (int i = 0; i < image->numPixels; i++)
    fprintf (file, "%d ", image->pixels[i].value);

  fclose (file);
}

int
main ()
{
  Image *image = loadImage ("input.pgm");
  if (image == NULL)
    return 1;

  int histogram[HISTOGRAM_LENGTH];
  computeHistogram (image, histogram);

  printf ("Histograma:\n");
  for (int i = 0; i <= image->maxGray; i++)
    printf ("%d: %d\n", i, histogram[i]);

  int threshold = 150;

  thresholdImage (image, threshold);

  saveImage (image, "imagem_limiarizada.pgm");

  freeImage (image);

  return 0;
}
