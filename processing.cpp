#include <cassert>
#include "processing.h"

using namespace std;

// v DO NOT CHANGE v ------------------------------------------------
// The implementation of rotate_left is provided for you.
// REQUIRES: img points to a valid Image
// MODIFIES: *img
// EFFECTS:  The image is rotated 90 degrees to the left (counterclockwise).
void rotate_left(Image* img) {

  // for convenience
  int width = Image_width(img);
  int height = Image_height(img);

  // auxiliary image to temporarily store rotated image
  Image *aux = new Image;
  Image_init(aux, height, width); // width and height switched

  // iterate through pixels and place each where it goes in temp
  for (int r = 0; r < height; ++r) {
    for (int c = 0; c < width; ++c) {
      Image_set_pixel(aux, width - 1 - c, r, Image_get_pixel(img, r, c));
    }
  }

  // Copy data back into original
  *img = *aux;
  delete aux;
}
// ^ DO NOT CHANGE ^ ------------------------------------------------

// v DO NOT CHANGE v ------------------------------------------------
// The implementation of rotate_right is provided for you.
// REQUIRES: img points to a valid Image.
// MODIFIES: *img
// EFFECTS:  The image is rotated 90 degrees to the right (clockwise).
void rotate_right(Image* img){

  // for convenience
  int width = Image_width(img);
  int height = Image_height(img);

  // auxiliary image to temporarily store rotated image
  Image *aux = new Image;
  Image_init(aux, height, width); // width and height switched

  // iterate through pixels and place each where it goes in temp
  for (int r = 0; r < height; ++r) {
    for (int c = 0; c < width; ++c) {
      Image_set_pixel(aux, c, height - 1 - r, Image_get_pixel(img, r, c));
    }
  }

  // Copy data back into original
  *img = *aux;
  delete aux;
}
// ^ DO NOT CHANGE ^ ------------------------------------------------


// v DO NOT CHANGE v ------------------------------------------------
// The implementation of diff2 is provided for you.
static int squared_difference(Pixel p1, Pixel p2) {
  int dr = p2.r - p1.r;
  int dg = p2.g - p1.g;
  int db = p2.b - p1.b;
  // Divide by 100 is to avoid possible overflows
  // later on in the algorithm.
  return (dr*dr + dg*dg + db*db) / 100;
}
// ^ DO NOT CHANGE ^ ------------------------------------------------


// ------------------------------------------------------------------
// You may change code below this line!



// REQUIRES: img points to a valid Image.
//           energy points to a Matrix.
// MODIFIES: *energy
// EFFECTS:  energy serves as an "output parameter".
//           The Matrix pointed to by energy is initialized to be the same
//           size as the given Image, and then the energy matrix for that
//           image is computed and written into it.
//           See the project spec for details on computing the energy matrix.
void compute_energy_matrix(const Image* img, Matrix* energy) {
  int i_width = Image_width(img);
  int i_height = Image_height(img);
  Matrix_init(energy, i_width, i_height); //init energy matrix
  Matrix_fill(energy, 0);
  for (int i = 1; i < i_height - 1; ++i)
  { // rows
    for (int l = 1; l < i_width - 1; ++l)
    { // columns
      int NS = squared_difference(Image_get_pixel(img, i - 1, l), 
      Image_get_pixel(img, i + 1, l));
      int EW = squared_difference(Image_get_pixel(img, i, l - 1), 
      Image_get_pixel(img, i, l + 1));
      *Matrix_at(energy, i, l) = NS + EW;
    }
  }
  int max_val = Matrix_max(energy);
  Matrix_fill_border(energy, max_val);
}


