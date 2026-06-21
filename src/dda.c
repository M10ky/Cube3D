/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dda.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: miokrako <miokrako@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/21 21:21:26 by miokrako          #+#    #+#             */
/*   Updated: 2026/06/21 23:50:25 by miokrako         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes_raycasting/cub3d.h"

void	calc_delta(t_ray *ray)
{
	if (ray->dir_x == 0.0)
		ray->delta_x = 1e30;
	else
		ray->delta_x = fabs(1.0 / ray->dir_x);
	if (ray->dir_y == 0.0)
		ray->delta_y = 1e30;
	else
		ray->delta_y = fabs(1.0 / ray->dir_y);
}

void	calc_step_x(t_game *game, t_ray *ray)
{
	t_player	*p;

	p = &game->player;
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
}

void	calc_step_y(t_game *game, t_ray *ray)
{
	t_player	*p;

	p = &game->player;
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
}

void	init_dda(t_game *game, t_ray *ray)
{
	calc_delta(ray);
	calc_step_x(game, ray);
	calc_step_y(game, ray);
}
