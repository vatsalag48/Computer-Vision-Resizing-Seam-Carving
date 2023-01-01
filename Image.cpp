#include <cassert>
#include <string>
#include <iostream>
#include "Matrix.h"
#include "Image.h"

using namespace std;

// REQUIRES: img points to an Image
//           0 < width && width <= MAX_MATRIX_WIDTH
//           0 < height && height <= MAX_MATRIX_HEIGHT
// MODIFIES: *img
// EFFECTS:  Initializes the Image with the given width and height.
// NOTE:     Do NOT use new or delete here.
void Image_init(Image* img, int width, int height) {
  //assert(false); // TODO Replace with your implementation!
  img->width = width;
  img->height = height;
  Matrix_init(&(img->red_channel), width, height);
  Matrix_init(&(img->green_channel), width, height);
  Matrix_init(&(img->blue_channel), width, height);

}
// REQUIRES: img points to an Image
//           is contains an image in PPM format without comments
//           (any kind of whitespace is ok)
// MODIFIES: *img
// EFFECTS:  Initializes the Image by reading in an image in PPM format
//           from the given input stream.
// NOTE:     See the project spec for a discussion of PPM format.
// NOTE:     Do NOT use new or delete here.
void Image_init(Image* img, std::istream& is) {
  //assert(false); // TODO Replace with your implementation!
  //First line should be P3
  //Second line should be WIDTH x HEIGHT. Use these to initialize a matrix
  //Third line should be max value for RGB intensities, always 255
  //Comment gave long line error plz fix EECS
  //Repeat reading in values to the is stream
  //TODO: Make sure file is good, and that my inputs are working
  string PPM_type;
  int max_pix_bright = 0;

  is >> PPM_type;
  is >> img->width >> img->height;
  Matrix_init(&img->red_channel, img->width, img->height);
  Matrix_init(&img->green_channel, img->width, img->height);
  Matrix_init(&img->blue_channel, img->width, img->height);
  //cout << "Channel's initialized" << endl;
  is >> max_pix_bright;
  /*img->red_channel.width = width;
  img->red_channel.height = height;
  img->green_channel.width = width;
  img->green_channel.height = height;
  img->blue_channel.width = width;
  img->blue_channel.height = height;*/

  //TODO: GET INTO SPEC, NEED TO INIT MATRIX I THINK
  for (int i = 0; i < img->height; i++) {
    for (int j = 0; j < img-> width; j++) {
      Pixel temp;
      int red = 0;
      int green = 0;
      int blue = 0;
      is >> red;
      is >> green;
      is >> blue;
      temp = {red, green, blue};
      //cout << "Temp Pix works" << endl;
      Image_set_pixel(img, i, j, temp);
      //cout << "Set_pix works" << endl;
    }
  }
}
    /*is >> img->red_channel.data[i];
    is >> img->green_channel.data[i];
    is >> img->blue_channel.data[i];*/

// REQUIRES: img points to a valid Image
// EFFECTS:  Writes the image to the given output stream in PPM format.
//           You must use the kind of whitespace specified here.
//           First, prints out the header for the image like this:
//             P3 [newline]
//             WIDTH [space] HEIGHT [newline]
//             255 [newline]
//           Next, prints out the rows of the image, each followed by a
//           newline. Each pixel in a row is printed as three ints
//           for its red, green, and blue components, in that order. Each
//           int is followed by a space. This means that there will be an
//           "extra" space at the end of each line. See the project spec
//           for an example.
void Image_print(const Image* img, std::ostream& os) {
  //assert(false); // TODO: Initialize Matrix, get dirty in the spec
  os << "P3" << "\n";
  os << img->width << ' ' << img->height << '\n';
  os << 255 << "\n";
  for (int i = 0; i < img->height; i++) {
    for (int j = 0; j < img->width; j++) {
      Pixel temp = Image_get_pixel(img, i, j);
      os << temp.r << " " << temp.g << " " << temp.b << " ";
    }
      os << '\n';
  }
}

// REQUIRES: img points to a valid Image
// EFFECTS:  Returns the width of the Image.
int Image_width(const Image* img) {
  //assert(false); // TODO Replace with your implementation!
  return img->width;
}

// REQUIRES: img points to a valid Image
// EFFECTS:  Returns the height of the Image.
int Image_height(const Image* img) {
  //assert(false); // TODO Replace with your implementation!
  return img->height;
}

// REQUIRES: img points to a valid Image
//           0 <= row && row < Image_height(img)
//           0 <= column && column < Image_width(img)
// EFFECTS:  Returns the pixel in the Image at the given row and column.
Pixel Image_get_pixel(const Image* img, int row, int column) {
  //assert(false); // TODO Replace with your implementation!
  Pixel pix;
  const Matrix* ptr_red = &(img->red_channel);
  const Matrix* ptr_gre = &(img->green_channel);
  const Matrix* ptr_blu = &(img->blue_channel);
  pix.r = *Matrix_at(ptr_red, row, column);
  pix.g = *Matrix_at(ptr_gre, row, column);
  pix.b = *Matrix_at(ptr_blu, row, column);
  
  
  return pix;
}

// REQUIRES: img points to a valid Image
//           0 <= row && row < Image_height(img)
//           0 <= column && column < Image_width(img)
// MODIFIES: *img
// EFFECTS:  Sets the pixel in the Image at the given row and column
//           to the given color.
void Image_set_pixel(Image* img, int row, int column, Pixel color) {
  //assert(false); // TODO Replace with your implementation!
  //Need to set value in each channel (RGB) to the value in color.(r/g/b)
  //Use *Matrix at with a pointer to find the memory location
  //Set the location equal to the
  
  Matrix* ptr_red = &(img->red_channel);
  Matrix* ptr_gre = &(img->green_channel);
  Matrix* ptr_blu = &(img->blue_channel);

  //cout << "Matrix Ptr's works" << endl; -> GOOD

  int *red_loc = Matrix_at(ptr_red, row, column);
  int *gre_loc = Matrix_at(ptr_gre, row, column);
  int *blu_loc = Matrix_at(ptr_blu, row, column);

  *red_loc = color.r;
  *gre_loc = color.g;
  *blu_loc = color.b;
  /*(int element = row * (img->width) + column;
  img->red_channel.data[element] = color.r;
  img->green_channel.data[element] = color.g;
  img->blue_channel.data[element] = color.b;*/
}

// REQUIRES: img points to a valid Image
// MODIFIES: *img
// EFFECTS:  Sets each pixel in the image to the given color.
void Image_fill(Image* img, Pixel color) {
  Matrix_fill(&(img->red_channel), color.r);
  Matrix_fill(&(img->green_channel), color.g);
  Matrix_fill(&(img->blue_channel), color.b);
  //assert(false); // TODO Replace with your implementation!
  /*void Matrix_fill(Matrix* mat, int value) {
  //assert(false); // TODO Replace with your implementation!
  for (int i = 0; i < (mat->width * mat->height); i++) {
    mat->data[i] = value;
  }
}*/


}
