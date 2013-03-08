#ifndef _IO_format_H
#define _IO_format_H

#include "addScalar.h"
#include "IO_type.h"

#include <iostream>




class IO_format
{

 public:
    int format_fun(char *b[]);
    int main_fun(char *b[]);

};


enum IOformat
{
    META,
    ANALYZE,
    RAW,
    BMP,
    JPEG,
    NIFT,
    NRRD,
    VTK,
    PNG,
    TIFF,
    FORMAT_UNDEFINED


};













#endif
