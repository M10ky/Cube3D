/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jonandri <jonandri@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/04 16:54:50 by jonandri          #+#    #+#             */
/*   Updated: 2026/06/21 20:27:43 by jonandri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cube.h"

static int	is_map_line(char *line)
{
	int	i;

	i = 0;
	while (line[i] == 32 || line[i] == '\t')
		i++;
	if (line[i] == '1' || line[i] == '0')
		return (0);
	return (1);
}

static int	search_i(char *line)
{
	int	i;

	i = 0;
	while (line[i] == ' ' || line[i] == '\t')
		(i)++;
	return (i);
}

static int	end_map(t_map *map)
{
	if (map->map_started == 1)
	{
		printf("Error\nThere must be something wrong.\n");
		printf("Double-check your settings !\n");
		return (1);
	}
	return (0);
}

static int	parse_floor_and_ceiling(t_map *map, char *line, int *i)
{
	if (line[*i] == 'F')
		return (parse_color(line + *i, &map->floor_c, &map->has_floor, map));
	return (parse_color(line + *i, &map->ceiling_c, &map->has_ceiling, map));
}

int	parse_line(char *line, t_map *map)
{
	int	i;
	int	j;

	j = 0;
	i = search_i(line);
	if (line[i] == '\n' || line[i] == '\0')
		return (end_map(map));
	remove_newline(line);
	if (map->map_started == 1)
		return (add_line_to_map(line, map));
	else if (i == 0 && line[i] == 'N' && line[i + 1] == 'O')
		return (parse_texture(line + i, &map->no_path, &map->has_no));
	else if (i == 0 && line[i] == 'S' && line[i + 1] == 'O')
		return (parse_texture(line + i, &map->so_path, &map->has_so));
	else if (i == 0 && line[i] == 'W' && line[i + 1] == 'E')
		return (parse_texture(line + i, &map->we_path, &map->has_we));
	else if (i == 0 && line[i] == 'E' && line[i + 1] == 'A')
		return (parse_texture(line + i, &map->ea_path, &map->has_ea));
	else if (i == 0 && (line[i] == 'F' || line[i] == 'C'))
		return (parse_floor_and_ceiling(map, line, &i));
	else if (is_map_line(line) == 0)
		return (start_map_parsing(line + j, map));
	printf("Error\nThere must be something wrong.\n");
	printf("Double-check your settings !\n");
	return (1);
}
