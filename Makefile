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
detectbarlines:
	mkdir -p build
	g++ -o build/barlinedetect src/detectBarlines.cpp `pkg-config --cflags --libs opencv`
drawstavelines:
	mkdir -p build
	g++ -o build/drawStavelines src/drawStavelines.cpp `pkg-config --cflags --libs opencv`
detectnotehead:
	mkdir -p build
	g++ -o build/detectnotehead src/detectnotehead.cpp `pkg-config --cflags --libs opencv`
removestafflines:
	mkdir -p build
	g++ -o build/removestafflines src/removestafflines.cpp `pkg-config --cflags --libs opencv`



clean:
	rm -rf build/
