/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   setup.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: miokrako <miokrako@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/21 21:16:21 by miokrako          #+#    #+#             */
/*   Updated: 2026/06/21 23:51:12 by miokrako         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes_raycasting/cub3d.h"

void	setup_direction(t_config *cfg, char dir)
{
	if (dir == 'N')
		set_dir_north(cfg);
	else if (dir == 'S')
		set_dir_south(cfg);
	else if (dir == 'E')
		set_dir_east(cfg);
	else if (dir == 'W')
		set_dir_west(cfg);
}

int	setup_player(t_config *cfg, t_map *map)
{
	if (!map || map->pos_x < 0 || map->pos_y < 0 || !map->has_dir)
		return (1);
	cfg->spawn_x = map->pos_x + 0.5;
	cfg->spawn_y = map->pos_y + 0.5;
	setup_direction(cfg, map->dir);
	return (0);
}

int	setup_textures(t_config *cfg, t_map *map)
{
	int	i;

	ft_memset(cfg->tex_path, 0, sizeof(cfg->tex_path));
	if (!map->no_path || !map->so_path || !map->we_path || !map->ea_path)
		return (1);
	cfg->tex_path[NORTH] = ft_strdup(map->no_path);
	cfg->tex_path[SOUTH] = ft_strdup(map->so_path);
	cfg->tex_path[EAST] = ft_strdup(map->ea_path);
	cfg->tex_path[WEST] = ft_strdup(map->we_path);
	i = 0;
	while (i < 4)
	{
		if (!cfg->tex_path[i])
		{
			free_mock_textures(cfg);
			return (1);
		}
		i++;
	}
	return (0);
}

void	setup_colors(t_config *cfg, t_map *map)
{
	cfg->floor_color = map->floor_c;
	cfg->ceil_color = map->ceiling_c;
}

int	setup_map(t_config *cfg, t_map *map)
{
	int	h;
	int	max_w;

	if (!map || !map->true_grid || map->height <= 0)
		return (1);
	h = count_map_height(map);
	cfg->map_h = h;
	cfg->map = malloc(sizeof(char *) * (h + 1));
	if (!cfg->map)
		return (1);
	max_w = duplicate_map_rows(cfg, map, h);
	if (max_w < 0)
		return (1);
	cfg->map[h] = NULL;
	cfg->map_w = max_w;
	return (0);
}
