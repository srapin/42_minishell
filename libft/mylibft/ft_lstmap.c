/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstmap.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: srapin <srapin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/11 04:24:34 by srapin            #+#    #+#             */
/*   Updated: 2022/11/12 03:41:25 by srapin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

t_list	*ft_lstmap(t_list *lst, void *(*f)(void *), void (*del)(void *))
{
	t_list	*start;
	t_list	*last;

	if (!lst)
		return (NULL);
	start = ft_lstnew(f(lst->content));
	if (!start)
		return (NULL);
	lst = lst->next;
	last = start;
	while (lst)
	{
		last->next = ft_lstnew(f(lst->content));
		if (!last->next)
		{
			ft_lstclear(&start, del);
			return (NULL);
		}
		last = last->next;
		lst = lst->next;
	}
	return (start);
}
