/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jgavairo <jgavairo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/19 16:50:46 by jgavairo          #+#    #+#             */
/*   Updated: 2024/04/16 17:56:58 by jgavairo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <stdlib.h>
# include <string.h>
# include <readline/readline.h>
# include <readline/history.h>
# include "libft/libft.h"
# include <fcntl.h>
# include <stdio.h>
# include <unistd.h>
# include <errno.h>
# include <sys/wait.h>
# include <stdbool.h>

typedef struct s_expand
{
	int		name_start;
	int		name_end;
	int		name_len;
	char	*name;
	int		value_len;
	char	*value;
}			t_expand;

typedef struct s_var
{
	char	*rl;
	char	*pwd;
	char	**input;
	char	**mini_env;
	int		len_env;
	int		i;
}			t_var;

typedef struct s_struct
{
	int		pipe_fd[2];
	int 	save_pipe;
}    		t_struct; 

typedef struct s_cmd
{
	int				nb_flags;
	int				nb_args;
	int				nb_red;
	int				nb_del;
	int				heredoc;
	char			**delimiter;
	char			**redirecter;
	char			**args;
	struct s_cmd	*next;
	struct s_cmd	*start;
	int				open;
	int				fd_infile;
	int				fd_outfile;
	char			**split_path;
	char			*slash_cmd;
	char			*path_cmd;
	int				i;
}					t_cmd;

t_cmd	*ft_lstnew_minishell(void);
t_cmd	*ft_lstlast_minishell(t_cmd *lst);
void    ft_lstadd_back_minishell(t_cmd **lst, t_cmd *new);
int		ft_lstlen(t_cmd *elem);
void    ft_redirecter(t_cmd *elem);
int		ft_check_access(t_cmd *elem, char **envp);
int		ft_first_fork(t_cmd *elem, t_struct *var, char **envp);
int		ft_middle_fork(t_cmd *elem, t_struct *var, char **envp);
int		ft_last_fork(t_cmd *elem, t_struct *var, char **envp);
void    display_error_cmd(t_cmd *elem);
void    display_no_such(t_cmd *elem);
void    ft_free_access(t_cmd *elem);
void    ft_free_token(t_cmd *elem);
void    ft_free_lst(t_cmd *lst);
void    ft_close(t_cmd *elem);

#endif