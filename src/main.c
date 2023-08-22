/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: huaydin <huaydin@student.42vienna.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/07 18:23:05 by hakaraka          #+#    #+#             */
/*   Updated: 2023/08/22 20:26:26 by huaydin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

/* if envp is empty (env -i), set the default values */
int	allocate_env_vars_if_envp_empty(t_cmd_set *p)
{
	size_t	i;
	char	*default_env[6];

	default_env[0] = "PATH=/usr/bin:/bin";
	default_env[1] = "HOME=/";
	default_env[2] = "PWD=/";
	default_env[3] = "OLDPWD=/";
	default_env[4] = "SHLVL=1";
	default_env[5] = "_=./minishell";
	i = 0;
	while (i < 6)
	{
		p->envp[i] = ft_strdup(default_env[i]);
		if (p->envp[i] == NULL)
			return (0);
		i++;
	}
	return (1);
}

/* inits some vars in struct to NULL or 0 
	it was not mandatory to implement, but: if "env -i ./minishell"
	was ran (empty envp), then will set call allocate_env_var func */
static void	ft_init_reset(t_cmd_set *p, char **envp)
{
	p->cmds = NULL;
	p->input_text = NULL;
	g_exit_status = 0;
	p->status_code = 0;
	p->pid_of_lst_cmd = 0;
	if (envp && *envp)
		p->envp = ft_dup_array(envp);
	if (!p->envp || !(envp && *envp))
	{
		p->envp = (char **)ft_calloc(6, sizeof(char *));
		if (p->envp != NULL)
		{
			if (!allocate_env_vars_if_envp_empty(p))
			{
				ft_putendl_fd("Error: Failed to duplicate env variables", 2);
				ft_free_exit(p, 1, NULL);
			}
		}
		else
		{
			ft_putendl_fd("Error: Failed to allocate memory for p->envp", 2);
			ft_free_exit(p, 1, NULL);
		}
	}
}

/* initilizes struct, sets signal & return codes to 0,
	copies envp or Path if they are not empty
	iterates the Shlvl and sets it to 1 if not set */
static void	ft_init(t_cmd_set *p, char **envp, char **argv, int argc)
{
	char	*path;
	char	*cwd;
	char	*shlvl;
	char	*num;

	if (argc > 1 || argv[1])
		ft_putendl_fd("Arguments have been ignored", 2);
	ft_init_reset(p, envp);
	path = ft_getenv("PATH", p->envp);
	if (path == NULL)
		p->envp = ft_setenv("PATH", "/usr/bin:/bin", p->envp);
	cwd = getcwd(NULL, 0);
	if (!cwd)
		cwd = ft_strdup("/");
	p->envp = ft_setenv("PWD", cwd, p->envp);
	shlvl = ft_getenv("SHLVL", p->envp);
	if (!shlvl || ft_atoi(shlvl) <= 0 || ft_atoi(shlvl) == 2147483647)
		num = ft_strdup("1");
	else
		num = ft_itoa(ft_atoi(shlvl) + 1);
	if (num == NULL)
		num = ft_strdup("");
	p->envp = ft_setenv("SHLVL", num, p->envp);
	ft_free_all(num, cwd, shlvl, path);
}

/* if there are args, shows warning that they are ignored
	call the ft_init function
	goes input while loop to get inputs
	checks if the user inputs on terminal or tester programming is
	sending test commands via get_next_line of stdin
	i.e.  echo "ls test" | ./minishell  is accepted from tester*/
int	main(int argc, char **argv, char **envp)
{
	t_cmd_set	p;
	char		*line;

	ft_init(&p, envp, argv, argc);
	while (1)
	{
		ft_set_signals(&p);
		if (isatty(0))
			p.input_text = readline("m1n1$hell> ");
		else
		{
			p.input_text = get_next_line(0);
			if (!p.input_text)
				ft_free_exit(&p, p.status_code, NULL);
			line = p.input_text;
			p.input_text = ft_strtrim(p.input_text, "\n");
			if (p.input_text == NULL)
				ft_free_exit(&p, 1, "exit");
			ft_free_all(line, NULL, NULL, NULL);
		}
		if (!ft_process_input(p.input_text, &p))
			break ;
	}
	ft_free_exit(&p, p.status_code, NULL);
}
