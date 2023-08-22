/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: huaydin <huaydin@student.42vienna.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/07 18:23:15 by hakaraka          #+#    #+#             */
/*   Updated: 2023/08/22 09:08:00 by huaydin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

/* loop for reading heredoc text via readline 
	exits loop when delimiter is entered or Ctrl+C is pressed */
char	*get_here_str(char *str[3], char *lim, t_cmd_set *p)
{
	p->status_code = 0;
	g_exit_status = 0;
	while (p->status_code != 130 && g_exit_status != 130)
	{
		if (!str[1])
			str[1] = ft_strdup("");
		if (!str[0])
			str[0] = ft_strdup("");
		str[2] = str[1];
		str[1] = ft_strjoin(str[1], str[0]);
		ft_free_all(str[0], str[2], NULL, NULL);
		str[0] = readline("> ");
		if (!str[0])
		{
			printf("%s (wanted `%s\')\n", "mini: warning: end-of-file", lim);
			break ;
		}
		str[2] = str[0];
		if (str[2] && str[2][0] && !ft_strncmp(str[2], lim, ft_strlen(str[2]))
			&& ft_strlen(str[2]) == ft_strlen(lim))
			break ;
		str[0] = ft_strjoin(str[0], "\n");
		free(str[2]);
	}
	return (ft_free_all(str[0], NULL, NULL, NULL), str[1]);
}

/* expand the env variables in text */
static void	process_here(char *str[1], int fd[2], t_cmd_set *p, int expand)
{
	int	i;
	int	j;

	i = 0;
	j = 0;
	while (str && str[1] && str[1][i])
	{
		j = i + 1;
		if (str[1][i] && str[1][i] == '$' && str[1][j] && ft_isalnum(str[1][j])
			&& expand)
			handle_env_vars(str[1], &i, fd, p);
		else if (str[1][i])
			write(fd[1], &str[1][i], 1);
		i++;
	}
}

char	*trim_all_quotes(char *str)
{
	size_t	len;
	size_t	result_len;
	size_t	i;
	char	*result;
	size_t	j;

	len = ft_strlen(str);
	result_len = 0;
	i = -1;
	while (++i < len)
		if (str[i] != '\'' && str[i] != '\"')
			result_len++;
	result = (char *)malloc(result_len + 1);
	if (!result)
	{
		return (NULL);
	}
	j = 0;
	i = -1;
	while (++i < len)
		if (str[i] != '\'' && str[i] != '\"')
			result[j++] = str[i];
	result[j] = '\0';
	return (result);
}

/* sets the signals for heredoc, creates a pipe to write the 
	content to the write end: fd[1] , returns the read end: fd[0]
	so that the content can later be read */
int	read_heredoc(char *str[3], char *tmpstr[2], t_cmd_set *p)
{
	int	fd[2];
	int	expand;

	expand = 1;
	signal(SIGINT, signals_heredoc);
	signal(SIGQUIT, SIG_IGN);
	if (pipe(fd) == -1)
		return (put_err("DupForkPipe_Failed", NULL, 1, p), -1);
	if (ft_strchrs_idx(tmpstr[0], "\"'") != -1)
		expand = 0;
	tmpstr[0] = trim_all_quotes(tmpstr[0]);
	str[1] = get_here_str(str, tmpstr[0], p);
	process_here(str, fd, p, expand);
	ft_free_all(str[1], tmpstr[0], NULL, NULL);
	if (fd[1] != -1)
		close(fd[1]);
	if (p->status_code == 130)
	{
		if (fd[0] != -1)
			close(fd[0]);
		return (-1);
	}
	return (fd[0]);
}

/* calls the read_heredoc to read from heredoc, saves the read end 
	fd[0] in the struct node->in_fd */
t_cmd	*in_fd_heredoc(t_cmd *node, char **args, int *i, t_cmd_set *p)
{
	char	*tmpstr[2];
	char	*str[3];

	str[0] = NULL;
	str[1] = NULL;
	str[2] = NULL;
	tmpstr[1] = "mini: warning: here-document delimited by end-of-file";
	(*i)++;
	if (args[++(*i)])
	{
		tmpstr[0] = args[*i];
		node->in_fd = read_heredoc(str, tmpstr, p);
	}
	if (!args[*i] || node->in_fd == -1)
	{
		*i = -1;
		if (node->in_fd != -1)
		{
			ft_putendl_fd("syntax error near unexpected token `newline'", 2);
			p->status_code = 2;
		}
	}
	return (node);
}
