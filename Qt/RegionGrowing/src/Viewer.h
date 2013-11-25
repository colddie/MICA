#ifndef VIEWER_H
#define VIEWER_H

#include "vtkSmartPointer.h"
#include "vtkInteractorStyleTrackballCamera.h"
#include <QMainWindow>


class Ui_Viewer;
class vtkImageViewer2;
class vtkObject;
class vtkCommand;
class vtkEventQtSlotConnect;
class vtkSphereSource;
class vtkActor;

// Forward Qt class declarations
//class Ui_Viewer;

class Viewer : public QMainWindow
{
    Q_OBJECT
public:
    explicit Viewer(QMainWindow *parent = 0);
    ~Viewer();

signals:
    
public slots:
    void slotOpenFile();
    void slotExit();
    void changeSlice(int slice);

    void mouseMoveCallback(vtkObject*,ulong,void*,void*,vtkCommand*);
    void mouseClickCallback(vtkObject*,ulong,void*,void*,vtkCommand*);
    void Mainfunction();

protected:

    void DisplayImage();

private:

    std::string m_filename;
    Ui_Viewer *ui;
    vtkSmartPointer<vtkImageViewer2> m_viewer2;
    double m_index[3];
    vtkSmartPointer<vtkSphereSource> m_sphereSource;
    vtkSmartPointer<vtkActor> m_actor;

    // must declare as class member instead define in the function
    vtkSmartPointer<vtkEventQtSlotConnect> connection;
};



// Overwrite default interaction style
class MouseInteractorStyle : public vtkInteractorStyleTrackballCamera// Or vtkInteractorImage
{
  public:
    static MouseInteractorStyle* New();
    vtkTypeMacro(MouseInteractorStyle, vtkInteractorStyleTrackballCamera);

    virtual void OnLeftButtonDown()
    {
      std::cout << "Pressed left mouse button." << std::endl;
      // Forward events
//      vtkInteractorStyleTrackballCamera::OnLeftButtonDown();
    }

//    virtual void OnMiddleButtonDown()
//    {
//      std::cout << "Pressed middle mouse button." << std::endl;
//      // Forward events
//      // vtkInteractorStyleTrackballCamera::OnMiddleButtonDown();
//    }

//    virtual void OnRightButtonDown()
//    {
//      std::cout << "Pressed right mouse button." << std::endl;
//      // Forward events
//      vtkInteractorStyleTrackballCamera::OnRightButtonDown();
//    }

    virtual void OnMouseWheelForward()
    {
        std::cout << "Forward scroll mouse wheel." << std::endl;
        // Forward events
//        vtkInteractorStyleTrackballCamera::OnMouseWheelForward();
    }

    virtual void OnMouseWheelBackward()
    {
        std::cout << "Forward scroll mouse wheel." << std::endl;
        // Forward events
//        vtkInteractorStyleTrackballCamera::OnMouseWheelBackward();
    }

};


#endif // VIEWER_H
