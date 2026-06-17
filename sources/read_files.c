/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   read_files.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jonandri <jonandri@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/03 19:44:54 by jonandri          #+#    #+#             */
/*   Updated: 2026/06/17 09:01:15 by jonandri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cube.h"

int	read_file(char *filename, t_map *map)
{
	int		fd;
	char	*line;
	
	init_map(map);
	fd = open(filename, O_RDONLY);
	if (fd < 0)
	{
		printf("Error\nFailed to open file.\n");
		return (0);
	}
	line = get_next_line(fd);
	while (line != NULL)
	{
		if (parse_line(line, map) == 1)
		{
			free(line);
			close(fd);
			return (0);
		}
		free(line);
		line = get_next_line(fd);
	}
	map->true_grid = to_grid(map);
	if (verify_error(map->true_grid, map) == 1)
	{
		close(fd);
		return (printf("Error\nError parsing dans le map.\n"), 0);
	}
	if (completing_flood(map) == 1)
	{
		close(fd);
		return (printf("Error\nError parsing dans le map.\n"), 0);	
	}
	close(fd);
	return (1);
}
