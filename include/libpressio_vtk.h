#ifndef vtkLibPressioImport_h
#define vtkLibPressioImport_h

#include <libpressio_ext/cpp/data.h>
#include <libpressio_ext/cpp/printers.h>
#include <vtkInformation.h>
#include <vtkDataObject.h>
#include <vtkObjectFactory.h>
#include <vtkDataArray.h>
#include <vtkPointData.h>
#include <vtkImageData.h>
#include <vtkInformationVector.h>
#include <vtkIOImageModule.h> // For export macro
#include <vtkImageAlgorithm.h>
#include <vtkStreamingDemandDrivenPipeline.h>

#include "vtkIOImageModule.h" // For export macro
#include "vtkStreamingDemandDrivenPipeline.h"

class VTKIOIMAGE_EXPORT vtkLibPressioImportExecutive : public vtkStreamingDemandDrivenPipeline
{
public:
  static vtkLibPressioImportExecutive* New();
  vtkTypeMacro(vtkLibPressioImportExecutive, vtkStreamingDemandDrivenPipeline);
  void PrintSelf(ostream& os, vtkIndent indent) override;

  /**
   * Override to implement some requests with callbacks.
   */
  vtkTypeBool ProcessRequest(
    vtkInformation* request, vtkInformationVector** inInfo, vtkInformationVector* outInfo) override;

protected:
  vtkLibPressioImportExecutive() = default;
  ~vtkLibPressioImportExecutive() override = default;

private:
  vtkLibPressioImportExecutive(const vtkLibPressioImportExecutive&) = delete;
  void operator=(const vtkLibPressioImportExecutive&) = delete;
};

class VTKIOIMAGE_EXPORT vtkLibPressioImport : public vtkImageAlgorithm {
  public:
  vtkLibPressioImport();
  ~vtkLibPressioImport();

  static vtkLibPressioImport* New();
  vtkTypeMacro(vtkLibPressioImport, vtkImageAlgorithm);
  void PrintSelf(ostream& os, vtkIndent indent) override;

  void TakePressioData(pressio_data const& data);
  void TakePressioData(pressio_data && data);
  vtkSetVector3Macro(DataSpacing, double);
  vtkGetVector3Macro(DataSpacing, double);
  vtkSetVector3Macro(DataOrigin, double);
  vtkGetVector3Macro(DataOrigin, double);
  vtkSetVectorMacro(DataDirection, double, 9);
  vtkGetVectorMacro(DataDirection, double, 9);
  vtkSetStringMacro(ScalarArrayName);
  vtkGetStringMacro(ScalarArrayName);

  protected:
  int RequestInformation(vtkInformation* vtkNotUsed(request), vtkInformationVector** vtkNotUsed(inputVector), vtkInformationVector* outputVector) override;
  void ExecuteDataWithInformation(vtkDataObject* output, vtkInformation* outInfo) override;

  private:
  vtkLibPressioImport(const vtkLibPressioImport&) = delete;
  void operator=(const vtkLibPressioImport&) = delete;
  double DataSpacing[3];
  double DataOrigin[3];
  double DataDirection[9];
  char* ScalarArrayName;
  pressio_data lp_data;
};

class VTKIOIMAGE_EXPORT vtkLibPressioExport : public vtkImageAlgorithm
{
public:
  static vtkLibPressioExport* New();
  vtkTypeMacro(vtkLibPressioExport, vtkImageAlgorithm);
  void PrintSelf(ostream& os, vtkIndent indent) override;
  void Export(pressio_data* out);
  int RequestData(vtkInformation* request, vtkInformationVector** inputVector,
    vtkInformationVector* outputVector) override;
};

#endif
