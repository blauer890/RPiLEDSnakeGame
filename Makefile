all: main.c LinkedList.c LEDMatrix.c
	gcc -o main main.c LinkedList.c LEDMatrix.c -lpthread -lwiringPi

clean:
	rm main
