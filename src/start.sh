g++ camera.cpp -o camera --std=c++11 `pkg-config --cflags --libs opencv` &&
g++ img2sketch.cpp -o img2sketch --std=c++11 `pkg-config --cflags --libs opencv` &&
g++ gen_coord.cpp -o gen_coord --std=c++11 `pkg-config --cflags --libs opencv` &&
python3 ASR_baidu.py &
python3 interaction.py &
python3 TTS_baidu.py 
