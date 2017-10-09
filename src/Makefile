.PHONY: all clean

all:rev_photo img2sketch img2sketch_carton gen_coord camera

rev_photo:rev_photo.cs
	mono-csc -out:rev_photo rev_photo.cs 
img2sketch:img2sketch.cpp
	g++ img2sketch.cpp -o img2sketch --std=c++11 `pkg-config --cflags --libs opencv`
img2sketch_carton:img2sketch_carton.cpp
	g++ img2sketch_carton.cpp -o img2sketch_carton --std=c++11 `pkg-config --cflags --libs opencv`
gen_coord:gen_coord.cpp
	g++ gen_coord.cpp -o gen_coord --std=c++11 `pkg-config --cflags --libs opencv`
camera:camera.cpp
	g++ camera.cpp -o camera --std=c++11 `pkg-config --cflags --libs opencv`

clean:
	rm rev_photo img2sketch_carton img2sketch gen_coord camera