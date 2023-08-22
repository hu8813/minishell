/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: huaydin <huaydin@student.42vienna.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/07 18:25:21 by hakaraka          #+#    #+#             */
/*   Updated: 2023/08/22 20:25:39 by huaydin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

/* Buffer_Size for the Get_next_line, as the Tester uses Gnl to try test cmds*/
# define BUFFER_SIZE 100

/* String and List Functions: 
	ft_strncmp, ft_putendl_fd, ft_putstr_fd, t_list, ft_lstadd_back,
	ft_lstadd_front, ft_lstclear, ft_lstsize, ft_lstlast, ft_split */
# include "../libft/libft.h"

/* Directory Functions: opendir, closedir */
# include <dirent.h>            

/* File Manipulation Functions: open (O_CREAT, O_WRONLY, O_APPEND) */
# include <fcntl.h>             

/* Signal Handling Functions: signal (SIGINT, SIGQUIT) */
# include <signal.h>            

/* Standard I/O Functions: printf */
# include <stdio.h>             

/* I/O Control Functions: ioctl (TIOCSTI) */
# include <sys/ioctl.h>         

/* Process Wait and Exit Functions: waitpid (WIFEXITED->WEXITSTATUS,
    WIFSIGNALED->WTERMSIG) */
# include <sys/wait.h>          

/* Readline Functions: readline 
    (rl_on_new_line, rl_redisplay, rl_replace_line) */
# include <readline/readline.h>

/* Readline History Functions: add_history */
# include <readline/history.h> 

# include <stdarg.h> 

/* return code of signal Ctrl+C Ctrl+\ Ctrl+D */
extern int	g_exit_status;

/* struct which are saved in the content of a linked list for each command
    cmd options(args), cmd_path, in_fd, output_pd */
typedef struct s_cmd
{
	char	**args;
	char	*cmd_path;
	int		in_fd;
	int		out_fd;
}			t_cmd;

/* struct with envp, status_code and pointer to linked list of cmds */
typedef struct s_cmd_set
{
	t_list	*cmds;
	char	**envp;
	char	*input_text;
	int		status_code;
	pid_t	pid_of_lst_cmd;
}			t_cmd_set;

/* Error Handling */

void	*put_err(char *err_type, char *param, int err, t_cmd_set *p);

/* Signal Handling */

void	signals_parent(int signal_code);
void	signals_child(int signal_code);
void	signals_heredoc(int signal_code);
void	ft_set_signals(t_cmd_set *p);

/* String and Array Manipulation */

int		ft_countc(char *s, char c);
int		ft_arr_len(char **m);
int		ft_strchrs_idx(const char *s, char *set);
int		ft_strchr_idx(const char *s, int c);
char	**ft_dup_array(char **m);
char	**ft_array_insert(char **in, char *newstr);
void	ft_free_array(char ***m);
void	ft_printarray(char **m);
char	**ft_array_replace(char ***big, char **small, int n);
char	**split_and_ignore_space_if_in_quote(char *s, char *set);
char	*remove_quotes(char const *s1, int squote, int dquote);
char	*var_expander(char *str, int quotes[2], t_cmd_set *p);
char	*ft_getenv(char *var, char **envp);
char	**ft_setenv(char *var, char *value, char **envp);
char	*ft_strjoin3(char *s1, char *s2, char *s3);
void	check_home_pwd_oldpwd(t_cmd_set *p);
int		ft_printf_fd(int fd, char *format, ...);
t_list	*free_tmp_lst(t_list *cmds, char **args, char **temp);

/* Built-in Commands */

int		cd_export_unset_exit(t_cmd_set *p, t_list *cmd, int *is_exit, int n);
int		is_builtin(t_cmd *n);
int		ft_cd(t_cmd_set *p, char **args);
int		ft_pwd(void);
int		ft_echo(t_list *cmd);
int		ft_export(t_cmd_set *p, char **args);
int		ft_unset(t_cmd_set *p, char **args);
void	ft_exit(t_list *cmd, int *is_exit, t_cmd_set *p);
void	ft_free_exit(t_cmd_set *p, int exit_code, char *msg);

/* Command Processing and Execution */

void	*ft_process_input(char *out, t_cmd_set *p);
void	find_cmd_path(t_cmd_set *p, t_list *cmd, char **s, char *path);
void	*chk_perm_call_child(t_cmd_set *p, t_list *cmd, int fd[2]);
void	*parse_nodes(char **args, t_cmd_set *p);
char	**split_with_special_chars(char **args);
t_cmd	*out_fd_truncate(t_cmd *node, char **args, int *i, t_cmd_set *p);
t_cmd	*out_fd_append(t_cmd *node, char **args, int *i, t_cmd_set *p);
t_cmd	*in_fd_read(t_cmd *node, char **args, int *i, t_cmd_set *p);
t_cmd	*in_fd_heredoc(t_cmd *node, char **args, int *i, t_cmd_set *p);
void	handle_env_vars(char *str, int *i, int fd[2], t_cmd_set *p);
void	ft_lst_free(void *content);
void	ft_free_all(char *s1, char *s2, char *s3, char *s4);
char	*get_next_line(int fd);
void	exec_cmd_and_wait(t_cmd_set *p, int status, int tmp[2], int *is_exit);
void	pwd_echo_env_and_external(t_cmd_set *p, t_cmd *n, int l, t_list *cmd);
t_cmd	*init_cmd(void);

#endif
