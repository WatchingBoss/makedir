CC = gcc

OBJ = makedir.o info.o
OUT = makedir

$(OUT): $(OBJ)
	$(CC) -o $(OUT) $(OBJ)

clean:
	$(RM) $(OBJ) $(OUT)
