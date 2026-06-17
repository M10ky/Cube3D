/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strrchr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jonandri <jonandri@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/05 13:34:18 by jonandri          #+#    #+#             */
/*   Updated: 2025/03/18 08:57:51 by jonandri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strrchr(const char *s, int c)
{
	int		i;
	char	*pt_s;

	i = ft_strlen(s);
	pt_s = (char *)s;
	while (i >= 0)
	{
		if (pt_s[i] == (char)c)
		{
			return (&pt_s[i++]);
		}
		i--;
	}
	return (0);
}
