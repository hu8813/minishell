/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_array.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: huaydin <huaydin@student.42vienna.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/22 16:19:18 by hakaraka          #+#    #+#             */
/*   Updated: 2023/08/21 03:04:12 by huaydin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

/* to print string array, (used in env cmd)*/
void	ft_printarray(char **m)
{
	int	i;

	i = 0;
	while (m && m[i])
	{
		ft_putendl_fd(m[i], 1);
		i++;
	}
}

/* returns the lenght of a string array */
int	ft_arr_len(char **m)
{
	int	i;

	i = 0;
	if (!m)
		return (i);
	while (m && m[i])
		i++;
	return (i);
}

/* makes copy of a string array (to copy envp in struct) */
char	**ft_dup_array(char **arr_ptr)
{
	char	**arr_copy;
	int		n_rows;
	int		i;

	i = 0;
	n_rows = ft_arr_len(arr_ptr);
	arr_copy = malloc(sizeof(char *) * (n_rows + 1));
	if (!arr_copy)
		return (NULL);
	while (arr_ptr[i])
	{
		arr_copy[i] = ft_strdup(arr_ptr[i]);
		if (!arr_copy[i])
		{
			ft_free_array(&arr_copy);
			return (NULL);
		}
		i++;
	}
	arr_copy[i] = NULL;
	return (arr_copy);
}

/* to add a new string into an array of strings
	i.e. to add a new variable into the envp */
char	**ft_array_insert(char **in, char *newstr)
{
	char	**out;
	int		len;
	int		i;

	i = -1;
	out = NULL;
	if (!newstr)
		return (in);
	len = ft_arr_len(in);
	out = ft_calloc(1, sizeof(char *) * (len + 2));
	if (!out)
		return (put_err(NULL, "Malloc failed", 1, NULL));
	out[len + 1] = NULL;
	while (++i < len)
	{
		out[i] = ft_strdup(in[i]);
		if (!out[i])
		{
			ft_free_array(&in);
			ft_free_array(&out);
		}
	}
	out[i] = ft_strdup(newstr);
	ft_free_array(&in);
	return (out);
}

/* to replace a string in an array of strings 
	i.e. to change value of a variable in envp */
char	**ft_array_replace(char ***big, char **small, int n)
{
	char	**tmpstr;
	int		i[3];

	i[0] = -1;
	i[1] = -1;
	i[2] = -1;
	if (!big || !*big || n < 0 || n >= ft_arr_len(*big))
		return (NULL);
	tmpstr = ft_calloc(ft_arr_len(*big) + ft_arr_len(small), sizeof(char *));
	if (!tmpstr)
		return (put_err(NULL, "Malloc failed", 1, NULL));
	while (tmpstr && big[0][++i[0]])
	{
		if (i[0] != n)
			tmpstr[++i[2]] = ft_strdup(big[0][i[0]]);
		else
		{
			while (small && small[++i[1]])
				tmpstr[++i[2]] = ft_strdup(small[i[1]]);
		}
	}
	ft_free_array(big);
	*big = tmpstr;
	return (*big);
}
