CC = gcc
CFLAGS = -Wall -Wextra
LIBS = -lSDL2
INCLUDES = -I/usr/include/SDL2/
SRCS = main.c
OBJS = ${SRCS:.c=.o}
MAIN = main.bin

$(MAIN) : ${OBJS}
	$(CC) $(CFLAGS) $(INCLUDES) -o $(MAIN) $(OBJS) $(LIBS)
.c.o:
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@
clean:
	$(RM) *.o *~ $(MAIN)
run: $(MAIN)
	./main.bin
