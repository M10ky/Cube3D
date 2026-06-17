/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jonandri <jonandri@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/13 10:54:02 by jonandri          #+#    #+#             */
/*   Updated: 2025/03/25 05:47:08 by jonandri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static size_t	ft_count_str(char const *src, char c)
{
	size_t	begin;
	size_t	str;

	begin = 0;
	str = 0;
	while (src[begin] != '\0')
	{
		if (src[begin] == c)
			begin++;
		else
		{
			while ((src[begin]) != c && (src[begin] != '\0'))
			{
				begin++;
			}
			str++;
		}
	}
	return (str);
}

static size_t	found_count(char const *str, char c)
{
	size_t	count;

	count = 0;
	while ((*(str) != c) && (*(str) != '\0'))
	{
		count++;
		str++;
	}
	return (count);
}

static	void	ft_free(char **str, size_t g)
{
	size_t	j;

	j = 0;
	while (j < g)
	{
		free(str[j]);
		j++;
	}
	free(str);
}

char	**ft_split(char const *s, char c)
{
	size_t	size;
	size_t	g;
	size_t	d;
	char	**dest;

	dest = malloc(sizeof(char *) * (ft_count_str(s, c) + 1));
	if (!s || !dest)
		return (NULL);
	size = ft_count_str(s, c);
	dest[size] = NULL;
	g = 0;
	while (*s && g < size)
	{
		while (*s == c)
			s++;
		d = 0;
		dest[g] = malloc(sizeof(char) * (found_count(s, c) + 1));
		if (!dest[g])
			return (ft_free(dest, g), NULL);
		while (*s && *s != c)
			dest[g][d++] = *s++;
		dest[g++][d] = '\0';
	}
	return (dest);
}
