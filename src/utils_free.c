/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_free.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: huaydin <huaydin@student.42vienna.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/22 17:21:07 by hakaraka          #+#    #+#             */
/*   Updated: 2023/08/21 16:53:23 by huaydin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

/* frees the content in the linked list, which are args, cmd_path
	closes the file descriptors if not -1 */
void	ft_lst_free(void *content)
{
	t_cmd	*node;

	node = content;
	ft_free_array(&node->args);
	if (node->cmd_path)
		free(node->cmd_path);
	if (node->in_fd != 0 && node->in_fd != -1)
		close(node->in_fd);
	if (node->out_fd && node->out_fd != 1 && node->out_fd != -1)
		close(node->out_fd);
	if (node)
		free(node);
}

/* gets a string array as parameter and frees all strings in it */
void	ft_free_array(char ***m)
{
	int	i;

	i = 0;
	if (!m || !*m || !**m)
		return ;
	while (m && m[0] && m[0][i])
	{
		if (m[0][i])
		{
			free(m[0][i]);
			m[0][i] = NULL;
		}
		i++;
	}
	if (m && m[0])
	{
		free(m[0]);
		*m = NULL;
	}
}

/* gets strings as parameter and frees them if not NULL
	all 3 args are optional, args can be 1, 2, 3 or 4 strings */
void	ft_free_all(char *s1, char *s2, char *s3, char *s4)
{
	if (s1 != NULL)
		free(s1);
	if (s2 != NULL)
		free(s2);
	if (s3 != NULL)
		free(s3);
	if (s4 != NULL)
		free(s4);
}

/* frees the tmp array, called when a pipe redir error found */
t_list	*free_tmp_lst(t_list *cmds, char **args, char **temp)
{
	ft_lstclear(&cmds, ft_lst_free);
	ft_free_array(&temp);
	ft_free_array(&args);
	return (NULL);
}
