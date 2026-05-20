/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jonandri <jonandri@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/04 16:54:50 by jonandri          #+#    #+#             */
/*   Updated: 2026/05/19 17:48:22 by jonandri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cube.h"

void copy_substring(char *dest, char *src, int start, int end)
{
    int k;

    k = 0;
    while (start < end)
    {
        dest[k] = src[start];
        start++;
        k++;
    }
    dest[k] = '\0';
}

int parse_texture(char *line, char **path, int *has)
{
    int i;
    int j;
    int k;
    
    if (*has)
        return (0);
    i = 2;
    while (line[i] == ' ' || line[i] == '\t')
        i++;
    if (line[i] == '\0')
        return (0);
    j = i;
    while (line[j] && line[j] != ' ' && line[j] != '\t')
        j++;
    k = j;
    while (line[k] == ' ' || line[k] == '\t')
        k++;
    if (line[k] != '\0')
        return (0);
    *path = malloc(j - i + 1);
    if (!*path)
        return (0);
    copy_substring(*path, line, i, j);
    *has = 1;
    if (access(*path, R_OK) != 0)
        return (0);
    return (1);
}

int start_map_parsing(char *line, t_map *map)
{
	t_line *new_line;
	size_t	j;

	j = 0;
    if (!map->has_no || !map->has_so || !map->has_we || !map->has_ea)
        return (0);
    if (!map->has_floor || !map->has_ceiling)
        return (0);
	new_line = malloc(sizeof(t_line));
	if (new_line == NULL)
		return (0);
	new_line->content = ft_strdup(line);
    if (!new_line->content)
    {
        free(new_line);
        return (0);
    }
	new_line->next = NULL;
	map->first_line = new_line;
	map->last_line = new_line;
	map->map_started = 1;
	map->height = 1;
	return (1);
}

static int	is_map_line(char *line)
{
	if (*line == "1" || *line == 32 || *line == '\t')
		return (1);
	return (0);
}

int parse_line(char *line, t_map *map)
{
    int i;

    i = 0;
    while (line[i] == ' ' || line[i] == '\t')
        i++;
    if (line[i] == '\n' || line[i] == '\0')
        return (1);
    remove_newline(line);
    if (map->map_started == 1)
		return (add_line_to_map(line, map));
	if (line[i] == 'N' && line[i + 1] == 'O')
		return (parse_texture(line + i, &map->no_path, &map->has_no));
	else if (line[i] == 'S' && line[i + 1] == 'O')
		return (parse_texture(line + i, &map->so_path, &map->has_so));
	else if (line[i] == 'W' && line[i + 1] == 'E')
		return (parse_texture(line + i, &map->we_path, &map->has_we));
	else if (line[i] == 'E' && line[i + 1] == 'A')
		return (parse_texture(line + i, &map->ea_path, &map->has_ea));
	else if (line[i] == 'F')
		return (parse_color(line + i, &map->floor_c, &map->has_floor));
	else if (line[i] == 'C')
		return (parse_color(line + i, &map->ceiling_c, &map->has_ceiling));
	else if (is_map_line(line + i) == 1)
	{
		if (start_map_parsing(line + i, map) == 0)
			return (error);
		return (1);
	}
	return (0);
}
