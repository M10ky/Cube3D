/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_main.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: miokrako <miokrako@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/21 23:32:36 by miokrako          #+#    #+#             */
/*   Updated: 2026/06/21 23:57:59 by miokrako         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes_raycasting/cub3d.h"

void	free_map(t_map *map)
{
	t_line	*tmp;

	if (!map)
		return ;
	while (map->first_line)
	{
		tmp = map->first_line->next;
		free(map->first_line->content);
		free(map->first_line);
		map->first_line = tmp;
	}
}

void	free_all(t_map *map)
{
	if (!map)
		return ;
	free_paths(map);
	free_grid(map);
}

void	free_paths(t_map *map)
{
	if (map->no_path)
	{
		free(map->no_path);
		map->no_path = NULL;
	}
	if (map->so_path)
	{
		free(map->so_path);
		map->so_path = NULL;
	}
	if (map->ea_path)
	{
		free(map->ea_path);
		map->ea_path = NULL;
	}
	if (map->we_path)
	{
		free(map->we_path);
		map->we_path = NULL;
	}
}

void	free_grid(t_map *map)
{
	int	i;

	if (!map->grid)
		return ;
	i = 0;
	while (map->grid[i] != NULL)
	{
		free(map->grid[i]);
		i++;
	}
	free(map->grid);
	map->grid = NULL;
	map->true_grid = NULL;
}
