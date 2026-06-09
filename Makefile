NAME = Cube3D
CC = gcc
CFLAGS = -Wall -Wextra -Werror

INCDIR = includes
MLXDIR = minilibx-linux
LIBFTDIR = libft

SRCS = src/cleanup.c \
       src/init.c \
       src/input.c \
       src/main.c \
       src/mock_parser.c \
       src/raycasting.c \
       src/render.c

OBJS = $(SRCS:.c=.o)

MLX_LIB = $(MLXDIR)/libmlx.a
MLX_FLAGS = -L$(MLXDIR) -lmlx -lXext -lX11 -lm

LIBFT = $(LIBFTDIR)/libft.a
LIBFT_FLAGS = -L$(LIBFTDIR) -lft

INCLUDES = -I $(INCDIR) -I $(MLXDIR) -I $(LIBFTDIR)


all: $(NAME)

$(NAME): $(MLX_LIB) $(LIBFT) $(OBJS)
	$(CC) $(CFLAGS) $(INCLUDES) -o $@ $(OBJS) $(LIBFT_FLAGS) $(MLX_FLAGS)

$(MLX_LIB):
	make -C $(MLXDIR)

$(LIBFT):
	make -C $(LIBFTDIR)

%.o: %.c
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

clean:
	rm -f $(OBJS)
	make -C $(MLXDIR) clean
	make -C $(LIBFTDIR) clean

fclean: clean
	rm -f $(NAME)
	make -C $(LIBFTDIR) fclean

re: fclean all

.PHONY: all clean fclean re
