NAME = LibTread.a

OBJ = LibThread.o

CC = clang++ -g -O3  -Wall -Wextra -Werror -std=c++11

all: $(NAME)

$(NAME): $(OBJ)
	ar rvs $(NAME) $(OBJ)

.cpp.o:
	$(CC) -c -o $@ $<

clean:
	rm -f $(OBJ)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: clean all re fclean