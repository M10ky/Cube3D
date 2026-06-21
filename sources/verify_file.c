/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   verify_file.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jonandri <jonandri@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/17 16:53:57 by jonandri          #+#    #+#             */
/*   Updated: 2026/06/17 17:14:54 by jonandri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cube.h"

int	is_xpm(char *str)
{
	size_t	len;

	len = ft_strlen(str);
	if (len < 4)
		return (1);
	if (ft_strncmp(str + (len - 4), ".xpm", 4) != 0)
		return (1);
	return (0);
}

int	is_cub(char *str)
{
	size_t	len;

	len = ft_strlen(str);
	if (len < 4)
		return (1);
	if (ft_strncmp(str + (len - 4), ".cub", 4) != 0)
		return (1);
	return (0);
}
