# brew install glew
# brew install glfw3
# brew install glm
# -H to see all paths

CC=clang++
FLAGS=-stdlib=libc++ -lGLEW -lglfw3 -framework Cocoa -framework OpenGL -framework IOKit -framework CoreVideo
all:
	$(CC) -o main main.cpp common/shaders.cpp common/texture.cpp common/controls.cpp $(FLAGS)
debug: 
	$(CC) -o main main.cpp common/shaders.cpp common/texture.cpp common/controls.cpp $(FLAGS) -g
clean:
	rm main
