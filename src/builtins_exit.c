/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_exit.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: huaydin <huaydin@student.42vienna.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/22 16:02:29 by hakaraka          #+#    #+#             */
/*   Updated: 2023/08/19 20:44:49 by huaydin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

/* frees the struct and linked list, prints a msg if given as parameter
	exits with given exit_code as parameter */
void	ft_free_exit(t_cmd_set *p, int exit_code, char *msg)
{
	if (msg)
		ft_putendl_fd(msg, 2);
	if (p && p->input_text != NULL)
		free(p->input_text);
	if (p && p->envp && *p->envp)
		ft_free_array(&p->envp);
	if (p && p->cmds)
		ft_lstclear(&p->cmds, ft_lst_free);
	exit(exit_code);
}

/* exit if there is no argument, else check if arg is number not
	if 1st argument is not numberic shows
	numeric arg required and exits
	if 1st is numeric and there are more than 1 arguments then
	shows too many arguments and does not exit! */
void	ft_exit(t_list *cmd, int *is_exit, t_cmd_set *p)
{
	long long int	status[2];
	char			**args;

	args = ((t_cmd *)cmd->content)->args;
	*is_exit = !cmd->next;
	if (*is_exit)
		printf("exit\n");
	if (!args || !args[1])
		ft_free_exit(p, 0, NULL);
	status[1] = ft_atoi(args[1]);
	if (args[1][0] == '\0' || (status[1] < 0 && ft_strlen(args[1]) > 18)
		|| (status[1] == 0 && args[1][0] != '0'))
	{
		*is_exit = 1;
		ft_putstr_fd("mini: exit: ", 2);
		ft_putstr_fd(args[1], 2);
		ft_free_exit(p, (2 + status[1]) & 255, ": numeric argument required");
	}
	else if (args[2])
	{
		*is_exit = 0;
		put_err(NULL, "mini: exit: too many arguments", 1, 0);
	}
	if (*is_exit)
		ft_free_exit(p, status[1] & 255, NULL);
}
