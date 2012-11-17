Troubleshooting
===============

Before reporting any trouble in installing the library, please ensure
that the following points are correctly configured Most of the
problems should be related to an incorrect configuration
in ``makefile.inc``.

* **Problems when linking**. The environment variable associated with
  dynamic library should be set to \tc{\yroot/yael}.

+--------------+--------------------+------------------------+-------------------------------+
| Architecture | \yael library name | environment variable   | command to check missing libs |
+==============+====================+========================+===============================+
| Linux 32/64  | ``libyael.so``     | ``LD_LIBRARY_PATH``    | ``ldd libyael.so``            |
+--------------+--------------------+------------------------+-------------------------------+
| MacOS X      | ``libyael.dylib``  | ``DYLD_LIBRARY_PATH``  | ``otool -L libyael.dylib``    |
+--------------+--------------------+------------------------+-------------------------------+

  Note that the path to the dynamic library can also be hardcoded in the
  code that used i, with ``-Wl,-rpath,YAELROOT``. This is done for
  Python's interface.

* **Python**. The ``PYTHONPATH`` environment variable should
  point to YAELROOT. The ``*_LIBRARY_PATH`` variable does not need to be set.

* Segfault in Mexfile: MacOS 32/64 bits. If you have a 32 bits Matlab
  version with MacOS, then you should check that you have used the
  flag ``--mac32`` when configuring Yael. 

