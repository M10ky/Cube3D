NAME = Cube3D
CC = gcc
CFLAGS = -Wall -Wextra -Werror -g

SRCDIR = src
INCDIR = includes
MLXDIR = minilibx-linux

SRCS = $(wildcard $(SRCDIR)/*.c)
OBJS = $(SRCS:.c=.o)

MLX_LIB = $(MLXDIR)/libmlx.a
MLX_FLAGS = -L$(MLXDIR) -lmlx -lXext -lX11 -lm

INCLUDES = -I $(INCDIR) -I $(MLXDIR)


all: $(NAME)

$(NAME): $(MLX_LIB) $(OBJS)
	$(CC) $(CFLAGS) $(INCLUDES) -o $@ $(OBJS) $(MLX_FLAGS)

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
.PHONY: all clean fclean re