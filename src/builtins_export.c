/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_export.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: huaydin <huaydin@student.42vienna.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/07 18:24:54 by hakaraka          #+#    #+#             */
/*   Updated: 2023/08/17 18:29:09 by huaydin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

/* returns 1 if variable is found in envp, otherwise 0 */
int	var_in_envp(char *argv, char **envp, int ij[2])
{
	int	pos;

	ij[1] = 0;
	pos = ft_strchr_idx(argv, '=');
	if (pos == -1)
		return (-1);
	while (envp[ij[1]])
	{
		if (!ft_strncmp(envp[ij[1]], argv, pos + 1))
			return (1);
		ij[1]++;
	}
	return (0);
}

/* checks if the variable name is valid, it must not begin with digit
	all characters must be alphanumeric, except "_"  */
int	is_valid_identifier(char *str)
{
	int	j;

	if (!ft_strchr_idx(str, '=') || !str || (str && str[0] == '='))
	{
		ft_putstr_fd("mini: export: not a valid identifier\n", 2);
		return (0);
	}
	if ((str[0]) && ft_isdigit(str[0]) && str[0] != '_')
	{
		ft_putstr_fd("mini: export: not a valid identifier\n", 2);
		return (0);
	}
	j = 0;
	while (str[j] && str[j] != '=' && str[j] != '_')
	{
		if (!ft_isalnum(str[j]))
		{
			ft_putstr_fd("mini: export: not a valid identifier\n", 2);
			return (0);
		}
		j++;
	}
	return (1);
}

/* loops through the given arguments, calls the is_valid_identifier func
	to check if it's a valid variable name.
	calls var_in_envp to check if it already exists in p->envp, 
	if exists, frees the old value and saves the new value
	if not exists, calls ft_array_insert to add the new variable
	if no argument is given, does nothing as it's undefined behavior */
int	ft_export(t_cmd_set *p, char **args)
{
	int		ij[2];
	int		pos;

	if (ft_arr_len(args) >= 2)
	{
		ij[0] = 1;
		while (args[ij[0]])
		{
			if (!is_valid_identifier(args[ij[0]]))
				return (1);
			pos = var_in_envp(args[ij[0]], p->envp, ij);
			if (pos == 1)
			{
				if (p->envp[ij[1]])
					free(p->envp[ij[1]]);
				p->envp[ij[1]] = ft_strdup(args[ij[0]]);
			}
			else if (!pos)
				p->envp = ft_array_insert(p->envp, args[ij[0]]);
			ij[0]++;
		}
	}
	return (0);
}
