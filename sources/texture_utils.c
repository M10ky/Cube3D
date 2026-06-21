/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   texture_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jonandri <jonandri@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/17 17:19:00 by jonandri          #+#    #+#             */
/*   Updated: 2026/06/21 20:20:18 by jonandri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cube.h"

static void	print_error(char *s, int a, int *i)
{
	if (*i == 0)
	{
		if (a == 1)
			printf("Error\nMissing '%s' texture.\n", s);
		else
			printf("Error\nMissing '%s' color.\n", s);
	}
	else
	{
		if (a == 1)
			printf("Missing '%s' texture.\n", s);
		else
			printf("Missing '%s' color.\n", s);
	}
	(*i)++;
}

static void	print_error_texture(int n, char a, char b)
{
	if (n == 0)
		printf("Error\n%c%c: Texture already exists\n", a, b);
	else if (n == 1)
		printf("Error\n%c%c: Missing 'xpm' texture\n", a, b);
	else if (n == 2)
		printf("Error\nInvalid %c%c: 'xpm' texture\n", a, b);
}

int	add_for_texture(char *line, int *i, int *j, int *has)
{
	int	k;

	*i = 2;
	if (*has)
		return (print_error_texture(0, line[0], line[1]), 1);
	if (line[*i] != 32 && line[*i] != '\t')
		return (print_error_texture(2, line[0], line[1]), 1);
	while (line[*i] == 32 || line[*i] == '\t')
		(*i)++;
	if (line[*i] == '\0')
		return (print_error_texture(1, line[0], line[1]), 1);
	*j = *i;
	while (line[*j] && line[*j] != 32 && line[*j] != '\t')
		(*j)++;
	k = (*j);
	while (line[k] == 32 || line[k] == '\t')
		k++;
	if (line[k] != '\0')
		return (print_error_texture(2, line[0], line[1]), 1);
	return (0);
}

int	verify_texture(t_map *map)
{
	int	i;

	i = 0;
	if (!map->has_no)
		print_error("NO", 1, &i);
	if (!map->has_so)
		print_error("SO", 1, &i);
	if (!map->has_we)
		print_error("WE", 1, &i);
	if (!map->has_ea)
		print_error("EA", 1, &i);
	if (!map->has_floor)
		print_error("F", 0, &i);
	if (!map->has_ceiling)
		print_error("C", 0, &i);
	if (i > 0)
	{
		printf("\nThere must be something wrong.\n");
		printf("Double-check your settings !\n");
		return (1);
	}
	return (0);
}

void	print_texture(int sign, char a, char b)
{
	if (sign == 0)
		printf("Error\nInvalid %c%c: 'xpm' texture.\n", a, b);
	if (sign == 1)
		printf("Error\nCannot read %c%c : 'xpm' texture.\n", a, b);
}
