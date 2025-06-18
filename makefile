CC = gcc
PKG = `pkg-config --cflags --libs gtk+-3.0 cairo`
SRC = src/main.c src/interface.c src/simulador.c src/processos.c
OBJ = $(SRC:.c=.o)
OUT = EscalonadorDeProcessos

all: $(OUT)

$(OUT): $(OBJ)
	$(CC) -o $@ $^ $(PKG)

%.o: %.c
	$(CC) -c $< -o $@ $(PKG)

clean:
	rm -f $(OBJ) $(OUT)
