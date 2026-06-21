/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map_parsing.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jonandri <jonandri@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/06 20:27:47 by jonandri          #+#    #+#             */
/*   Updated: 2026/06/18 07:11:43 by jonandri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cube.h"

static void	free_map_error(t_map *map, int *i)
{
	while (--(*i) >= 0)
		free(map->grid[*i]);
	free(map->grid);
}

static t_line	*is_current(t_map *map, int *i)
{
	map->width = get_map_width(map);
	*i = 0;
	return (map->first_line);
}

static void	for_grid(t_map *map, t_line *cur, int *i, int *j)
{
	if (((*j) < map->len) && (cur->content[*j] != '\0'))
		map->grid[*i][*j] = cur->content[*j];
	else
		map->grid[*i][*j] = ' ';
}

char	**to_grid(t_map *map)
{
	t_line	*current;
	int		i;
	int		j;

	map->grid = malloc(sizeof(char *) * (map->height + 1));
	if (!map->grid)
		return (NULL);
	current = is_current(map, &i);
	while (current && i < map->height)
	{
		map->grid[i] = malloc(sizeof(char) * (map->width + 1));
		if (!map->grid[i])
			return (free_map_error(map, &i), NULL);
		map->len = ft_strlen_cube(current->content);
		j = 0;
		while (j < map->width)
		{
			for_grid(map, current, &i, &j);
			j++;
		}
		map->grid[i++][map->width] = '\0';
		current = current->next;
	}
	map->grid[i] = NULL;
	return (map->grid);
}

int	verify_error(char **map, t_map *s)
{
	int	i;
	int	j;

	i = 0;
	s->has_dir = 0;
	while (map[i] != NULL)
	{
		j = 0;
		while (map[i][j] != '\0')
		{
			if (map[i][j] == 'N' || map[i][j] == 'S'
				|| map[i][j] == 'E' || map[i][j] == 'W')
			{
				s->has_dir++;
				s->dir = map[i][j];
			}
			else if (map[i][j] != 32 && map[i][j] != '0' && map[i][j] != '1')
				return (1);
			j++;
		}
		i++;
	}
	if (s->has_dir > 1 || s->has_dir == 0)
		return (1);
	return (0);
}
