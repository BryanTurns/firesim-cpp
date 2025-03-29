dev:
	g++ -o main main.cpp gl.c -lglfw -lGL -lX11 -lpthread -lXrandr -lXi -ldl -ggdb -g3 -Wall -Wextra -pedantic -O0 -D_GLIBCXX_DEBUG -D_GLIBCXX_ASSERTIONS

perf:
	g++ -o main main.cpp gl.c -lglfw -Ofast
test: 
	g++ -o test test.cpp grid.cpp gl.c -lglfw -lGL -lX11 -lpthread -lXrandr -lXi -ldl -ggdb -g3 -Wall -Wextra -pedantic -O0 -D_GLIBCXX_DEBUG -D_GLIBCXX_ASSERTIONS
clean:
	rm main || true
	rm test