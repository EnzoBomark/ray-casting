build_and_run: build run

build:
	gcc -Wall -fsanitize=address -std=c99 ./src/*.c -I/opt/homebrew/include -L/opt/homebrew/lib -lSDL2 -lSDL2_TTF -o raycast

run:
	./raycast

clean:
	rm raycast
	