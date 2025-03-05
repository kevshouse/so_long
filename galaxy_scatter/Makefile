NAME = so_long
#AR = ar rcs

# Compiler
CC = gcc

# Compiler flags
CFLAGS = 

SRCS = get_start_mlx.c
OBJ = $(SRCS:.c=.o)

%.o: %.c
	$(CC) $(CFLAGS) -I/usr/include -Imlx_linux -O3 -c $< -o $@

all: $(NAME)
# Object files 

$(NAME): $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -Lmlx_linux -lmlx_Linux -L/usr/lib -Imlx_linux -lXext -lX11 -lm -lz -o $(NAME)

clean:
	rm -f $(OBJ)

fclean: clean
		rm -f $(NAME)

re : fclean all