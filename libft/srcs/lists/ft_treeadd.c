/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_treeadd.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emuckens <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/26 13:23:48 by emuckens          #+#    #+#             */
/*   Updated: 2018/11/26 13:34:55 by emuckens         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_treeadd(t_lbtree **tree, void *content,
		int size, int (*f)(void *c1, void *c2))
{
	if (!content)
		return ;
	if (!(*tree))
	{
		*tree = (t_lbtree *)ft_memalloc(sizeof(t_lbtree));
		(*tree)->content = ft_memalloc(size + 1);
		memmove((*tree)->content, content, size);
		ft_memdel((void **)&content);
		(*tree)->left = NULL;
		(*tree)->right = NULL;
		return ;
	}
	else if (f(content, (*tree)->content))
		ft_treeadd(&(*tree)->left, content, size, f);
	else
		ft_treeadd(&(*tree)->right, content, size, f);
}
