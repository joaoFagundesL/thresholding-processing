#include "threshold.h"

int main(int argc, char *argv[])
{
	if (argc != ARGS_QUANTITY) {
		printf("run with: %s <input_file> <output_file>\n", argv[0]);
		return EXIT_FAILURE;
	}

	const char *input_filename = argv[1];
	const char *output_filename = argv[2];

	Pgm_image *image = read_pgm_image(input_filename);

	if (!image)
		return EXIT_FAILURE;

	threshold_generate(image);

	write_pgm_image(output_filename, image);

	free_pgm_image(image);

	return EXIT_SUCCESS;
}
