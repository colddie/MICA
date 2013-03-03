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
    float mean = 0;
    float stdev = 0;


    /* check the arguments */
    if (argc != 2) {
        cout<<"Usage:"<<argv[0]<<"infile"<<endl;
        return -1;
    }

    /* open the file */
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
        imagetype = "short";
    } else if(image_type ==4) {
        imagetype = "float";
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


    /* copute the mean */
    vector<float> temp;
    float tempf;
    unsigned int pixels = image_size/(16/image_type);
    file1.seekg(PIXELS_OFFSET, ios::beg);
    for (unsigned int i=0;i<pixels-1;i++) {
        file1.read((char*)&tempf, 16/image_type);
        temp.push_back(tempf);
    }
    for(unsigned int i=0;i<temp.size()-1;i++) {
        mean += temp[i];
    }
    mean /= pixels;
    cout<<"mean of the image"<<mean<<endl;

    //    for( unsigned int i = 0; i < temp.size(); i++ )
    //       {
    //         stdev += ( temp[ i ] - mean ) * ( temp[ i ] - mean );
    //       }
    //       stdev = sqrt( stdev / ( temp.size() - 1.0 ) );
    //    cout<<"stdev of the image"<<stdev<<endl;


    file1.close();



}


