#include <cassert>
#include "Matrix.h"

// REQUIRES: mat points to a Matrix
//           0 < width && width <= MAX_MATRIX_WIDTH
//           0 < height && height <= MAX_MATRIX_HEIGHT
// MODIFIES: *mat
// EFFECTS:  Initializes *mat as a Matrix with the given width and height.
// NOTE:     Do NOT use new or delete here.
void Matrix_init(Matrix* mat, int width, int height) {
  //assert(false); // TODO Replace with your implementation!
  mat->width = width;
  mat->height = height;
  //mat->data = width * height; Determine if I need to modigy data

}

// REQUIRES: mat points to a valid Matrix
// MODIFIES: os
// EFFECTS:  First, prints the width and height for the Matrix to os:
//             WIDTH [space] HEIGHT [newline]
//           Then prints the rows of the Matrix to os with one row per line.
//           Each element is followed by a space and each row is followed
//           by a newline. This means there will be an "extra" space at
//           the end of each line.
void Matrix_print(const Matrix* mat, std::ostream& os) {
  //assert(false); // TODO Replace with your implementation!
  os << mat->width << " " << mat->height << "\n";
  int mat_size = Matrix_width(mat) * Matrix_height(mat);
  int count_max = mat->width;
  int counter = 0;
  for (int i = 0; i < mat_size; i++) {
    counter++;
    if (counter >= count_max) { 
      os << mat->data[i] << " " << "\n"; 
      counter = 0;
    }
    else {
      os << mat ->data[i] << " ";
    }
  }
}

// REQUIRES: mat points to an valid Matrix
// EFFECTS:  Returns the width of the Matrix.
int Matrix_width(const Matrix* mat) {
  //assert(false); // TODO Replace with your implementation!
  return mat->width;
}

// REQUIRES: mat points to a valid Matrix
// EFFECTS:  Returns the height of the Matrix.
int Matrix_height(const Matrix* mat) {
  //assert(false); // TODO Replace with your implementation!
  return mat->height;
}

// REQUIRES: mat points to a valid Matrix
//           ptr points to an element in the Matrix
// EFFECTS:  Returns the row of the element pointed to by ptr.
int Matrix_row(const Matrix* mat, const int* ptr) {
  //assert(false); // TODO Replace with your implementation!
  int element_location = ptr - &(mat->data[0]);
  int row_num = element_location / (mat->width);
  return row_num;
}

// REQUIRES: mat points to a valid Matrix
//           ptr point to an element in the Matrix
// EFFECTS:  Returns the column of the element pointed to by ptr.
int Matrix_column(const Matrix* mat, const int* ptr) {
  //assert(false); // TODO Replace with your implementation!
  //1) get mem location of mat->data first int
  //2) use loop for(int *i = ptr; i > mat->data; i - mat->width)
  int element_location = ptr - &(mat->data[0]);
  int column_num = element_location % (mat->width);
  return column_num;
}

// REQUIRES: mat points to a valid Matrix
//           0 <= row && row < Matrix_height(mat)
//           0 <= column && column < Matrix_width(mat)
//
// MODIFIES: (The returned pointer may be used to modify an
//            element in the Matrix.)
// EFFECTS:  Returns a pointer to the element in the Matrix
//           at the given row and column.
int* Matrix_at(Matrix* mat, int row, int column) {
  int traversal = ((mat->width) * row) + column;
  int* location = &(mat->data[traversal]); 
  return location;
}

// REQUIRES: mat points to a valid Matrix
//           0 <= row && row < Matrix_height(mat)
//           0 <= column && column < Matrix_width(mat)
//
// EFFECTS:  Returns a pointer-to-const to the element in
//           the Matrix at the given row and column.
const int* Matrix_at(const Matrix* mat, int row, int column) {
  int traversal = ((mat->width) * row) + column;
  const int* location = &(mat->data[traversal]); 
  return location;
}

// REQUIRES: mat points to a valid Matrix
// MODIFIES: *mat
// EFFECTS:  Sets each element of the Matrix to the given value.
void Matrix_fill(Matrix* mat, int value) {
  //assert(false); // TODO Replace with your implementation!
  for (int i = 0; i < (mat->width * mat->height); i++) {
    mat->data[i] = value;
  }
}

// REQUIRES: mat points to a valid Matrix
// MODIFIES: *mat
// EFFECTS:  Sets each element on the border of the Matrix to
//           the given value. These are all elements in the first/last
//           row or the first/last column.
void Matrix_fill_border(Matrix* mat, int value) {
  for(int i = 0; i < (mat->width); ++i) { //first & last row
    mat->data[i] = value;
    mat->data[(mat->width * mat->height) - i - 1] = value;
  }
  for(int i = 1; i < mat->height; ++i) { //first & last col
    mat->data[i * mat->width] = value;
    mat->data[i*mat->width - 1] = value; 
  }
}

// REQUIRES: mat points to a valid Matrix
// EFFECTS:  Returns the value of the maximum element in the Matrix
int Matrix_max(const Matrix* mat) {
  int max_location = 0;
  if (mat->height ==1){
    max_location = mat->width - 1;
  }
  else {
    max_location = ((mat->width) * ((mat->height) - 1));
  }
  int matrix_max = -100000;
  for (int i = 0; i < max_location; i++) {
    if (mat->data[i] >= matrix_max) {
      matrix_max = mat->data[i];
    }
  }
  return matrix_max;
}

// REQUIRES: mat points to a valid Matrix
//           0 <= row && row < Matrix_height(mat)
//           0 <= column_start && column_end <= Matrix_width(mat)
//           column_start < column_end
// EFFECTS:  Returns the column of the element with the minimal value
//           in a particular region. The region is defined as elements
//           in the given row and between column_start (inclusive) and
//           column_end (exclusive).
//           If multiple elements are minimal, returns the column of
//           the leftmost one.
int Matrix_column_of_min_value_in_row(const Matrix* mat, int row,
                                      int column_start, int column_end) {
  //assert(false); // TODO Replace with your implementation!
  int start_elem = ((mat->width) * row) + column_start;
  int end_elem = ((mat->width) * row) + column_end;
  const int* min_ptr = &(mat->data[start_elem]);
  int min = mat->data[start_elem];

  for (int i = start_elem; i < end_elem; i++) {
    if ((mat->data[i]) < min) {
      min = mat->data[i];
      min_ptr = &(mat->data[i]);
    }
  }
  int col_num = Matrix_column(mat, min_ptr);
  return col_num;
}

// REQUIRES: mat points to a valid Matrix
//           0 <= row && row < Matrix_height(mat)
//           0 <= column_start && column_end <= Matrix_width(mat)
//           column_start < column_end
// EFFECTS:  Returns the minimal value in a particular region. The region
//           is defined as elements in the given row and between
//           column_start (inclusive) and column_end (exclusive).
int Matrix_min_value_in_row(const Matrix* mat, int row,
                            int column_start, int column_end) {
  //assert(false); // TODO Replace with your implementation!
  int start_elem = ((mat->width) * row) + column_start;
  int end_elem = ((mat->width) * row) + column_end;
  int min = mat->data[start_elem];

  for (int i = start_elem; i < end_elem; i++) {
    if ((mat->data[i]) < min) {
      min = mat->data[i];
    }
  }
  return min;
}
