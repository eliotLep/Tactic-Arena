CC=gcc
FLAGS =-Wall -g
SDL_DIR=${HOME}/projet/SDL2
SDL_LIB_DIR=${SDL_DIR}/lib
SDL_INC_DIR=${SDL_DIR}/include/
LIBS=-L${SDL_LIB_DIR} -lSDL2 -lSDL2_image -lSDL2_ttf -lm -lmingw32
INCS=-I${SDL_INC_DIR}
PROG=main

all: sdl_text clean

sdl_text: main.c
	${CC} -c main.c ${LIBS} ${INCS} ${FLAGS}
	${CC} -c interface.c ${LIBS} ${INCS} ${FLAGS}
	${CC} -c piece.c ${LIBS} ${INCS} ${FLAGS}
	${CC} -c logs.c ${LIBS} ${INCS} ${FLAGS}
	${CC} -c grille.c ${LIBS} ${INCS} ${FLAGS}
	${CC} -c bot.c ${LIBS} ${INCS} ${FLAGS}
	${CC} -c camera.c ${LIBS} ${INCS} ${FLAGS}
	${CC} -c deplacement.c ${LIBS} ${INCS} ${FLAGS}
	${CC} -c combat.c ${LIBS} ${INCS} ${FLAGS}
	${CC} -o main main.o grille.o piece.o interface.o logs.o bot.o camera.o deplacement.o combat.o ${LIBS} ${INCS} ${FLAGS}

clean:
	rm -f *.o

cleanall:
	clean rm -f ${PROG}

#include "grille.h"
#include "piece.h"
#include "interface.h"
#include "logs.h"

#include "bot.h"
#include "camera.h"
#include "deplacement.h"
#include "combat.h"