/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   read_files.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jonandri <jonandri@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/03 19:44:54 by jonandri          #+#    #+#             */
/*   Updated: 2026/05/05 10:26:58 by jonandri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cube.h"

int	read_file(char *filename, t_map *map)
{
	int		fd;
	char	*line;
	
	fd = open(filename, O_RDONLY);
	if (fd < 0)
		return (0);
	init_map(map);
	line = get_next_line(fd);
	while (line != NULL)
	{
		if (parse_line(line, map) == error)
		{
			free(line);
			close(fd);
			printf("Error/n");
			return (0);
		}
		free(line);
		line = get_next_line(fd);
	}
	return (0);
}
