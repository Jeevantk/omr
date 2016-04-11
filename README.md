# omr
Its under devolopment only.Now I have used tesseract for detection and removal of text in the sheet music

An implementation of Optical Music Recognition

#Installation instructions

dependencies for opencv

sudo apt-get install build-essential

sudo apt-get install cmake git libgtk2.0-dev pkg-config libavcodec-dev libavformat-dev libswscale-dev

sudo apt-get install python-dev python-numpy libtbb2 libtbb-dev libjpeg-dev libpng-dev libtiff-dev libjasper-dev
libdc1394-22-dev

cd ~

git clone https://github.com/Itseez/opencv

cd opencv

mkdir build

cd build

there are many other flags which can be given for cmake.Check opencv build instructions for those

cmake -D CMAKE_BUILD_TYPE=RELEASE -D CMAKE_INSTALL_PREFIX=/usr/local -DINSTALL_CREATE_DISTRIB=ON ..

Use -j(%number of processosors for optimisation)

make -j4 

sudo make install

installing Tesseract

sudo apt-get install tesseract-ocr

sudo apt-get install tesseract-ocr-eng

An implementattion of tesseract is there in OpenCV contrib Modules.SI have used that module.So Opencv contrib is required

 cd ~
 
 git clone https://github.com/Itseez/opencv_contrib.git
 
 now copy this folder to opencv/build
 
 change directory into opencv build folder
 
 the exact name may vary depending on your folder name
 
 cmake -DOPENCV_EXTRA_MODULES_PATH= opencv_contrib/modules -DBUILD_opencv_legacy=OFF ..
 
 make -j4
 
 sudo make install
 
If you get no errors then you are good to go
 
 compile the program by using the make file
 

