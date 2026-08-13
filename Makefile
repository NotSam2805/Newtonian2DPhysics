all:
	g++ -I SDL2\include -L SDL2\lib -o main main.cpp Maths/*.cpp Physics/*.cpp Shapes/*.cpp Rendering/*.cpp -lmingw32 -lSDL2main -lSDL2