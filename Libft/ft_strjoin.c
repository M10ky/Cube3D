/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strjoin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jonandri <jonandri@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/11 08:54:49 by jonandri          #+#    #+#             */
/*   Updated: 2025/03/19 08:19:33 by jonandri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static char	*ft_strcpy(char *dest, char *src)
{
	int	i;

	i = 0;
	while (src[i] != '\0')
	{
		dest[i] = src[i];
		i++;
	}
	dest[i] = '\0';
	return (dest);
}

static char	*ft_strcat(char *dest, char *src)
{
	int	i;
	int	l;

	l = ft_strlen(dest);
	i = 0;
	while (src[i] != '\0')
	{
		dest[l + i] = src[i];
		i++;
	}
	dest[l + i] = '\0';
	return (dest);
}

char	*ft_strjoin(char const *s1, char const *s2)
{
	char	*dest;
	char	*src1;
	char	*src2;
	size_t	lenght;

	src1 = (char *)s1;
	src2 = (char *)s2;
	lenght = (ft_strlen(s1) + (ft_strlen(s2)));
	dest = malloc(lenght + 1 * sizeof(char));
	if (dest == NULL)
		return (NULL);
	ft_strcpy(dest, src1);
	ft_strcat(dest, src2);
	return (dest);
}
