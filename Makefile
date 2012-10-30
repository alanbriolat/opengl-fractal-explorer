objects = complex.o queue.o fractal.o worker.o ui.o main.o
headers = complex.h queue.h fractal.h worker.h ui.h

CFLAGS += -std=c99 -O2 #-DNDEBUG

libs = -lm -lgd -lGL -lGLU -lglut -lpthread

all: fractal


$(objects): $(headers)


%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<

fractal: $(objects)
	$(CC) $(CFLAGS) $(libs) -o $@ $(objects)

clean:
	rm -f *.o fractal
