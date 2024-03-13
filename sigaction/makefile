FILES = $(wildcard ./*)
HEADS = $(wildcard ./*.h)
STARTRCS = $(wildcard ./start*.c)
STARTOBJS = $(patsubst %.c, %.o, $(STARTRCS))
SHOWRCS = $(wildcard ./show*.c)
SHOWOBJS = $(patsubst %.c, %.o, $(SHOWRCS))
CFLAGS = `pkg-config --cflags --libs gtk4`
all: start show
start: $(STARTOBJS)
	$(CC) $^ -o $@ $(CFLAGS)
show: $(SHOWOBJS)
	$(CC) $^ -o $@ $(CFLAGS)
%.o: %.c
	$(CC) -c $< $@ -g $(CFLAGS)
clean:
	$(RM) $(filter-out $(STARTRCS) $(HEADS) $(SHOWRCS) ./makefile, $(FILES)) -r