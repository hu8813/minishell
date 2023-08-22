/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   put_errs.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: huaydin <huaydin@student.42vienna.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/07 18:24:47 by hakaraka          #+#    #+#             */
/*   Updated: 2023/08/17 16:57:45 by huaydin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

/* shows error message on stderr, sets the p->status_code to err_code */
void	*put_err(char *err_msg, char *cmd, int err_code, t_cmd_set *p)
{
	if (p && err_code >= 0)
		p->status_code = err_code;
	g_exit_status = err_code;
	if (err_msg)
	{
		if (!ft_strncmp(err_msg, "Unclosed_Quote", 14))
			ft_printf_fd(2, "mini: unexpected EOF while looking for matching");
		else if (err_msg && !ft_strncmp(err_msg, "NoFile_NoDir", 12))
			ft_printf_fd(2, "mini: %s: No such file or directory\n", cmd);
		else if (!ft_strncmp(err_msg, "Perm_Denied", 11))
			ft_printf_fd(2, "mini: %s: Permission denied\n", cmd);
		else if (!ft_strncmp(err_msg, "No_Cmd", 6) && p->status_code != 2)
			ft_printf_fd(2, "mini: %s: command not found\n", cmd);
		else if (!ft_strncmp(err_msg, "DupForkPipe_Failed", 18))
			ft_printf_fd(2, "mini: %s: dup2,fork or pipe failed\n", cmd);
		else if (!ft_strncmp(err_msg, "Empty_Pipe", 10) && p->status_code != 2)
			ft_printf_fd(2, "mini: %s: invalid null command\n", cmd);
		else if (!ft_strncmp(err_msg, "Is_Directory", 12))
			ft_printf_fd(2, "mini: %s: Is a directory\n", cmd);
		else if (!ft_strncmp(err_msg, "Not_Directory", 13))
			ft_printf_fd(2, "mini: %s: Not a directory\n", cmd);
	}
	else if (cmd && cmd[0] != '\0')
		ft_printf_fd(2, "mini: %s\n", cmd);
	return (NULL);
}
