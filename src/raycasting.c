/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   raycasting.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: miokrako <miokrako@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/30 06:41:13 by miokrako          #+#    #+#             */
/*   Updated: 2026/06/20 11:16:53 by miokrako         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/cub3d.h"

static	char	map_safe(t_config *cfg, int x, int y)
{
	if (y < 0 || y >= cfg->map_h || x < 0 || x >= cfg->map_w)
		return ('1');
	if (x >= (int)ft_strlen_cube(cfg->map[y]))
		return ('1');
	return (cfg->map[y][x]);
}

static	void	init_ray(t_game *game, int col, t_ray *ray)
{
	t_player	*p;

	p = &game->player;
	ft_memset(ray, 0, sizeof(t_ray));
	ray->cam = 2.0 * col / (double)SCREEN_W - 1.0;
	ray->dir_x = p->dir_x + p->plane_x * ray->cam;
	ray->dir_y = p->dir_y + p->plane_y * ray->cam;
	ray->map_x = (int)p->pos_x;
	ray->map_y = (int)p->pos_y;
}

static	void	init_dda(t_game *game, t_ray *ray)
{
	t_player	*p;

	p = &game->player;
	if (ray->dir_x == 0.0)
		ray->delta_x = 1e30;
	else
		ray->delta_x = fabs(1.0 / ray->dir_x);
	if (ray->dir_y == 0.0)
		ray->delta_y = 1e30;
	else
		ray->delta_y = fabs(1.0 / ray->dir_y);
	if (ray->dir_x < 0.0)
	{
		ray->step_x = -1;
		ray->side_x = (p->pos_x - ray->map_x) * ray->delta_x;
	}
	else
	{
		ray->step_x = 1;
		ray->side_x = (ray->map_x + 1.0 - p->pos_x) * ray->delta_x;
	}
	if (ray->dir_y < 0.0)
	{
		ray->step_y = -1;
		ray->side_y = (p->pos_y - ray->map_y) * ray->delta_y;
	}
	else
	{
		ray->step_y = 1;
		ray->side_y = (ray->map_y + 1.0 - p->pos_y) * ray->delta_y;
	}
	(void)game;
}

static	void	do_dda(t_game *game, t_ray *ray)
{
	ray->hit = 0;
	while (ray->hit == 0)
	{
		if (ray->side_x < ray->side_y)
		{
			ray->side_x += ray->delta_x;
			ray->map_x += ray->step_x;
			ray->side = 0;
		}
		else
		{
			ray->side_y += ray->delta_y;
			ray->map_y += ray->step_y;
			ray->side = 1;
		}
		if (map_safe(&game->config, ray->map_x, ray->map_y) == '1')
			ray->hit = 1;
	}
}

static	void	calc_wall(t_game *game, t_ray *ray)
{
	double		wall_hit;
	t_player	*p;

	p = &game->player;

	/* Distance au mur */
	if (ray->side == 0)
		ray->wall_dist = ray->side_x - ray->delta_x;
	else
		ray->wall_dist = ray->side_y - ray->delta_y;

	if (ray->wall_dist < 0.001)
		ray->wall_dist = 0.001;

	ray->line_h = (int)(SCREEN_H / ray->wall_dist);
	ray->draw_y0 = SCREEN_H / 2 - ray->line_h / 2;
	if (ray->draw_y0 < 0)
		ray->draw_y0 = 0;
	ray->draw_y1 = SCREEN_H / 2 + ray->line_h / 2;
	if (ray->draw_y1 >= SCREEN_H)
		ray->draw_y1 = SCREEN_H - 1;

	/* Détermination de la face (texture) */
	if (ray->side == 0)  /* mur vertical (E/W) */
	{
		if (ray->step_x > 0)
			ray->face = EAST;
		else
			ray->face = WEST;
	}
	else                /* mur horizontal (N/S) */
	{
		if (ray->step_y > 0)
			ray->face = SOUTH;
		else
			ray->face = NORTH;
	}

	/* Calcul de la position sur la texture */
	if (ray->side == 0)
		wall_hit = p->pos_y + ray->wall_dist * ray->dir_y;
	else
		wall_hit = p->pos_x + ray->wall_dist * ray->dir_x;

	wall_hit -= floor(wall_hit);
	ray->tex_x = (int)(wall_hit * TEX_W);

	/* === MIRRORING CORRECT (le plus important) === */
	if ((ray->side == 0 && ray->step_x < 0) ||      /* Est */
	    (ray->side == 1 && ray->step_y > 0))        /* Sud */
		ray->tex_x = TEX_W - ray->tex_x - 1;

	if (ray->tex_x < 0)
		ray->tex_x = 0;
	if (ray->tex_x >= TEX_W)
		ray->tex_x = TEX_W - 1;
}

void	cast_ray(t_game *game, int col, t_ray *ray)
{
	init_ray(game, col, ray);
	init_dda(game, ray);
	do_dda(game, ray);
	calc_wall(game, ray);
}
