/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: miokrako <miokrako@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/21 20:43:34 by miokrako          #+#    #+#             */
/*   Updated: 2026/06/21 20:44:01 by miokrako         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/cub3d.h"

int	print_error(const char *msg)
{
	write(2, "Error\n", 6);
	write(2, msg, ft_strlen(msg));
	write(2, "\n", 1);
	return (1);
}

void	free_cfg_textures(t_config *cfg)
{
	int	i;

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

void	free_cfg_map(t_config *cfg)
{
	int	i;

	if (!cfg->map)
		return ;
	i = 0;
	while (cfg->map[i])
	{
		free(cfg->map[i]);
		cfg->map[i] = NULL;
		i++;
	}
	free(cfg->map);
	cfg->map = NULL;
}

void	cleanup_mock_config(t_config *cfg)
{
	if (!cfg)
		return ;
	free_cfg_textures(cfg);
	free_cfg_map(cfg);
	cfg->map_h = 0;
	cfg->map_w = 0;
}
