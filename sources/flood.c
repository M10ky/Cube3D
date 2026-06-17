/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   flood.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jonandri <jonandri@map->true_grid[y]tudent.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/02 10:03:27 by jonandri          #+#    #+#             */
/*   Updated: 2026/06/02 13:15:37 by jonandri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cube.h"

void	flood_fill(t_map *map, int x, int y)
{
	if (y < 0 || y >= map->height || x < 0 || x >= map->width)
		return ;
	if (map->true_grid[y][x] == '1' || map->true_grid[y][x] == 'V')
		return ;
	if (map->true_grid[y][x] == '0' || map->true_grid[y][x] == ' ')
		map->true_grid[y][x] = 'V';
	flood_fill(map, x + 1, y);
	flood_fill(map, x - 1, y);
	flood_fill(map, x, y + 1);
	flood_fill(map, x, y - 1);
}

void	find_to_flood(t_map *map, int *col, int *raw)
{
	int	x;
	int	y;

	x = 0;
	y = 0;
	while (map->true_grid[y] != NULL)
	{
		while (map->true_grid[y][x] != '\0')
		{
			if (map->true_grid[y][x] == 'N'
				|| map->true_grid[y][x] == 'S' || map->true_grid[y][x] == 'E'
				|| map->true_grid[y][x] == 'W')
			{
				*col = x;
				*raw = y;
				map->pos_x = x;
				map->pos_y = y;
				return ;
			}
			x++;
		}
		y++;
		x = 0;
	}
}

void reinitialize_map(t_map *map)
{
	int	i;
	int j;

	i = 0;
	while (map->true_grid[i] != NULL)
	{
		j = 0;
		while (map->true_grid[i][j] != '\0')
		{
			if (map->true_grid[i][j] == 'V')
			{
				if (i == map->pos_y && j == map->pos_x)
					map->true_grid[i][j] = map->dir;
				map->true_grid[i][j] = '0';
			}
			j++;
		}
		i++;
	}
}

int	completing_flood(t_map *map)
{
	int	x;
	int	y;

	find_to_flood(map, &x, &y);
	flood_fill(map, x, y);
	verify_flood(map);
	if (verify_map(map) == 1)
		return (1);
	reinitialize_map(map);
	return (0);
}

