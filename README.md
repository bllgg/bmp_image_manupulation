# bmp_image_manupulation
The "bmptool" is a simple tool for manupulate bitmap images. Simply we can enlarge an image, flip vertically and horizontally and rotate image by multiples of 90 degrees.

## Usage
```
Usage: ./bmptool [-s scale | -r rotation | -f| -v] [-o output_file] [input file]
```

Ex:
```
$ ./bmptool -s 2 -r 90 -f -v -o output.bmp input.bmp
```
