##
## Makefile for Makefile in /home/duhieu_b/Concurrence/PSU_2016_lemipc
##
## Made by duhieu_b
## Login   <benjamin.duhieu@epitech.eu>
##
## Started on  Mon Mar 20 10:48:22 2017 duhieu_b
## Last update Fri Mar 31 11:28:56 2017 duhieu_b
##

SANITIZE=	no

DEBUG=		no

MAKE_DIR=	./

NAME=		lemipc

SRC_DIR=	./src/

SRC_FIL=	lemipc.c \
		firstProcess.c \
		otherProcess.c \
		loop.c \
		map.c \
		logic.c \
		checkBotLeft.c \
		checkTopRight.c \
		order.c \
		go.c \
		tryToMove.c

SRC= 		$(addprefix $(SRC_DIR), $(SRC_FIL))

OBJ=		$(SRC:.c=.o)

INC=		./include

CC=		gcc

INC= 		-I./include

ifeq ($(DEBUG), yes)
CFLAGS=	  	-g -W -Wall -Wextra -Werror $(INC)
else
CFLAGS=	  	-W -Wall -Wextra -Werror $(INC)
endif

ifeq ($(SANITIZE), yes)
CC +=		-fsanitize=thread
endif

RM=		rm -rf

$(NAME): $(OBJ)
	$(CC) -o $(NAME) $(OBJ) $(INC)

all: $(NAME)

clean:
	$(RM) $(OBJ)

fclean:
	$(RM) $(OBJ)
	$(RM) $(NAME)

re: fclean all
