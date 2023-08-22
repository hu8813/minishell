/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_nodes.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: huaydin <huaydin@student.42vienna.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/07 18:24:22 by hakaraka          #+#    #+#             */
/*   Updated: 2023/08/21 20:47:46 by huaydin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

/* checks for Syntax errors i.e. <| or || >| */
static int	is_invalid_syntax(char *t[], int *i)
{
	int	syntax_error;

	syntax_error = 0;
	syntax_error = ((t[*i] && t[*i][0] == '<'
				&& ((t[*i + 1] == NULL) || (t[*i + 1][0]
						&& t[*i + 1][0] == '<' && ((t[*i + 2]
								&& t[*i + 2] == NULL) || (t[*i + 2]
								&& t[*i + 2][0] == '<'))))) || ((t[*i][0]
					&& t[*i][0] == '>' && t[*i + 1] == NULL) || (t[*i][0] == '>'
					&& t[*i + 1] && t[*i + 1][0]
					&& t[*i + 1][0] == '|') || (t[*i][0]
					&& t[*i][0] == '<' && t[*i + 1]
					&& t[*i + 1][0] == '|') || (t[*i][0]
					&& t[*i][0] == '|' && t[*i + 1]
					&& t[*i + 1][0] == '|')));
	if (syntax_error)
		*i = -1;
	return (syntax_error);
}

/* syntax check for redir or pipe chars and sets file descriptor */
static t_cmd	*check_redir_pipe(t_cmd *t, char **a[2], int *i, t_cmd_set *p)
{
	if (*i >= 0 && a && a[0] && a[0][*i])
	{
		if (is_invalid_syntax(a[0], i))
			return (put_err(0, "syntax error near unexpected token", 2, p), t);
		else if (a[0][*i][0] == '>' && a[0][*i + 1] && a[0][*i + 1][0] == '>'
			&& g_exit_status != 130)
			t = out_fd_append(t, a[1], i, p);
		else if (a[0][*i][0] == '>' && g_exit_status != 130)
			t = out_fd_truncate(t, a[1], i, p);
		else if (a[0][*i][0] == '<' && a[0][*i + 1] && a[0][*i + 1][0] == '<')
			t = in_fd_heredoc(t, a[1], i, p);
		else if (a[0][*i][0] == '<')
			t = in_fd_read(t, a[1], i, p);
		else if (a[0][*i][0] != '|')
			t->args = ft_array_insert(t->args, a[1][*i]);
		else
		{
			put_err("Empty_Pipe", NULL, 2, p);
			*i = -1;
		}
		return (t);
	}
	*i = -1;
	return (put_err("Empty_Pipe", NULL, 2, p), t);
}

/* returns args after quotes removed */
static char	**args_after_quotes_removed(char **args)
{
	char	**temp;
	char	*tmpstr;
	int		j;

	j = -1;
	temp = ft_dup_array(args);
	while (temp && temp[++j])
	{
		tmpstr = remove_quotes(temp[j], 0, 0);
		free(temp[j]);
		temp[j] = tmpstr;
	}
	return (temp);
}

/* calls args_after_quotes_removed func
	adds the command at the end of the linked list
	calls check_redir_pipe to check for errors, free_tmp_lst on error */
static t_list	*parse_cmds(char **args, int i, t_cmd_set *p)
{
	t_list	*cmds[2];
	char	**temp[2];
	t_cmd	*tmp;

	cmds[0] = NULL;
	temp[1] = args_after_quotes_removed(args);
	while (args[++i])
	{
		cmds[1] = ft_lstlast(cmds[0]);
		if (i == 0 || (args[i][0] == '|' && args[i + 1] && args[i + 1][0]))
		{
			i += args[i][0] == '|';
			ft_lstadd_back(&cmds[0], ft_lstnew(init_cmd()));
			cmds[1] = ft_lstlast(cmds[0]);
		}
		temp[0] = args;
		tmp = cmds[1]->content;
		cmds[1]->content = check_redir_pipe(tmp, temp, &i, p);
		if (i < 0)
			return (free_tmp_lst(cmds[0], args, temp[1]));
	}
	ft_free_array(&temp[1]);
	ft_free_array(&args);
	return (cmds[0]);
}

/* call parse_cmds func and gives the splitted args as parameter
	sets the $_ env value if only 1 command is entered (last parameter)
	calls exec_cmd_and_wait
	clears the cmds and list if an exit cmd was recently entered */
void	*parse_nodes(char **args, t_cmd_set *p)
{
	int	is_exit;
	int	status;
	int	tmp[2];

	is_exit = 0;
	status = 0;
	tmp[0] = -1;
	tmp[1] = -1;
	p->cmds = parse_cmds(split_with_special_chars(args), -1, p);
	if (!p->cmds)
		return (p);
	if (ft_lstsize(p->cmds) == 1 && ((t_cmd *)(p->cmds->content))->args
		&& ft_arr_len(((t_cmd *)(p->cmds->content))->args))
		ft_setenv("_", ((t_cmd *)(p->cmds->content))->args[ft_arr_len(((t_cmd *)
					(p->cmds->content))->args) - 1], p->envp);
	exec_cmd_and_wait(p, status, tmp, &is_exit);
	if (is_exit)
		ft_free_exit(p, p->status_code, NULL);
	if (p->cmds && is_exit)
	{
		ft_lstclear(&p->cmds, ft_lst_free);
		return (NULL);
	}
	return (p);
}
