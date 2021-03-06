/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_sequence.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tboos <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/11/14 12:54:52 by tboos             #+#    #+#             */
/*   Updated: 2016/11/14 12:54:54 by tboos            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int		*ft_close_pipe(int *new, int *old)
{
	if (old && old[0] && old[1])
	{
		close(old[0]);
		close(old[1]);
	}
	if (new)
		return (new);
	return (NULL);
}

static void		ft_pipe_process(int *r_pipe, t_list *pipe)
{
	t_pipe	*w_pipe;

	if (r_pipe && r_pipe[0] && r_pipe[1])
	{
		if (-1 == dup2(r_pipe[0], STDIN_FILENO))
			ft_error(SHNAME, "dup error", "writing end", CR_ERROR);
		ft_close_pipe(NULL, r_pipe);
	}
	if (pipe && pipe->data_size == PIPE)
	{
		w_pipe = (t_pipe*)pipe->data;
		if (((int*)w_pipe)[0] && ((int*)w_pipe)[1])
		{
			if (-1 == dup2(((int*)w_pipe)[1], STDOUT_FILENO))
				ft_error(SHNAME, "dup error", "writing end", CR_ERROR);
			ft_close_pipe(NULL, (int *)w_pipe);
		}
		if (w_pipe->others_fd)
			ft_handle_multiplefd(w_pipe->others_fd);
	}
}

static void		ft_pack_process(t_list *begin, t_config *config, int *r_pipe,
				char *path)
{
	t_list	*sshell;

	config->shell_state = RUNNING_SON;
	ft_pipe_process(r_pipe, begin->next);
	ft_signal_reset();
	if (begin->data_size == SSHELL && (config->shell_state = RUNNING_SSHELL))
	{
		if ((config->last_exit = ft_strtabifindstart(config->env,
			"SHLVL")) != -1)
			ft_setenv("SHLVL",
				ft_st_itoa(ft_atoi(config->env[config->last_exit] + 6) - 1),
			config);
		sshell = (t_list*)begin->data;
		begin->data = NULL;
		ft_freelist(&config->chimera);
		ft_freelist(&config->chimera_tail);
		config->chimera = sshell;
		ft_parse(config);
	}
	else
		ft_launch_process(path, begin->data, config);
	ft_status(config->last_exit);
	ft_shell_exit(config);
}

static t_list	*ft_fork_process(t_list *begin, t_config *config, int *r_pipe)
{
	t_list	*new;
	pid_t	pid;
	pid_t	*mem;
	char	*path;

	new = NULL;
	if (!begin->data_size && (ft_is_no_fork_builtin(begin->data, config)
		|| !(path = ft_path_handle(begin->data, config))))
		return (NULL);
	else if ((pid = fork()) == -1
		&& ft_error(SHNAME, "parser", "fork error", CR_ERROR))
		return (NULL);
	else if (!pid)
		ft_pack_process(begin, config, r_pipe, path);
	else if (!(mem = (pid_t*)ft_memalloc(sizeof(pid_t)))
		|| !(*mem = pid)
		|| !(new = ft_lstnew((void *)mem, PROS)))
		ft_error(SHNAME, "parser", "malloc error on process control", CR_ERROR);
	return (new);
}

t_list			*ft_run_sentence(t_list *begin, t_config *config, int *r_pipe)
{
	t_list	*tmp;
	t_list	*process;

	process = NULL;
	while (begin)
	{
		if (!begin->data_size || begin->data_size == SSHELL)
		{
			if ((tmp = ft_fork_process(begin, config, r_pipe)))
				ft_list_push_back(&process, tmp);
		}
		else if (begin->data_size == PIPE)
		{
			r_pipe = ft_close_pipe((int*)(begin->data), r_pipe);
			if (!ft_build_pipe(begin->next, config, &r_pipe))
				return (process);
		}
		begin = begin->next;
	}
	ft_close_pipe(NULL, r_pipe);
	return (process);
}
