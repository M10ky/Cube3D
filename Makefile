NAME = Cube3D
CC = gcc
CFLAGS = -Wall -Wextra -Werror -g

# Dossiers
SRCDIR = src
INCDIR = includes
MLXDIR = minilibx-linux

# Sources
SRCS = $(wildcard $(SRCDIR)/*.c)
OBJS = $(SRCS:.c=.o)

# MiniLibX (Added -L to find libmlx.a)
MLX_FLAGS = -L$(MLXDIR) -lmlx -lXext -lX11 -lm

# Includes (Added -I $(MLXDIR) so you can do #include "mlx.h")
INCLUDES = -I $(INCDIR) -I $(MLXDIR)

.PHONY: all clean fclean re

all: $(NAME)

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) $(INCLUDES) -o $@ $^ $(MLX_FLAGS)

%.o: %.c
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

clean:
	rm -f $(OBJS)

fclean: clean
	rm -f $(NAME)

re: fclean all
