#include "iterationUpdate.h"



unsigned int RmsCounter = 0;
double MaxRmsE[4] = {0.8, 0.75, 0.4, 0.2};


void CommandIterationUpdate::Execute(const itk::Object *, const itk::EventObject & )
  {
  std::cout << "Warning: The const Execute method shouldn't be called" << std::endl;
  }


void CommandIterationUpdate::Execute(itk::Object *caller, const itk::EventObject & event)
  {
     RegistrationFilterType * filter =
      dynamic_cast<  RegistrationFilterType * >( caller );

     if( !(itk::IterationEvent().CheckEvent( &event )) )
      {
      return;
      }
    if(filter)
      {
      filter->SetMaximumRMSError(MaxRmsE[RmsCounter]);
      std::cout << filter->GetMetric() <<  "  RMS Change: " << filter->GetRMSChange() << std::endl;

       std::cout << "Level Tolerance=  "<<filter->GetMaximumRMSError ()<<std::endl;
  }

}



void CommandResolutionLevelUpdate::Execute(itk::Object *caller, const itk::EventObject & event)
{
    Execute( (const itk::Object *)caller, event);
}

void CommandResolutionLevelUpdate::Execute(const itk::Object *, const itk::EventObject & )
{
    std::cout << "----------------------------------" << std::endl;
    RmsCounter = RmsCounter + 1;
    std::cout << "----------------------------------" << std::endl;
}