// REQUIRES: energy points to a valid Matrix.
//           cost points to a Matrix.
//           energy and cost aren't pointing to the same Matrix
// MODIFIES: *cost
// EFFECTS:  cost serves as an "output parameter".
//           The Matrix pointed to by cost is initialized to be the same
//           size as the given energy Matrix, and then the cost matrix is
//           computed and written into it.
//           See the project spec for details on computing the cost matrix.
void compute_vertical_cost_matrix(const Matrix* energy, Matrix *cost) {
  int e_width = Matrix_width(energy);
  int e_height = Matrix_height(energy);
  Matrix_init(cost, e_width, e_height);
  Matrix_fill(cost, 0);
  int c_width = Matrix_width(cost);
  int c_height = Matrix_height(cost);
  for (int i = 0; i < c_width; ++i)
  { // fill first row with energy values
    int* c_ptr = Matrix_at(cost, 0, i);
    int const * e_prt = Matrix_at(energy, 0, i);
    *c_ptr = *e_prt;
  }
  for (int i = 1; i < c_height; ++i)
  {
    if (c_width != 1)
    { // this if else does the first element in a row
      int* loc_ptr = Matrix_at(cost, i, 0);
      const int* val1 = Matrix_at(energy, i, 0);
      *loc_ptr = (Matrix_min_value_in_row(cost, i - 1, 0, 2) + *val1);
    }
    else
    {
      int* loc_ptr = Matrix_at(cost, i, 0);
      const int* val1 = Matrix_at(energy, i - 1, 0);
      const int* val2 = Matrix_at(energy, i, 0);
      *loc_ptr = *val1 + *val2;
    }

    for (int l = 1; l < c_width - 1; ++l)
    {
      int* loc_ptr = Matrix_at(cost, i, l);
      int val1 = Matrix_min_value_in_row(cost, i - 1, l - 1, l + 2);
      const int* val2 = Matrix_at(energy, i, l);
      *loc_ptr = val1 + *val2; // this is for all middle elements in a row
    }

    if (c_width != 1)
    { // this if else does the last element in a row
      int* loc_ptr = Matrix_at(cost, i, c_width - 1);
      int val1 = Matrix_min_value_in_row(cost, i - 1, c_width - 2, c_width - 1);
      const int* val2 = Matrix_at(energy, i, e_width - 1);
      *loc_ptr = val1 + *val2;
    }
    else
    {
      int *loc_ptr = Matrix_at(cost, i, 0);
      const int* val1 = Matrix_at(energy, i - 1, 0);
      const int* val2 = Matrix_at(energy, i, e_width - 1);
      *loc_ptr = *val1 + *val2;
    }
  }
}

// REQUIRES: cost points to a valid Matrix
//           seam points to an array
//           the size of seam is >= Matrix_height(cost)
// MODIFIES: seam[0]...seam[Matrix_height(cost)-1]
// EFFECTS:  seam serves as an "output parameter".
//           The vertical seam with the minimal cost according to the given
//           cost matrix is found and the seam array is filled with the column
//           numbers for each pixel along the seam, with the pixel for each
//           row r placed at seam[r]. While determining the seam, if any pixels
//           tie for lowest cost, the leftmost one (i.e. with the lowest
//           column number) is used.
//           See the project spec for details on computing the minimal seam.
// NOTE:     You should compute the seam in reverse order, starting
//           with the bottom of the image and proceeding to the top,
//           as described in the project spec.
void find_minimal_vertical_seam(const Matrix* cost, int seam[]) {
  //assert(false); // TODO Replace with your implementation!
  int Width = Matrix_width(cost);
  int Height_Adj = Matrix_height(cost) - 1;
  //TODO: Make seam correct. First remove should go in last elem of seam.
  //***This is so the arr reads top to bottom for rmv vert seam
  int start_col = Matrix_column_of_min_value_in_row(cost, Height_Adj, 0, Width);
  int seam_elem = (Matrix_height(cost) - 1);
  seam[seam_elem] = start_col;
  for (int i = 1; i < (Height_Adj + 1); i++) {
    seam_elem = seam_elem - 1;
    int Height = Height_Adj - i;
    if (start_col == 0) { 
      start_col = Matrix_column_of_min_value_in_row(cost, (Height), 0, 2);
      seam[seam_elem] = start_col;
    }
    else if (start_col == (Width - 1)) {
      start_col = Matrix_column_of_min_value_in_row(cost, (Height), (Width - 2), Width);
      seam[seam_elem] = start_col;
    }
    else {
      int l_col = start_col - 1;
      int r_col = start_col + 2;
      start_col = Matrix_column_of_min_value_in_row(cost, (Height), (l_col), (r_col));
      seam[seam_elem] = start_col;
    }
  }
}


