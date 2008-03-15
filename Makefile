objects = complex.o queue.o pixel.o image.o worker.o ui.o main.o
headers = complex.h queue.h pixel.h image.h worker.h ui.h

CFLAGS += -DDEBUG -std=c99 -O2

libs = -lm -lgd -lGL -lGLU -lglut

all: fractal


$(objects): $(headers)


%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<

fractal: $(objects)
	$(CC) $(CFLAGS) $(libs) -o $@ $(objects)

clean:
	rm -f *.o fractal
