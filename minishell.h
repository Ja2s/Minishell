/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jgavairo <jgavairo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/19 16:50:46 by jgavairo          #+#    #+#             */
/*   Updated: 2024/04/16 11:25:46 by jgavairo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <stdbool.h>
# include <string.h>
# include <readline/readline.h>
# include <readline/history.h>
# include "libft/libft.h"

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

#endif