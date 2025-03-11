# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: keanders <marvin@42.fr>                    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/03/07 15:29:26 by keanders          #+#    #+#              #
#    Updated: 2025/03/07 15:29:28 by keanders         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #


# Project Name
NAME = minip

# Compiler
CC = gcc

# Compiler Flags
CFLAGS = -Wall -Wextra -Werror -O3

# Include Directories
INCLUDES = -I/usr/include -Imlx_linux

# Libraries and Linking Flags
LDFLAGS = -Lmlx_linux -lmlx_Linux -L/usr/lib -lXext -lX11 -lm -lz

# Source Files
SRCS = minip.c square.c

# Object Files
OBJ = $(SRCS:.c=.o)

# Default Target
all: $(NAME)

# Rule to compile .c files into .o files
%.o: %.c
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

# Rule to build the final executable
$(NAME): $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) $(LDFLAGS) -o $(NAME)

# Clean up object files
clean:
	rm -f $(OBJ)

# Clean up object files and the executable
fclean: clean
	rm -f $(NAME)

# Rebuild the project
re: fclean all

# Phony targets (targets that are not actual files)
.PHONY: all clean fclean re
