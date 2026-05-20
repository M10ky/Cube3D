/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jonandri <jonandri@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/05 12:46:30 by jonandri          #+#    #+#             */
/*   Updated: 2025/03/19 13:43:34 by jonandri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strchr(const char *s, int c)
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
