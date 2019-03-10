/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_ls.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emuckens <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/10/25 15:13:54 by emuckens          #+#    #+#             */
/*   Updated: 2018/12/02 18:02:46 by emuckens         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_LS_H
# define FT_LS_H

# include "libft.h"
# include "color.h"
# include <dirent.h>
# include <sys/stat.h>
# include <limits.h>

# define NAME				"ft_ls"
# define ENV				t_environment
# define NB_MODES			9
# define NB_TYPES			7
# define NB_SORT			4
# define DEFAULT_BLOCKSIZE	512
# define STDERR				2
# define MIN_BTW_COL		1
# define MIN_COLW			6

/*
** USER OPTIONS
** 00000000 00000000 00000000 00000000
** ***CGRzy xwvutsrq ponmlkji hgfedcba
*/

# define OPTIONS			"CGRacdfiklnorstx1"
# define OPT_HIDDEN			(1 << ('a' - 'a'))
# define OPT_DIRPLAIN		(1 << ('d' - 'a'))
# define OPT_INCOMING		(1 << ('f' - 'a'))
# define OPT_LONG			(1 << ('l' - 'a'))
# define OPT_INODE			(1 << ('i' - 'a'))
# define OPT_REVERSE		(1 << ('r' - 'a'))
# define OPT_MTIME			(1 << ('t' - 'a'))
# define OPT_CTIME			(1 << ('c' - 'a'))
# define OPT_NONAME			(1 << ('n' - 'a'))
# define OPT_NOGID			(1 << ('o' - 'a'))
# define OPT_BLOCKSIZE		(1 << ('s' - 'a'))
# define OPT_KILOBYTES		(1 << ('k' - 'a'))
# define OPT_ATIME			(1 << ('u' - 'a'))
# define OPT_COLDIR			(1 << ('x' - 'a'))
# define OPT_RECUR			(1 << 27)
# define OPT_COLOR			(1 << 28)
# define OPT_COLUMNS		(1 << 29)
# define OPT_ONECOLUMN		(1 << 30)

/*
** ERROR CODES
*/

# define ERR_CYCLE			"directory causes a cycle"
# define ERR_BLOCKSIZE		"minimum blocksize is"
# define ERR_OPTION			"illegal option --"

/*
** ENVIRONMENT VARIABLES
*/

# define CLICOLOR			"CLICOLOR"
# define BLOCKSIZE			"BLOCKSIZE"
# define COLUMNS 			"COLUMNS"
# define LSCOLORS			"LSCOLORS"

typedef struct	s_padding
{
	unsigned char	nlen;
	unsigned char	llen;
	unsigned char	blen;
	unsigned char	ilen;
	unsigned char	ulen;
	unsigned char	glen;
	unsigned char	slen;
	unsigned char	majlen;
	unsigned char	minlen;
}				t_padding;

typedef struct	s_colfile
{
	char	name[NAME_MAX];
	int		blocksize;
	ino_t	inode;
}				t_colfile;

typedef struct	s_column
{
	int				win_w;
	int				nb_files;
	int				nbcol;
	int				nbrow;
	int				col;
	int				row;
	int				len;
	t_colfile		**files;
}				t_column;

typedef struct	s_colorstrings
{
	char			ref[2 * NB_COLTYPES];
	char			fg[10];
	char			bg[7];
	char			foff[7];
	char			boff[8];
}				t_colorstrings;

typedef struct	s_file
{
	t_padding		len;
	long			ntime;
	int				type;
	int				links;
	long			size;
	int				blocksize;
	time_t			time;
	ino_t			inode;
	char			name[NAME_MAX];
	char			mode[NB_MODES + 2];
	char			uid[NAME_MAX];
	char			gid[NAME_MAX];
	char			link[PATH_MAX];
	char			timestr[30];
	char			major[11];
	char			minor[11];
	char			disp;
	char			exe;
	char			cycle;
}				t_file;

typedef struct	s_environment
{
	t_padding		max;
	t_column		col;
	t_lbtree		*filelist;
	t_lbtree		*errlist;
	t_colorstrings	clr;
	int				(*fsort[NB_SORT])(struct s_environment *e,
						void *c1, void *c2);
	float			total;
	float			blocksize_coef;
	int				options;
	int				exit;
	int				argerr;
	int				error;
	int				nb_envvar;
	ino_t			fdinode;
	char			***env_var;
	unsigned char	sort;
	unsigned char	nl;
}				t_environment;

enum			e_nlconditions
{
	NOCOND, NOARG, MULTIARG, FIRST
};

enum			e_sort
{
	ARG, LEXI, TIME, INCOMING
};

enum			e_types
{
	DIRECTORY, LINK, SOCKET, FIFO, BLOCK, CHAR, REG
};

enum			e_bool
{
	FALSE, TRUE
};

enum			e_exit_status
{
	SUCCESS, FAIL
};

enum			e_dir_display
{
	SHORT, COLVER, COLHOR, LONG, BL_CH
};

enum			e_tree
{
	T_ERR, T_ARG
};

/*
** DISPLAY | COLOR AND PADDING
*/

int				display_file(ENV *e, t_file *file);
int				display_all(ENV *e, t_lbtree *tree);
void			display_columns(ENV *e);
void			get_padding_info(ENV *e, t_file *file);
void			reset_padding(ENV *e);
void			set_colors(ENV *e, t_file *file, char *cfront, char *cback);
void			set_coldisplay_dim(ENV *e);
void			set_color_ref(ENV *e);
void			reset_colors(ENV *e, int iter);
void			store_col_ver(ENV *e, t_file *file, char *fore, char *back);
void			store_col_hor(ENV *e, t_file *file, char *fore, char *back);

/*
** SCAN FILES AND FOLDER
*/

t_file			*get_fileinfo(ENV *e, char *name, char *path, int error);
void			get_id(ENV *e, struct stat *filestat, char *uid, char *gid);
void			get_mode(mode_t m, char *str, char *path);
void			get_optional_details(t_file **file, struct stat filestat);
void			get_time(ENV *e, time_t t, t_file **file, struct stat filestat);
int				get_stat(ENV *e, struct stat *buf, char *path, int show_err);
void			get_type(mode_t m, int *t);
int				is_asdir(ENV *e, char *path);
int				open_dir(DIR **dir, char **path);
int				list_files(ENV *e, char *path, t_lbtree *tree, int fd);
void			list_all(ENV *e, t_lbtree *tree, t_file *file);
int				sort_dir(ENV *e, DIR *dir, char *path, t_lbtree **tree);
void			scan_dir(ENV *e, t_lbtree *tree, char *path);

/*
** SORT
*/

void			ft_treeadd_cust(ENV *e, t_lbtree **tree, t_file *content,
				int sort);
int				sort_lexi(ENV *e, void *current, void *content);
int				sort_arg(ENV *e, void *current, void *content);
int				sort_mtime(ENV *e, void *current, void *content);
int				sort_incoming(ENV *e, void *current, void *content);

/*
** OPTIONS
*/

int				read_options(ENV *e, char **argv, int argc);

/*
** ENV
*/

int				allocate_files(ENV *e);
int				free_env(ENV *e, int exit_code);
void			free_files(ENV *e);
int				get_envvar(ENV *e, char **env);
int				get_nbvar(char **env);
int				get_envvar_index(ENV *e, char *var);
void			handle_environment_var(ENV *e);
int				init_env(ENV *e, char **env);

/*
** UTILS
*/

int				is_dir(ENV *e, t_lbtree *tree);
int				is_child(t_lbtree *node);
#endif
