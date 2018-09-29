CC = gcc
CFLAGS = -Wall -g

OBJ = makedir.o info.o
OUT = makedir

$(OUT): $(OBJ)
	$(CC) $(CFLAGS) -o $(OUT) $(OBJ)

clean:
	$(RM) $(OBJ) $(OUT)
