#include "affineMultiReg.h"
#include "toolBase.h"

#include "itkCenteredTransformInitializer.h"




namespace itk{

 const unsigned int Dimension = 3;

 /* Initialize image and transform pyramids */
 void AffineMultiReg::Initialize(Global &var)
 {

  InitializePyramid(var.levelnumber,var.inputfile1,var.inputfile2,
                    var.downsample,m_fixedImage,m_movingImage
                    );

 }

/* Run the filter */
void AffineMultiReg::Update(Global &var, std::vector<Parameters> &par)
{

    //    const unsigned int level_num = 3;
    const unsigned int thread_num = 4;
    m_currentLevel = 0;


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


    BuildTransform();
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



/* Write to output image and deform field */
void AffineMultiReg::Write(Global &var)
{

WarpToImage(_registration, m_fixedImage.back(),
             m_movingImage.back(), var.outputfile
             );

GenerateDeformation(_registration, m_fixedImage.back(),
                    m_movingImage.back(), var.outputdeformfile
                    );
}




///* Set Multi levels number */
//void AffineMultiReg::InitializeTransform(const unsigned int &level)
//{
//    m_transform.reserve(level);
//    for (unsigned int i=0; i<level; i++) {
//        AffineTransformType::Pointer transform = AffineTransformType::New();
//        m_transform.push_back(transform);
//    }

//}

///* Set Transform */
//void AffineMultiReg::SetTransform()
//{

//    _registration->SetTransform(m_transform[m_currentLevel]);

//}




void AffineMultiReg::BuildTransform() const
{

    typedef AffineTransformType *TransformPointer;
    TransformPointer transform = dynamic_cast<TransformPointer>(_registration->GetTransform());

    //An initializer is constructed by calling the method and assigning
    //  the result to a smartpointer.
    typedef CenteredTransformInitializer
            <AffineTransformType, TImage, TImage> TransformInitializerType;
    TransformInitializerType::Pointer initializer = TransformInitializerType::New();
    initializer->SetTransform(transform);
    initializer->SetFixedImage(m_fixedImage.front());
    initializer->SetMovingImage(m_fixedImage.front());

    //   The use of the geometrical centers is selected by calling
    //   GeometryOn() while the use of center of mass is selected by
    //   calling MomentsOn(). Below we select the center of mass mode.
    initializer->MomentsOn();
    // initializer->GeometryOn();

    //   The resulting values will be passed directly to the transform
    initializer->InitializeTransform();

      if (m_currentLevel == 0) {
    //  We now pass the parameters of the current transform as the initial
    //  parameters to be used when the registration process starts.
    _registration->SetInitialTransformParameters(transform->GetParameters());
    _registration->SetTransform(transform);
  }
}


int AffineMultiReg::Run()
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

  //  Get the transform parameters out
  LBFGSBOptimizerType::ParametersType finalParameters = _registration->GetLastTransformParameters();
  //  const unsigned int numberOfIterations = _registration->GetOptimizer()->GetCurrentIteration();
  //  const double bestValue = _registration->GetOptimizer()->GetValue();
  m_currentParameters.clear();
  m_currentParameters.SetSize(finalParameters.GetSize());
  m_currentParameters = finalParameters;
  //  std::cout<<finalParameters<<std::endl;
  //  std::cout<<m_currentParameters;

//  // Print out results
//  std::cout << std::endl << std::endl;
//  std::cout << "Result = " << std::endl;
//  std::cout << "Last Trasnsform Parameters = "<<std::endl<<finalParameters<<std::endl;
//  std::cout << " Iterations    = " << numberOfIterations << std::endl;
//  std::cout << " Metric value  = " << bestValue          << std::endl;

  // Report the time taken by the registration
//  chronometer.Report(std::cout);
//  memorymeter.Report(std::cout);

}




void AffineMultiReg::GoToNextResolution() const
{

    _registration->SetInitialTransformParameters(m_currentParameters);

}

}
