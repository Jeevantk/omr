#Compiler
CXX=g++

#Compilation Flags using O3 optimization
CXXFLAGS= `pkg-config --cflags --libs opencv` -O3

#Make Directory
DIR=mkdir -p build


all: build/removeText.o build/stavedetect.o build/removestafflines.o build/detectnotehead.o 

#removes the text from the source image
build/removeText.o:
	$(DIR)
	$(CXX) src/removeText.cpp $(CXXFLAGS) -o $@ $<

build/stavedetect.o:
	$(DIR)
	$(CXX) src/stavedetect.cpp $(CXXFLAGS) -o $@ $<	

build/removestafflines.o:
	$(DIR)
	$(CXX) src/removestafflines.cpp $(CXXFLAGS) -o $@ $<

build/detectnotehead.o:
	$(DIR)
	$(CXX) src/detectnotehead.cpp $(CXXFLAGS) -o $@ $<	


clean:
	rm -rf build/*.opencv



