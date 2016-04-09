all:
	g++ -o test test.cpp `pkg-config --cflags --libs opencv` -I/usr/local/include/tesseract -llept -ltesseract
