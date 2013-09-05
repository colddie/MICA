#include "itkImage.h"
#include "itkGDCMImageIO.h"
#include "itkGDCMSeriesFileNames.h"
#include "itkImageSeriesReader.h"
#include "itkImageFileWriter.h"



int main (int argc, char* argv[])
{

    if (argc < 3) {
        std::cout << "Usage:" <<std::endl;
        std::cout << argv[0] << " DicomDirectory outputMetaFile [seriesName]"
                  <<std::endl;

        return EXIT_FAILURE;
    }


//   the dimensionality of the image is 3, and we assume a
//   signed short pixel type that is commonly used for X-Rays CT scanners.

    typedef signed short PixelType;
    const unsigned int   Dimension = 3;

    typedef itk::Image< PixelType, Dimension >   ImageType;


//    instantiating the type of the series reader
    typedef itk::ImageSeriesReader< ImageType >   ReaderType;
    ReaderType::Pointer reader = ReaderType::New();

//    GDCMImageIO object is created and connected to the reader
    typedef itk::GDCMImageIO    ImageIOType;
    ImageIOType::Pointer dicomIO = ImageIOType::New();

    reader->SetImageIO(dicomIO);


//   call the SetUseSeriesDetails function
//   that tells the GDCMSereiesFileNames object to use additional DICOM
//   information to distinguish unique volumes within the directory
//   If this is not enough for your specific case you can always add some more
//   restrictions using the \code{AddSeriesRestriction()} method. In this example we will use
//   the DICOM Tag: 0008 0021 DA 1 Series Date
    typedef itk::GDCMSeriesFileNames NamesGeneratorType;
    NamesGeneratorType::Pointer nameGenerator = NamesGeneratorType::New();

    nameGenerator->SetUseSeriesDetails(true);
    nameGenerator->AddSeriesRestriction("0008|0021");
    nameGenerator->SetDirectory(argv[1]);

    try
    {


        std::cout << std::endl << "The directory: " << std::endl;
        std::cout << std::endl << argv[1] <<std::endl;
        std::cout << "contains the following DICOM Series: ";
        std::cout << std::endl;


//    identifies the list of DICOM series that are present
        typedef std::vector< std::string >    SeriesIdContainer;
        const SeriesIdContainer & seriesUID = nameGenerator->GetSeriesUIDs();
        SeriesIdContainer::const_iterator seriesItr = seriesUID.begin();
        SeriesIdContainer::const_iterator seriesEnd = seriesUID.end();
        while (seriesItr != seriesEnd)
        {
            std::cout << seriesItr->c_str() << std::endl;
            seriesItr++;
        }

//     check if user provides a series identifider, or
//     use the first series found during the exploration of the directory
        std::string seriesIdentifier;

        if (argc > 3) {
            seriesIdentifier = argv[3];
        } else {
            seriesIdentifier = seriesUID.begin()->c_str();
        }


        std::cout << std::endl;
        std::cout << "Now reading series: " << std::endl;
        std::cout << seriesIdentifier << std::endl;

//      pass the series identifier to the name generator and ask for all the
//      filenames associated to that series
        typedef std::vector< std::string > FileNameContainer;
        FileNameContainer filenames = nameGenerator->GetFileNames(seriesIdentifier);


//      read in the identified series
        reader->SetFileNames(filenames);
        try {
            reader->Update();
        }
        catch (itk::ExceptionObject &ex) {
            std::cout << ex << std::endl;
            return EXIT_FAILURE;
        }


//      write in the meta file
        typedef itk::ImageFileWriter< ImageType >  WriterType;
        WriterType::Pointer writer = WriterType::New();

        writer->SetFileName(argv[2]);
        writer->SetInput(reader->GetOutput());

        std::cout << "Writing the image as " << std::endl;
        std::cout << argv[2] << std::endl;

        try {
            writer->Update();
        }
        catch (itk::ExceptionObject &ex) {
            std::cout << ex <<std::endl;
            return EXIT_FAILURE;
        }





    }
    catch (itk::ExceptionObject &ex) {
        std::cout << ex << std::endl;
        return EXIT_FAILURE;
    }

return EXIT_SUCCESS;

}
