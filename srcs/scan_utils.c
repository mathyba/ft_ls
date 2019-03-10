/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   scan_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emuckens <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/12/02 13:50:14 by emuckens          #+#    #+#             */
/*   Updated: 2018/12/02 14:08:45 by emuckens         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ls.h"

/*
** Checks if file is non hidden directory
** return true if so, false otherwise
*/

int		is_dir(ENV *e, t_lbtree *tree)
{
	if (((t_file *)tree->content)->type == DIRECTORY
			&& (((t_file *)tree->content)->name[0] != '.'
				|| e->options & OPT_HIDDEN))
		return (TRUE);
	return (FALSE);
}

/*
** Checks if file contained in node is current or parent directory
** Returns false if so, true otherwise
*/

int		is_child(t_lbtree *node)
{
	return (!ft_strequ(((t_file *)node->content)->name, ".")
			&& !ft_strequ(((t_file *)node->content)->name, ".."));
}
