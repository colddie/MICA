#include "itkImage.h"
#include "itkImageFileReader.h"
#include "itkImageToVTKImageFilter.h"
#include <itkMetaImageIO.h>

#include "vtkPolyDataMapper.h"
#include "vtkRenderWindow.h"
#include "vtkColorTransferFunction.h"
#include "vtkPiecewiseFunction.h" 
#include "vtkThreshold.h"
#include "vtkRenderer.h"
#include "vtkVolumeProperty.h"
#include "vtkRenderWindowInteractor.h"
#include <vtkDataSetTriangleFilter.h>
#include <vtkVolumeRayCastMapper.h>
#include <vtkSmartPointer.h>
#include <vtkImageData.h>
#include <vtkVolumeRayCastCompositeFunction.h>
#include <vtkVolume.h>




int main( int argc, char **argv ) {

//	class ITK_VTK
//	{
//	private:
//
//	};

	int windowhigh;
	int windowlow;
	const char *window;
	const char *filename;

	if( argc < 2)
	{
		//std::cout<<"argument error."<<std::endl;
		std::cout<<"usage: "<<argv[0]<<" "<<"imageFile"<<std::endl;

		return -1;
	}

	else if(argc == 2 || argc > 3)
	{
		std::cout<<"incorrect opacity parameters,set to default CT window"<<std::endl;
		windowhigh = 400;
		windowlow = 40;
	}
	else
	{
		window = argv[2];
		sscanf(window,"%d %d",&windowlow,&windowhigh);
		std::cout<<window<<std::endl;
		std::cout<<windowlow<<" "<<windowhigh<<std::endl;
	}

	filename = argv[1];

  //
  typedef itk::Image< unsigned short, 3 >    	 ImageType;

  // Declare a filter
  typedef itk::ImageToVTKImageFilter< ImageType >   FilterType;
  FilterType::Pointer connector = FilterType::New();
  
  // Declare a reader
  typedef itk::ImageFileReader< ImageType > VolumeReaderType;
  VolumeReaderType::Pointer reader = VolumeReaderType::New();
  reader->SetFileName( filename );
  
  //  Connect itk with vtk
  connector->SetInput( reader->GetOutput() );
  
  //  Declare vtk renderwindow
  vtkSmartPointer<vtkRenderer> ren1 =vtkSmartPointer<vtkRenderer>::New();
  vtkSmartPointer<vtkRenderWindow> renWin = vtkSmartPointer<vtkRenderWindow>::New();
  vtkSmartPointer<vtkRenderWindowInteractor> iren = vtkSmartPointer<vtkRenderWindowInteractor>::New();
  renWin->AddRenderer( ren1 ); 
  iren->SetRenderWindow( renWin ); 
  // Note that funny arithematic on the
  // SetDesiredUpdateRate - the vtkRenderWindow divides it
  // allocated time across all renderers, and the renderer
  // divides it time across all props. If clip is
  // true then there are two props
  //iren->SetDesiredUpdateRate(frameRate / (1+clip) );

  //  Define false color
  vtkSmartPointer<vtkColorTransferFunction> colorTransferFunction = vtkSmartPointer<vtkColorTransferFunction>::New();
  colorTransferFunction->AddRGBPoint(-1024, 0, 0, 0);
  colorTransferFunction->AddRGBPoint(windowlow, .62, .36, .18);
  colorTransferFunction->AddRGBPoint(windowhigh, .88, .60, .29);
  //colorTransferFunction->AddRGBPoint(windowhigh+1, .0, .0, 0);
  colorTransferFunction->ClampingOff();

  //  Define opacity
  vtkSmartPointer<vtkPiecewiseFunction> opacityTransferFunction = vtkSmartPointer<vtkPiecewiseFunction>::New();
  opacityTransferFunction->AddPoint(-1024,0);
  opacityTransferFunction->AddPoint(windowlow, 0);
  opacityTransferFunction->AddPoint(windowhigh, 1);
  //opacityTransferFunction->AddPoint(windowhigh+1, 0);
  opacityTransferFunction->ClampingOff();


  //
  //vtkPiecewiseFunction *gradientTransferFunction = vtkGradientTransferFunction::New();
  //gradientTransferFunction->AddPoint();



  //  Define volumeproperty
  vtkSmartPointer<vtkVolumeProperty> volumeProperty = vtkSmartPointer<vtkVolumeProperty>::New();
  volumeProperty->SetColor( colorTransferFunction );
  volumeProperty->SetScalarOpacity( opacityTransferFunction );
  // volumeProperty->SetGradientOpacity();
  volumeProperty->ShadeOn();
  volumeProperty->SetInterpolationTypeToLinear(); 
//  volumeProperty->SetAmbient(0.4);
//  volumeProperty->SetDiffuse(0.6);
//  volumeProperty->SetSpecular(0.2);
  
  //  Raycast mapper
  vtkSmartPointer<vtkVolumeRayCastCompositeFunction>  compositeFunction = vtkSmartPointer<vtkVolumeRayCastCompositeFunction>::New();
  vtkSmartPointer<vtkVolumeRayCastMapper> volumeMapper = vtkVolumeRayCastMapper::New();
  volumeMapper->SetVolumeRayCastFunction( compositeFunction );
  volumeMapper->SetInput( connector->GetOutput() );
  volumeMapper->SetBlendModeToComposite();

  //  Set mapper and volumeproperty to volume
  vtkSmartPointer<vtkVolume> volume = vtkSmartPointer<vtkVolume>::New();
  volume->SetMapper( volumeMapper ) ; 
  volume->SetProperty ( volumeProperty );

  //  Add volume to renderwindow
  ren1->AddVolume( volume ); 
  ren1->SetBackground(0.1,0.21,0.31); 

  renWin->SetWindowName("Volume"); 
  renWin->SetSize(500,500); 
  renWin->Render(); 

  // Set up an initial view of the volume.  The focal point will be the
  // center of the volume, and the camera position will be 400mm to the
  // patient's left (which is our right).
  //  vtkCamera *camera = ren->GetActiveCamera();
  //  double *c = volume->GetCenter();
  //  camera->SetFocalPoint(c[0], c[1], c[2]);
  //  camera->SetPosition(c[0] + 400, c[1], c[2]);
  //  camera->SetViewUp(0, 0, -1);
  iren->Start(); 

//  opacityTransferFunction->Delete();
//  colorTransferFunction->Delete();
////  gradientTransferFunction->Delete();
//  volumeProperty->Delete();
//  volumeMapper->Delete();
//  ren1->Delete();
//  renWin->Delete();
//  iren->Delete();

  return 0;
}
