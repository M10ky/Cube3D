/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: miokrako <miokrako@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/01 23:35:19 by miokrako          #+#    #+#             */
/*   Updated: 2026/06/15 22:00:48 by miokrako         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/cub3d.h"

void	put_pixel(t_img *buf, int x, int y, unsigned int color)
{
	char	*dst;

	if (x < 0 || x >= SCREEN_W || y < 0 || y >= SCREEN_H)
		return ;
	dst = buf->addr + y * buf->line_len + x * (buf->bpp / 8);
	*(unsigned int *)dst = color;
}

static	int	init_mlx(t_game *game)
{
	game->mlx = mlx_init();
	if (!game->mlx)
		return (print_error("mlx_init() failed"));
	game->win = mlx_new_window(game->mlx, SCREEN_W, SCREEN_H, WIN_TITLE);
	if (!game->win)
		return (print_error("mlx_new_window() failed"));
	return (0);
}

static	int	init_framebuffer(t_game *game)
{
	game->buf.img = mlx_new_image(game->mlx, SCREEN_W, SCREEN_H);
	if (!game->buf.img)
		return (print_error("mlx_new_image (framebuffer) failed"));
	game->buf.addr = mlx_get_data_addr(game->buf.img, &game->buf.bpp,
			&game->buf.line_len, &game->buf.endian);
	if (!game->buf.addr)
		return (print_error("mlx_get_data_addr (framebuffer) failed"));
	return (0);
}

static	int	load_texture(t_game *game, int idx)
{
	t_texture	*tex;

	tex = &game->tex[idx];
	tex->img = mlx_xpm_file_to_image(game->mlx, game->config.tex_path[idx],
			&tex->width, &tex->height);
	if (!tex->img)
	{
		print_error("mlx_get_data_addr failed");
		cleanup(game);
		exit(1);
	}
	tex->addr = mlx_get_data_addr(tex->img, &tex->bpp,
			&tex->line_len, &tex->endian);
	return (0);
}

static	int	init_textures(t_game *game)
{
	if (load_texture(game, NORTH) != 0)
		return (1);
	if (load_texture(game, SOUTH) != 0)
		return (1);
	if (load_texture(game, EAST) != 0)
		return (1);
	if (load_texture(game, WEST) != 0)
		return (1);
	return (0);
}

void	init_player(t_game *game)
{
	t_config	*cfg;
	t_player	*p;

	cfg = &game->config;
	p = &game->player;
	p->pos_x = cfg->spawn_x;
	p->pos_y = cfg->spawn_y;
	p->dir_x = cfg->spawn_dir_x;
	p->dir_y = cfg->spawn_dir_y;
	p->plane_x = cfg->spawn_plane_x;
	p->plane_y = cfg->spawn_plane_y;
}

int	init_game(t_game *game, t_map *map)
{
	if (init_mock_config(&game->config, map) != 0)
		return (1);
	if (init_mlx(game) != 0)
		return (1);
	if (init_framebuffer(game) != 0)
		return (1);
	if (init_textures(game) != 0)
		return (1);
	init_player(game);
	return (0);
}
