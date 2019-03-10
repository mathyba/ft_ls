/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   file_supportft.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emuckens <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/26 11:39:00 by emuckens          #+#    #+#             */
/*   Updated: 2018/12/02 13:34:20 by emuckens         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ls.h"
#include <time.h>
#include <pwd.h>
#include <grp.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/acl.h>
#include <sys/xattr.h>

/*
** Get detailed information needed for short format in columns
** (inode and blocksize)
*/

void		get_optional_details(t_file **f, struct stat filestat)
{
	(*f)->inode = filestat.st_ino;
	(*f)->exe = (filestat.st_mode & S_IEXEC ? 1 : 0);
	(*f)->blocksize = filestat.st_blocks;
}

/*
** Set mode string by examining activated bits in stat mode
** Input: stat mode, string to store result, ready for display
** Handle special case of sticky bits
*/

void		get_mode(mode_t m, char *str, char *path)
{
	static mode_t	mode[NB_MODES] = {S_IRUSR, S_IWUSR, S_IXUSR, S_IRGRP,
		S_IWGRP, S_IXGRP, S_IROTH, S_IWOTH, S_IXOTH};
	static char		code[3] = "rwx";
	int				i;
	acl_t			acl;

	i = -1;
	acl = NULL;
	while (++i < NB_MODES)
	{
		str[i] = m & mode[i] ? code[i % 3] : '-';
		if (m & S_ISUID && i == 2)
			str[i] == 'x' ? ft_strrep(str + 2, "s") : ft_strrep(str, "--S");
		else if (m & S_ISGID && i == 5)
			str[i] == 'x' ? ft_strrep(str + 5, "s") : ft_strrep(str + 3, "--S");
		else if (m & S_ISVTX && i == 8)
			str[i] == 'x' ? ft_strrep(str + 8, "t") : ft_strrep(str + 6, "--T");
	}
	if (listxattr(path, NULL, 0, XATTR_NOFOLLOW) > 0)
		str[i] = '@';
	else if ((acl = acl_get_link_np(path, ACL_TYPE_EXTENDED)))
		(str[i] = '+');
	acl_free(acl);
}

/*
** Determine file type by examining activated bit in stat mode
** Input: stat mode, pointer on time index, later used to retrieve corresponding
** char in display string
*/

void		get_type(mode_t m, int *t)
{
	static mode_t	type[NB_TYPES] = {S_IFDIR, S_IFLNK, S_IFSOCK, S_IFIFO,
		S_IFBLK, S_IFCHR, S_IFREG};
	int				i;

	i = -1;
	*t = REG;
	while (++i < NB_TYPES)
		if ((m & S_IFMT) == type[i])
			*t = i;
}

/*
** Get modification or change time, depending on user option (respect. -t or -c)
** Input: file for info modification, file stats for retrieving info
** Handle special case of time stamp more than 6 months ago or in the future
*/

void		get_time(ENV *e, time_t t, t_file **file, struct stat fstat)
{
	time_t current;

	(*file)->timestr[0] = 0;
	if (e->options & OPT_CTIME)
	{
		(*file)->time = fstat.st_ctime;
		(*file)->ntime = fstat.st_ctimespec.tv_nsec;
	}
	else if (e->options & OPT_MTIME)
	{
		(*file)->time = fstat.st_mtime;
		(*file)->ntime = fstat.st_mtimespec.tv_nsec;
	}
	if ((current = time(NULL)) == (time_t)-1)
		perror("time");
	if (ft_abs((current - t)) < (time_t)(15552000))
		ft_strncat((*file)->timestr, ctime(&t) + 4, 12);
	else
	{
		ft_strncat((*file)->timestr, ctime(&t) + 4, 7);
		ft_strnccat((*file)->timestr, ctime(&t) + 20, 4, ' ');
	}
}

/*
** Get user and group id stat, store as strings
** Input: file stats, strings to store id names
** Handle -n (numerical id instead of name) and -o (no gid) options
*/

void		get_id(ENV *e, struct stat *filestat, char *uid, char *gid)
{
	struct passwd	*pw;
	struct group	*gr;
	char			*id;

	if ((pw = getpwuid(filestat->st_uid)) && !(e->options & OPT_NONAME))
		ft_strcat(uid, pw->pw_name);
	else
	{
		id = ft_itoa(filestat->st_uid);
		ft_strcpy(uid, id);
		ft_strdel(&id);
	}
	if (e->options & OPT_NOGID)
		return ;
	if ((gr = getgrgid(filestat->st_gid)) && !(e->options & OPT_NONAME))
		ft_strcpy(gid, gr->gr_name);
	else
	{
		id = ft_itoa(filestat->st_gid);
		ft_strcat(gid, id);
		ft_strdel(&id);
	}
}
