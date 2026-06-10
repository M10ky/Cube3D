/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mock_parser.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: miokrako <miokrako@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/29 20:39:02 by miokrako          #+#    #+#             */
/*   Updated: 2026/06/11 00:07:35 by miokrako         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "../includes/cub3d.h"

static char *g_mock_map[] = {
    "111111111111111",   /* y=0  */
    "100000001000001",   /* y=1  */
    "100110001000001",   /* y=2  */
    "100110000000001",   /* y=3  */
    "100000000000001",   /* y=4  */
    "100000000111001",   /* y=5  */
    "100000001000001",   /* y=6  */
    "100000001000001",   /* y=7  */
    "100000001111001",   /* y=8  */
    "100000000000001",   /* y=9  */
    "111111111111111",   /* y=10 */
    NULL                 /* faranana am NULL                     */
};

static	int	setup_map(t_config *cfg)
{
	int	h;
	int	w;
	int	len;

	h = 0;
	while (g_mock_map[h] != NULL)
		h++;
	cfg->map_h = h;
	cfg->map = (char **)malloc(sizeof(char *) * (h + 1));
	if (!cfg->map)
		return (1);
	w = 0;
	h = 0;
	while (g_mock_map[h] != NULL)
	{
		cfg->map[h] = ft_strdup(g_mock_map[h]);
		if (!cfg->map[h])
			return (1);
		len = (int)ft_strlen(g_mock_map[h]);
		if (len > w)
			w = len;
		h++;
	}
	cfg->map[h] = NULL;
	cfg->map_w = w;
	return (0);
}


static	void	setup_player(t_config *cfg)
{
	cfg->spawn_x = 7;
	cfg->spawn_y = 5.5;
	cfg->spawn_dir_x = 0.0;
	cfg->spawn_dir_y = -1.0;
	cfg->spawn_plane_x = 0.66;
	cfg->spawn_plane_y = 0.0;
}

static	int	setup_textures(t_config *cfg)
{
    cfg->tex_path[NORTH] = ft_strdup("./textures/1.xpm");
    cfg->tex_path[SOUTH] = ft_strdup("./textures/5.xpm");
    cfg->tex_path[EAST]  = ft_strdup("./textures/3.xpm");
    cfg->tex_path[WEST]  = ft_strdup("./textures/4.xpm");
    if (!cfg->tex_path[NORTH] || !cfg->tex_path[SOUTH]
        || !cfg->tex_path[EAST] || !cfg->tex_path[WEST])
        return (1);
    return (0);
}

static	void	setup_colors(t_config *cfg)
{
	cfg->floor_color = (100 << 16) | (80 << 8) | 50;

	cfg->ceil_color = (60 << 16) | (120 << 8) | 200;
}


int	init_mock_config(t_config *cfg)
{
	setup_player(cfg);
	setup_colors(cfg);
	if (setup_textures(cfg) != 0)
		return (print_error("Mock: failed to duplicate texture paths"));
	if (setup_map(cfg) != 0)
		return (print_error("Mock: failed to duplicate map"));
	return (0);
}
