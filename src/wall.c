/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wall.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: miokrako <miokrako@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/21 21:25:54 by miokrako          #+#    #+#             */
/*   Updated: 2026/06/21 21:29:25 by miokrako         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/cub3d.h"

void	calc_wall_dist(t_ray *ray)
{
	if (ray->side == 0)
		ray->wall_dist = ray->side_x - ray->delta_x;
	else
		ray->wall_dist = ray->side_y - ray->delta_y;
	if (ray->wall_dist < 0.001)
		ray->wall_dist = 0.001;
}

void	calc_wall_face(t_ray *ray)
{
	if (ray->side == 0)
	{
		if (ray->step_x > 0)
			ray->face = WEST;
		else
			ray->face = EAST;
	}
	else
	{
		if (ray->step_y > 0)
			ray->face = NORTH;
		else
			ray->face = SOUTH;
	}
}

void	calc_draw_bounds(t_ray *ray)
{
	ray->line_h = (int)(SCREEN_H / ray->wall_dist);
	ray->draw_y0 = SCREEN_H / 2 - ray->line_h / 2;
	if (ray->draw_y0 < 0)
		ray->draw_y0 = 0;
	ray->draw_y1 = SCREEN_H / 2 + ray->line_h / 2;
	if (ray->draw_y1 >= SCREEN_H)
		ray->draw_y1 = SCREEN_H - 1;
}

void	calc_tex_x(t_player *p, t_ray *ray)
{
	double	wall_hit;

	if (ray->side == 0)
		wall_hit = p->pos_y + ray->wall_dist * ray->dir_y;
	else
		wall_hit = p->pos_x + ray->wall_dist * ray->dir_x;
	wall_hit -= floor(wall_hit);
	ray->tex_x = (int)(wall_hit * TEX_W);
	if ((ray->side == 0 && ray->step_x < 0)
		|| (ray->side == 1 && ray->step_y > 0))
		ray->tex_x = TEX_W - ray->tex_x - 1;
	if (ray->tex_x < 0)
		ray->tex_x = 0;
	if (ray->tex_x >= TEX_W)
		ray->tex_x = TEX_W - 1;
}

void	calc_wall(t_game *game, t_ray *ray)
{
	calc_wall_dist(ray);
	calc_draw_bounds(ray);
	calc_wall_face(ray);
	calc_tex_x(&game->player, ray);
}
