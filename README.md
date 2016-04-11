# omr
An implementation of Optical Music Recognition

#Installation instructions
dependencies for opencv
sudo apt-get install build-essential\n
sudo apt-get install cmake git libgtk2.0-dev pkg-config libavcodec-dev libavformat-dev libswscale-dev\n
sudo apt-get install python-dev python-numpy libtbb2 libtbb-dev libjpeg-dev libpng-dev libtiff-dev libjasper-dev\n libdc1394-22-dev\n
cd ~\n
git clone https://github.com/Itseez/opencv\n
cd opencv\n
mkdir build\n
cd build\n
there are many other flags which can be given for cmake.Check opencv build instructions for those\n
cmake -D CMAKE_BUILD_TYPE=RELEASE -D CMAKE_INSTALL_PREFIX=/usr/local -DINSTALL_CREATE_DISTRIB=ON ..\n
Use -j(%number of processosors for optimisation)\n
make -j4 \n  
sudo make install\n
installing Tesseract\n
sudo apt-get install tesseract-ocr\n
sudo apt-get install tesseract-ocr-eng\n

An implementattion of tesseract is there in OpenCV contrib Modules.SI have used that module.So Opencv contrib is required\n
 cd ~\n
 git clone https://github.com/Itseez/opencv_contrib.git\n
 now copy this folder to opencv/build\n
 change directory into opencv build folder\n
 the exact name may vary depending on your folder name\n
 cmake -DOPENCV_EXTRA_MODULES_PATH= opencv_contrib/modules -DBUILD_opencv_legacy=OFF ..\n
 make -j4\n
 sudo make install\n
 
If you get no errors then you are good to go\n
 
 compile the program by using the make file\n
 

