all:
	mkdir -p build
	g++ -o build/linedetect src/linedetect.cpp `pkg-config --cflags --libs opencv`
	g++ -o build/makedataset src/makedataset.cpp `pkg-config --cflags --libs opencv`
	g++ -o build/trim src/trim.cpp `pkg-config --cflags --libs opencv`
	g++ -o build/detectTrebleCleff src/trebleCleff.cpp `pkg-config --cflags --libs opencv`
	g++ -o build/trainHog src/train_HOG.cpp `pkg-config --cflags --libs opencv`
linedetect:
	mkdir -p build
	g++ -o build/linedetect src/linedetect.cpp `pkg-config --cflags --libs opencv`
data:
	mkdir -p build
	g++ -o build/makedataset src/makedataset.cpp `pkg-config --cflags --libs opencv`
trim:
	mkdir -p build
	g++ -o build/trim src/trim.cpp `pkg-config --cflags --libs opencv`
trainHog:
	mkdir -p build
	g++ -o build/trainHog src/train_HOG.cpp `pkg-config --cflags --libs opencv`
detectTrebleCleff:
	mkdir -p build
	g++ -o build/detectTrebleCleff src/trebleCleff.cpp `pkg-config --cflags --libs opencv`
detectBassCleff:
	mkdir -p build
	g++ -o build/detectBassCleff src/bassCleff.cpp `pkg-config --cflags --libs opencv` 
removeText:
	mkdir -p build
	g++ -o build/removeText src/removeText.cpp `pkg-config --cflags --libs opencv`
detectstaves:
	mkdir -p build
	g++ -o build/stavedetect src/stavedetect.cpp `pkg-config --cflags --libs opencv`

clean:
	rm -rf build/
