/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_if_builtin.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: huaydin <huaydin@student.42vienna.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/07 18:25:09 by hakaraka          #+#    #+#             */
/*   Updated: 2023/08/21 17:47:32 by huaydin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

/* checks if the cmd is a builtin cmd */
int	is_builtin(t_cmd *n)
{
	int	len;

	if (!n->args)
		return (0);
	if ((n->args && ft_strchr(*n->args, '/')) || (n->cmd_path
			&& ft_strchr(n->cmd_path, '/')))
		return (0);
	len = ft_strlen(*n->args);
	if (!ft_strncmp(*n->args, "pwd", len) && len == 3)
		return (1);
	if (!ft_strncmp(*n->args, "env", len) && len == 3)
		return (1);
	if (!ft_strncmp(*n->args, "cd", len) && len == 2)
		return (1);
	if (!ft_strncmp(*n->args, "export", len) && len == 6)
		return (1);
	if (!ft_strncmp(*n->args, "unset", len) && len == 5)
		return (1);
	if (!ft_strncmp(*n->args, "echo", len) && len == 4)
		return (1);
	if (!ft_strncmp(*n->args, "exit", len) && len == 4)
		return (1);
	return (0);
}

/* call find_cmd_path to get path of a cmd
	creates a pipe, calls chk_perm_call_child which creates fork if needed
	closes the write end of the pipe 
	sets in_fd if there is a next cmd
	else closes the read end of the fd 
	closes the input and output fds */
void	*forked_builtins_and_rest(t_cmd_set *p, t_list *cmd)
{
	int	fd[2];

	signal(SIGINT, signals_child);
	signal(SIGQUIT, signals_child);
	find_cmd_path(p, cmd, NULL, NULL);
	if (pipe(fd) == -1)
		return (put_err("DupForkPipe_Failed", NULL, 1, p));
	if (!chk_perm_call_child(p, cmd, fd))
		return (NULL);
	close(fd[1]);
	if (cmd->next && !((t_cmd *)cmd->next->content)->in_fd)
		((t_cmd *)cmd->next->content)->in_fd = fd[0];
	else
		close(fd[0]);
	if (((t_cmd *)cmd->content)->in_fd > 2)
		close(((t_cmd *)cmd->content)->in_fd);
	if (((t_cmd *)cmd->content)->out_fd > 2)
		close(((t_cmd *)cmd->content)->out_fd);
	return (NULL);
}

/*  cd, export, unset, exit are not programs but shell built-in cmds */
int	cd_export_unset_exit(t_cmd_set *p, t_list *cmd, int *is_exit, int n)
{
	char	**a;
	int		status_code;

	while (cmd)
	{
		status_code = 0;
		a = ((t_cmd *)cmd->content)->args;
		n = 0;
		if (a)
			n = ft_strlen(*a);
		if (a && !ft_strncmp(*a, "exit", n) && n == 4)
			ft_exit(cmd, is_exit, p);
		else if (!cmd->next && a && !ft_strncmp(*a, "cd", n) && n == 2)
			status_code = ft_cd(p, a);
		else if (!cmd->next && a && !ft_strncmp(*a, "export", n) && n == 6)
			status_code = ft_export(p, a);
		else if (!cmd->next && a && !ft_strncmp(*a, "unset", n) && n == 5)
			status_code = ft_unset(p, a);
		else
			forked_builtins_and_rest(p, cmd);
		if (cmd->next)
			status_code = 0;
		cmd = cmd->next;
	}
	return (status_code);
}

/* if the cmd is not builtin, runs the execve
	otherwise checks if it's pwd, echo or env: which are not
	shell builtin cmds but separate programs in bash */
void	pwd_echo_env_and_external(t_cmd_set *p, t_cmd *n, int l, t_list *cmd)
{
	signal(SIGINT, signals_child);
	signal(SIGQUIT, signals_child);
	p->status_code = 0;
	if (!is_builtin(n) && n->args)
		execve(n->cmd_path, n->args, p->envp);
	else if (n->args && !ft_strncmp(*n->args, "pwd", l) && l == 3)
		p->status_code = ft_pwd();
	else if (is_builtin(n) && n->args
		&& !ft_strncmp(*n->args, "echo", l) && l == 4)
		p->status_code = ft_echo(cmd);
	else if (is_builtin(n) && n->args
		&& !ft_strncmp(*n->args, "env", l) && l == 3)
		ft_printarray(p->envp);
	if (cmd->next)
		p->status_code = 0;
}
