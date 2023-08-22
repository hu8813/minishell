/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstmap.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: huaydin <huaydin@student.42vienna.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/24 16:47:04 by hakaraka          #+#    #+#             */
/*   Updated: 2023/07/16 17:33:37 by huaydin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

t_list	*ft_lstmap(t_list *lst, void *(*f)(void *),
void (*del)(void *))
{
	t_list	*mylist;
	void	*tmp;

	if (!lst)
		return (NULL);
	mylist = NULL;
	while (lst)
	{
		tmp = f(lst->content);
		if (!tmp)
		{
			ft_lstclear(&mylist, del);
			return (NULL);
		}
		ft_lstadd_back(&mylist, ft_lstnew(tmp));
		lst = lst->next;
	}
	return (mylist);
}
