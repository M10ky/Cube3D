/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_for_map.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jonandri <jonandri@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/29 18:03:31 by jonandri          #+#    #+#             */
/*   Updated: 2026/06/14 20:30:12 by jonandri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cube.h"

void	search_player(t_map *map)
{
	int	y;
	int	x;

	y = 0;
	while (map->true_grid[y] != NULL)
	{
		x = 0;
		while (map->true_grid[y][x] != '\0')
		{
			if (map->true_grid[y][x] == 'N' || map->true_grid[y][x] == 'S'
				|| map->true_grid[y][x] == 'E' || map->true_grid[y][x] == 'W')
			{
				map->dir = map->true_grid[y][x];
				map->pos_x = x;
				map->pos_y = y;
			}
			x++;
		}
		y++;
	}
}
