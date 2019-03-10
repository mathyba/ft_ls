/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_treedo_order.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emuckens <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/26 13:30:11 by emuckens          #+#    #+#             */
/*   Updated: 2018/12/02 15:46:16 by emuckens         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void		ft_treedo_order(t_lbtree *tree, void (*f)(t_lbtree *node))
{
	if (!tree)
		return ;
	if (tree->left)
		ft_treedo_order(tree->left, f);
	f(tree);
	if (tree->right)
		ft_treedo_order(tree->right, f);
}
