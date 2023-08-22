/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   create_fds.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: huaydin <huaydin@student.42vienna.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/07 18:23:28 by hakaraka          #+#    #+#             */
/*   Updated: 2023/08/21 19:59:23 by huaydin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

/* checks for permission if file is readable or writeable,
	then creates files depending flags which are set by the functions below */
int	get_fd(int oldfd, char *path, int flags[2], t_cmd_set *p)
{
	int	fd;

	if (oldfd > 2)
		close(oldfd);
	if (!path)
		return (-1);
	if (access(path, F_OK) == -1 && !flags[0] && ft_lstsize(p->cmds) == 1)
		put_err("NoFile_NoDir", path, 0, p);
	else if (access(path, F_OK) == -1 && !flags[0])
		put_err("NoFile_NoDir", path, 1, p);
	else if (!flags[0] && (access(path, R_OK) == -1)
		&& ft_lstsize(p->cmds) == 1)
		put_err("Perm_Denied", path, 126, p);
	else if (flags[0] && access(path, W_OK) == -1 && access(path, F_OK) == 0
		&& ft_lstsize(p->cmds) == 1)
		put_err("Perm_Denied", path, 126, p);
	if (flags[0] && flags[1])
		fd = open(path, O_CREAT | O_WRONLY | O_APPEND, 0664);
	else if (flags[0] && !flags[1])
		fd = open(path, O_CREAT | O_WRONLY | O_TRUNC, 0664);
	else if (!flags[0] && oldfd != -1)
		fd = open(path, O_RDONLY);
	else
		fd = oldfd;
	return (fd);
}

/* sets the out_fd in truncate mode i.e. ls > out.txt
	calls the get_fd to create the file.
	truncate will delete the old content of file and add new content */
t_cmd	*out_fd_truncate(t_cmd *node, char **args, int *i, t_cmd_set *p)
{
	int	flags[2];

	flags[0] = 1;
	flags[1] = 0;
	(*i)++;
	if (args[*i])
		node->out_fd = get_fd(node->out_fd, args[*i], flags, p);
	if (!args[*i] || node->out_fd == -1)
	{
		*i = -1;
		if (node->out_fd != -1)
		{
			ft_putendl_fd("syntax error near unexpected token `newline'", 2);
			p->status_code = 2;
		}
		else
			p->status_code = 1;
	}
	return (node);
}

/* sets the out_fd in append mode i.e. ls >> out.txt
	calls the get_fd to create the file */
t_cmd	*out_fd_append(t_cmd *node, char **args, int *i, t_cmd_set *p)
{
	int	flags[2];

	flags[0] = 1;
	flags[1] = 1;
	(*i)++;
	if (args[++(*i)])
		node->out_fd = get_fd(node->out_fd, args[*i], flags, p);
	if (!args[*i] || node->out_fd == -1)
	{
		*i = -1;
		if (node->out_fd != -1)
		{
			ft_putendl_fd("syntax error near unexpected token `newline'", 2);
			p->status_code = 2;
		}
		else
			p->status_code = 1;
	}
	return (node);
}

/* sets in_fd to read from a file i.e.  <input.txt cat
	calls the get_fd to create the file.
	append will not delete old content, but add text to the end of file */
t_cmd	*in_fd_read(t_cmd *node, char **args, int *i, t_cmd_set *p)
{
	int	flags[2];

	flags[0] = 0;
	flags[1] = 0;
	(*i)++;
	if (args[*i])
		node->in_fd = get_fd(node->in_fd, args[*i], flags, p);
	if (!args[*i] || node->in_fd == -1)
	{
		if (node->in_fd != -1)
		{
			ft_putendl_fd("syntax error near unexpected token `newline'", 2);
			p->status_code = 2;
		}
		if (p && p->cmds && ft_lstsize(p->cmds) != 1)
			p->status_code = 0;
		else
			(*i)--;
	}
	return (node);
}
