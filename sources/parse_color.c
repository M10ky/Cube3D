/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_color.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jonandri <jonandri@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/04 18:10:35 by jonandri          #+#    #+#             */
/*   Updated: 2026/05/04 18:14:38 by jonandri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cube.h"

int	parse_color(char *line, int *color, int *has)
{
    int	i;
    int r;
	int	g;
	int	b;
    
    if (*has)
        return (0);
    i = 1;
    while (line[i] == ' ' || line[i] == '\t')
        i++;
    if (!parse_rgb(line, &i, &r, &g, &b))
        return (0);
    *color = (r << 16) | (g << 8) | b;
    *has = 1;
    return (1);
}

int	parse_rgb(char *line, int *i, int *r, int *g, int *b)
{
    *r = get_number(line, i);
    if (line[*i] != ',')
		return (0);
    (*i)++;
    *g = get_number(line, i);
    if (line[*i] != ',')
		return (0);
    (*i)++;
    *b = get_number(line, i);
    if (*r < 0 || *r > 255 || *g < 0 || *g > 255 || *b < 0 || *b > 255)
        return (0);
    return (1);
}

int	get_number(char *line, int *i)
{
    int num;
    
    num = 0;
    while (line[*i] == ' ' || line[*i] == '\t')
        (*i)++;
    while (line[*i] >= '0' && line[*i] <= '9')
    {
        num = num * 10 + (line[*i] - '0');
        (*i)++;
    }
    return (num);
}