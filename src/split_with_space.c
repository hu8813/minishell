/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   split_with_space.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: huaydin <huaydin@student.42vienna.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/07 18:24:03 by hakaraka          #+#    #+#             */
/*   Updated: 2023/08/19 16:50:08 by huaydin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

/* helper func for split with space, to count how many words are there */
static int	ft_count_tokens(const char *s, char *c, int i[2])
{
	int	q[2];

	q[0] = 0;
	q[1] = 0;
	while (s[i[0]] != '\0')
	{
		if (!ft_strchr(c, s[i[0]]))
		{
			i[1]++;
			while (s[i[0]] && (!ft_strchr(c, s[i[0]]) || q[0]))
			{
				if (!q[1] && s[i[0]] && (s[i[0]] == '\"' || s[i[0]] == '\''))
					q[1] = s[i[0]];
				q[0] = (q[0] + (s[i[0]] == q[1])) % 2;
				q[1] *= q[0] != 0;
				i[0]++;
			}
			if (q[0])
				return (-1);
		}
		else
			i[0]++;
	}
	return (i[1]);
}

/* helper func for split with space
	which gets substrings and put in an array */
static char	**ft_add_to_arr(char **tmpstr, char *s, char *set, int i[3])
{
	int	s_len;
	int	q[2];

	q[0] = 0;
	q[1] = 0;
	s_len = ft_strlen(s);
	while (s[i[0]])
	{
		while (s[i[0]] && ft_strchr(set, s[i[0]]) && s[i[0]] != '\0')
			i[0]++;
		i[1] = i[0];
		while (s[i[0]] && (!ft_strchr(set, s[i[0]]) || q[0] || q[1]))
		{
			q[0] = (q[0] + (!q[1] && s[i[0]] == '\'')) % 2;
			q[1] = (q[1] + (!q[0] && s[i[0]] == '\"')) % 2;
			i[0]++;
		}
		if (i[1] >= s_len)
			tmpstr[i[2]++] = "\0";
		else
			tmpstr[i[2]++] = ft_substr(s, i[1], i[0] - i[1]);
	}
	return (tmpstr);
}

/* splits with space as delimiter but only  if it's not in quotes */
char	**split_and_ignore_space_if_in_quote(char *s, char *set)
{
	char	**tmpstr;
	int		nwords;
	int		i[3];
	int		counts[2];

	i[0] = 0;
	i[1] = 0;
	i[2] = 0;
	counts[0] = 0;
	counts[1] = 0;
	if (!s)
		return (NULL);
	nwords = ft_count_tokens(s, set, counts);
	if (nwords == -1)
		return (NULL);
	tmpstr = malloc((nwords + 1) * sizeof(char *));
	if (tmpstr == NULL)
		return (put_err(NULL, "Malloc failed", 1, NULL), NULL);
	tmpstr = ft_add_to_arr(tmpstr, s, set, i);
	tmpstr[nwords] = NULL;
	return (tmpstr);
}
