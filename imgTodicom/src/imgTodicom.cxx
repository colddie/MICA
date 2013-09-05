#include "itkImage.h"
#include "itkImageFileReader.h"
#include "itkImageSeriesReader.h"
#include "itkGDCMImageIO.h"
#include "itkGDCMSeriesFileNames.h"
#include "itkImageSeriesWriter.h"
#include "itkNumericSeriesFileNames.h"
#include "itkMetaDataObject.h"

int main (int argc, char* argv[])
{

    if (argc < 4) {
        std::cout << "Usage:" <<std::endl;
        std::cout << argv[0] << " inputMetaFile outputDicomDirectory headerDirectory"
                  <<std::endl;

        return EXIT_FAILURE;
    }


    //
    typedef signed short PixelType;
    typedef itk::Image<PixelType, 3> ImageType;
    typedef itk::ImageFileReader<ImageType>   ReaderType;

    ReaderType::Pointer imageReader = ReaderType::New();
    imageReader->SetFileName(argv[1]);
    try
      {
      imageReader->Update();
      }
    catch (itk::ExceptionObject &excp)
      {
      std::cerr << "Exception thrown while reading the image" << std::endl;
      std::cerr << excp << std::endl;
      return EXIT_FAILURE;
      }



    //
    typedef itk::ImageSeriesReader<ImageType> DicomReaderType;
    DicomReaderType::Pointer dicomReader = DicomReaderType::New();

    typedef itk::GDCMImageIO ImageIOType;
    ImageIOType::Pointer dicomIO = ImageIOType::New();

    typedef itk::GDCMSeriesFileNames NamesGeneratorType;
    NamesGeneratorType::Pointer nameGenerator = NamesGeneratorType::New();

//    typedef std::vector<std::string> FileNamesContainer;
//    FileNamesContainer fileNames;

    dicomReader->SetImageIO(dicomIO);
    nameGenerator->SetInputDirectory(argv[3]);
    dicomReader->SetFileNames(nameGenerator->GetInputFileNames());
    try
      {
      dicomReader->Update();
      }
    catch (itk::ExceptionObject &excp)
      {
      std::cerr << "Exception thrown while reading the image" << std::endl;
      std::cerr << excp << std::endl;
      return EXIT_FAILURE;
      }


    //
    typedef itk::Image<PixelType, 2>  Image2DType;
    typedef itk::ImageSeriesWriter<ImageType, Image2DType>  WriterType;
    WriterType::Pointer dicomWriter = WriterType::New();

    ImageType::RegionType region = imageReader->GetOutput()->GetLargestPossibleRegion();
    ImageType::IndexType  start  = region.GetIndex();
    ImageType::SizeType   size   = region.GetSize();

    itksys::SystemTools::MakeDirectory(argv[2]);
    itk::NumericSeriesFileNames::Pointer outputNames = itk::NumericSeriesFileNames::New();
    std::string format(argv[2]);
    format = format + "/" + "IM%03d.dcm";
    outputNames->SetSeriesFormat(format.c_str());
    outputNames->SetStartIndex(start[2]);
    outputNames->SetEndIndex(start[2]+size[2]-1);
    outputNames->SetIncrementIndex(1);

    const DicomReaderType::DictionaryArrayType *outputArray = dicomReader->GetMetaDataDictionaryArray();
    dicomWriter->SetFileNames(outputNames->GetFileNames());
    dicomWriter->SetInput(imageReader->GetOutput());
    dicomWriter->SetImageIO(dicomIO);
    dicomWriter->SetMetaDataDictionaryArray(outputArray);
    try
      {
      dicomWriter->Update();
      }
    catch (itk::ExceptionObject &excp)
      {
      std::cerr << "Exception thrown while reading the image" << std::endl;
      std::cerr << excp << std::endl;
      return EXIT_FAILURE;
      }



}
