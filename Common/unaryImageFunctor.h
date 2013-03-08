#ifndef _unaryImageFunctor_H_
#define _unaryImageFunctor_H_





namespace itk {

/* All functor defined below */
  namespace Functor {

  /* Add scalar functor */
  template <class TInput, class TOutput>
    class Add {

    public:
        Add() {};
        ~Add() {};
        typedef typename NumericTraits<TInput>::RealType InputRealType;

        /* VectorImage can't be looped, use Vector Image instead */
        inline TOutput operator() (const TInput & A) const
        {
            TOutput output;
            //              const unsigned int numOfComponents = m_Channel;
            // const unsigned int numberOfComponents = Statistics::MeasurementVectorTraits::GetLength(A);
            //              for (unsigned int cc=0; cc<numOfComponents; cc++) {
            //                  output[cc] =  A[cc] * m_Multiply;
            //              }
            //              return output;
            output = A+m_Add;
            return output;
            }

        void SetIndex(double & value)
        { m_Add = value; }

        //  void SetChannel(unsigned int & value)
        //  { m_Channel = value; }

    private:
        // InputRealType m_Multiply;
        double m_Add;
        //  unsigned int m_Channel;


    };



  /* Multiply scalar functor */
    template <class TInput, class TOutput>
      class Multiply {

      public:
          Multiply() {};
          ~Multiply() {};
          typedef typename NumericTraits<TInput>::RealType InputRealType;

          /* VectorImage can't be looped, use Vector Image instead */
          inline TOutput operator() (const TInput & A) const
          {
              TOutput output;
              //              const unsigned int numOfComponents = m_Channel;
              // const unsigned int numberOfComponents = Statistics::MeasurementVectorTraits::GetLength(A);
              //              for (unsigned int cc=0; cc<numOfComponents; cc++) {
              //                  output[cc] =  A[cc] * m_Multiply;
              //              }
              //              return output;
              output = A*m_Multiply;
              return output;
              }

          void SetIndex(double & value)
          { m_Multiply = value; }

          //  void SetChannel(unsigned int & value)
          //  { m_Channel = value; }

      private:
          // InputRealType m_Multiply;
          double m_Multiply;
          //  unsigned int m_Channel;


      };

  }



/* All filter defined below */

  /* Add scalar filter */
  template <class TInputImage>
class AddScalarFilter :
        public UnaryFunctorImageFilter<TInputImage, TInputImage,
        Functor::Add<typename TInputImage::PixelType, typename TInputImage::PixelType> >
{
public:
    typedef AddScalarFilter                 Self;
    typedef UnaryFunctorImageFilter<TInputImage, TInputImage,
    Functor::Add<typename TInputImage::PixelType, typename TInputImage::PixelType> >
                                                 SuperClass;
    typedef typename SuperClass::FunctorType     FunctorType;
    typedef typename FunctorType::InputRealType  InputRealType;

    typedef SmartPointer<Self>        Pointer;

    itkNewMacro(Self);

    itkTypeMacro(AddScalarFilter, Functor::Add);

    void SetIndex (double & value)
    {
        //        this->GetFunctor().SetIndex(InputRealType(value));
        this->GetFunctor().SetIndex(value);
    }
    //    void SetChannel (unsigned int & value)
    //    {
    //        this->GetFunctor().SetChannel(value);
    //    }
};






  /* Multiply scalar filter */
  template <class TInputImage>
class MultiplyScalarFilter :
        public UnaryFunctorImageFilter<TInputImage, TInputImage,
        Functor::Multiply<typename TInputImage::PixelType, typename TInputImage::PixelType> >
{
public:
    typedef MultiplyScalarFilter                 Self;
    typedef UnaryFunctorImageFilter<TInputImage, TInputImage,
    Functor::Multiply<typename TInputImage::PixelType, typename TInputImage::PixelType> >
                                                 SuperClass;
    typedef typename SuperClass::FunctorType     FunctorType;
    typedef typename FunctorType::InputRealType  InputRealType;

    typedef SmartPointer<Self>        Pointer;

    itkNewMacro(Self);

    itkTypeMacro(MultiplyScalarFilter, Functor::Multiply);

    void SetIndex (double & value)
    {
        //        this->GetFunctor().SetIndex(InputRealType(value));
        this->GetFunctor().SetIndex(value);
    }
    //    void SetChannel (unsigned int & value)
    //    {
    //        this->GetFunctor().SetChannel(value);
    //    }
};




}



#endif
