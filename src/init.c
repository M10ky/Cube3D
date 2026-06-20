/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: miokrako <miokrako@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/29 20:39:02 by miokrako          #+#    #+#             */
/*   Updated: 2026/06/17 xx:xx:xx by Grok              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/cub3d.h"
#include "../header/cube.h"

void	put_pixel(t_img *buf, int x, int y, unsigned int color)
{
	char	*dst;

	if (x < 0 || x >= SCREEN_W || y < 0 || y >= SCREEN_H)
		return ;
	dst = buf->addr + y * buf->line_len + x * (buf->bpp / 8);
	*(unsigned int *)dst = color;
}
static int	init_mlx(t_game *game)
{
	game->mlx = mlx_init();
	if (!game->mlx)
		return (print_error("MLX: failed to initialize"));

	game->win = mlx_new_window(game->mlx, SCREEN_W, SCREEN_H, "Cub3D");
	if (!game->win)
	{
		mlx_destroy_display(game->mlx);
		free(game->mlx);
		game->mlx = NULL;
		return (print_error("MLX: failed to create window"));
	}

	return (0);
}


static int	init_framebuffer(t_game *game)
{
	game->buf.img = mlx_new_image(game->mlx, SCREEN_W, SCREEN_H);
	if (!game->buf.img)
		return (print_error("MLX: failed to create image buffer"));

	game->buf.addr = mlx_get_data_addr(game->buf.img, &game->buf.bpp,
			&game->buf.line_len, &game->buf.endian);
	if (!game->buf.addr)
		return (print_error("MLX: failed to get image address"));

	return (0);
}

static int	load_texture(t_game *game, int index, char *path)
{
	game->tex[index].img = mlx_xpm_file_to_image(game->mlx, path,
			&game->tex[index].width, &game->tex[index].height);
	if (!game->tex[index].img)
		return (print_error("Texture: failed to load XPM"));

	game->tex[index].addr = mlx_get_data_addr(game->tex[index].img,
			&game->tex[index].bpp, &game->tex[index].line_len,
			&game->tex[index].endian);
	if (!game->tex[index].addr)
		return (print_error("Texture: failed to get data address"));

	return (0);
}

static int	init_textures(t_game *game)
{
	int	i;

	i = 0;
	while (i < 4)
	{
		if (load_texture(game, i, game->config.tex_path[i]) != 0)
		{
			while (--i >= 0)
			{
				if (game->tex[i].img)
					mlx_destroy_image(game->mlx, game->tex[i].img);
			}
			return (1);
		}
		i++;
	}
	return (0);
}

void	init_player(t_game *game)
{
	game->player.pos_x = game->config.spawn_x;
	game->player.pos_y = game->config.spawn_y;

	game->player.dir_x = game->config.spawn_dir_x;
	game->player.dir_y = game->config.spawn_dir_y;

	game->player.plane_x = game->config.spawn_plane_x;
	game->player.plane_y = game->config.spawn_plane_y;

	game->player.move_speed = 0.1;
	game->player.rot_speed = 0.05;
}

int	init_game(t_game *game, t_map *map)
{
	ft_memset(game, 0, sizeof(t_game));
	if (init_mock_config(&game->config, map) != 0)
	{
		cleanup_mock_config(&game->config);
		return (1);
	}
	if (init_mlx(game) != 0)
	{
		cleanup_mock_config(&game->config);
		return (1);
	}
	if (init_framebuffer(game) != 0)
	{
		cleanup(game);
		return (1);
	}
	if (init_textures(game) != 0)
	{
		cleanup(game);
		return (1);
	}
	game->has_focus = 1;
	init_player(game);
	return (0);
}
