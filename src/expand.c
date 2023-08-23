/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: huaydin <huaydin@student.42vienna.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/07 18:24:28 by hakaraka          #+#    #+#             */
/*   Updated: 2023/08/17 14:11:42 by huaydin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

/* although "~"" was not mandatory according to Subject
	expands the string containing only ~ and strings starting with ~/ 
	but cd ~/Downloads etc. is not implemented */
static void	expand_tilde(char **str, t_cmd_set *p, int quotes[2])
{
	char	*home_path;
	char	*tmp;
	char	*expanded_str;

	quotes[0] = 0;
	quotes[1] = 0;
	if ((*str)[0] == '~' && (!(*str)[1] || (*str)[1] == '/'))
	{
		home_path = ft_getenv("HOME", p->envp);
		if (home_path)
		{
			if ((*str)[1])
				expanded_str = ft_strjoin(home_path, &(*str)[1]);
			else
				expanded_str = ft_strdup(home_path);
			ft_free_all(*str, home_path, NULL, NULL);
			*str = expanded_str;
		}
	}
	if (*str && ft_strchr(*str, '$'))
	{
		tmp = *str;
		*str = ft_strjoin(*str, "\"\"");
		free(tmp);
	}
}

/* helper function of expand vars, which finds the end_index 
	of a variable i.e. $VAR.. then end_index is  where "." is.
	Curly brackets are handled here by setting the start to after the beginning {
	and the end to before the ending }. */
void	find_var_start_end(char *var, int *j)
{
	int	i;

	j[0] = (int)ft_strlen(var) - 1;
	j[1] = j[0];
	i = 0;
	while (i <= j[0] && (ft_isalnum(var[i]) || var[i] == '_'))
		i++;
	if (var[i])
		i--;
	j[0] = i;
}

/* helper func of exppand_path_var
	calls getenv func if a $var is to be expanded
	returns status_code if $? is to be expanded
	$$ is not implemented, as getpid not allowed and subject doesn't ask it */
static char	*find_substitution(char first, char *tmp2, t_cmd_set *p)
{
	char	*var;

	var = NULL;
	if (first == '$')
		var = ft_strdup("");
	else if (first == '?')
	{
		if (g_exit_status > 0)
		{
			p->status_code = g_exit_status;
			g_exit_status = 0;
		}
		var = ft_itoa(p->status_code);
	}
	else
		var = ft_getenv(tmp2, p->envp);
	ft_free_all(tmp2, NULL, NULL, NULL);
	if (!var)
		var = ft_strdup("");
	return (var);
}

/* helper func of var_expander which calls find_substitution of a variable */
static char	*var_or_path_expander(char *str, int i, t_cmd_set *p, char *s[4])
{
	int	j[3];

	j[2] = ft_strchrs_idx(&str[i], "|\"\'$?>< ") + (ft_strchr("$?", str[i]) != 0);
	s[0] = ft_substr(str, i, j[2]);
	find_var_start_end(s[0], j);
	s[2] = ft_substr(s[0], 0, j[0] + 1);
	s[3] = ft_substr(s[0], j[0] + 1, j[1] - j[0]);
	free(s[0]);
	s[0] = find_substitution(str[i], s[2], p);
	if (!s[3])
		s[3] = ft_strdup("");
	s[1] = s[3];
	if (s[3][0] && s[3][0] == '?')
		s[3]++;
	s[2] = s[0];
	s[0] = ft_strjoin(s[0], s[3]);
	ft_free_all(s[2], s[1], NULL, NULL);
	if (!s[0])
		s[0] = ft_strdup("");
	s[3] = ft_substr(str, 0, i - 1);
	s[2] = ft_strjoin(s[3], s[0]);
	free(s[3]);
	s[3] = ft_strjoin(s[2], &str[i + j[2]]);
	ft_free_all(s[2], s[0], str, NULL);
	return (var_expander(s[3], j, p));
}

/* recursive func which expands variables in the input_str  */
char	*var_expander(char *str, int quotes[2], t_cmd_set *p)
{
	char	*tmp[2];
	char	*s[4];
	int		i;

	i = -1;
	expand_tilde(&str, p, quotes);
	while (str && str[++i] && i < (int)ft_strlen(str))
	{
		quotes[0] = (quotes[0] + (!quotes[1] && str[i] == '\'')) % 2;
		quotes[1] = (quotes[1] + (!quotes[0] && str[i] == '\"')) % 2;
		if (!quotes[0] && str[i] && str[i] == '$' && str[i + 1]
			&& ((ft_strchrs_idx(&str[i + 1], "/~%^{}:; ") && !quotes[1])
				|| (ft_strchrs_idx(&str[i + 1], "/~%^{}:;\" ") && quotes[1])))
		{
			tmp[1] = var_or_path_expander(str, ++i, p, s);
			tmp[0] = tmp[1];
			tmp[1] = ft_strjoin(tmp[1], "\"\"");
			free(tmp[0]);
			return (var_expander(tmp[1], quotes, p));
		}
	}
	return (str);
}
