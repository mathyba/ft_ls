/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_destroy_tree.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emuckens <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/26 13:28:55 by emuckens          #+#    #+#             */
/*   Updated: 2018/11/26 13:37:23 by emuckens         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <stdlib.h>

void	ft_destroy_tree(t_lbtree *tree)
{
	if (!tree)
		return ;
	if (tree->content)
	{
		ft_destroy_tree(tree->left);
		ft_destroy_tree(tree->right);
	}
	free(tree->content);
	tree->content = NULL;
	ft_memdel((void **)&tree);
}
