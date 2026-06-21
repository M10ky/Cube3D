/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   read_files.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jonandri <jonandri@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/03 19:44:54 by jonandri          #+#    #+#             */
/*   Updated: 2026/06/21 19:35:42 by jonandri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cube.h"

static void	print_error_map(void)
{
	printf("Error\nMap invalid\n");
}

static void	print_error_file_cub(void)
{
	printf("Error\nInvalid file.cub name\n");
}

static void	print_error_open(void)
{
	printf("Error\nFailed to open file .cub\n");
}

int	read_file(char *filename, t_map *map)
{
	int		fd;
	char	*line;

	init_map(map);
	if (is_cub(filename) == 1)
		return (print_error_file_cub(), 0);
	fd = open(filename, O_RDONLY);
	if (fd < 0)
		return (print_error_open(), 0);
	line = get_next_line(fd);
	while (line != NULL)
	{
		if (parse_line(line, map) == 1)
			return (free(line), close(fd), 0);
		free(line);
		line = get_next_line(fd);
	}
	map->true_grid = to_grid(map);
	if (verify_error(map->true_grid, map) == 1)
		return (close(fd), print_error_map(), 0);
	if (completing_flood(map) == 1)
		return (close(fd), print_error_map(), 0);
	close(fd);
	return (1);
}
