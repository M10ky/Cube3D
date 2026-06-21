# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: miokrako <miokrako@student.42antananari    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2026/04/16 12:01:50 by jonandri          #+#    #+#              #
#    Updated: 2026/06/21 21:48:23 by miokrako         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = cube3D

SRCS = main.c sources/pixel_utils.c sources/read_files.c sources/map_init.c sources/get_next_line.c \
	sources/get_next_line_utils.c sources/parse_utils.c sources/parse_color.c sources/map_parsing.c \
	sources/strlen.c sources/utils_for_map.c sources/flood.c sources/verify_flood.c \
	src/cleanup.c \
    src/init.c \
    src/init_mlx.c \
    src/input.c \
    src/dda.c \
    src/draw.c \
    src/wall.c \
    src/input_utils.c \
    src/free.c \
    src/setup.c \
    src/set_dir.c \
    src/mock_parser.c \
    src/raycasting.c \
    src/render.c

OBJS = $(SRCS:.c=.o)

MLX_PATH = minilibx-linux
LIBFT_PATH = Libft
LIBFT = $(LIBFT_PATH)/libft.a

CC = cc
CFLAGS = -Wall -Wextra -Werror -g

HEADER = -I$(MLX_PATH) -I./header -I$(LIBFT_PATH)

MLX_FLAG = -L$(MLX_PATH) -lmlx -lXext -lX11 -lm

RM = rm -f

all: $(NAME)

%.o: %.c
	$(CC) $(CFLAGS) $(HEADER) -c $< -o $@

$(LIBFT):
	$(MAKE) -C $(LIBFT_PATH)

$(MLX_PATH)/libmlx.a:
	$(MAKE) -C $(MLX_PATH)

$(NAME): $(OBJS) $(LIBFT) $(MLX_PATH)/libmlx.a
	$(CC) $(CFLAGS) $(OBJS) $(LIBFT) $(MLX_FLAG) -o $(NAME)

clean:
	$(RM) $(OBJS)
	$(MAKE) -C $(MLX_PATH) clean
	$(MAKE) -C $(LIBFT_PATH) clean

fclean: clean
	$(RM) $(NAME)
	$(MAKE) -C $(LIBFT_PATH) fclean

re: fclean all

.PHONY: all clean fclean re

