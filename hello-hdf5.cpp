#include <iostream>
#include <string>
#include <H5Cpp.h>
#define DATATYPE H5::PredType::NATIVE_DOUBLE

using std::cout;
using std::endl;

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
  double timestoadd[1];
  double datatoadd[DIMS[1]][DIMS[2]];

  cout << "Defining parameters." << endl;
  for (int i=0; i < DIMS[0]; i++) {
    times[i] = 1.5*i;
    for (int j = 0; j < DIMS[1]; j++) {
      for (int k = 0; k < DIMS[2]; k++) {
	count+=0.5;
	data[i][j][k] = count;
	}
    }
  }
  timestoadd[0] = 0.25;
  for (int i = 0; i < DIMS[1]; i++) {
    for (int j = 0; j < DIMS[2]; j++) {
      count+=0.5;
      datatoadd[i][j] = count;
    }
  }

  hsize_t dimsf[NDIMS];
  hsize_t dimst[TNDIMS];
  hsize_t maxdimsf[NDIMS];
  hsize_t maxdimst[TNDIMS];
  hsize_t fieldChunk[NDIMS];
  hsize_t timeChunk[TNDIMS];
  hsize_t tOffset[TNDIMS];
  hsize_t fOffset[NDIMS];
  for (int i = 0; i < NDIMS; i++) {
    dimsf[i] = DIMS[i];
    maxdimsf[i] = DIMS[i];
    fieldChunk[i] = DIMS[i];
    fOffset[i] = 0;
  }
  for (int i = 0; i < TNDIMS; i++) {
    dimst[i] = dimsf[i];
    maxdimst[i] = DIMS[i];
    timeChunk[i] = dimsf[i];
    tOffset[i] = 0;
  }
  maxdimsf[0] = H5S_UNLIMITED;
  maxdimst[0] = H5S_UNLIMITED;
  fieldChunk[0] = 1;
  timeChunk[0] = 1;

  cout << "Opening the file." << endl;
  
  // Open the file
  // The file
  H5::H5File file(FILENAME,H5F_ACC_TRUNC);

  cout << "Create a parameter list for chunking." << endl;
  H5::DSetCreatPropList fieldParms;
  H5::DSetCreatPropList timeParms;
  fieldParms.setChunk(NDIMS,fieldChunk);
  timeParms.setChunk(TNDIMS,timeChunk);


  // Write the data

  // the scalar field
  H5::DataSpace fieldDataspace(NDIMS,dimsf,maxdimsf);
  H5::IntType datatype(DATATYPE);
  H5::Group propertiesGroup(file.createGroup(PROPERTIESNAME));
  H5::Group dataGroup(file.createGroup(DATANAME));
  H5::DataSet fieldDataset =
    dataGroup.createDataSet(FIELDNAME,
			    datatype,
			    fieldDataspace,
			    fieldParms);
  fieldDataset.write(data,DATATYPE);
  fieldDataset.close();
  // The times
  H5::DataSpace timeDataspace(TNDIMS,dimst,maxdimst);
  H5::DataSet timeDataset =
    dataGroup.createDataSet(TIMENAME,
			    datatype,
			    timeDataspace,
			    timeParms);
  timeDataset.write(times,DATATYPE);
  timeDataset.close();
  cout << "closing file." << endl;
  file.close();

  // Reopen everything
  H5::DataSpace timeDataspaceSource(TNDIMS,timeChunk);
  H5::DataSpace fieldDataspaceSource(NDIMS,fieldChunk);
  H5::DataSpace timeDataspaceDest;
  H5::DataSpace fieldDataspaceDest;
  cout << "Reopening everything." << endl;
  file.openFile(FILENAME,H5F_ACC_RDWR);
  fieldDataset = file.openDataSet(DATANAME + "/" + FIELDNAME);
  timeDataset = file.openDataSet(DATANAME + "/" + TIMENAME);
    cout << "Extend the datasets." << endl;
  dimsf[0] += 1;
  dimst[0] += 1;
  fieldDataset.extend(dimsf);
  timeDataset.extend(dimst);
  cout << "Write to the datasets." << endl;
  fOffset[0] = 3;
  tOffset[0] = 3;
  timeDataspaceDest = timeDataset.getSpace();
  timeDataspaceDest.selectHyperslab(H5S_SELECT_SET,
				    timeChunk,
				    tOffset);
  timeDataset.write(timestoadd,
		    DATATYPE,
		    timeDataspaceSource,
		    timeDataspaceDest);
  fieldDataspaceDest = fieldDataset.getSpace();
  fieldDataspaceDest.selectHyperslab(H5S_SELECT_SET,
				     fieldChunk,
				     fOffset);
  fieldDataset.write(datatoadd,
		     DATATYPE,
		     fieldDataspaceSource,
		     fieldDataspaceDest);
  file.close();
  return 0;
}
