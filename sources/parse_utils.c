/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jonandri <jonandri@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/04 16:54:50 by jonandri          #+#    #+#             */
/*   Updated: 2026/06/17 09:09:53 by jonandri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cube.h"

static int	is_xpm(char *str)
{
	size_t	len;

	len = ft_strlen(str);
	if (len < 4)
		return (1);
	if (ft_strncmp(str + (len - 4), ".xpm", 4) != 0)
		return (1);
	return (0);
}

static void remove_newline(char *line)
{
    int len;

    len = ft_strlen_cube(line);
    if (len > 0 && line[len - 1] == '\n')
        line[len - 1] = '\0';
}

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

static int	add_for_texture(char *line, int *i, int *j, int *has)
{
	int	k;

	*i = 2;
	if (*has)
	{
		printf("Error\n%c%c : Texture already exists.\n", line[0], line[1]);
		return (1);
	}
	while (line[*i] == 32 || line[*i] == '\t')
		(*i)++;
	if (line[*i] == '\0')
	{
		printf("Error\n%c%c : Missing 'xpm' texture.\n", line[0], line[1]);
		return (1);
	}
	*j = *i;
	while (line[*j] && line[*j] != 32 && line[*j] != '\t')
		(*j)++;
	k = (*j);
	while (line[k] == 32 || line[k] == '\t')
		k++;
	if (line[k] != '\0')
	{
		printf("Error\nInvalid %c%c : 'xpm' texture.\n", line[0], line[1]);
		return (1);
	}
	return (0);
}

static int	print_error_texture(int sign, char a, char b)
{
	if (sign == 0)
	{
		printf("Error\nInvalid %c%c: 'xpm' texture.\n", a, b);
		return (1);
	}
	if (sign == 1)
	{
		printf("Error\nCannot read %c%c : 'xpm' texture.\n", a, b);
		return (1);
	}
	return (0);
}

int parse_texture(char *line, char **path, int *has)
{
    int		i;
    int		j;
    char	*tmp;

	if (add_for_texture(line, &i, &j, has) == 1)
		return (1);
    *path = malloc(j - i + 1);
    if (!*path)
        return (1);
    copy_substring(*path, line, i, j);
    *has = 1;
	if (!path || !*path)
		return (1);
	tmp = *path;
	*path = ft_strtrim(tmp, " \t\n");
	free(tmp);
	if (!*path)
		return (1);
	if (is_xpm(*path) == 1)
		return (print_error_texture(0, line[0], line[1]));
	if (access(*path, R_OK) != 0)
		return (print_error_texture(1, line[0], line[1]));
    return (0);
}

static int print_error(char *s)
{
	printf("Error\nMissing '%s' texture.\n", s);
	return (1);
}

static int verify_texture(t_map *map)
{
	if (!map->has_no)
		return (print_error("NO"));
	if (!map->has_so)
		return (print_error("SO"));
	if (!map->has_we)
		return (print_error("WE"));
	if (!map->has_ea)
		return (print_error("EA"));
	if (!map->has_floor)
		return (print_error("F"));
	if (!map->has_ceiling)
		return (print_error("C"));
	return (0);
}

int start_map_parsing(char *line, t_map *map)
{
	t_line *new_line;

	if (verify_texture(map) == 1)
        return (1);
	new_line = malloc(sizeof(t_line));
	if (new_line == NULL)
		return (1);
	new_line->content = ft_strdup(line);
    if (!new_line->content)
    {
        free(new_line);
        return (1);
    }
	new_line->next = NULL;
	map->first_line = new_line;
	map->last_line = new_line;
	map->map_started = 1;
	map->height = 1;
	return (0);
}

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
		return (1);
	return (0);
}

static int	parse_floor_and_ceiling(t_map *map, char *line, int *i)
{
	if (line[*i] == 'F')
		return (parse_color(line + *i, &map->floor_c, &map->has_floor));
	return (parse_color(line + *i, &map->ceiling_c, &map->has_ceiling));
}

int parse_line(char *line, t_map *map)
{
    int		i;
    int     j;

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
	printf("Error\nSomething went wrong.\n");
    return (1);
}
