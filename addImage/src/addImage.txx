#ifndef ADDIMAGE_TXX
#define ADDIMAGE_TXX

#include "addImage.h"
#include "unaryImageFunctor.h"


namespace itk{


/* Update image type */
template <class InputType, unsigned int Dimension>
void AddImage::Update(){


    typedef Image<InputType, Dimension> TImage;
    Run<TImage>();
}


/* Run the filter */
template <class TImage>
int AddImage::Run()

{
//    unsigned int inputsize = inputfile.size();
//    struct F {

//        static double divide_val(double val)
//        { return val/inputsize; }
//    };

  /* Read the input */
  typedef ImageFileReader<TImage> ReaderType;
  typename ReaderType::Pointer reader = ReaderType::New();




  /* Run the filter */
    typename TImage::Pointer inputimage = TImage::New();
    typedef NaryAddImageFilter<TImage, TImage> AddImageFilterType;
    typename AddImageFilterType::Pointer addFilter = AddImageFilterType::New();
    typedef MultiplyScalarFilter<TImage> MultiplyScalarFilterType;
    typename MultiplyScalarFilterType::Pointer multiplyFilter = MultiplyScalarFilterType::New();

    /* if no weight specified, set to default 1 */
    if (!isweight) {
        for (int i=0; i<inputfile.size(); i++)
            weight.push_back(1);
    }

    /* if average divide all weight */
    if (isaverage) {
        transform(weight.begin(), weight.end(), weight.begin(),
                  bind2nd(divides<double>(), double(inputfile.size())));
    }

    for (unsigned int j=0; j<inputfile.size(); j++) {
        reader->SetFileName(inputfile[j]);
        reader->UpdateLargestPossibleRegion();
        inputimage = reader->GetOutput();
        inputimage->DisconnectPipeline();    // Disconnect to avoid accumulation
        multiplyFilter->SetInput(inputimage);
        multiplyFilter->SetIndex(weight[j]);cout<<weight[j];
        multiplyFilter->Update();
        inputimage = multiplyFilter->GetOutput();
        inputimage->DisconnectPipeline();    // Disconnect to avoid accumulation
        addFilter->SetInput(j, inputimage);

    }

    addFilter->Update();


  /* Write to output */
  typedef ImageFileWriter<TImage> WriterType;
  typename WriterType::Pointer writer = WriterType::New();
  writer->SetFileName(outputfile);
  writer->SetInput(addFilter->GetOutput());

  try
      {
      writer->Update();
      }
    catch( ExceptionObject & err )
      {
      std::cerr << "ExceptionObject caught !" << std::endl;
      std::cerr << err << std::endl;
      return EXIT_FAILURE;
      }

  return EXIT_SUCCESS;
}

}





#endif // ADDIMAGE_TXX
