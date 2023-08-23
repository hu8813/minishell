/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process_input.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: huaydin <huaydin@student.42vienna.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/07 18:22:53 by hakaraka          #+#    #+#             */
/*   Updated: 2023/08/17 13:49:37 by huaydin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

/* helper function of remove_curly_brackets to check if current chars are ${ */
int	update_quotes_chk_curly_bracket(int *quotes, char ch, int i[3], char **s)
{
	quotes[0] = (quotes[0] + (!quotes[1] && ch == '\'')) % 2;
	quotes[1] = (quotes[1] + (!quotes[0] && ch == '\"')) % 2;
	if (!quotes[0] && !quotes[1] && (*s)[i[0]] && (*s)[i[0]] == '$' && (*s)[i[0]
		+ 1] && (*s)[i[0] + 1] == '{' && (*s)[i[0] + 2])
		return (1);
	else
		return (0);
}

/* converts ${VAR} to $VAR so that later it can be expanded correctly */
void	remove_curly_brackets(char **s, int i[3], int quotes[2], char *tmp[3])
{
	while (*s && i[0] < (int)ft_strlen(*s) && (*s)[++i[0]])
	{
		if (update_quotes_chk_curly_bracket(quotes, (*s)[i[0]], i, s))
		{
			i[1] = i[0] + 2;
			i[2] = i[1];
			while ((*s)[i[1]] && (*s)[i[1]] != ' ' && (*s)[i[1]] != '}')
				i[1]++;
			if ((*s)[i[1]] == '}')
				tmp[2] = ft_substr(*s, i[2], i[1] - i[2]);
			else
				tmp[2] = ft_substr(*s, i[2] - 1, i[1] - i[2] + 1);
			tmp[0] = ft_substr(*s, 0, i[0]);
			tmp[1] = ft_strjoin(tmp[0], "$");
			free(tmp[0]);
			tmp[0] = ft_strjoin(tmp[1], tmp[2]);
			ft_free_all(tmp[1], tmp[2], NULL, NULL);
			tmp[1] = ft_strjoin(tmp[0], "");
			free(tmp[0]);
			tmp[0] = ft_strdup(&(*s)[i[1] + 1]);
			ft_free_all(*s, NULL, NULL, NULL);
			*s = ft_strjoin(tmp[1], tmp[0]);
			ft_free_all(tmp[0], tmp[1], NULL, NULL);
		}
	}
}

/* helper function of process_heredoc to check if current chars are << */
int	update_quotes_chk_heredoc(int *quo, char ch, int i[3], char **s)
{
	quo[0] = (quo[0] + (!quo[1] && ch == '\'')) % 2;
	quo[1] = (quo[1] + (!quo[0] && ch == '\"')) % 2;
	if (!quo[0] && !quo[1] && (*s)[i[0]] && (*s)[i[0]] == '<' && (*s)[i[0] + 1]
		&& (*s)[i[0] + 1] == '<' && (*s)[i[0] + 2] && (*s)[i[0] + 2] == '<')
	{
		put_err("syntax error near unexpected token `<'", NULL, 2, NULL);
		return (0);
	}
	else if (!quo[0] && !quo[1] && (*s)[i[0]] && (*s)[i[0]] == '<'
		&& (*s)[i[0] + 1] && (*s)[i[0] + 1] == '<' && (*s)[i[0] + 2])
		return (1);
	else
		return (0);
}

/* converts <<$VAR as heredoc delimiter to <<'$VAR' */
void	process_heredoc(char **s, int i[3], int quotes[2], char *tmp[3])
{
	while (*s && i[0] < (int)ft_strlen(*s) && (*s)[++i[0]])
	{
		if (update_quotes_chk_heredoc(quotes, (*s)[i[0]], i, s))
		{
			i[1] = i[0] + 2;
			while ((*s)[i[1]] && (ft_strchr(" \\t", (*s)[i[1]])))
				i[1]++;
			i[2] = i[1];
			while ((*s)[i[1]] && (*s)[i[1]] != ' ' && (*s)[i[1]] != '\t')
				i[1]++;
			tmp[2] = ft_substr(*s, i[2], i[1] - i[2]);
			tmp[0] = ft_substr(*s, 0, i[0]);
			tmp[1] = ft_strjoin(tmp[0], "<<'");
			free(tmp[0]);
			tmp[0] = ft_strjoin(tmp[1], tmp[2]);
			ft_free_all(tmp[1], tmp[2], NULL, NULL);
			tmp[1] = ft_strjoin(tmp[0], "'");
			free(tmp[0]);
			tmp[0] = ft_strdup(&(*s)[i[1]]);
			ft_free_all(*s, NULL, NULL, NULL);
			*s = ft_strjoin(tmp[1], tmp[0]);
			ft_free_all(tmp[0], tmp[1], NULL, NULL);
		}
	}
}

/* 	checks if EOF (Ctrl+D) is pressed (input_str == NULL)
	if yes return, else add to history if it's not empty
	then prepares input string before expanding
	i.e. for edge cases where a variable is a heredoc delimiter <<$VAR
	or variable is in curly brackets ${VAR} 
	then expands the variables
	calls split function to split the input string into tokens
	then parses the tokens and executes the commands
	clears the linked list of tokens and input string */
void	*ft_process_input(char *input, t_cmd_set *p)
{
	char	**a;
	char	*tmp[3];
	int		i[3];
	int		quotes[2];

	i[0] = -1;
	quotes[0] = 0;
	quotes[1] = 0;
	if (!input)
		return (NULL);
	if (input && input[0] != '\0')
		add_history(input);
	if (ft_strnstr(input, "<<", ft_strlen(input)))
		process_heredoc(&input, i, quotes, tmp);
	if (ft_strnstr(input, "${", ft_strlen(input)))
		remove_curly_brackets(&input, i, quotes, tmp);
	input = var_expander(input, quotes, p);
	a = split_and_ignore_space_if_in_quote(input, " ");
	if (!a)
		ft_free_exit(p, 1, "Unexpected Error");
	p = parse_nodes(a, p);
	ft_free_all(input, NULL, NULL, NULL);
	if (p && p->cmds)
		ft_lstclear(&p->cmds, ft_lst_free);
	return (p);
}
