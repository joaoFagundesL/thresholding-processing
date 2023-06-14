# How to use it

### Compile

Using `make` will compile the program with all required files and
with some useful flags.

### Run

The program takes four arguments

1. ./main (executable file)
2. path to the grayscale image
3. desired path to save the image with the name to save
4. threshold desired (**optional**).

**If you don't want to choose a threshold the program will calculate one using the Otsu's algorithm**

### Example

```bash
./main ./original_images/moca.pgm ./threshold_image/moca.pgm
```

This one generates a threshold automatically

```bash
./main ./original_images/moca.pgm ./threshold_image/moca.pgm 128
```

In this case the threshold is $128$
