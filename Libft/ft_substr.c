/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_substr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jonandri <jonandri@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/11 08:20:57 by jonandri          #+#    #+#             */
/*   Updated: 2025/03/24 22:37:35 by jonandri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_substr(char const *s, unsigned int start, size_t len)
{
	char	*src;
	char	*dest;
	size_t	i;
	size_t	j;

	j = (size_t)start;
	src = (char *)s;
	if (j > ft_strlen(s))
		return (ft_strdup(""));
	if (len > ft_strlen(src) - j)
		len = (ft_strlen(src) - j);
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
