/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   trim_quotes.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: huaydin <huaydin@student.42vienna.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/07 18:23:55 by hakaraka          #+#    #+#             */
/*   Updated: 2023/08/16 07:46:18 by huaydin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

/* checks if there are unclosed quotes, and returns number of quotes*/
static int	check_quotes_get_len(char const *s1)
{
	int	count;
	int	i;
	int	dquote;
	int	squote;

	i = 0;
	count = 0;
	dquote = 0;
	squote = 0;
	while (s1 && s1[i])
	{
		squote = (squote + (!dquote && s1[i] == '\'')) % 2;
		dquote = (dquote + (!squote && s1[i] == '\"')) % 2;
		if ((s1[i] == '\"' && !squote) || (s1[i] == '\'' && !dquote))
			count++;
		i++;
	}
	if (squote || dquote)
		return (-1);
	return (count);
}

/* trims starting and ending quotes */
char	*remove_quotes(char const *s1, int squote, int dquote)
{
	int		i;
	int		j;
	char	*str;

	i = -1;
	j = -1;
	if (!s1 || check_quotes_get_len(s1) == -1)
		return (NULL);
	str = malloc(sizeof(char) * (ft_strlen(s1) - check_quotes_get_len(s1) + 1));
	if (!str)
		return (NULL);
	while (s1[++j])
	{
		squote = (squote + (!dquote && s1[j] == '\'')) % 2;
		dquote = (dquote + (!squote && s1[j] == '\"')) % 2;
		if ((s1[j] != '\"' || squote) && (s1[j] != '\'' || dquote)
			&& ++i >= 0)
			str[i] = s1[j];
	}
	str[++i] = '\0';
	return (str);
}
