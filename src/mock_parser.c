/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mock_parser.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: miokrako <miokrako@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/29 20:39:02 by miokrako          #+#    #+#             */
/*   Updated: 2026/06/17 xx:xx:xx by Grok              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/cub3d.h"
#include "../header/cube.h"

static void	free_mock_textures(t_config *cfg)
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

// static void	free_mock_map(t_config *cfg)
// {
// 	int	i;

// 	if (!cfg || !cfg->map)
// 		return ;
// 	i = 0;
// 	while (cfg->map[i])
// 	{
// 		free(cfg->map[i]);
// 		cfg->map[i] = NULL;
// 		i++;
// 	}
// 	free(cfg->map);
// 	cfg->map = NULL;
// }

// void	cleanup_mock_config(t_config *cfg)
// {
// 	if (!cfg)
// 		return ;
// 	free_mock_textures(cfg);
// 	free_mock_map(cfg);
// }


static void	setup_direction(t_config *cfg, char dir)
{
	if (dir == 'N')
	{
		cfg->spawn_dir_x = -1.0;
		cfg->spawn_dir_y = 0.0;
		cfg->spawn_plane_x = 0.0;
		cfg->spawn_plane_y = 0.66;
	}
	else if (dir == 'S')
	{
		cfg->spawn_dir_x = 1.0;
		cfg->spawn_dir_y = 0.0;
		cfg->spawn_plane_x = 0.0;
		cfg->spawn_plane_y = -0.66;
	}
	else if (dir == 'E')
	{
		cfg->spawn_dir_x = 0.0;
		cfg->spawn_dir_y = 1.0;
		cfg->spawn_plane_x = -0.66;
		cfg->spawn_plane_y = 0.0;
	}
	else if (dir == 'W')
	{
		cfg->spawn_dir_x = 0.0;
		cfg->spawn_dir_y = -1.0;
		cfg->spawn_plane_x = 0.66;
		cfg->spawn_plane_y = 0.0;
	}
}


static int	setup_player(t_config *cfg, t_map *map)
{
	if (!map || map->pos_x < 0 || map->pos_y < 0 || !map->has_dir)
		return (1);

	/* Position centrée dans la case */
	cfg->spawn_x = map->pos_x + 0.5;
	cfg->spawn_y = map->pos_y + 0.5;

	setup_direction(cfg, map->dir);

	return (0);
}


static int	setup_textures(t_config *cfg, t_map *map)
{
	int	i;

	ft_memset(cfg->tex_path, 0, sizeof(cfg->tex_path));

	if (!map->no_path || !map->so_path || !map->we_path || !map->ea_path)
		return (1);

	cfg->tex_path[NORTH] = ft_strdup(map->no_path);
	cfg->tex_path[SOUTH] = ft_strdup(map->so_path);
	cfg->tex_path[EAST]  = ft_strdup(map->ea_path);
	cfg->tex_path[WEST]  = ft_strdup(map->we_path);

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

static void	setup_colors(t_config *cfg, t_map *map)
{
	cfg->floor_color = map->floor_c;
	cfg->ceil_color  = map->ceiling_c;
}

static int	setup_map(t_config *cfg, t_map *map)
{
	int	h;
	int	i;
	int	len;
	int	max_w;

	if (!map || !map->true_grid || map->height <= 0)
		return (1);

	/* Calcul hauteur */
	h = 0;
	while (map->true_grid[h])
		h++;
	cfg->map_h = h;

	cfg->map = malloc(sizeof(char *) * (h + 1));
	if (!cfg->map)
		return (1);

	max_w = 0;
	i = 0;
	while (i < h)
	{
		cfg->map[i] = ft_strdup(map->true_grid[i]);
		if (!cfg->map[i])
		{
			while (--i >= 0)
				free(cfg->map[i]);
			free(cfg->map);
			cfg->map = NULL;
			return (1);
		}
		len = (int)ft_strlen(map->true_grid[i]);
		if (len > max_w)
			max_w = len;
		i++;
	}
	cfg->map[h] = NULL;
	cfg->map_w = max_w;

	return (0);
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