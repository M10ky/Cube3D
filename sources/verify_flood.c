/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   verify_flood.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jonandri <jonandri@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/02 13:32:43 by jonandri          #+#    #+#             */
/*   Updated: 2026/06/17 17:15:35 by jonandri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cube.h"

void	verify_flood(t_map *map)
{
	int	y;
	int	x;

	y = 0;
	while (map->true_grid[y] != NULL)
	{
		x = 0;
		while (map->true_grid[y][x] != '\0')
		{
			if (map->true_grid[y][x] == '0')
				flood_fill(map, x, y);
			x++;
		}
		y++;
	}
}

static int	if_not_v(t_map *map, int x, int y)
{
	if (y == 0 || y == (map->height - 1) || x == 0 || x == (map->width - 1))
		return (1);
	if (map->true_grid[y][x + 1] == ' ' || map->true_grid[y][x - 1] == ' ')
		return (1);
	return (0);
}

int	verify_map(t_map *map)
{
	int	y;
	int	x;

	y = 0;
	while (map->true_grid[y] != NULL)
	{
		x = 0;
		while (map->true_grid[y][x] != '\0')
		{
			if (map->true_grid[y][x] == 'V' || map->true_grid[y][x] == map->dir)
			{
				if (if_not_v(map, x, y) == 1)
					return (1);
			}
			x++;
		}
		y++;
	}
	return (0);
}
