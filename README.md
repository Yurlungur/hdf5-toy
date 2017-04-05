hdf5 Toy
========

Author: Jonah Miller (jonah.maxwell.miller@gmail.com)

This is a small script showing how to use and read hdf5 files in C++
and Python. There are two included programs:

1. `hello-hdf5.cpp` is a C++ script that writes a simple dataset to an
   hdf5 file using the C++ API. After writing to the file, it closes
   the file and then re-opens it and expands the dataset.

2. `hello-hdf5.py` is a Python script that uses the `h5py` module to
   read the HDF5 file.

# Using

Just clone the repository and type `make test`.
