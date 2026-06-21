/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_color.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jonandri <jonandri@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/04 18:10:35 by jonandri          #+#    #+#             */
/*   Updated: 2026/06/21 21:10:24 by jonandri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cube.h"

int	parse_color(char *line, int *color, int *has, t_map *map)
{
	int	i;

	if (*has)
	{
		printf("Error\ncolor '%c' already exists.\n", line[0]);
		return (1);
	}
	i = 1;
	if (line[i] != 32 && line[i] != '\t')
	{
		printf("Error\nInvalid %c color.\n", line[0]);
		return (1);
	}
	while (line[i] == ' ' || line[i] == '\t')
		i++;
	if (parse_rgb(line, &i, map) == 1)
	{
		printf("Error\nInvalid %c color.\n", line[0]);
		return (1);
	}
	*color = (map->r << 16) | (map->g << 8) | map->b;
	*has = 1;
	return (0);
}

int	parse_rgb(char *line, int *i, t_map *map)
{
	if (line[*i] < '0' || line[*i] > '9')
		return (1);
	map->r = get_number(line, i);
	if (line[*i] != ',')
		return (1);
	(*i)++;
	if (line[*i] < '0' || line[*i] > '9')
		return (1);
	map->g = get_number(line, i);
	if (line[*i] != ',')
		return (1);
	(*i)++;
	if (line[*i] < '0' || line[*i] > '9')
		return (1);
	map->b = get_number(line, i);
	while (line[*i] != '\0')
	{
		if (line[*i] != 32 && line[*i] != '\t')
			return (1);
		(*i)++;
	}
	if (map->r < 0 || map->r > 255 || map->g < 0
		|| map->g > 255 || map->b < 0 || map->b > 255)
		return (1);
	return (0);
}

int	get_number(char *line, int *i)
{
	int	num;

	num = 0;
	while (line[*i] >= '0' && line[*i] <= '9')
	{
		num = num * 10 + (line[*i] - '0');
		(*i)++;
	}
	return (num);
}
