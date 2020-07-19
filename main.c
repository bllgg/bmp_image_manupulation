#include <stdlib.h>
#include <stdio.h>
#include <getopt.h>
#include "bmplib.h"

void print_usage() {
    printf("Usage: ./bmptool [-s scale | -r rotation | -f| -v] [-o output_file] [input file]\n");
}

/*
 * This method enlarges a 24-bit, uncompressed .bmp file
 * that has been read in using readFile()
 *
 * original - an array containing the original PIXELs, 3 bytes per each
 * rows     - the original number of rows
 * cols     - the original number of columns
 *
 * scale    - the multiplier applied to EACH OF the rows and columns, e.g.
 *           if scale=2, then 2* rows and 2*cols
 *
 * new      - the new array containing the PIXELs, allocated within
 * newrows  - the new number of rows (scale*rows)
 * newcols  - the new number of cols (scale*cols)
 */
int enlarge(PIXEL* original, int rows, int cols, int scale,
	    PIXEL** new, int* newrows, int* newcols)
{
  /* THIS IS THE METHOD THAT YOU SHOULD WRITE */
  int row, col;
  *newcols = cols * scale;
  *newrows = rows * scale;

  if ((rows <= 0) || (cols <= 0)) return -1;

  *new = (PIXEL*)malloc(rows * scale * cols * scale * sizeof(PIXEL));

  for (row=0; row < rows; row++){
    for (col=0; col < cols; col++) {
      PIXEL* o = original + row*cols + col;
      PIXEL* n;
      /*
      for (int i = 0; i < scale; i++)
      {
        n = (*new) + row*cols*scale*scale + col*scale + i;
        *n = *o;
      }*/
      for (int i = 0; i < scale; i++)
      {
        for (int j = 0; j < scale; j++)
        {
          n = (*new) + row*cols*scale*scale + col*scale + cols*scale*i + j;
          *n = *o;
        }
      }
    }
  }
  
  return 0;
}

/*
 * This method rotates a 24-bit, uncompressed .bmp file that has been read
 * in using readFile(). The rotation is expressed in degrees and can be
 * positive, negative, or 0 -- but it must be a multiple of 90 degrees
 *
 * original - an array containing the original PIXELs, 3 bytes per each
 * rows     - the number of rows
 * cols     - the number of columns
 * rotation - a positive or negative rotation,
 *
 * new      - the new array containing the PIXELs, allocated within
 * newrows  - the new number of rows
 * newcols  - the new number of cols
 */
int rotate(PIXEL* original, int rows, int cols, int rotation,
	   PIXEL** new, int* newrows, int* newcols)
{
  /* THIS IS THE METHOD THAT YOU SHOULD WRITE */
  int row, col;

  if ((rows <= 0) || (cols <= 0)) return -1;
  if (rotation % 90 != 0) return -1;
  rotation = rotation % 360;
  rotation = rotation / 90;

  *new = (PIXEL*)malloc(rows*cols*sizeof(PIXEL));

  if ((rotation == 1) || (rotation == -3))
  {
    *newcols = rows;
    *newrows = cols;

    *new = (PIXEL*)malloc(rows*cols*sizeof(PIXEL));

    for (row=0; row < rows; row++)
      for (col=0; col < cols; col++) {
        PIXEL* o = original + row*cols + col;
        PIXEL* n = (*new) + (cols-1-col)*rows + row;
        *n = *o;
      }

  }
  else if ((rotation == 2) || (rotation == -2))
  {
    *newcols = cols;
    *newrows = rows;

    for (row=0; row < rows; row++)
      for (col=0; col < cols; col++) {
        PIXEL* o = original + row*cols + col;
        PIXEL* n = (*new) + (rows-1-row)*cols + (cols-1-col);
        *n = *o;
      }

  }
  else if ((rotation == 3) || (rotation == -1))
  {
    *newcols = rows;
    *newrows = cols;

    for (row=0; row < rows; row++)
      for (col=0; col < cols; col++) {
        PIXEL* o = original + row*cols + col;
        PIXEL* n = (*new) + col*rows + (rows-1-row);
        *n = *o;
      }
  }else
  {
    *newcols = cols;
    *newrows = rows;

    for (row=0; row < rows; row++)
      for (col=0; col < cols; col++) {
        PIXEL* o = original + row*cols + col;
        PIXEL* n = (*new) + row*cols + col;
        *n = *o;
      }
  }

  return 0;
}

