/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strnstr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jonandri <jonandri@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/07 11:27:43 by jonandri          #+#    #+#             */
/*   Updated: 2025/03/25 00:27:44 by jonandri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strnstr(const char *big, const char *little, size_t len)
{
	size_t	i;
	size_t	j;
	char	*pt_s1;
	char	*pt_s2;

	i = 0;
	pt_s1 = (char *)big;
	pt_s2 = (char *)little;
	if (pt_s2[i] == '\0')
		return (pt_s1);
	while ((pt_s1[i] != '\0') && (i < len))
	{
		j = 0;
		while (pt_s1[i + j] == pt_s2[j] && (i + j < len))
		{
			if (pt_s2[j + 1] == '\0')
				return (&pt_s1[i]);
			j++;
		}
		i++;
	}
	return (NULL);
}
