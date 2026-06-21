/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   set_dir.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: miokrako <miokrako@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/21 21:10:17 by miokrako          #+#    #+#             */
/*   Updated: 2026/06/21 21:11:46 by miokrako         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/cub3d.h"

void	set_dir_north(t_config *cfg)
{
	cfg->spawn_dir_x = 0.0;
	cfg->spawn_dir_y = -1.0;
	cfg->spawn_plane_x = 0.66;
	cfg->spawn_plane_y = 0.0;
}

void	set_dir_south(t_config *cfg)
{
	cfg->spawn_dir_x = 0.0;
	cfg->spawn_dir_y = 1.0;
	cfg->spawn_plane_x = -0.66;
	cfg->spawn_plane_y = 0.0;
}

void	set_dir_east(t_config *cfg)
{
	cfg->spawn_dir_x = 1.0;
	cfg->spawn_dir_y = 0.0;
	cfg->spawn_plane_x = 0.0;
	cfg->spawn_plane_y = 0.66;
}

void	set_dir_west(t_config *cfg)
{
	cfg->spawn_dir_x = -1.0;
	cfg->spawn_dir_y = 0.0;
	cfg->spawn_plane_x = 0.0;
	cfg->spawn_plane_y = -0.66;
}
