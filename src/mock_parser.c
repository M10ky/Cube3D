/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mock_parser.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: miokrako <miokrako@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/29 20:39:02 by miokrako          #+#    #+#             */
/*   Updated: 2026/06/21 21:22:16 by miokrako         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/cub3d.h"

void	free_mock_textures(t_config *cfg)
{
	int	i;

	if (!cfg)
		return ;
	i = 0;
	while (i < 4)
	{
		if (cfg->tex_path[i])
		{
			free(cfg->tex_path[i]);
			cfg->tex_path[i] = NULL;
		}
		i++;
	}
}

void	free_partial_map(char **map, int i)
{
	while (--i >= 0)
		free(map[i]);
	free(map);
}

int	duplicate_map_rows(t_config *cfg, t_map *map, int h)
{
	int	i;
	int	len;
	int	max_w;

	max_w = 0;
	i = 0;
	while (i < h)
	{
		cfg->map[i] = ft_strdup(map->true_grid[i]);
		if (!cfg->map[i])
		{
			free_partial_map(cfg->map, i);
			cfg->map = NULL;
			return (-1);
		}
		len = (int)ft_strlen(map->true_grid[i]);
		if (len > max_w)
			max_w = len;
		i++;
	}
	return (max_w);
}

int	count_map_height(t_map *map)
{
	int	h;

	h = 0;
	while (map->true_grid[h])
		h++;
	return (h);
}

int	init_mock_config(t_config *cfg, t_map *map)
{
	if (!cfg || !map)
		return (print_error("Mock: invalid parameters"));
	ft_memset(cfg, 0, sizeof(t_config));
	if (setup_player(cfg, map) != 0)
		return (print_error("Mock: invalid player position or direction"));
	setup_colors(cfg, map);
	if (setup_textures(cfg, map) != 0)
	{
		cleanup_mock_config(cfg);
		return (print_error("Mock: failed to duplicate texture paths"));
	}
	if (setup_map(cfg, map) != 0)
	{
		cleanup_mock_config(cfg);
		return (print_error("Mock: failed to duplicate map"));
	}
	return (0);
}
