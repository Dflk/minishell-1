/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tboos <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/03/17 15:23:28 by tboos             #+#    #+#             */
/*   Updated: 2016/03/23 17:59:08 by tboos            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H
# include <sys/types.h>
# include <dirent.h>
# include <stdio.h>
# include "libft.h"

typedef struct	dirent	t_dirent;
typedef struct	s_bin
{
	char		*name;
	char		*path_name;
}				t_bin;
typedef struct	s_config
{
	char		**env;
	t_list		*bin;
	t_list		*h_bin[34]; /*map[yr + yo][xr + xo] == '.'*/
}				t_config;

/*
**streamscan.c
*/
char			*ft_streamscan(t_config *config);
/*
**hash.c
*/
int				ft_pathtohash(t_config *config);
char			*ft_return_binpath(t_config *config, char *name);
/*
**cmp.c
*/
int				ft_ascii_cmp(t_bin *s1, t_bin *s2);
/*
**free.c
*/
void			ft_freebin(void *data, size_t data_size);
void			ft_free_config(t_config *config);
/*
**minishell.c
*/
void			ft_minishell(t_config *config);
/*
**prompt.c
*/
int				ft_prompt(t_config *config);

#endif
