main:
	g++ main.cpp Shader.cpp -o main -lsfml-window -lsfml-system -lGL -lGLU -lGLEW -lglfw
	

clean:
	rm -f main

