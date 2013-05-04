#include "bsplineMultiReg.h"

#include "toolBase.h"

#include "itkImageDuplicator.h"



namespace itk{

 const unsigned int SplineOrder = 3;
 const unsigned int Dimension = 3;







void BsplineMultiReg::Initialize(Global &var)
{
 /* Initialize image and transform pyramids */
 InitializePyramid(var.levelnumber,var.inputfile1,var.inputfile2,
                   var.downsample,m_fixedImage,m_movingImage
                   );

}

/* Run the filter */
void BsplineMultiReg::Update(Global &var, std::vector<Parameters> &par)

{
    //    const unsigned int GridNode = 6;
    //    const unsigned int level_num = 3;
    const unsigned int thread_num = 4;
    m_currentLevel = 0;



    //    InitializeTransform(m_levelNumber);

    /* iterate through levels */
    while (m_currentLevel != var.levelnumber) {
        std::cout<<"Resolution "<<m_currentLevel<<"----------------------------------------------------"<<std::endl;

    _registration = NULL;
    _registration = RegistrationType::New();
    _registration->SetNumberOfThreads(thread_num);

    SetImages(_registration,m_fixedImage[m_currentLevel],
              m_movingImage[m_currentLevel]
              );

    SetTransform(_registration,par[m_currentLevel].transformtype);
    SetMetric(_registration,par[m_currentLevel].metrictype);
    SetInterpolator(_registration,par[m_currentLevel].interpolatortype);
    SetOptimizer(_registration,par[m_currentLevel].optimizertype);

    std::cout<<_registration->GetTransform()->GetNameOfClass()<<std::endl;
    std::cout<<_registration->GetOptimizer()->GetNameOfClass()<<std::endl;
    std::cout<<_registration->GetMetric()->GetNameOfClass()<<std::endl;
    std::cout<<_registration->GetInterpolator()->GetNameOfClass();


    BuildTransform(par[m_currentLevel].gridnode);
    if (m_currentLevel != 0) { GoToNextResolution(); }

    BuildOptimizer(_registration, par[m_currentLevel]
                   );

    AddObserver(_registration, par[m_currentLevel].optimizertype
                );

    Run();
//    std::cout<<m_transform[0]->GetNumberOfParameters();
    m_currentLevel++;
}


}


void BsplineMultiReg::Write(Global &var)
{
/* Write to output image and deform field */
WarpToImage(_registration, m_fixedImage.back(),
             m_movingImage.back(), var.outputfile
             );

GenerateDeformation(_registration, m_fixedImage.back(),
                    m_movingImage.back(), var.outputdeformfile
                    );
}


/* Set Multi levels number */
//void BsplineMultiReg::InitializeTransform(const unsigned int &level)
//{
//    m_transform.reserve(level);
//    for (unsigned int i=0; i<level; i++) {
//        BSplineTransformType::Pointer transform = BSplineTransformType::New();
//        m_transform.push_back(transform);
//    }

//}

///* Set Transform */
//void BsplineMultiReg::SetTransform()
//{

//    _registration->SetTransform(m_transform[m_currentLevel]);

//}




void BsplineMultiReg::BuildTransform(const unsigned int & nodes) const
{
  //  An initializer of bspline grid is constructed by calling the method
  //  See also bsplineTransformInitializer


  unsigned int currentGridNodes = nodes;
  BSplineTransformType::PhysicalDimensionsType fixedPhysicalDimensions;
  BSplineTransformType::MeshSizeType meshSize;
  BSplineTransformType::OriginType fixedOrigin;


  for(unsigned int i=0; i<3; i++) {
      fixedOrigin[i] = m_fixedImage[m_currentLevel]->GetOrigin()[i];
      fixedPhysicalDimensions[i] = m_fixedImage[m_currentLevel]->GetSpacing()[i]*
              static_cast<double>(m_fixedImage[m_currentLevel]->GetLargestPossibleRegion().GetSize(i));
  }

  meshSize.Fill(currentGridNodes - SplineOrder);

  typedef BSplineTransformType *TransformPointer;
  TransformPointer transform = dynamic_cast<TransformPointer>(_registration->GetTransform());

  transform->SetTransformDomainOrigin( fixedOrigin );
  transform->SetTransformDomainPhysicalDimensions( fixedPhysicalDimensions );
  transform->SetTransformDomainMeshSize( meshSize );
  transform->SetTransformDomainDirection( m_fixedImage[m_currentLevel]->GetDirection() );

  const unsigned int numberOfParameters =
               transform->GetNumberOfParameters();

  ParametersType parameters( numberOfParameters );

  parameters.Fill( 0.0 );

  // Force parameters update, instead of SetParameters
  //  m_transform[m_currentLevel]->SetParameters(parameters);
  transform->SetParametersByValue( parameters );

  if (m_currentLevel == 0) {
  //  We now pass the parameters of the current transform as the initial
  //  parameters to be used when the registration process starts.
  _registration->SetInitialTransformParameters(parameters);
  _registration->SetTransform(transform);
  }

  //  There are two ways to set the ITK bspline parameters:

  //    SetParameters()
  //    SetCoefficientImage()

  //    SetParameters()
  //    ---------------
  //    You pass in a (const ParametersType&), and then it stashes a pointer
  //    to that in m_InputParametersPointer.  Finally it calls WrapAsImages(),
  //    which maps m_CoefficientImage[] arrays into memory of
  //    m_InputParametersPointer

  //    SetCoefficientImage()
  //    ---------------------
  //    You pass in a (ImagePointer[]), and these get copied into
  //    m_CoefficientImage[] arrays.   m_InputParametersPointer gets reset
  //    to zero.
  // Iterate through coefficient image array to store new coefficients
}


int BsplineMultiReg::Run()
{
    // Add a time probe
    //  itk::TimeProbesCollectorBase chronometer;
    //  itk::MemoryProbesCollectorBase memorymeter;
    std::cout << std::endl << "Starting Registration" << std::endl;

    //  Now registration can begin
    try
    {
        //      memorymeter.Start("Registration");
        //      chronometer.Start("Registration");
        _registration->Update();
        //      chronometer.Stop("Registration");
        //      memorymeter.Stop("Registration");

        std::cout << "Optimizer stop condition: "
                  << _registration->GetOptimizer()->GetStopConditionDescription()
                  << std::endl;
    }
    catch( itk::ExceptionObject & err )
    {
        std::cerr << "ExceptionObject caught !" << std::endl;
        std::cerr << err << std::endl;
        return EXIT_FAILURE;
    }

    typedef BSplineTransformType *TransformPointer;
    TransformPointer transform = dynamic_cast<TransformPointer>(_registration->GetTransform());

    //  Get the transform parameters out
    //  LBFGSBOptimizerType::ParametersType finalParameters = _registration->GetLastTransformParameters();

    // Get current coefficient images, for next resolution
    typedef itk::ImageDuplicator< CoefficientImageType > DuplicatorType;
    for (unsigned int k=0; k<3; k++) {
        m_currentCoefficientImages[k] = CoefficientImageType::New();
        DuplicatorType::Pointer duplicator = DuplicatorType::New();
        duplicator->SetInputImage(transform->GetCoefficientImages()[k]);
        duplicator->Update();
        m_currentCoefficientImages[k] = duplicator->GetOutput();

    }
    //  const unsigned int numberOfIterations = _registration->GetOptimizer()->GetCurrentIteration();
    //  const double bestValue = _registration->GetOptimizer()->GetValue();

    // Print out results
    std::cout << std::endl << std::endl;
    std::cout << "Result = " << std::endl;
    //    std::cout << "Last Trasnsform Parameters = "<<std::endl<<finalParameters<<std::endl;
    //  std::cout << " Iterations    = " << numberOfIterations << std::endl;
    //  std::cout << " Metric value  = " << bestValue          << std::endl;

    // Report the time taken by the registration
    //  chronometer.Report(std::cout);
    //  memorymeter.Report(std::cout);
}


void BsplineMultiReg::GoToNextResolution() const
{

  //  Now we need to initialize the BSpline coefficients of the higher resolution
  //  transform. This is done by first computing the actual deformation field
  //  at the higher resolution from the lower resolution BSpline coefficients.
  //  Then a BSpline decomposition is done to obtain the BSpline coefficient of
  //  the higher resolution transform.
  unsigned int counter = 0;
  ParametersType parametersHigh = _registration->GetTransform()->GetParameters();

  typedef BSplineTransformType::ImageType ParametersImageType;
  typedef ResampleImageFilter<ParametersImageType, ParametersImageType> ResamplerType;
  ResamplerType::Pointer upsampler = ResamplerType::New();

  //  Resample image intensity from a BSpline coefficient image
  typedef BSplineResampleImageFunction<ParametersImageType, double> FunctionType;
  FunctionType::Pointer function = FunctionType::New();

  typedef IdentityTransform<double, 3> IdentityTransfromType;
  IdentityTransfromType::Pointer identity = IdentityTransfromType::New();

  typedef BSplineDecompositionImageFilter<ParametersImageType, ParametersImageType> DecompositionType;
  DecompositionType::Pointer decomposition = DecompositionType::New();

  typedef BSplineTransformType *TransformPointer;
  TransformPointer transform = dynamic_cast<TransformPointer>(_registration->GetTransform());

  for(unsigned int k=0; k<3; k++) {
      upsampler->SetInput(m_currentCoefficientImages[k]);
      upsampler->SetInterpolator(function);
      upsampler->SetTransform(identity);
      upsampler->SetSize(transform->GetCoefficientImages()[k]->GetLargestPossibleRegion().GetSize());
      upsampler->SetOutputSpacing(transform->GetCoefficientImages()[k]->GetSpacing());
      upsampler->SetOutputOrigin(transform->GetCoefficientImages()[k]->GetOrigin());
      upsampler->SetOutputDirection(m_fixedImage[m_currentLevel]->GetDirection());

      decomposition->SetSplineOrder(SplineOrder);
      decomposition->SetInput(upsampler->GetOutput());
      decomposition->Update();

      ParametersImageType::Pointer newCoefficients = decomposition->GetOutput();
      typedef ImageRegionIterator<ParametersImageType> Iterator;
      Iterator it(newCoefficients, transform->GetCoefficientImages()[k]->GetLargestPossibleRegion());
      while(!it.IsAtEnd()) {
          parametersHigh[counter++] = it.Get();
          ++it;
      }
  }

  // Force parameters update, instead of SetParameters
  //  m_transform[m_currentLevel]->SetParameters(parameters);
  transform->SetParametersByValue(parametersHigh);

  //  We now pass the parameters of the high resolution transform as the initial
  //  parameters to be used in a second stage of the registration process.
  _registration->SetInitialTransformParameters(parametersHigh);
  _registration->SetTransform(transform);

}


} // namespace itk
