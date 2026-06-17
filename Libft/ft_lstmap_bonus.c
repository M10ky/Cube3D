/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstmap_bonus.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jonandri <jonandri@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/24 10:14:38 by jonandri          #+#    #+#             */
/*   Updated: 2025/03/25 01:30:42 by jonandri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

t_list	*ft_lstmap(t_list *lst, void *(*f)(void *), void (*del)(void *))
{
	t_list	*new_el;
	t_list	*head;

	head = NULL;
	if (lst == NULL)
		return (NULL);
	while (lst)
	{
		new_el = ft_lstnew((*f)(lst -> content));
		if (new_el == NULL)
		{
			ft_lstclear(&head, del);
			return (NULL);
		}
		ft_lstadd_back(&head, new_el);
		lst = lst -> next;
	}
	return (head);
}
