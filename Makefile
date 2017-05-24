all:
	g++ -o build/linedetect src/linedetect.cpp `pkg-config --cflags --libs opencv`
data:
	g++ -o build/makedataset src/makedataset.cpp `pkg-config --cflags --libs opencv`
trim:
	g++ -o build/trim src/trim.cpp `pkg-config --cflags --libs opencv`
trainHog:
	g++ -o build/trainHog src/train_HOG.cpp `pkg-config --cflags --libs opencv`
detectTrebleCleff:
	g++ -o build/detectTrebleCleff src/trebleCleff.cpp `pkg-config --cflags --libs opencv`
detectBassCleff:
	g++ -o build/detectBassCleff src/bassCleff.cpp `pkg-config --cflags --libs opencv` 
