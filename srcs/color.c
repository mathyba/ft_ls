/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   color.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emuckens <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/22 13:25:13 by emuckens          #+#    #+#             */
/*   Updated: 2018/12/02 16:06:28 by emuckens         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ls.h"

/*
** Reset colors to default
*/

void			reset_colors(ENV *e, int iter)
{
	if (e->options & OPT_COLOR && !iter)
	{
		ft_strcpy(e->clr.foff, COLF_OFF);
		ft_strcpy(e->clr.boff, COLB_OFF);
	}
}

/*
** Set color reference string
** Retrieve string in environment or set default as in man ls
*/

void			set_color_ref(ENV *e)
{
	int	index;

	if ((index = get_envvar_index(e, LSCOLORS)) >= 0)
		ft_strcpy(e->clr.ref, e->env_var[index][1]);
	else
		ft_strcpy(e->clr.ref, COL_DEFAULT);
}

/*
** Determine which color to apply to current file
** Input: file
** Return : code for appropriate file type (cf enum in .h)
*/

static int		set_colorindex(ENV *e, t_file *file)
{
	int		index;

	index = -1;
	if (!(e->options & OPT_COLOR))
		return (-1);
	if (file->type <= FIFO)
		index = file->type;
	index = (file->type + (file->type == CHAR || file->type == BLOCK ? 1 : 0));
	if (file->exe && (file->mode[2] == 's' || file->mode[2] == 'S'))
		index = 7;
	else if (file->exe && (file->mode[5] == 's' || file->mode[5] == 'S'))
		index = 8;
	else if (file->exe && file->type == REG)
		index = 4;
	else if ((file->mode[8] == 't' || file->mode[8] == 'T')
			&& file->mode[7] == 'w' && file->type == DIRECTORY)
		index = 9;
	else if (file->mode[7] == 'w' && file->type == DIRECTORY)
		index = 10;
	else if (file->type > CHAR)
		index = -1;
	return (index);
}

/*
** Set colors corresponding to current file type and properties
** Input: file type code, string to store foreground color, string to store
** background color
** Filetype / color correspondance determined by string of type
** "exfxcxdxbxegedabagacad" as defined in ls man, where each pair of char
** define foregroundd and background color for a typecode
*/

void			set_colors(ENV *e, t_file *file, char *cfront, char *cback)
{
	static char *color_front[NB_DESIGNATOR] = {COLF_BLACK, COLF_RED, COLF_GREEN,
		COLF_BROWN, COLF_BLUE, COLF_MAGENTA, COLF_CYAN, COLF_GREY, COLF_BBLACK,
		COLF_BRED, COLF_BGREEN, COLF_BBROWN, COLF_BBLUE, COLF_BMAGENTA,
		COLF_BCYAN, COLF_BGREY, COLF_OFF};
	static char *color_back[NB_DESIGNATOR] = {COLB_BLACK, COLB_RED, COLB_GREEN,
		COLB_BROWN, COLB_BBLUE, COLB_MAGENTA, COLB_CYAN, COLB_GREY, COLB_BBLACK,
		COLB_BRED, COLB_BGREEN, COLB_BBROWN, COLB_BBLUE, COLB_BMAGENTA,
		COLB_BCYAN, COLB_BGREY, COLB_OFF};
	int			i;
	int			coltype;

	if (e->options & OPT_COLOR)
		if ((coltype = set_colorindex(e, file)) >= 0)
		{
			i = *(e->clr.ref + coltype * 2);
			ft_strcpy((cfront), color_front[ft_strchri(COL_DESIGNATOR, i, 0)]);
			i = *(e->clr.ref + coltype * 2 + 1);
			ft_strcpy((cback), color_back[ft_strchri(COL_DESIGNATOR, i, 0)]);
			return ;
		}
	ft_bzero(cfront, 10);
	ft_bzero(cback, 7);
}
