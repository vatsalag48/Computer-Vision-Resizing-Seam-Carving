#include <iostream>
#include <fstream>
#include "Image.h"
#include "Matrix.h"
#include <cassert>
#include "processing.h"
#include <cstdlib>
#include <ostream>
#include <string>
using namespace std;
using std::string;

string filename_in;
string filename_out;
int cur_width = 0;
int cur_height = 0;
int desired_width = 0;
int desired_height = 0;
int main(int argc, char *argv[])
{
    filename_in = argv[1];
    filename_out = argv[2];
    ifstream in(filename_in);
    ofstream out(filename_out);
    desired_width = atoi(argv[3]);
    desired_height = atoi(argv[4]);
    Image *img_read = new Image;
    Image_init(img_read, in);

    //Add open read in files

    if (desired_width > Image_width(img_read))
    {
        cout << "Usage: resize.exe IN_FILENAME OUT_FILENAME WIDTH [HEIGHT]\n"
             << "WIDTH and HEIGHT must be less than or equal to original" << endl;
        return -1;
    }

    if (desired_height > Image_height(img_read))
    {
        cout << "Usage: resize.exe IN_FILENAME OUT_FILENAME WIDTH [HEIGHT]\n"
             << "WIDTH and HEIGHT must be less than or equal to original" << endl;
        return -1;
    }

    if (argc < 4 || argc >= 6)
    {
        cout << "Usage: resize.exe IN_FILENAME OUT_FILENAME WIDTH [HEIGHT]\n"
             << "WIDTH and HEIGHT must be less than or equal to original" << endl;
        return -1;
    }

    seam_carve(img_read, desired_width, desired_height);
    Image_print(img_read, out);
    delete img_read;
}
