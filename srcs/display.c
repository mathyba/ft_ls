/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   display.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emuckens <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/26 11:26:18 by emuckens          #+#    #+#             */
/*   Updated: 2018/12/02 17:49:17 by emuckens         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ls.h"

/*
** Display file name with color
*/

static void		disp_reg(ENV *e, t_file *file, char *fore, char *back)
{
	(void)e;
	ft_printf("%s%s%s%s%s\n",
			fore, back, file->name, e->clr.foff, e->clr.boff);
}

/*
** Display file long format for char and block types (size replaced by major
** and minor info
*/

static void		disp_long_blch(ENV *e, t_file *file, char *fore, char *back)
{
	static char		code[NB_TYPES] = "dlspbc-";

	ft_printf("%c%s %*d %-*s  %-*s   %*s%c %*s % s %s%s%s%s%s%s\n",
			code[file->type], file->mode,
			e->max.llen + (file->mode[NB_MODES] ? 0 : 1), file->links,
			e->max.ulen, file->uid, e->max.glen, file->gid,
			e->max.majlen, file->major, ',', e->max.minlen, file->minor,
			file->timestr, fore, back, file->name, e->clr.foff, e->clr.boff,
			file->link[4] ? file->link : "");
}

/*
** Display file long format for all types except block and char
*/

static void		disp_long(ENV *e, t_file *file, char *fore, char *back)
{
	static char		code[NB_TYPES] = "dlspbc-";
	char			size[20];

	ft_itoa_mod(size, file->size);
	ft_printf("%c%s %*d %-*s  %-*s%*s %*s %s %s%s%s%s%s%s\n",
			code[file->type], file->mode,
			e->max.llen + (file->mode[NB_MODES] ? 0 : 1), file->links,
			e->max.ulen, file->uid, e->max.glen, file->gid,
			0, "", e->max.slen, size,
			file->timestr, fore, back, file->name, e->clr.foff, e->clr.boff,
			file->link[4] ? file->link : "");
}

/*
** Select color, display optional information, display file according to type
** Input: file
** Return: failure if cycle error, success otherwise
*/

int				display_file(ENV *e, t_file *file)
{
	static void	(*disp[5])(ENV *e, t_file *f, char *fg, char *bg) = {&disp_reg,
		&store_col_hor, &store_col_ver, &disp_long, &disp_long_blch};

	if (!file)
		return (SUCCESS);
	set_colors(e, file, e->clr.fg, e->clr.bg);
	if (!(e->options & OPT_COLUMNS))
	{
		if (e->options & OPT_INODE)
			ft_printf("%*d ", e->max.ilen, file->inode);
		if (e->options & OPT_BLOCKSIZE)
			ft_printf("%*d ", e->max.blen, file->blocksize);
	}
	disp[(int)file->disp](e, file, e->clr.fg, e->clr.bg);
	return (SUCCESS);
}

/*
** Display whole tree content in order
** Input: tree
** Return: Failure if at least one file display failure, success otherwise
*/

int				display_all(ENV *e, t_lbtree *tree)
{
	static int		nb_files;
	static int		iter;

	if (!tree)
		return (SUCCESS);
	if (!iter && e->options & OPT_COLUMNS && allocate_files(e) == FAIL)
		return (FAIL);
	iter++;
	if (display_all(e, tree->left) == FAIL)
		return (FAIL);
	if (display_file(e, tree->content) == FAIL)
		return (FAIL);
	++nb_files;
	if (e->options & OPT_COLUMNS && nb_files == e->col.nb_files)
	{
		e->col.col = 0;
		e->col.row = 0;
		display_columns(e);
		nb_files = 0;
		iter = 0;
	}
	if (display_all(e, tree->right) == FAIL)
		return (FAIL);
	return (SUCCESS);
}
