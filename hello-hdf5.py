#!/usr/bin/env python
from __future__ import print_function
import numpy as np
import h5py

print("Python reading hdf5")
with h5py.File("test.h5",'r') as f:
    times = f['/data/time'].value
    scalarfield = f['/data/scalarField'].value
print("Read finished. Times are:")
print(times)

print("And integral of scalar field at each time is:")
integrated_field = np.sum(np.sum(scalarfield,
                                 axis=1),
                          axis=1)
print(integrated_field)
print("Done.")