// REQUIRES: img points to a valid Image with width >= 2
//           seam points to an array
//           the size of seam is == Image_height(img)
//           each element x in seam satisfies 0 <= x < Image_width(img)
// MODIFIES: *img
// EFFECTS:  Removes the given vertical seam from the Image. That is, one
//           pixel will be removed from every row in the image. The pixel
//           removed from row r will be the one with column equal to seam[r].
//           The width of the image will be one less than before.
//           See the project spec for details on removing a vertical seam.
// NOTE:     Use the new operator here to create the smaller Image,
//           and then use delete when you are done with it.
void remove_vertical_seam(Image *img, const int seam[]) {
  //assert(false); // TODO Replace with your implementation!
  Image *temp = new Image;
  int height = Image_height(img);
  int width = Image_width(img);
  Image_init(temp, (width - 1), height);
  //Loop creating a skip pointer to a value in seam that need to be removed
  //then checking Matrix_at call and checking if the returned pnt is equal to the skip ptr
  //if check != skip, it is copied
  //if check == skip, it is not copied
  //   Need to make sure to check every element in the img row
  //Loop conditions should start at i = 0
  //   break condition is i > height
  //Use i to pull element from seam.
  //Need to use a decreasing counter starting at height - 1 for row

  //Boolian has_skiped == true
  // then Image_set_pixel(&temp, i, (j-1), img_copy, pix)
  for (int i = 0; i < height; i++) {
    int skip_column = seam[i];
    bool has_skipped = false;
    for (int j = 0; j < width; j++) {
      if (j == skip_column) {
        has_skipped = true;
      }
      if (j != skip_column && !has_skipped) {
        Pixel img_copy_pix = Image_get_pixel(img, i, j);
        Image_set_pixel(temp, i, j, img_copy_pix);
      }
      else if (j != skip_column && has_skipped == true) {
        Pixel img_copy_pix = Image_get_pixel(img, i, j);
        Image_set_pixel(temp, i, (j - 1), img_copy_pix);
      }
    }
  }
  //Copy temp into img
  *img = *temp;
  delete temp;
  //TODO: Make sure this works!
  //delete temp_ptr;
}


// REQUIRES: img points to a valid Image
//           0 < newWidth && newWidth <= Image_width(img)
// MODIFIES: *img
// EFFECTS:  Reduces the width of the given Image to be newWidth by using
//           the seam carving algorithm. See the spec for details.
// NOTE:     Use the new operator here to create Matrix objects, and
//           then use delete when you are done with them.
void seam_carve_width(Image *img, int newWidth) {
  //assert(false); // TODO Replace with your implementation!
  int i_width = Image_width(img);
  while (i_width != newWidth) {
    Matrix *energy = new Matrix;
    compute_energy_matrix(img, energy);
    Matrix *cost = new Matrix;
    compute_vertical_cost_matrix(energy, cost);
    int seam[MAX_MATRIX_HEIGHT];
    find_minimal_vertical_seam(cost, seam);
    remove_vertical_seam(img, seam);
    delete energy;
    delete cost;
    i_width = Image_width(img);
  }
}

// REQUIRES: img points to a valid Image
//           0 < newHeight && newHeight <= Image_height(img)
// MODIFIES: *img
// EFFECTS:  Reduces the height of the given Image to be newHeight.
// NOTE:     This is equivalent to first rotating the Image 90 degrees left,
//           then applying seam_carve_width(img, newHeight), then rotating
//           90 degrees right.
void seam_carve_height(Image *img, int newHeight) {
  //assert(false); // TODO Replace with your implementation!
  rotate_left(img);
  seam_carve_width(img, newHeight);
  rotate_right(img);
}

// REQUIRES: img points to a valid Image
//           0 < newWidth && newWidth <= Image_width(img)
//           0 < newHeight && newHeight <= Image_height(img)
// MODIFIES: *img
// EFFECTS:  Reduces the width and height of the given Image to be newWidth
//           and newHeight, respectively.
// NOTE:     This is equivalent to applying seam_carve_width(img, newWidth)
//           and then applying seam_carve_height(img, newHeight).
void seam_carve(Image *img, int newWidth, int newHeight) {
  //assert(false); // TODO Replace with your implementation!
  seam_carve_width(img, newWidth);
  seam_carve_height(img, newHeight);
}