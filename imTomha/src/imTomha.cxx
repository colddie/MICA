#include <iostream>
#include <fstream>
#include <vector>
//#include <algorithm>
//#include "math.h"

using namespace std;



int main (int argc, char* argv[])
{

    int SIZE_OFFSET = 7;
    int TYPE_OFFSET = 8321;
    int DIMSIZE_OFFSET = 8325;
    int DIM_OFFSET = 8329;
    int PIXELS_OFFSET = 8369;
    int image_size;
    int image_type;
    int image_ndim;
    int image_dimx, image_dimy, image_dimz;
    string imagetype;
    string ps = "1 1 1";
    string os = "0 0 0";


    /* check the arguments */
    if (argc < 3 || argc > 5) {
        printf("Usage imTomha infile outfile [pixel_spacing] [origin_offset]");
        return -1;
    }
    if (argc == 5) {
        ps = string(argv[3]);
        os = string(argv[4]);
    }



    /* open the input file */
    ifstream file1;
    file1.open(argv[1], ios::binary);
    if (!file1) {
        cout<<"Error opening file"<<argv[1]<<"for read"<<endl;
        return -1;
    }

    //    file1.seekg(SIZE_OFFSET*sizeof(int), ios::beg);
    //    file1.read((char*)&image_size, sizeof(int));


    /* parse the parameters */
    file1.seekg(TYPE_OFFSET, ios::beg);
    file1.read((char*)&image_type, sizeof(int));

    if (image_type == 8) {
        imagetype = "MET_SHORT";
    } else if(image_type ==4) {
        imagetype = "MET_FLOAT";
    }

    //    file1.seekg(DIMSIZE_OFFSET ,ios::beg);
    file1.read((char*)&image_ndim, sizeof(int));

    //    file1.seekg(DIM_OFFSET, ios::beg);
    file1.read((char*)&image_dimz, sizeof(int));

    //    file1.seekg((DIM_OFFSET+4), ios::beg);
    file1.read((char*)&image_dimx, sizeof(int));

    if (image_ndim == 3) {
        file1.seekg((DIM_OFFSET+8), ios::beg);
        file1.read((char*)&image_dimy, sizeof(int));
    } else {
        image_dimz = 1;
    }

    image_size = image_dimx*image_dimy*image_dimz*(16/image_type);
    cout<<"image size"<<image_size<<endl;
    cout<<"image type"<<imagetype<<endl;
    cout<<"image dimension"<<image_ndim<<endl;
    cout<<"dimension size"<<image_dimx<<image_dimy<<image_dimz<<endl;


    /* write meta header */
    ofstream file2;
    file2.open(argv[2], ios::binary);
    if (!file2) {
        cout<<"Error opening file"<<argv[1]<<"for read"<<endl;
        return -1;
    }
    file2.flush();
    file2<<"ObjectType = Image\n";
    file2<<"NDims = 3\n"
         <<"BinaryData = True\n"
         <<"BinaryDataByteOrderMSB = False\n"
         <<"TransformMatrix = 1 0 0 0 1 0 0 0 1\n"
         <<"Offset = "<<os<<endl
         <<"ElementSpacing = "<<ps<<endl
         <<"DimSize = "<<image_dimx<<" "<<image_dimy<<" "<<image_dimz<<endl
         <<"AnatomicalOrientation = RPI\n"
         <<"ElementType = "<<imagetype<<endl
         <<"ElementDataFile = LOCAL\n";


    /* write to meta binary */
    unsigned char *c;
    // read 1 byte from inputfile
    file1.read((char*)&c, 1);
    while(!(file1.eof())) {
    // write 1 byte to outputfile
        file2.write(reinterpret_cast<const char*>(&c), 1);

    // read again
        file1.read((char*)&c, 1);
    }

    file1.close();
    file2.close();


}


