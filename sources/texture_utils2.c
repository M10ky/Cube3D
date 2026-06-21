/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   texture_utils2.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jonandri <jonandri@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/17 17:51:22 by jonandri          #+#    #+#             */
/*   Updated: 2026/06/18 07:06:37 by jonandri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cube.h"

int	parse_texture(char *line, char **path, int *has)
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
		return (print_texture(0, line[0], line[1]), 1);
	if (access(*path, R_OK) != 0)
		return (print_texture(1, line[0], line[1]), 1);
	return (0);
}

void	remove_newline(char *line)
{
	int	len;

	len = ft_strlen_cube(line);
	if (len > 0 && line[len - 1] == '\n')
		line[len - 1] = '\0';
}

void	copy_substring(char *dest, char *src, int start, int end)
{
	int	k;

	k = 0;
	while (start < end)
	{
		dest[k] = src[start];
		start++;
		k++;
	}
	dest[k] = '\0';
}

int	start_map_parsing(char *line, t_map *map)
{
	t_line	*new_line;

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

int	add_line_to_map(char *line, t_map *map)
{
	t_line	*new_line;

	new_line = malloc(sizeof(t_line));
	if (!new_line)
		return (0);
	new_line->content = ft_strdup(line);
	if (!new_line->content)
	{
		free(new_line);
		return (0);
	}
	new_line->next = NULL;
	map->last_line->next = new_line;
	map->last_line = new_line;
	map->height++;
	return (-1);
}
