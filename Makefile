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

# MiniLibX paths and targets
MLX_LIB = $(MLXDIR)/libmlx.a
MLX_FLAGS = -L$(MLXDIR) -lmlx -lXext -lX11 -lm

# Includes
INCLUDES = -I $(INCDIR) -I $(MLXDIR)

.PHONY: all clean fclean re

all: $(NAME)

# 1. Added $(MLX_LIB) as a dependency here
$(NAME): $(MLX_LIB) $(OBJS)
	$(CC) $(CFLAGS) $(INCLUDES) -o $@ $(OBJS) $(MLX_FLAGS)

# 2. This rule compiles MiniLibX if libmlx.a doesn't exist
$(MLX_LIB):
	make -C $(MLXDIR)

%.o: %.c
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

clean:
	rm -f $(OBJS)
	make -C $(MLXDIR) clean

fclean: clean
	rm -f $(NAME)

re: fclean all