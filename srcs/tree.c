/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tree.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emuckens <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/26 13:37:50 by emuckens          #+#    #+#             */
/*   Updated: 2018/12/02 17:55:43 by emuckens         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ls.h"

void	ft_treeadd_cust(ENV *e, t_lbtree **tree, t_file *content, int sort)
{
	int cond;

	if (!content)
		return ;
	if (!(*tree))
	{
		*tree = (t_lbtree *)ft_memalloc(sizeof(t_lbtree));
		(*tree)->content = (t_file *)ft_memalloc(sizeof(t_file));
		ft_memmove((*tree)->content, content, sizeof(t_file));
		ft_memdel((void **)&content);
		(*tree)->left = NULL;
		(*tree)->right = NULL;
		return ;
	}
	if ((cond = e->fsort[sort](e, content, (*tree)->content)))
		ft_treeadd_cust(e, &(*tree)->left, content, sort);
	else
		ft_treeadd_cust(e, &(*tree)->right, content, sort);
}
