/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_echo.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: huaydin <huaydin@student.42vienna.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/07 18:23:05 by hakaraka          #+#    #+#             */
/*   Updated: 2023/08/14 21:55:35 by huaydin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

/* looks for -n if found then prints new line at the end of text otherwise not
	if there are more n i.e. -nnnnnn then ignores multiple ones */
int	ft_echo(t_list *cmd)
{
	int		print_newline;
	int		i;
	int		j;
	char	**args;

	i = 0;
	j = 0;
	print_newline = 1;
	args = ((t_cmd *)cmd->content)->args;
	while (args && args[++i])
	{
		if (!j && !ft_strncmp(args[i], "-n", 2) && (ft_countc(args[i],
					'n') == (int)(ft_strlen(args[i]) - 1)))
			print_newline = 0;
		else
		{
			j = 1;
			ft_putstr_fd(args[i], 1);
			if (args[i + 1])
				ft_putchar_fd(' ', 1);
		}
	}
	if (print_newline)
		write(1, "\n", 1);
	return (0);
}
