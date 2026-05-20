/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cube.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jonandri <jonandri@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/16 11:13:57 by jonandri          #+#    #+#             */
/*   Updated: 2026/05/06 20:47:58 by jonandri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CUBE_H
# define CUBE_H
# define BUFFER_SIZE 1
# include "libft.h"
# include <mlx.h>
# include <stdio.h>
# include <unistd.h>
# include <fcntl.h>
# include <stdlib.h>
# include <stddef.h>

typedef struct  s_data
{
	void	*mlx;
	void	*win;
	void	*img;
	char	*addr;
	int		bits_per_pixel;
	int		line_lenght;
	int		endian;
}				t_data;

typedef struct s_line
{
	char			*content;
	struct	s_line	*next;
}				t_line;

typedef struct s_map
{
	char	*no_path;
	char	*so_path;
	char	*we_path;
	char	*ea_path;

	int		floor_c;
	int		ceiling_c;

	t_line	*first_line;
	t_line	*last_line;
	int		height;
	char	**grid;
	int 	map_started;
	int		width;
	int		height;

	int		pos_x;
	int		pos_y;
	char	dir;

	int		has_no;
	int		has_so;
	int		has_we;
	int		has_ea;
	int		has_floor;
	int		has_ceiling;
}				t_map;

typedef enum s_bool
{
	error,	
}			t_bool;

void	my_mlx_pixel_put(t_data *data, int x, int y, int color);
void	init_map(t_map *map);
int		read_file(char *filename, t_map *map);
size_t	ft_strlen_get(const char *s);
char	*ft_strdup_get(const char *s);
char	*ft_strchr_get(const char *s, int c);
char	*ft_strjoin_get(char const *s1, char const *s2);
char	*ft_substr_get(char const *s, unsigned int start, size_t len);
char	*get_next_line(int fd);
int		parse_line(char *line, t_map *map);
int		parse_texture(char *line, char **path, int *has);
int		parse_color(char *line, int *color, int *has);
int		parse_rgb(char *line, int *i, int *r, int *g, int *b);
int		get_number(char *line, int *i);
int		start_map_parsing(char *line, t_map *map);
int		add_line_to_map(char *line, t_map *map);
void	copy_substring(char *dest, char *src, int start, int end);
int		get_map_width(t_map *map);

#endif