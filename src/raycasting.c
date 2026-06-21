/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   raycasting.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: miokrako <miokrako@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/30 06:41:13 by miokrako          #+#    #+#             */
/*   Updated: 2026/06/21 23:51:11 by miokrako         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes_raycasting/cub3d.h"

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

void	cast_ray(t_game *game, int col, t_ray *ray)
{
	init_ray(game, col, ray);
	init_dda(game, ray);
	do_dda(game, ray);
	calc_wall(game, ray);
}
