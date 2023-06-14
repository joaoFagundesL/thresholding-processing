#include "threshold.h"

int main(int argc, char *argv[])
{
	if (argc < ARGS_MIN_QUANTITY || argc > ARGS_MAX_QUANTITY) {
		printf
		    ("run with: %s <input_file> <output_file> <threshold (optional)>\n",
		     argv[0]);
		return EXIT_FAILURE;
    }

	Pgm_image *image = read_pgm_image(argv[1]);

	if (!image)
		return EXIT_FAILURE;

	int threshold = argc == ARGS_MIN_QUANTITY ? threshold_generate(image,
						       0) : atoi(argv[3]);

  threshold_image(image, threshold);
  
	write_pgm_image(argv[2], image);

	free_pgm_image(image);

	return EXIT_SUCCESS;
}
