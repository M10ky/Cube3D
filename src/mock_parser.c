/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mock_parser.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: miokrako <miokrako@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/29 20:39:02 by miokrako          #+#    #+#             */
/*   Updated: 2026/06/15 22:24:00 by miokrako         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "../includes/cub3d.h"
#include "../header/cube.h"

// static char *g_mock_map[] = {
//     "111111111111111",   /* y=0  */
//     "100000001000001",   /* y=1  */
//     "100110001000001",   /* y=2  */
//     "100110000000001",   /* y=3  */
//     "100000000000001",   /* y=4  */
//     "100000000111001",   /* y=5  */
//     "100000001000001",   /* y=6  */
//     "100000001000001",   /* y=7  */
//     "100000001111001",   /* y=8  */
//     "100000000000001",   /* y=9  */
//     "111111111111111",   /* y=10 */
//     NULL                 /* faranana am NULL                     */
// };

static	int	setup_map(t_config *cfg, t_map *map)
{
	int	h;
	int	w;
	int	len;

	h = 0;
	while (map->true_grid[h] != NULL)
		h++;
	cfg->map_h = h;
	cfg->map = (char **)malloc(sizeof(char *) * (h + 1));
	if (!cfg->map)
		return (1);
	w = 0;
	h = 0;
	while (map->true_grid[h] != NULL)
	{
		cfg->map[h] = ft_strdup(map->true_grid[h]);
		if (!cfg->map[h])
			return (1);
		len = (int)ft_strlen(map->true_grid[h]);
		if (len > w)
			w = len;
		h++;
	}
	cfg->map[h] = NULL;
	cfg->map_w = w;
	return (0);
}


static	void	setup_player(t_config *cfg, t_map *map)
{
	cfg->spawn_x = map->pos_x;
	cfg->spawn_y = map->pos_y;

	printf("\n x = %f et y = %f \n", cfg->spawn_x, cfg->spawn_y);
	cfg->spawn_dir_x = 0.0;
	cfg->spawn_dir_y = 1.0;
	cfg->spawn_plane_x = 0.66;
	cfg->spawn_plane_y = 0.0;
}

static	int	setup_textures(t_config *cfg, t_map *map)
{
    cfg->tex_path[NORTH] = map->no_path;
    cfg->tex_path[SOUTH] = map->so_path;
    cfg->tex_path[EAST]  = map->ea_path;
    cfg->tex_path[WEST]  = map->we_path;
    if (!cfg->tex_path[NORTH] || !cfg->tex_path[SOUTH]
        || !cfg->tex_path[EAST] || !cfg->tex_path[WEST])
        return (1);
    return (0);
}

static	void	setup_colors(t_config *cfg, t_map *map)
{
	cfg->floor_color = map->floor_c;
	cfg->ceil_color = map->ceiling_c;
}


int	init_mock_config(t_config *cfg, t_map *map)
{
	setup_player(cfg, map);
	setup_colors(cfg, map);
	if (setup_textures(cfg, map) != 0)
		return (print_error("Mock: failed to duplicate texture paths"));
	if (setup_map(cfg, map) != 0)
		return (print_error("Mock: failed to duplicate map"));
	// cfg->map = map->true_grid;

	// for (int y = 0; y < 5; y++)
	// {
	// 	for (int x = 0; x < 5; x++)
	// 		printf("%d", map->true_grid[y][x]);
	// 	printf("\n");
	// }
	return (0);
}
