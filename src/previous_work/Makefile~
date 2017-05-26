all:
	g++ -o bar bar.cpp `pkg-config --cflags --libs opencv` -I/usr/local/include/tesseract -llept -ltesseract
bar:
	g++ -o bar bar.cpp `pkg-config --cflags --libs opencv`
test:
	g++ -o test test.cpp `pkg-config --cflags --libs opencv` -I/usr/local/include/tesseract -llept -ltesseract
