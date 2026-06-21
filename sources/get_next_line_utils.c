/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jonandri <jonandri@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/03 20:01:05 by jonandri          #+#    #+#             */
/*   Updated: 2026/06/17 18:11:30 by jonandri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cube.h"

size_t	ft_strlen_get(const char *s)
{
	size_t	lenght ;

	lenght = 0;
	while (s[lenght] != '\0')
	{
		lenght++;
	}
	return (lenght);
}

char	*ft_strdup_get(const char *s)
{
	char	*dest;
	char	*src;
	size_t	size;
	int		i;

	if (!s)
		return (NULL);
	src = (char *)s;
	size = ft_strlen_get(src) + 1;
	i = 0;
	dest = malloc(size * sizeof(char));
	if (dest == NULL)
	{
		free(dest);
		return (NULL);
	}
	while (src[i] != '\0')
	{
		dest[i] = src[i];
		i++;
	}
	dest[i] = '\0';
	return (dest);
}

char	*ft_strchr_get(const char *s, int c)
{
	int		i;
	char	*pt_s;

	pt_s = (char *)s;
	i = 0;
	while (pt_s[i] != '\0')
	{
		if (pt_s[i] == (char)c)
		{
			return (&pt_s[i++]);
		}
		i++;
	}
	if ((char) c == 0)
		return (&pt_s[i]);
	return (0);
}

char	*ft_strjoin_get(char const *s1, char const *s2)
{
	char	*dest;
	int		i;
	int		l;

	i = 0;
	l = 0;
	dest = malloc(((ft_strlen_get(s1) + (ft_strlen_get(s2))) + 1)
			* sizeof(char));
	if (dest == NULL)
		return (NULL);
	while (s1[i] != '\0')
	{
		dest[i] = s1[i];
		i++;
	}
	dest[i] = '\0';
	l = ft_strlen_get(dest);
	i = 0;
	while (s2[i] != '\0')
	{
		dest[l + i] = s2[i];
		i++;
	}
	dest[l + i] = '\0';
	return (dest);
}

char	*ft_substr_get(char const *s, unsigned int start, size_t len)
{
	char	*src;
	char	*dest;
	size_t	i;
	size_t	j;

	if (!s || start >= ft_strlen_get(s) || len > ft_strlen_get(s) - start)
		return (NULL);
	j = (size_t)start;
	src = (char *)s;
	i = 0;
	dest = malloc((len + 1) * sizeof(char));
	if (dest == NULL)
		return (0);
	while ((src[j] != '\0') && (i < len))
	{
		dest[i] = src[j];
		i++;
		j++;
	}
	dest[i] = '\0';
	return (dest);
}
