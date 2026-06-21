/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: miokrako <miokrako@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/21 21:45:15 by miokrako          #+#    #+#             */
/*   Updated: 2026/06/21 23:50:29 by miokrako         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes_raycasting/cub3d.h"

void	draw_ceiling_strip(t_game *game, int col, int end_y)
{
	int	y;

	y = 0;
	while (y < end_y)
	{
		put_pixel(&game->buf, col, y, (unsigned int)game->config.ceil_color);
		y++;
	}
}

void	draw_floor_strip(t_game *game, int col, int start_y)
{
	int	y;

	y = start_y;
	while (y < SCREEN_H)
	{
		put_pixel(&game->buf, col, y, (unsigned int)game->config.floor_color);
		y++;
	}
}

void	draw_wall_strip(t_game *game, int col, t_ray *ray)
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

void	draw_column(t_game *game, int col, t_ray *ray)
{
	draw_ceiling_strip(game, col, ray->draw_y0);
	draw_wall_strip(game, col, ray);
	draw_floor_strip(game, col, ray->draw_y1 + 1);
}
