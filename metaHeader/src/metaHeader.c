/* -----------------------------------------------------------------------
   See COPYRIGHT.TXT and LICENSE.TXT for copyright and license information
   ----------------------------------------------------------------------- */
/* Print out the header info of the metafile */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#define BUFLEN 1024

// void swap_short(unsigned char *pntr)
// {
    // unsigned char b0, b1;

    // b0 = *pntr;
    // b1 = *(pntr+1);
        
    // *pntr = b1;
    // *(pntr+1) = b0;
// }

int main (int argc, char* argv[])
{
    FILE *fp1;
    char *key[] = {"Comment", "ObjectType", "TransformType", "TransformMatrix",
    "BinaryData", "ElementByteOderMSB", "ElementDataByteOderMSB", "Color",
    "Position", "AnatomicalOrientation", "ElementSpacing",
	"HeaderSize", "Modality", "SequenceID", "ElementMin", "ElementMax", 
	"ElementNumberOfChannels", "ElementSize", "ElementType", "ElementDataFile",
    "OffSet", "CenterOfRotation", "CompressedData", "DimSize"};

    int i,j;
    char buf[1024];

    if (argc != 2) {
        printf ("Usage: %s infile\n", argv[0]);
        exit (1);
    }
    if (!(fp1 = fopen(argv[1],"rb"))) {
        printf ("Error opening file \"%s\" for read\n", argv[1]);
        exit (1);
    }

	// 30 lines of MHA should have passed the header 
    for (i=0; i<30; i++) { 
        //read one line from fp1 to buf
		if (fgets(buf,1024,fp1) == NULL) {
            printf ("File error.\n");
            exit (1);
        }

        for(j=0; j<(sizeof(key)/sizeof(char*)); j++) {
            if(strstr(buf, key[j])!=NULL) {
                printf(buf);
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

    fclose(fp1);
//    fclose(fp2);
    return 0;
}
