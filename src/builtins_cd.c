/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_cd.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: huaydin <huaydin@student.42vienna.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/07 18:23:05 by hakaraka          #+#    #+#             */
/*   Updated: 2023/08/21 03:33:46 by huaydin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

/* if a directory is given as parameter tries to open directory with opendir
	if the parameter doesn't exist, shows error message: No such File or Dir
	if it's a File, shows error message: not a Directory
	if no parameter is given and home is not set, shows error: Home not set
	cd ~ or cd - are not mandatory, but we tried to implement	*/
void	ft_chdir(char ***av, char *home, char *oldpwd, t_cmd_set *p)
{
	DIR	*dir;

	dir = NULL;
	if (!ft_strncmp(av[0][0], "cd", ft_strlen(av[0][0])) && av[0][1])
		dir = opendir(av[0][1]);
	if (home && !home[0] && !dir)
		put_err(NULL, "mini: HOME not set", 1, p);
	else if (home && ((!dir && !av[0][1]) || (av[0][1] && av[0][1][0]
		&& av[0][1][0] == '~')))
		p->status_code = chdir(home) == -1;
	else if (oldpwd && (!av[0][1] || (av[0][1] && av[0][1][0]
		&& av[0][1][0] == '-')))
		p->status_code = chdir(oldpwd) == -1;
	else if (av[0][1] && dir && access(av[0][1], F_OK) != -1)
		chdir(av[0][1]);
	else if (!ft_strncmp(av[0][0], "cd", ft_strlen(av[0][0])) && av[0][1]
		&& access(av[0][1], F_OK) == -1)
		put_err("NoFile_NoDir", av[0][1], 1, p);
	else if (!ft_strncmp(av[0][0], "cd", ft_strlen(av[0][0]))
		&& !dir && av[0][1])
		put_err("Not_Directory", av[0][1], 1, p);
	if (dir)
		closedir(dir);
}

/* ensure that PWD and OLDPWD exist in the env.
	first it gets the current WD and the variables.
	if one of the variables is not set, it sets to the current directory */
void	check_home_pwd_oldpwd(t_cmd_set *p)
{
	char	*pwd;
	char	*oldpwd;
	char	*cwd;

	cwd = getcwd(NULL, 0);
	pwd = ft_getenv("PWD", p->envp);
	oldpwd = ft_getenv("OLDPWD", p->envp);
	if (!pwd)
		p->envp = ft_setenv("PWD", cwd, p->envp);
	if (!oldpwd)
		p->envp = ft_setenv("OLDPWD", cwd, p->envp);
	ft_free_all(pwd, oldpwd, cwd, NULL);
}

/* this is the builtin that is actually called.
	it verifies, that there is only one argument.
	it ensures that PWD and OLDPWD are set in the env.
	it ensures that at least "" home and oldpwd exist.
	it uses ft_chdir to do the actual change of directory.
	then it sets the PWD, frees memory, and returns. */
int	ft_cd(t_cmd_set *p, char **cmd_args)
{
	char	*home;
	char	*oldpwd;
	char	*newpwd;

	p->status_code = 0;
	if (cmd_args[1] && cmd_args[2])
		return (put_err(NULL, "cd: too many arguments", 1, p), 1);
	check_home_pwd_oldpwd(p);
	home = ft_getenv("HOME", p->envp);
	if (!home)
		home = ft_strdup("");
	oldpwd = ft_getenv("OLDPWD", p->envp);
	if (!oldpwd)
		oldpwd = ft_strdup("");
	newpwd = getcwd(NULL, 0);
	ft_chdir(&cmd_args, home, oldpwd, p);
	p->envp = ft_setenv("OLDPWD", newpwd, p->envp);
	ft_free_all(newpwd, NULL, NULL, NULL);
	newpwd = getcwd(NULL, 0);
	if (!newpwd)
		newpwd = ft_strdup("");
	p->envp = ft_setenv("PWD", newpwd, p->envp);
	ft_free_all(home, oldpwd, newpwd, NULL);
	return (p->status_code);
}
