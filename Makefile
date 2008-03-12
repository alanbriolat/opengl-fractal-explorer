objects = complex.o queue.o pixel.o image.o worker.o main.o
headers = complex.h queue.h pixel.h image.h worker.h

CFLAGS += -DDEBUG -std=c99 -O2

libs = -lm -lgd

all: fractal


$(objects): $(headers)


%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<

fractal: $(objects)
	$(CC) $(CFLAGS) $(libs) -o $@ $(objects)

clean:
	rm -f *.o fractal
