/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_env.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: huaydin <huaydin@student.42vienna.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/07 18:24:54 by hakaraka          #+#    #+#             */
/*   Updated: 2023/08/21 03:38:09 by huaydin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

/* find the entry that contains the value of var in envp.
	length of var (n) is needed to address the = after the variable name. */
int	find_env_var_index(char *var, char **envp)
{
	int	i;
	int	n;
	int	n2;

	i = 0;
	n = ft_strlen(var);
	while (envp && envp[i])
	{
		n2 = n;
		if (n2 < ft_strchr_idx(envp[i], '='))
			n2 = ft_strchr_idx(envp[i], '=');
		if (!ft_strncmp(envp[i], var, n2))
		{
			return (i);
		}
		i++;
	}
	return (-1);
}

/* get environment variable by name.
	the variable is looked up by index.
	if not found, returns NULL.
	if found, duplicates the string of the value and returns it. */
char	*ft_getenv(char *var, char **envp)
{
	int			var_index;
	const char	*env_var;
	const char	*value_start;

	if (var[0] == 0)
		return (ft_strdup("'$'"));
	var_index = find_env_var_index(var, envp);
	if (var_index == -1)
		return (NULL);
	env_var = envp[var_index];
	value_start = ft_strchr(env_var, '=');
	if (value_start)
	{
		value_start++;
		return (ft_strdup(value_start));
	}
	return (NULL);
}

/* set value of variable var to value in envp.
	first finds the variable.
	if not found, adds a new entry to envp.
	if found, replaces the entry after freeing the old entry. */
char	**ft_setenv(char *var, char *value, char **envp)
{
	int		index;
	char	*tmpstr[2];

	index = find_env_var_index(var, envp);
	if (index != -1)
	{
		tmpstr[0] = ft_strjoin(var, "=");
		if (tmpstr[0])
		{
			free(envp[index]);
			envp[index] = ft_strjoin(tmpstr[0], value);
			free(tmpstr[0]);
		}
	}
	else
	{
		tmpstr[0] = ft_strjoin(var, "=");
		if (tmpstr[0])
		{
			tmpstr[1] = ft_strjoin(tmpstr[0], value);
			envp = ft_array_insert(envp, tmpstr[1]);
			ft_free_all(tmpstr[0], tmpstr[1], NULL, NULL);
		}
	}
	return (envp);
}

/* this is the called builtin.
	it takes multiple arguments, each one is a variable name.
	it unsets all variables in the environment that are given. */
int	ft_unset(t_cmd_set *p, char **args)
{
	int		i;
	char	*tmpstr;
	int		var_index;

	i = -1;
	if (ft_arr_len(args) >= 2)
	{
		while (args[++i])
		{
			if (ft_strlen(args[i]) && args[i][ft_strlen(args[i]) - 1]
				&& args[i][ft_strlen(args[i]) - 1] != '=')
			{
				tmpstr = ft_strjoin(args[i], "=");
				free(args[i]);
				args[i] = tmpstr;
			}
			var_index = find_env_var_index(args[i], p->envp);
			if (var_index != -1)
				ft_array_replace(&p->envp, NULL, var_index);
		}
	}
	return (0);
}

/* handle environment variables for heredoc.
	it finds the end of the variable starting at str[*i].
	it extracts the variable name, looks up the variable,
    and outputs to the given fd.
	it sets *i to the next character after the variable name */
void	handle_env_vars(char *str, int *i, int fd[2], t_cmd_set *p)
{
	int		j;
	char	*tmp;
	char	*tmp2;

	j = *i + 1;
	while (str[j])
	{
		if (!ft_isalnum(str[j]) || str[j] == ' ' || str[j] == '\n'
			|| str[j] == '\t' || str[j] == '$' || str[j] == '\"'
			|| str[j] == '\'' || str[j] == '<')
			break ;
		j++;
	}
	tmp = ft_substr(str, *i + 1, j - *i - 1);
	tmp2 = ft_getenv(tmp, p->envp);
	write(fd[1], tmp2, ft_strlen(tmp2));
	ft_free_all(tmp, tmp2, NULL, NULL);
	*i = j - 1;
}
