============================================
MPEG-A Photo-Player Reference Software

       ISO/IEC 23000-3 PDAM1

            February 2007

       Mitsubishi Electric ITE
    Visual Information Laboratory
============================================


This file describes steps necessary to compile the MPEG-A Photo-Player Reference
Software. Project files for Microsoft Visual C++ 2005 are included in this 
distribution.

For the documentation of the program itself see:

./PhotoPlayer/win32/README.txt

Note that compiled binary (.exe) files for win32 are included in:

./bin

To run the compiled executables you will need a copy of xerces-c_2_7.dll which has
been compiled with Visual C++ 2005. See below for details of how to obtain the
Xerces-C++ library.



External libraries required for compilation:
============================================

* Independent JPEG Group JPEG software

This software is based in part on the work of the Independent JPEG Group. The
Independent JPEG Group's JPEG software source code can be obtained from:

http://www.ijg.org/

The version used in the development of the current software is 6b-27/03/1998
Once downloaded, the source files should be copied to:

./jpeglib

Copy jconfig.vc to jconfig.h as described in the installation instructions
----------------------------------------------------------------------------------

* Xerces-C++ XML Parser

This software links against the Xerces-C++ XML Parser, which can be obtained from:

http://xml.apache.org/xerces-c/

Xerces-C++ Version 2.7.0 was  used in the development of the current software.
Once downloaded and built, header files should be copied to:

./xercesc/include/xercesc

and object files (*.lib) to:

./xercesc/lib

----------------------------------------------------------------------------------


Please note the important copyright and liability disclaimer in the respective
source files.
