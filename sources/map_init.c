/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map_init.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jonandri <jonandri@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/03 19:50:12 by jonandri          #+#    #+#             */
/*   Updated: 2026/05/03 20:25:03 by jonandri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cube.h"

void	init_map(t_map *map)
{
	map->no_path = NULL;
	map->so_path = NULL;
	map->we_path = NULL;
	map->ea_path = NULL;
	map->grid = NULL;
	map->has_ceiling = 0;
	map->has_floor = 0;
	map->has_no = 0;
	map->has_so = 0;
	map->has_we = 0;
	map->has_ea = 0;
}
