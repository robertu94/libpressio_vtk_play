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

#include "libpressio_vtk.h"

void vtkLibPressioImportExecutive::PrintSelf(ostream& os, vtkIndent indent)  {
  this->Superclass::PrintSelf(os, indent);
}

vtkLibPressioImport::vtkLibPressioImport() {
    this->SetNumberOfInputPorts(0);
    vtkExecutive* exec = vtkLibPressioImportExecutive::New();
    this->SetExecutive(exec);

    this->ScalarArrayName = nullptr;
    this->SetScalarArrayName("scalars");
  }
vtkLibPressioImport::~vtkLibPressioImport() {
    this->SetScalarArrayName(nullptr);
  }

void vtkLibPressioImport::PrintSelf(ostream& os, vtkIndent indent) {
  this->Superclass::PrintSelf(os,indent);
  os << indent << "LibPressioData: " << lp_data << "\n";
}

void vtkLibPressioImport::TakePressioData(pressio_data const& data) {
  lp_data = pressio_data::clone(data);
}
void vtkLibPressioImport::TakePressioData(pressio_data && data) {
  lp_data = data;
}

int vtkLibPressioImport::RequestInformation(vtkInformation* vtkNotUsed(request), vtkInformationVector** vtkNotUsed(inputVector), vtkInformationVector* outputVector)  {
  vtkInformation* outInfo = outputVector->GetInformationObject(0);
  int wholeExtent[6] = {
    0,
    static_cast<int>(lp_data.get_dimension(0)),
    0,
    static_cast<int>(lp_data.get_dimension(1)),
    0,
    static_cast<int>(lp_data.get_dimension(2))
  };
  outInfo->Set(vtkStreamingDemandDrivenPipeline::WHOLE_EXTENT(), wholeExtent, 6);
  outInfo->Set(vtkDataObject::SPACING(), this->DataSpacing, 3);
  outInfo->Set(vtkDataObject::ORIGIN(), this->DataOrigin, 3);
  outInfo->Set(vtkDataObject::DIRECTION(), this->DataDirection, 9);
  int dataScalarType;
  switch(lp_data.dtype()) {
    case pressio_int8_dtype:
      dataScalarType = VTK_SIGNED_CHAR;
      break;
    case pressio_int16_dtype:
      dataScalarType = VTK_SHORT;
      break;
    case pressio_int32_dtype:
      dataScalarType = VTK_INT;
      break;
    case pressio_int64_dtype:
      dataScalarType = VTK_LONG_LONG;
      break;
    case pressio_uint8_dtype:
    case pressio_byte_dtype:
      dataScalarType = VTK_UNSIGNED_CHAR;
      break;
    case pressio_uint16_dtype:
      dataScalarType = VTK_UNSIGNED_SHORT;
      break;
    case pressio_uint32_dtype:
      dataScalarType = VTK_UNSIGNED_INT;
      break;
    case pressio_uint64_dtype:
      dataScalarType = VTK_UNSIGNED_LONG_LONG;
      break;
    case pressio_float_dtype:
      dataScalarType = VTK_FLOAT;
      break;
    case pressio_double_dtype:
      dataScalarType = VTK_DOUBLE;
      break;
  }

  vtkDataObject::SetPointDataActiveScalarInfo(outInfo, dataScalarType, 1);
  return 1;
}

void vtkLibPressioImport::ExecuteDataWithInformation(vtkDataObject* output, vtkInformation* outInfo)  {
    vtkImageData* data = vtkImageData::SafeDownCast(output);
    data->SetExtent(0, 0, 0, 0, 0, 0);
    data->AllocateScalars(outInfo);
    void* ptr = this->lp_data.data();
    vtkIdType size = lp_data.num_elements();
    int wholeExtent[6] = {
      0,
      static_cast<int>(lp_data.get_dimension(0)),
      0,
      static_cast<int>(lp_data.get_dimension(1)),
      0,
      static_cast<int>(lp_data.get_dimension(2))
    };
    data->SetExtent(wholeExtent);
    data->GetPointData()->GetScalars()->SetVoidArray(ptr, size, 1);
    data->GetPointData()->GetScalars()->SetName(this->ScalarArrayName);
}

vtkStandardNewMacro(vtkLibPressioImport);

vtkStandardNewMacro(vtkLibPressioImportExecutive);

vtkTypeBool vtkLibPressioImportExecutive::ProcessRequest(
    vtkInformation* request, vtkInformationVector** inInfoVec, vtkInformationVector* outInfoVec
    )
{
    if (this->Algorithm && request->Has(REQUEST_INFORMATION()))
  {
    vtkLibPressioImport* ii = vtkLibPressioImport::SafeDownCast(this->Algorithm);
  }

  return this->Superclass::ProcessRequest(request, inInfoVec, outInfoVec);
}

vtkStandardNewMacro(vtkLibPressioExport);
void vtkLibPressioExport::PrintSelf(ostream& os, vtkIndent indent)  {
  this->Superclass::PrintSelf(os, indent);
}
void vtkLibPressioExport::Export(pressio_data* out) {

}

int vtkLibPressioExport::RequestData(vtkInformation* request, vtkInformationVector** inputVector,
  vtkInformationVector* outputVector) {
  return 1;
}
