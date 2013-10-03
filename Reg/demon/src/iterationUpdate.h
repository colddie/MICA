#ifndef _iterationUpdate_H_
#define _iterationUpdate_H_


#include "itkCommand.h"
#include "itkImage.h"
#include "itkDemonsRegistrationFilter.h"

//  The following section of code implements a Command observer
//  that will monitor the evolution of the registration process.
//  This observer has a layer of intelligence, for deciding what
//  MaximumRMS convergence criteria to use at every resolution level.
//
class CommandIterationUpdate : public itk::Command
{
public:
  typedef  CommandIterationUpdate   Self;
  typedef  itk::Command             Superclass;
  typedef  itk::SmartPointer<Self>  Pointer;
  itkNewMacro( Self );

protected:
  CommandIterationUpdate() {};

    typedef float                                PixelType;
    typedef itk::Image< PixelType, 3 >           ImageType;
    typedef itk::Vector< float, 3 >              VectorPixelType;
    typedef itk::Image< VectorPixelType, 3 >     DisplacementFieldType;
    typedef itk::DemonsRegistrationFilter< ImageType,
      ImageType, DisplacementFieldType>  RegistrationFilterType;


public:

  void Execute(const itk::Object *, const itk::EventObject & );

  void Execute(itk::Object *caller, const itk::EventObject & event);

};


// The following command observer reports the progress of the registration
// inside a given resolution level.
//
class CommandResolutionLevelUpdate : public itk::Command
{
public:
    typedef  CommandResolutionLevelUpdate   Self;
    typedef  itk::Command                   Superclass;
    typedef  itk::SmartPointer<Self>        Pointer;
    itkNewMacro( Self );

protected:
    CommandResolutionLevelUpdate() {};

public:
    void Execute(itk::Object *caller, const itk::EventObject & event);

    void Execute(const itk::Object *, const itk::EventObject & );

};






#endif
