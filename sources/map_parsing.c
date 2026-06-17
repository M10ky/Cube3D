/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map_parsing.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jonandri <jonandri@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/06 20:27:47 by jonandri          #+#    #+#             */
/*   Updated: 2026/06/17 09:16:02 by jonandri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cube.h"

int add_line_to_map(char *line, t_map *map)
{
    t_line *new_line;

    new_line = malloc(sizeof(t_line));
    if (!new_line)
        return (1);
    new_line->content = ft_strdup(line);
    if (!new_line->content)
    {
        free(new_line);
        return (1);   
    }
    new_line->next = NULL;
    map->last_line->next = new_line;
    map->last_line = new_line;
    map->height++;
    return (0);
}

int	get_map_width(t_map *map)
{
	t_line *current;
	int		max;

	max = 0;
	current = map->first_line;
	while (current)
	{
		if (ft_strlen_cube(current->content) > max)
			max = ft_strlen_cube(current->content);
		current = current->next;
	}
	return (max);
}

char **to_grid(t_map *map)
{
    t_line	*current;
    int		i;
    int		j;
    int		max_width;
    int		len;

	max_width = get_map_width(map);
	map->width = max_width;
	map->grid = malloc(sizeof(char *) * (map->height + 1));
	if (!map->grid)
		return (NULL);
	current = map->first_line;
	i = 0;
	while (current && i < map->height)
    {
        map->grid[i] = malloc(sizeof(char) * (max_width + 1));
        if (!map->grid[i])
        {
            while (--i >= 0)
                free(map->grid[i]);
            free(map->grid);
            return (NULL);
        }
        len = ft_strlen_cube(current->content);
        j = 0;
        while (j < max_width)
        {
            if (j < len && current->content[j] != '\0')
                    map->grid[i][j] = current->content[j];
            else
                map->grid[i][j] = ' ';
            j++;
        }
        map->grid[i][max_width] = '\0';
        i++;
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
			if (map[i][j] == 'N' || map[i][j] == 'S')
            {
				s->has_dir++;
                s->dir = map[i][j];
            }
			else if (map[i][j] == 'E' || map[i][j] == 'W')
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
