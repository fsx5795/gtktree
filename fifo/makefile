FILES = $(wildcard ./*)
HEADS = $(wildcard ./*.h)
STARTSRCS = $(wildcard ./start*.c)
STARTOBJS = $(patsubst %.c, %.o, $(STARTSRCS))
SHOWSRCS = $(wildcard ./show*.c)
SHOWOBJS = $(patsubst %.c, %.o, $(SHOWSRCS))
CFLAGS = `pkg-config --cflags --libs gtk4`
all: start show
start: $(STARTOBJS)
	$(CC) $^ -o $@ $(CFLAGS)
show: $(SHOWOBJS)
	$(CC) $^ -o $@ $(CFLAGS)
%.o: %.c
	$(CC) -c $< -o $@ -g $(CFLAGS)
clean:
	$(RM) $(filter-out $(STARTSRCS) $(HEADS) $(SHOWSRCS) ./makefile, $(FILES)) -r
