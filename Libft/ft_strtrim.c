/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtrim.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jonandri <jonandri@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/11 09:32:05 by jonandri          #+#    #+#             */
/*   Updated: 2025/03/18 13:22:26 by jonandri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static	size_t	ft_found_start(const char *str1, const char *str2, size_t count)
{
	size_t	i;
	size_t	j;
	size_t	found;

	i = 0;
	count = 0;
	while (str1[i] != '\0')
	{
		j = 0;
		found = 0;
		while (str2[j] != '\0')
		{
			if (str1[i] == str2[j])
			{
				found = 1;
				count++;
				break ;
			}
			j++;
		}
		if (!found)
			break ;
		i++;
	}
	return (count);
}

static	size_t	ft_found_end(const char *str1, const char *str2, size_t count)
{
	size_t	j;
	size_t	found;
	size_t	l;

	count = 0;
	l = ft_strlen(str1) - 1;
	while (l > 0)
	{
		j = 0;
		found = 0;
		while (str2[j] != '\0')
		{
			if (str1[l] == str2[j])
			{
				found = 1;
				count++;
				break ;
			}
			j++;
		}
		if (!found)
			break ;
		l--;
	}
	return (count);
}

char	*ft_strtrim(char const *s1, char const *set)
{
	size_t	j;
	size_t	i;
	size_t	l;
	size_t	count;
	char	*dest;

	count = 0;
	i = ft_found_start(s1, set, count);
	if ((i == ft_strlen(s1)) || (s1[0] == '\0'))
		return (ft_strdup(""));
	j = ft_found_end(s1, set, count);
	l = ft_strlen(s1) - i - j + 1;
	dest = malloc(l * sizeof(char));
	if (dest == NULL)
		return (NULL);
	while (count < l - 1)
	{
		dest[count] = s1[i];
		i++;
		count++;
	}
	dest[count] = '\0';
	return (dest);
}
