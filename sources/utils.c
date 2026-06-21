/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jonandri <jonandri@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/18 07:08:54 by jonandri          #+#    #+#             */
/*   Updated: 2026/06/18 07:09:22 by jonandri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cube.h"

int	get_map_width(t_map *map)
{
	t_line	*current;
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
