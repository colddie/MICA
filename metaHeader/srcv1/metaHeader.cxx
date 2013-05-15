
#include <iostream>
#include <fstream>

using namespace std;



int main (int argc, char* argv[])
{


    string key[] = {"Comment", "ObjectType", "TransformType", "TransformMatrix",
    "BinaryData", "ElementByteOderMSB", "ElementDataByteOderMSB", "Color",
    "Position", "AnatomicalOrientation", "ElementSpacing","HeaderSize",
    "Modality", "SequenceID", "ElementMin", "ElementMax",
	"ElementNumberOfChannels", "ElementSize", "ElementType", "ElementDataFile",
    "Offset", "CenterOfRotation", "CompressedData", "DimSize"};

    int i,j;

    if (argc != 2) {
        cout<<"Usage:"<<argv[0]<<"infile"<<endl;
        return -1;
    }

    ifstream file;
    file.open(argv[1],ios::in);
    if (!file) {
        cout<<"Error opening file"<<argv[1]<<"for read"<<endl;
        return -1;
    }


	// 30 lines of MHA should have passed the header 
    string line;
    for (i=0; i<30; i++) {
        //read one line from fp1 to buf
        getline(file,line,'\n');

        for(j=0; j<(sizeof(key)/sizeof(char*)); j++) {
            if(line.find(string(key[j]))!=string::npos) {
                cout<<line<<endl;
            }

        }

        }

//        if (strstr(buf, "DimSize")!=NULL) {
////            sscanf(&(buf[9]), "%d%d%d", &nx, &ny, &nz);

//        } else if (strstr(buf, "ElementSpacing")!=NULL) {
////            sscanf(&(buf[16]), "%f%f%f", &sx, &sy, &sz);

//        } else if (strstr(buf, "ElementType")!=NULL) {
//            if (!strcmp(buf, "ElementType = MET_SHORT\n")) {
////                nb = 2;
//            } else if (!strcmp(buf, "ElementType = MET_USHORT\n")) {
////                nb = 2;
//            } else if (!strcmp(buf, "ElementType = MET_UCHAR\n")) {
////                nb = 1;
//            } else if (!strcmp(buf, "ElementType = MET_FLOAT\n")) {
////                nb = 4;
//            }
//        }

//    printf("Writing...\n");
//    //image buffer allocate
//    bytesz = (long) nx * ny * nz * nb;
//    printf("%ld bytes of image data!\n", bytesz);
//	//locate buffer length back from end
//    fseek(fp1, -bytesz, SEEK_END);

//	//read 1 byte by 1 byte from fp1
//    fread(&c,1,1,fp1);
//	//see if reach end of the file
//    while (!feof(fp1)) {
//	//write c 1 byte by 1 byte to fp2
//        fwrite(&c,1,1,fp2);
//	//read again
//        fread(&c,1,1,fp1);
//    }

    file.close();

}
