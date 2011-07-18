This document lists the procedures to install my own libraries and related 3rd part libraries. My libraries so far has been tested on Microsoft Visual Studio 6, Microsoft Visual Studio .NET (VC2003), Microsoft Visual Studio 2005 (VC2005) and Linux. It also requires several public libraries. The compilation and installation of those libraries are listed in the section below. 

[Prerequisite]
* CMake
My libraries require CMake to generate the makefiles. Please install CMake and add the path of the CMake binaries to the PATH environment variable.

[Public Libraries]

The following libraries are required:
1.	OpenCV
Please download and install OpenCV 2.X. 

2.	CUDA
Both CUDA Toolkit and GPU Computing SDK code samples are needed. Note that your hardware does not beed to support CUDA since I only need their software features. They can be downloaded from the official website of nVidia CUDA.

3.	GLUT/GLUI/GLEW
My programs also rely on several OpenGL libraries, including 
	a. 	GLUT/FREEGLUT
	b. 	GLEW
	c.	GLUI
	d.	LIB3DS

The headers, pre-compiled libraries, and sources codes for these libraries are contained in another archive lib.zip. In the archive, the header are placed in the folder 'include/,' The pre-compiled libraries are in the folder 'lib/,' and the source codes are in the folder 'src/.' If you need to recompile the libraries, the generated .lib should be placed under the folder 'lib/'.

[Procedures for Unix/Linux]
Once CMake and these libraries have been installed, now you can begin to build the library. For Unix/Linux users, please follow the steps below:

1. Configure CMake. 
Change to the folder of the libraries and type the following command:

cmake -i . -g "Unix Makefiles"

Then you need to provide the paths of CUDA, CUDA SDK, the CMake installation path (CMAKE_INSTALL_PREFIX), OpenCV (OPENCV_DIR), OpenCV Version (OPENCV_VER), and the location to the 3rd party libraries including GLUT/GLUI/GLEW (THIRDPARTY_DIR). 

2. Now you can type the command 'make' to build the libraries.

3. Type the command 'make install' to copy the headers and libraries to the folder 'CMAKE_INSTALL_PREFIX.'

[Procedures for Windows]
Open the cmake-gui, and specify the variables PROJECT_SOURCE_DIR and PROJECT_BINARY_DIR. PROJECT_SOURCE_DIR represents the path of the source codes for my libraries, and PROJECT_BINARY_DIR represents the path to store the intermediate output. Then specify the following cache values:

CMAKE_INSTALL_PREFIX
OPENCV_DIR
OPENCV_VER
THIRDPARTY_DIR

Once these variables have been specified, press the buttons "configure" and then "generate."

2. Open the solution file, which should be in the folder PROJECT_BINARY_DIR. Then rebuild the project.

3. Build the project INSTALL to copy the headers and libraries to the folder 'CMAKE_INSTALL_PREFIX.'
