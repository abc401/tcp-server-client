CC = gcc
CFLAGS = -Wall -lWs2_32
OUT_DIR = out

all: server client

server: src/server.c out-dir
	${CC} $< -o ${OUT_DIR}/$@.exe ${CFLAGS}

client: src/client.c out-dir
	${CC} $< -o ${OUT_DIR}/$@.exe ${CFLAGS}

out-dir:
	mkdir ${OUT_DIR}