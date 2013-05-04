#ifndef REGISTRATIONINTERFACECOMMAND_H
#define REGISTRATIONINTERFACECOMMAND_H

#include "itkRegularStepGradientDescentOptimizer.h"
#include "itkVersorRigid3DTransformOptimizer.h"
#include "itkGradientDescentOptimizer.h"
#include "itkWeakPointer.h"


//  The following section of code implements a Command observer
//  that will control the modification of optimizer parameters
//  at every change of resolution level.
//


template <typename TRegistration, typename TOptimizer>
class RegistrationInterfaceCommand : public itk::Command
{
public:
  typedef  RegistrationInterfaceCommand   Self;
  typedef  itk::Command                   Superclass;
  typedef  itk::SmartPointer<Self>        Pointer;
  itkNewMacro( Self );

protected:
  RegistrationInterfaceCommand() {};

public:
  typedef itk::RegularStepGradientDescentOptimizer RegularStepType;

  typedef RegularStepType *          RSGD;

  typedef   TRegistration                              RegistrationType;
  typedef   RegistrationType *                         RegistrationPointer;
  typedef   TOptimizer                                 OptimizerType;
  typedef   OptimizerType *                            OptimizerPointer;
  void Execute(itk::Object * object, const itk::EventObject & event)
  {
    if( !(itk::IterationEvent().CheckEvent( &event )) )
      {
      return;
      }

    RegistrationPointer registration =
                        dynamic_cast<RegistrationPointer>( object );
    OptimizerPointer optimizer = dynamic_cast< OptimizerPointer >(
                       registration->GetOptimizer() );
    std::cout << "-------------------------------------" << std::endl;
    std::cout << "MultiResolution Level : ";
//              << registration->GetCurrentLevel()  << std::endl;
//    std::cout << std::endl;
//    std::cout << registration->GetLastTransformParameters() << std::endl;

    //  Update optimizer with levels

    if ( std::string(optimizer->GetNameOfClass()) == "RegularStepGradientDescent") {
        RSGD optimizer = dynamic_cast< RSGD >(
                           registration->GetOptimizer() );

//        if ( registration->GetCurrentLevel() == 0 )
//      {
//      optimizer->SetMaximumStepLength( 16.00 );
//      optimizer->SetMinimumStepLength(  0.01 );
//      }
//    else
//      {
//      optimizer->SetMaximumStepLength( optimizer->GetMaximumStepLength() / 4.0 );
//      optimizer->SetMinimumStepLength( optimizer->GetMinimumStepLength() / 10.0 );
//      }
      }

//    if ( std::string(m_optimizer->GetNameOfClass()) == "LBFGSBOptimizer") {
//    optimizer->SetCostFunctionConvergenceFactor (1e+5);
//    optimizer->SetProjectedGradientTolerance (1e-5);
//    }
  }
  void Execute(const itk::Object * , const itk::EventObject & )
    { return; }

  /**
   * Set Optimizer
   */
  void SetRegistration(RegistrationType *registration, OptimizerType *optimizer)
  {
      m_registration = registration;
//      m_optimizer = dynamic_cast< OptimizerPointer >(
//                         m_registration->GetModifiableOptimizer() );
      m_optimizer = optimizer;
  }

  void AddResolutionObserver()
  {
      m_registration->AddObserver(itk::IterationEvent(), this);
  }

private:
  /**
   *  WeakPointer to the Optimizer
   */
  typename itk::WeakPointer<OptimizerType>   m_optimizer;
  typename itk::WeakPointer<RegistrationType> m_registration;
};



#endif // REGISTRATIONINTERFACECOMMAND_H
