#include <array>
#include <libpressio.h>
#include <libpressio_ext/io/posix.h>
#include <vtkNew.h>
#include <vtkImageReader.h>
#include <vtkImageActor.h>
#include <vtkImageMapper3D.h>
#include <vtkMapper.h>
#include <vtkRenderer.h>
#include <vtkNamedColors.h>
#include <vtkRenderWindowInteractor3D.h>
#include <vtkInteractorStyle3D.h>
#include <vtkRenderWindow.h>
#include <vtkContourFilter.h>
#include <vtkImageData.h>
#include <vtkPolyDataMapper.h>
#include <vtkCutter.h>
#include <vtkPlane.h>


int main()
{
  vtkNew<vtkImageReader2> reader;
  reader->SetFileName("/home/runderwood/git/datasets/hurricane/100x500x500/PRECIPf48.bin.f32");
  reader->SetDataExtent(0, 500, 0, 500, 0, 100);
  reader->SetDataScalarTypeToFloat();
  reader->Update();

  vtkNew<vtkPlane> plane;
  plane->SetOrigin(reader->GetOutput()->GetCenter());
  plane->SetNormal(-.28, 0, .9);

  vtkNew<vtkCutter> cutter;
  cutter->SetInputConnection(reader->GetOutputPort());
  cutter->SetCutFunction(plane);

  vtkNew<vtkPolyDataMapper> mapper;
  mapper->SetInputConnection(cutter->GetOutputPort());
  mapper->SetScalarRange(reader->GetOutput()->GetScalarRange());

  vtkNew<vtkActor> actor;
  actor->SetMapper(mapper);

  vtkNew<vtkNamedColors> colors;
  vtkNew<vtkRenderer> renderer;
  renderer->AddActor(actor);
  renderer->SetBackground(colors->GetColor3d("DarkSlateGray").GetData());
  renderer->ResetCamera();

  vtkNew<vtkRenderWindowInteractor> renderWindowInteractor;
  vtkNew<vtkInteractorStyle> style;

  vtkNew<vtkRenderWindow> renderWindow;
  renderWindow->AddRenderer(renderer);
  renderWindow->SetWindowName("MetaImageReader");


  renderWindowInteractor->SetInteractorStyle(style);
  renderWindowInteractor->SetRenderWindow(renderWindow);
  renderWindow->Render();
  renderWindowInteractor->Initialize();
  renderWindowInteractor->Start();
  

}
