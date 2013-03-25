#ifndef _IO_type_h_
#define _IO_type_h_

#include "itkImage.h"
#include "itkImageFileReader.h"
#include "itkImageFileWriter.h"
#include "itkMetaImageIO.h"

#include <string>

class IO_type
{

public:

    IO_type(){};
    ~IO_type(){};

    //    int main_fun(const char *a, std::string &imagetype, unsigned int &dim, unsigned int &channel);

    /* Create ImageIO */
    void Create_IO(const char *a);

    /* Return ByteOrder */
    std::string Return_ByteOrder() const;

    /* Return the image type string */
    std::string Return_ImageType() const;

    /* Return the pixel type string */
    std::string Return_PixelType() const;

    /* Return the image dimesion number */
    unsigned int Return_DimensionNumber() const;

    /* Return the image component number */
    unsigned int Return_ChannelNumber() const;

    /* Return the image dimension size */
    std::vector<unsigned int> Return_DimensionSize() const;

    /* Return the spacing */
    std::vector<double> Return_Spacing() const;

    /* Return the origin */
    std::vector<double> Return_Origin() const;

    /* Return the direction */
    itk::Matrix<double, 3, 3> Return_Direction() const;

    /* Return the direcion in one axes */
    std::vector<double> Return_Direction(unsigned int i) const;

protected:



private:
    itk::ImageIOBase::Pointer imageio;


};






#endif
