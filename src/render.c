/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: miokrako <miokrako@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/04 09:59:25 by miokrako          #+#    #+#             */
/*   Updated: 2026/06/20 12:00:26 by miokrako         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/cub3d.h"

static	unsigned int	sample_texture(t_texture *tex, int tx, int ty)
{
	char	*pixel;

	if (!tex || !tex->addr)
		return (1);
	pixel = tex->addr + ty * tex->line_len + tx * (tex->bpp / 8);
	return (*(unsigned int *)pixel);
}

static	void	draw_ceiling_strip(t_game *game, int col, int end_y)
{
	int	y;

	y = 0;
	while (y < end_y)
	{
		put_pixel(&game->buf, col, y, (unsigned int)game->config.ceil_color);
		y++;
	}
}

static	void	draw_floor_strip(t_game *game, int col, int start_y)
{
	int	y;

	y = start_y;
	while (y < SCREEN_H)
	{
		put_pixel(&game->buf, col, y, (unsigned int)game->config.floor_color);
		y++;
	}
}

static	t_texture	*get_wall_texture(t_game *game, t_ray *ray,
		double *tex_pos, double *tex_step)
{
	t_texture	*tex;

	tex = &game->tex[ray->face];
	*tex_step = (double)TEX_H / (double)ray->line_h;
	*tex_pos = ((double)ray->draw_y0 - (double)SCREEN_H / 2.0
			+ (double)ray->line_h / 2.0) * (*tex_step);
	return (tex);
}

static	void	draw_wall_strip(t_game *game, int col, t_ray *ray)
{
	unsigned int	color;
	t_texture		*tex;
	double			tex_step;
	double			tex_pos;
	int				y;

	tex = get_wall_texture(game, ray, &tex_pos, &tex_step);
	y = ray->draw_y0;
	while (y < ray->draw_y1)
	{
		color = sample_texture(tex, ray->tex_x, (int)tex_pos & (TEX_H - 1));
		tex_pos += tex_step;
		put_pixel(&game->buf, col, y, color);
		y++;
	}
}

static	void	draw_column(t_game *game, int col, t_ray *ray)
{
	draw_ceiling_strip(game, col, ray->draw_y0);
	draw_wall_strip(game, col, ray);
	draw_floor_strip(game, col, ray->draw_y1 + 1);
}

void	render_frame(t_game *game)
{
	t_ray	ray;
	int		col;

	col = 0;
	while (col < SCREEN_W)
	{
		cast_ray(game, col, &ray);
		draw_column(game, col, &ray);
		col++;
	}
	mlx_put_image_to_window(game->mlx, game->win, game->buf.img, 0, 0);
}

int	game_loop(void *param)
{
	t_game	*game;

	game = (t_game *)param;
	if (game->has_focus == 0)
	{
		ft_memset(&game->keys, 0, sizeof(t_keys));
		render_frame(game);
		return (0);
	}
	handle_input(game);
	render_frame(game);
	return (0);
}
