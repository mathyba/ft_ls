/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   column.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emuckens <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/26 20:46:11 by emuckens          #+#    #+#             */
/*   Updated: 2018/12/02 18:34:15 by emuckens         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ls.h"
#include <sys/ioctl.h>

/*
** Retrieve window width in environment if variable declard, or else from
** system
*/

void		get_window_w(ENV *e)
{
	int				index;
	struct winsize	w;

	if ((index = get_envvar_index(e, COLUMNS)) != -1)
		e->col.win_w = ft_atoi(e->env_var[index][1]);
	else
	{
		ioctl(0, TIOCGWINSZ, &w);
		e->col.win_w = w.ws_col;
	}
}

/*
** Compute number of columns and rows for column display
** Adjust padding size for optional inode and blocksize display
*/

void		set_coldisplay_dim(ENV *e)
{
	int		files_per_row;
	int		baselen;

	if (!(baselen = 0) && e->options & OPT_COLUMNS)
	{
		baselen += (e->options & OPT_BLOCKSIZE ? e->max.blen + MIN_BTW_COL : 0);
		baselen += (e->options & OPT_INODE ? e->max.ilen + MIN_BTW_COL : 0);
	}
	if (!e->max.nlen)
	{
		e->col.nbcol = 0;
		e->col.nbrow = 0;
		return ;
	}
	files_per_row = e->col.win_w / (e->max.nlen + baselen + MIN_BTW_COL);
	if (files_per_row - baselen)
		e->col.len = e->col.win_w / (files_per_row - baselen);
	else
		e->col.len = e->col.win_w;
	e->col.nbcol = files_per_row ? files_per_row : 1;
	e->col.nbrow = e->col.nb_files / (e->col.nbcol);
	if ((!e->col.nbrow && e->col.nbcol)
			|| (e->col.nbrow * e->col.nbcol < e->col.nb_files))
		++e->col.nbrow;
}

/*
** Display file name with color and column display column-wise
*/

void		store_col_ver(ENV *e, t_file *file, char *fore, char *back)
{
	char			*name;

	if (!e->col.nb_files)
		return ;
	if (e->col.row == e->col.nbrow && (++e->col.col))
		e->col.row = 0;
	name = (char *)e->col.files[e->col.col][e->col.row].name;
	ft_memcpy(name, fore, 10);
	ft_memcpy(name + 10, back, 7);
	ft_memcpy(name + 17, file->name, file->len.nlen);
	e->col.files[e->col.col][e->col.row].blocksize = file->blocksize;
	e->col.files[e->col.col][e->col.row].inode = file->inode;
	++e->col.row;
}

/*
** Display file name with color and column display row-wise
*/

void		store_col_hor(ENV *e, t_file *file, char *fore, char *back)
{
	char			*name;

	if (!e->col.nb_files)
		return ;
	if (e->col.col == e->col.nbcol && (++e->col.row))
		e->col.col = 0;
	name = (char *)e->col.files[e->col.col][e->col.row].name;
	ft_memcpy(name, fore, 10);
	ft_memcpy(name + 10, back, 7);
	ft_memcpy(name + 17, file->name, file->len.nlen);
	e->col.files[e->col.col][e->col.row].blocksize = file->blocksize;
	e->col.files[e->col.col][e->col.row].inode = file->inode;
	++e->col.col;
}

/*
** Retrieve info for all directory files from stored tab and display in columns
*/

void		display_columns(ENV *e)
{
	int		i;
	int		j;
	int		curfile;

	j = -1;
	curfile = 0;
	while (++j < e->col.nbrow)
	{
		i = -1;
		while (++i < e->col.nbcol)
		{
			++curfile;
			if (e->options & OPT_INODE && e->col.files[i][j].inode)
				ft_printf("%*d ", e->max.ilen, e->col.files[i][j].inode);
			if (e->options & OPT_BLOCKSIZE && e->col.files[i][j].inode)
				ft_printf("%*d ", e->max.blen, e->col.files[i][j].blocksize);
			ft_printf("%s%s%-s%s%*s%s", e->col.files[i][j].name,
					e->col.files[i][j].name + 10,
					e->col.files[i][j].name + 17, e->clr.boff,
					e->col.len - ft_strlen(e->col.files[i][j].name + 17),
					"", e->clr.foff);
		}
		ft_putchar('\n');
	}
	free_files(e);
}
