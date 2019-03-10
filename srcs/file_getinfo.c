/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   file_getinfo.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emuckens <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/26 11:58:20 by emuckens          #+#    #+#             */
/*   Updated: 2018/12/02 16:14:18 by emuckens         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ls.h"
#include <sys/stat.h>
#include <sys/types.h>
#include <errno.h>
#include <unistd.h>

/*
** Call lstat on file
** Handle special error cases
** Return success or failure
*/

int			get_stat(ENV *e, struct stat *buf, char *path, int show_err)
{
	int		fd;

	fd = STDERR;
	if (!path[0])
		return (SUCCESS);
	if (lstat(path, buf) == -1)
	{
		if (show_err && (e->error = 1))
			ft_printf("{FD!}%s: %s: %s\n{EOO}", &fd, NAME, path,
					strerror(errno));
		e->exit = FAIL;
		return (FAIL);
	}
	return (SUCCESS);
}

/*
** Retrieve detailed file info from the system and store in structure, only if
** long option activated
** Input: file (for modification of info), filestat for retrieving info,
** path of file (from program location)
*/

static void	get_detailed_info(ENV *e, t_file **f, struct stat fstat, char *path)
{
	((*f)->disp = e->options & OPT_LONG ? LONG : COLHOR);
	if ((*f)->disp == COLHOR)
	{
		if (!(e->options & OPT_COLUMNS))
			(*f)->disp = SHORT;
		else if (e->options & OPT_COLDIR)
			(*f)->disp = COLVER;
		return ;
	}
	else if ((*f)->type == CHAR || (*f)->type == BLOCK)
		(*f)->disp = BL_CH;
	(*f)->links = fstat.st_nlink;
	ft_strcpy((*f)->link, " -> ");
	readlink(path, (*f)->link + 4, PATH_MAX - 4);
	(*f)->size = fstat.st_size;
	get_id(e, &fstat, (*f)->uid, (*f)->gid);
	ft_itoa_mod((*f)->major, major(fstat.st_rdev));
	ft_itoa_mod((*f)->minor, minor(fstat.st_rdev));
}

/*
** Retrieve file info from the system and store in structure
** Input: name of file, path of file (from program location)
** Return file, to store in file tree
*/

t_file		*get_fileinfo(ENV *e, char *name, char *path, int err)
{
	t_file			*file;
	struct stat		filestat;

	file = (t_file *)ft_memalloc(sizeof(t_file));
	if (!file && !(e->argerr = 0))
		return (NULL);
	ft_strcpy(file->name, name);
	path = path[0] && name ? ft_strcjoin(path, name, '/') : ft_strdup(name);
	file->blocksize = 0;
	if (get_stat(e, &filestat, path, err) == FAIL && (file->type = errno) >= 0)
	{
		ft_strdel(&path);
		return (file);
	}
	e->argerr = 2;
	get_type(filestat.st_mode, &file->type);
	get_mode(filestat.st_mode, file->mode, path);
	get_time(e, filestat.st_mtime, &file, filestat);
	get_optional_details(&file, filestat);
	e->total += file->blocksize * e->blocksize_coef;
	get_detailed_info(e, &file, filestat, path);
	get_padding_info(e, file);
	ft_strdel(&path);
	return (file);
}

/*
** Alternate file->ntime values
** Linux: filestat.st_mtim.tv_nsec
** Mac	: filestat.st_mtimespec.tv_nsec
*/
