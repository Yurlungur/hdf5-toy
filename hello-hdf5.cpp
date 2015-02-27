#include <iostream>
#include <string>
#include <H5Cpp.h>
#define DATATYPE H5::PredType::NATIVE_DOUBLE

// these can ben changed later
const std::string FILENAME("test.h5"); 
const std::string FIELDNAME("scalarField");
const std::string TIMENAME("time");
const std::string DATANAME("/data");
const std::string PROPERTIESNAME("/elementProperties");
const int TNDIMS = 1;
const int NDIMS = 3;
const int DIMS[] = {3,3,3};


int main() {
  double data[DIMS[0]][DIMS[1]][DIMS[2]];
  double times[DIMS[0]];
  double count = 0.0;
  for (int i=0; i < DIMS[0]; i++) {
    times[i] = 1.5*i;
    for (int j = 0; j < DIMS[1]; j++) {
      for (int k = 0; k < DIMS[2]; k++) {
	count+=0.5;
	data[i][j][k] = count;
	}
    }
  }

  hsize_t dimsf[NDIMS];
  hsize_t dimst[TNDIMS];
  for (int i = 0; i < NDIMS; i++) {
    dimsf[i] = DIMS[i];
  }
  for (int i = 0; i < TNDIMS; i++) {
    dimst[i] = dimsf[i];
  }
  // The file
  H5::H5File file(FILENAME,H5F_ACC_TRUNC);
  // The scalar field
  H5::DataSpace fieldDataspace(NDIMS,dimsf);
  H5::IntType datatype(DATATYPE);
  H5::Group propertiesGroup(file.createGroup(PROPERTIESNAME));
  H5::Group dataGroup(file.createGroup(DATANAME));
  H5::DataSet fieldDataset =
    dataGroup.createDataSet(FIELDNAME,
			    datatype,
			    fieldDataspace);
  fieldDataset.write(data,DATATYPE);
  fieldDataset.close();
  // The times
  H5::DataSpace timeDataspace(TNDIMS,dimst);
  H5::DataSet timeDataset =
    dataGroup.createDataSet(TIMENAME,
			    datatype,
			    timeDataspace);
  timeDataset.write(times,DATATYPE);
  timeDataset.close();
  file.close();
  return 0;
}
