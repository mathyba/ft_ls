/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dir.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emuckens <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/26 12:01:42 by emuckens          #+#    #+#             */
/*   Updated: 2018/12/02 17:41:56 by emuckens         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ls.h"
#include <errno.h>
#include <unistd.h>

/*
** Open directory, adjust path in case of error display
** Input: dir, full path (from program location)
** return success or failure
*/

int			open_dir(DIR **dir, char **path)
{
	char			*name;
	int				len;
	int				fd;

	fd = STDERR;
	if ((*dir = opendir(*path)))
		return (SUCCESS);
	name = *path;
	while ((len = ft_strcstr(*path, "/")) != -1)
		*path += len + 1;
	ft_printf("{FD!}%s: %s: %s\n{EOO}", &fd, NAME, *path, strerror(errno));
	return (FAIL);
}

/*
** force read (in case of hidden files ignored by readdir call, in /dev/fd)
*/

static int	force_read_hidden(ENV *e, t_lbtree **tree, char *name, char *path)
{
	t_file *file;

	if (!(file = get_fileinfo(e, name, path, 1)))
		return (FAIL);
	ft_treeadd_cust(e, tree, file, e->sort);
	get_padding_info(e, file);
	return (SUCCESS);
}

/*
** Go through directory files, get info and add to tree
** Handle special case of . and .. for /dev/fd, identified with inode number
** return success or failure
*/

int			sort_dir(ENV *e, DIR *dir, char *path, t_lbtree **tree)
{
	struct dirent	*entry;
	t_file			*file;
	struct stat		dirstat;

	file = NULL;
	e->col.nb_files = 0;
	if (e->options & OPT_HIDDEN && get_stat(e, &dirstat, path, 1) == SUCCESS)
		if (dirstat.st_ino == e->fdinode)
		{
			e->col.nb_files += 2;
			force_read_hidden(e, tree, ".", path);
			force_read_hidden(e, tree, "..", path);
		}
	while ((entry = readdir(dir)))
	{
		if (entry->d_name[0] != '.' || e->options & OPT_HIDDEN)
		{
			if (!(file = get_fileinfo(e, entry->d_name, path, 1)))
				return (FAIL);
			++e->col.nb_files;
			get_padding_info(e, file);
			ft_treeadd_cust(e, &(*tree), file, e->sort);
		}
	}
	return (SUCCESS);
}

/*
** Checks if file is directory or link pointing to a directory
** Input: file real path
** Returns true if so, false otherwise
*/

int			is_asdir(ENV *e, char *path)
{
	struct stat		filestat;
	struct stat		linkstat;
	char			buf[1024];
	int				type;

	type = -1;
	ft_bzero(buf, 1024);
	if (get_stat(e, &filestat, path[0] ? path : ".", 1) == FAIL)
		return (FALSE);
	if (e->options & OPT_DIRPLAIN)
		return (FALSE);
	if ((filestat.st_mode & S_IFMT) == S_IFLNK)
	{
		readlink(path, buf, 1024);
		stat(path, &linkstat);
		get_type(linkstat.st_mode, &type);
	}
	if ((filestat.st_mode & S_IFMT) == S_IFDIR
			|| ((linkstat.st_mode & S_IFMT) == S_IFDIR
				&& (!(e->options & OPT_LONG
						|| ((e->options & OPT_LONG)
							&& path[ft_strlen(path) - 1] != '/')))))
		return (TRUE);
	return (FALSE);
}

/*
** Recursively scan whole tree for directories, adjust path, upon reaching
** directory, display path and call function to display files
** Input: filetree, full path (from program location)
*/

void		scan_dir(ENV *e, t_lbtree *tree, char *path)
{
	char	*newpath;

	if (!tree)
		return ;
	scan_dir(e, tree->left, path);
	if (is_dir(e, tree) && is_child(tree))
	{
		if (!ft_strequ(path, "/"))
			newpath = ft_strcjoin(path, ((t_file *)tree->content)->name, '/');
		else
			newpath = ft_strjoin(path, ((t_file *)tree->content)->name);
		ft_printf("%c", '\n');
		list_files(e, newpath, NULL, STDERR);
		ft_strdel(&newpath);
	}
	scan_dir(e, tree->right, path);
}
