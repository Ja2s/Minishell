/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jgavairo <jgavairo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/29 14:07:18 by jgavairo          #+#    #+#             */
/*   Updated: 2024/06/03 14:39:15 by jgavairo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <stdlib.h>
# include <string.h>
# include <readline/readline.h>
# include <readline/history.h>
# include "../libft/libft.h"
# include <fcntl.h>
# include <stdio.h>
# include <unistd.h>
# include <errno.h>
# include <sys/wait.h>
# include <stdbool.h>
# include <signal.h>
# include <sys/stat.h>

extern volatile sig_atomic_t g_sig;

typedef struct s_expand
{
	int		name_start;
	int		name_end;
	int		name_len;
	char	*name;
	int		value_len;
	char	*value;
	int 	nb_numbers;
	int		code_copy;
	int		in_cote;
	bool	in_redirecter;
	char	*output;
	int		p;
	int		i;
	int		pos_doll;
}			t_expand;

typedef struct s_var
{
	char	*rl;
	char	*pwd;
	char	**input;
	char	**pipes;
	char	**input_copy;
	char	**mini_env;
	int		len_env;
	int		i;
}			t_var;

typedef struct s_env
{
	char			*name;
	char			*value;
	struct	s_env	*next;
}					t_env;

typedef struct s_cmd
{
	int				nb_flags;
	int				nb_args;
	int				nb_red;
	int				nb_del;
	
	int				heredoc;
	int				expand_heredoc;
	int				end_heredoc;
	int				fd_str_rand;
	int				del_one;
	char			**heredoc_content;
	char			**delimiter;
	char			**redirecter;
	char			**red_copy;
	char			**args;
	struct s_cmd	*next;
	struct s_cmd	*start;
	int				fd_infile;
	int				fd_outfile;
	char			**split_path;
	char			*slash_cmd;
	char			*path_cmd;
	int				i;
}					t_cmd;

typedef struct s_data
{
	t_env		*mini_env;
	t_cmd		*cmd;
	t_cmd		*tmp;
	t_var		var;
	int			pipe_fd[2];
	int			save_pipe;
	int			exit_code;
	int			ambigous;
}			t_data;

typedef struct s_int
{
	int			start;
	int			x;
	int			p;
	int			i;
	int			len;
}			t_int;

t_cmd		*ft_lstnew_minishell(void);
t_cmd		*ft_lstlast_minishell(t_cmd *lst);
void    	ft_lstadd_back_minishell(t_cmd **lst, t_cmd *new);
int			ft_lstlen(t_cmd *elem);
int			ft_redirecter(t_data *data);
int			ft_check_access(t_data *data, t_cmd *lst);
int			ft_first_fork(t_data *data, t_cmd *lst);
int			ft_middle_fork(t_data *data, t_cmd *lst);
int			ft_last_fork(t_data *data, t_cmd *lst);
void    	display_error_cmd(t_cmd *elem);
void    	display_no_such(char *str);
void    	ft_free_access(t_cmd *elem);
void    	ft_free_token(t_cmd *elem);
void    	ft_free_lst(t_cmd *lst);
void    	ft_close(t_cmd *elem);
int			rafters_checker(char *rl);
int			double_pipe_checker(char *rl);
int			cote_checker(char *rl);
int			syntaxe_error(t_data *data, char *rl);
t_cmd		*ft_lstlast(t_cmd *lst);
void		args_free(t_cmd *lst);
void		red_free(t_cmd *lst);
void		del_free(t_cmd *lst);
void		split_path_free(t_cmd *lst);
void		path_cmd_free(t_cmd *lst);
void		slash_cmd_free(t_cmd *lst);
void		ft_lstdelone(t_cmd *lst);
void		ft_lstclear(t_cmd **lst);
int			ft_lstlen(t_cmd *elem);
void		command_stocker(char **input, t_cmd **cmd);
int			args_memory_alloc(char **input, t_cmd **cmd);
int			memory_alloc(char **input, t_cmd **cmd);
int			stock_input(char **input, t_cmd **cmd);
void		negative_checker(char *rl);
int			expand_initializer(t_expand **var);
char		*dolls_expander(char *rl, t_data *data);
int			redirect_counter(char *pipes);
int			redirecter(char *pipes, t_cmd **cmd);
int			len_calculator(char	*pipes);
char		*redirect_deleter(char	*pipes);
int			ft_printf_struct(t_cmd *cmd);
void		printf_title();
int			pipes_counter(char *rl);
int			env_copyer(char **envp, t_env **mini_env);
int			heredoc_counter(char *pipes);
int			heredoc_memory_allocer(char *pipes, t_cmd **cmd);
int			heredoc_copyer(char *pipes, t_cmd **cmd, int i, int del);
int			heredoc_checker(char *pipes, t_cmd **cmd);
void		free_pipes(char **pipes);
void		command_positiver(char *pipes);
t_env		*env_new(void);
void		env_cmd(t_env *env);
char		*ft_getenv(char *name, t_env *mini_env);
char		*line_extractor(t_env *mini_env);
int			ft_envsize(t_env *mini_env);
char		**ft_list_to_tab(t_env *mini_env);
int			ft_builtins(t_cmd *lst);
int			ft_heredoc(t_data *data);
void		ft_display_heredoc(t_cmd *lst);
int			minishell_starter(char **env, t_data *data);
int 		prompt_customer(t_data *data);
int			parser(t_data *data);
int			node_precreator(t_data *data, int i);
int 		node_creator(t_data *data);
int			final_parse(t_data *data);
void		command_positiver(char *pipes);
char		*copy_w_cote(char *src, char *dest);
char		**input_copyer(char **input, char **input_copy);
void		data_initializer(t_data *data);
int			launch_exec(t_data *data);
int			rafter_checker_one(char *rl, int p);
int			rafter_checker_two(char *rl, int p);
int			starter_pipe(char *rl);
void		exit_status(t_data *data, int code, char *message);
t_env		*ft_envlast(t_env *lst);
void		ft_envadd_back(t_env **env, t_env *new);
int			ft_export(t_data *data, t_env **mini_env, t_cmd *cmd);
char		**ft_list_to_tab_cote(t_env *mini_env);
int			ft_isspace(char c);
void		exit_status_n_free(t_data *data, int code, char *message);
int			spec_export(char *cmd);
void		ft_unset(t_data **data);
int			ft_cd(t_data *data);
void		ft_free_heredoc(t_data *data);
void		ft_free_all_heredoc(t_data *begin);
int			if_condition_expand(t_expand *var, int choice);
char		*value_extractor(char *env);
char		*name_extractor(char *env);
char		*env_extractor(char	*env, int choice);
int			doll_heredoc_helper(char **rl, int i);
void		doll_heredoc(char **rl);
void		double_negativer(int i, char **rl);
void		simple_negativer(int i, char **rl);
char		*del_doll(char *output, int i);
void		negative_checker_sp(char **rl);
void		in_cote_checker(t_expand **var, char *output, int i);
void		in_redirection_checker(t_expand **var, char *output, int i);
int			space_in_value_checker(t_expand *var);
int			exitcode_expander(t_expand **var, t_data **data, char *rl);
void		expand_nd(t_expand **var);
int			skip_cote(char *rl, int i, int choice);
int			redirecter_finisher(t_data *data);
int			condition_n_one(t_expand *var);
int			condition_n_two(t_expand *var);
void		end_heredoc(char *pipes, t_cmd **cmd);
void		init_var(t_int *var);
int			redirecter_helper(char *pipes, t_cmd **cmd, t_int *var);
int			negative_checker_variable(t_expand **var, t_data **data);
int			check_variable(t_env **mini_env, char *name, char *value);
void		display_is_dir(t_cmd *lst);
int			ft_is_builtins_no_access(t_cmd *lst);



#endif