/*
 * This method Vertically flips a 24-bit, uncompressed bmp file
 * that has been read in using readFile().
 *
 * original - an array containing the original PIXELs, 3 bytes per each
 * rows     - the number of rows
 * cols     - the number of columns
 *
 * new      - the new array containing the PIXELs, allocated within
 */

int verticalflip (PIXEL *original, PIXEL **new, int rows, int cols)
{
  /* THIS IS THE METHOD THAT YOU SHOULD WRITE */
  int row, col;

  if ((rows <= 0) || (cols <= 0)) return -1;

  *new = (PIXEL*)malloc(rows*cols*sizeof(PIXEL));

  for (col=0; col < cols; col++)
    for (row=0; row < rows; row++) {
      PIXEL* o = original + row*cols + col; // done
      PIXEL* n = (*new) + (rows-1-row)*cols + col;
      *n = *o;
    }

  return 0;
}


/*
 * This method horizontally flips a 24-bit, uncompressed bmp file
 * that has been read in using readFile().
 *
 * THIS IS GIVEN TO YOU SOLELY TO LOOK AT AS AN EXAMPLE
 * TRY TO UNDERSTAND HOW IT WORKS
 *
 * original - an array containing the original PIXELs, 3 bytes per each
 * rows     - the number of rows
 * cols     - the number of columns
 *
 * new      - the new array containing the PIXELs, allocated within
 */
int flip (PIXEL *original, PIXEL **new, int rows, int cols)
{
  int row, col;

  if ((rows <= 0) || (cols <= 0)) return -1;

  *new = (PIXEL*)malloc(rows*cols*sizeof(PIXEL));

  for (row=0; row < rows; row++)
    for (col=0; col < cols; col++) {
      PIXEL* o = original + row*cols + col;
      PIXEL* n = (*new) + row*cols + (cols-1-col);
      *n = *o;
    }

  return 0;
}

int main(int argc, char **argv)
{
  int chr;
  int scale = 1, rotation = 0, h_flip = 0, v_flip = 0; 
  char* input_filename = "sample_input.bmp";
  char* output_filename = "sample_output.bmp";
  while ((chr = getopt (argc, argv, "fvo:s:r:")) != -1){
    switch (chr)
    {
    case 's':
      scale = atoi(optarg);
      break;

    case 'r':
      rotation = atoi(optarg);
      break;

    case 'f':
      h_flip = 1;
      break;

    case 'v':
      v_flip = 1;
      break;
    
    case 'o':
      output_filename = optarg;
      break;

    default:
      print_usage();
      exit(EXIT_FAILURE);
    }
  }
  
  input_filename = argv[optind];
  if (input_filename == NULL)
  {
    input_filename = "sample_input.bmp";
  }
  

  if (scale < 0)
  {
    print_usage();
    exit(EXIT_FAILURE);
  }
  
  PIXEL *b, *nsb, *nrb, *nfb, *nhb = NULL, *nvb = NULL;
  int r, c;
  readFile(input_filename, &r, &c, &b);
  int e_n_r, e_n_c;
  enlarge(b, r, c, scale, &nsb, &e_n_r, &e_n_c);
  int r_n_r, r_n_c;
  rotate(nsb, e_n_r, e_n_c, rotation, &nrb, &r_n_r, &r_n_c);
  nfb = nrb;
  
  if (v_flip == 1)
  {
    verticalflip(nfb, &nvb, r_n_r, r_n_c);
    nfb = nvb;
  }
  if (h_flip == 1)
  {
    flip(nfb, &nhb, r_n_r, r_n_c);
    nfb = nhb;
  }
  
  writeFile(output_filename, r_n_r, r_n_c, nfb);
  
  //freeing pointers
  free(b);
  free(nsb);
  free(nrb);
  free(nfb);
  return 0;
}


