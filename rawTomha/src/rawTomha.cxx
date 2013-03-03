#include <iostream>
#include <fstream>
//#include <string>
//#include <boost/format.hpp>

using namespace std;


int main (int argc, char* argv[])
{


//    string header[] =
//        "ObjectType = Image\n"
//        "NDims = 3\n"
//        "BinaryData = True\n"
//        "BinaryDataByteOrderMSB = False\n"
//        "TransformMatrix = 1 0 0 0 1 0 0 0 1\n"
//        "Offset = %s\n"
//        "ElementSpacing = %s\n"
//        "DimSize = %s\n"
//        "AnatomicalOrientation = RPI\n"
//        "ElementType = %s\n"
//        "ElementDataFile = LOCAL\n"
//        ;
    string dim;
    string ps;
    string os = "0 0 0";
    string type;


    /* read in parameters */
    if (argc > 7 || argc <= 5) {
        printf("Usage:rawTomha infile outfile dimension pixel_spcaing image_type [origin_offset]\n");
        return -1;
    }

    dim = string(argv[3]);
    ps = string(argv[4]);
    if (argv[5] == "char") {
        type = "MET_CHAR";
    } else if (argv[5]== "uchar") {
        type = "MET_UCHAR";
    } else if (argv[5]== "short") {
        type = "MET_SHORT";
    } else if (argv[5]== "ushort") {
        type = "MET_USHORT";
    } else if (argv[5]== "int") {
        type = "MET_INT";
    } else if (argv[5]== "uint") {
        type = "MET_UINT";
    } else if (argv[5]== "float") {
        type = "MET_FLOAT";
    } else if (argv[5]== "double") {
        type = "MET_DOUBLE";
    } else {
        return -1;
    }

    if (argc == 7) {
        os = string(argv[6]);
    }


    /* write the header to output meta image */
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
         <<"DimSize = "<<dim<<endl
         <<"AnatomicalOrientation = RPI\n"
         <<"ElementType = "<<type<<endl
         <<"ElementDataFile = LOCAL\n";


    /* open the raw image */
    ifstream file1;
    file1.open(argv[1], ios::binary);
    if (!file1) {
        cout<<"Error opening file"<<argv[1]<<"for read"<<endl;
        return -1;
    }

    /* write the data to output meta image */
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
    return 0;

}
