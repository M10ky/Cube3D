/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jonandri <jonandri@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/03 19:59:05 by jonandri          #+#    #+#             */
/*   Updated: 2026/05/03 20:05:31 by jonandri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cube.h"

static char	*ft_reverse_and_return(char **tab, int *j)
{
	char	*tab2;
	char	*tab3;

	tab2 = ft_substr_get(*tab, *j, ft_strlen_get((*tab) + (*j)));
	(*tab)[(*j)] = '\0';
	tab3 = ft_strdup_get(*tab);
	free(*tab);
	*tab = NULL;
	*tab = ft_strdup_get(tab2);
	free(tab2);
	tab2 = NULL;
	return (tab3);
}

static int	ft_result(char **tab, int j)
{
	j = 0;
	while ((*tab)[j] != '\n' && (*tab)[j] != '\0')
		j++;
	if ((*tab)[j] == '\n')
		j++;
	return (j);
}

static char	*ft_tab(char **tab, char **temp, char **buff)
{
	if (*tab == NULL)
		*tab = ft_strdup_get("");
	*temp = ft_strdup_get(*tab);
	free(*tab);
	*tab = NULL;
	*tab = ft_strjoin_get(*temp, *buff);
	free(*temp);
	*temp = NULL;
	return (*tab);
}

static void	*check_tab(char *tab)
{
	if (tab == NULL)
		return (NULL);
	if (tab[0] == '\0')
	{
		free(tab);
		return (NULL);
	}
	return (tab);
}

char	*get_next_line(int fd)
{
	static char	*tab;
	char		*temp;
	char		*buff;
	int			j;

	if (fd < 0 || BUFFER_SIZE <= 0)
		return (NULL);
	buff = malloc(sizeof(char) * (BUFFER_SIZE + 1));
	if (buff == NULL)
		return (NULL);
	j = read(fd, buff, BUFFER_SIZE);
	if (j < 0)
		return (free(buff), free(tab), tab = NULL, NULL);
	while (j > 0)
	{
		buff[j] = '\0';
		if (ft_strchr_get((ft_tab(&tab, &temp, &buff)), '\n') != NULL)
			break ;
		j = read(fd, buff, BUFFER_SIZE);
	}
	free(buff);
	tab = check_tab(tab);
	if (!tab)
		return (NULL);
	return (j = ft_result(&tab, j), ft_reverse_and_return(&tab, &j));
}
