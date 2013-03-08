#ifndef _addScalar_H_
#define _addScalar_H_

#include "itkUnaryFunctorImageFilter.h"
#include "itkMeasurementVectorTraits.h"

#include "unaryImageFunctor.h"
#include "IO_type.h"
#include "argumentsParse.h"


namespace itk {


/* Define the base class to initialize the pointer to the real class */
  class AddScalarBase : public Object
  {
  public:
      std::string inputfile;
      std::string outputfile;
      double index;
      unsigned int channel;

      AddScalarBase(){};
      ~AddScalarBase(){};

      /** All sub-classes should overwrite Run() to implement functionality. */
      virtual int Update( void ) = 0;


  };




/* Real functional class */
 template <class InputType, unsigned int Dimension>
    class AddScalar : public AddScalarBase
    {
    public:
    typedef AddScalar Self;

        /* Static function to return the class object with template arguments */
        static Self * New( unsigned int dim,
                           itk::ImageIOBase::IOComponentType componentType)
        {
            if(componentType == itk::ImageIOBase::MapPixelType<InputType>::CType)
            {
                return new Self;
            }
            return 0;
        }

        /* Method for creation through the object factoAddry */
        itkNewMacro(Self);

        /* Runtime type information */
        itkTypeMacro(AddScalar, AddScalarBase);

        // void Run();

        /* Redefine the vitural function from base*/
        int Update() {

            typedef VectorImage<InputType, Dimension> TImage;  // Not support loop in the Functor
            // typedef Image<Vector<InputType, 3>, Dimension> TImage;   // support

            /* Read the input */
            typedef ImageFileReader<TImage> ReaderType;
            typename ReaderType::Pointer reader = ReaderType::New();
            reader->SetFileName(inputfile);

            try
            {
                reader->Update();
            }
            catch( ExceptionObject & err )
            {
                std::cerr << "ExceptionObject caught !" << std::endl;
                std::cerr << err << std::endl;
                return EXIT_FAILURE;
            }


            /* Run the filter */
            typedef AddScalarFilter<TImage> FilterType;
            typename FilterType::Pointer addScalarFilter = FilterType::New();
            addScalarFilter->SetIndex(index);
            // addScalarFilter->SetChannel(channel);
            addScalarFilter->SetInput(reader->GetOutput());


            /* Write to output */
            typedef ImageFileWriter<TImage> WriterType;
            typename WriterType::Pointer writer = WriterType::New();
            writer->SetFileName(outputfile);
            writer->SetInput(addScalarFilter->GetOutput());

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



    protected:

        AddScalar(){};
        virtual ~AddScalar(){}; // use virtual function from base



    private:

        AddScalar(const Self &);
        void operator =(const Self &);
};


}





//#ifndef ITK_MANUAL_INSTANTIATION
//#include "addScalar.txx"
//#endif

#endif
