/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   list.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emuckens <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/21 21:00:01 by emuckens          #+#    #+#             */
/*   Updated: 2018/12/02 17:43:05 by emuckens         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ls.h"
#include "color.h"
#include <errno.h>
#include <unistd.h>
#include <stdlib.h>

/*
** Display directory
** Only if multiple directories, or in case of recursive scan: if not root
** directory, or if files in arguments, or if errors in output
*/

static void		display_directory(ENV *e, int iter, char *path)
{
	if ((e->nl & (1 << MULTIARG))
		|| (e->options & OPT_RECUR
			&& (iter || e->nl & (1 << FIRST) || e->error)))
		ft_printf("%s:\n", path);
}

/*
** Display total blocksize, if recursive scan or option -s (blocksize) activated
*/

static void		display_total(ENV *e, t_lbtree *tree)
{
	if ((e->options & OPT_LONG || e->options & OPT_BLOCKSIZE) && tree)
	{
		if (e->total / (int)e->total > 1)
			++e->total;
		ft_printf("total %d\n", (int)e->total);
	}
}

/*
** List files in a given directory:
** Open directory, print total blocks, display file tree, reset padding, go one
** level deeper if -R option activated, destroy tree
** Input: full path (from program location), tree for initialization to NULL,
** fd for initiliazation to STDERR
** Return success or failure
*/

int				list_files(ENV *e, char *path, t_lbtree *tree, int fd)
{
	DIR				*dir;
	char			tmpbuf[1024];
	static int		iter;

	ft_bzero(tmpbuf, 1024);
	display_directory(e, iter, path);
	++iter;
	e->total = 0;
	if (open_dir(&dir, &path) == FAIL || sort_dir(e, dir, path, &tree) == FAIL)
		return (FAIL);
	set_coldisplay_dim(e);
	display_total(e, tree);
	if (display_all(e, tree) == FAIL)
	{
		free_files(e);
		ft_destroy_tree(tree);
		return (FAIL);
	}
	if (dir && closedir(dir) == FAIL)
		ft_printf("{FD!}%s: %s: %s\n{EOO}", &fd, NAME, path, strerror(errno));
	reset_padding(e);
	if (e->options & OPT_RECUR)
		scan_dir(e, tree, path);
	ft_destroy_tree(tree);
	return (SUCCESS);
}

/*
** Scans who file tree in order (left, node, right): if file is directory or
** link pointing to a directory >> open and list files, else display file
** Input: file tree
*/

void			list_all(ENV *e, t_lbtree *t, t_file *file)
{
	char		*path;
	static int	iter;

	if (!t && !(e->nl & (1 << NOARG)))
		return ;
	path = ft_strdup(t ? ((t_file *)t->content)->name : "");
	if (t && t->left)
		list_all(e, t->left, NULL);
	if (is_asdir(e, path))
	{
		e->exit = FAIL;
		if ((iter++ || e->nl & (1 << FIRST)))
			ft_printf("\n");
		if (list_files(e, path[0] ? path : ".", NULL, STDERR) == FAIL)
			e->exit = FAIL;
	}
	else if ((e->nl |= (1 << FIRST)))
	{
		file = get_fileinfo(e, path[0] ? path : ".", "", 1);
		display_file(e, file);
		ft_memdel((void **)&file);
	}
	ft_strdel(&path);
	if (t && t->right)
		list_all(e, t->right, NULL);
}
