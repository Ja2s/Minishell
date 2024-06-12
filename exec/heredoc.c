/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rasamad <rasamad@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/03 15:49:33 by jgavairo          #+#    #+#             */
/*   Updated: 2024/06/12 13:07:33 by rasamad          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	fill_heredoc(t_cmd *lst, char *line)
{
	char	*tmp;

	tmp = ft_strjoin(lst->heredoc_content, line);
	free(line);
	free(lst->heredoc_content);
	lst->heredoc_content = tmp;
	tmp = ft_strjoin(lst->heredoc_content, "\n");
	if (!tmp)
		return (-1);
	free(lst->heredoc_content);
	lst->heredoc_content = tmp;
	return (0);
}

int	line_eof(t_data *data, t_cmd *lst, int i, char **line)
{
	printf("minishell: warning: here-document delimited");
	printf(" by end-of-file (wanted `%s')\n", lst->delimiter[i]);
	(*line) = ft_strdup(lst->delimiter[i]);
	if (!(*line))
		return (exit_status(data, 1, \
		"malloc error from [ft_strdup 2] heredoc.c\n"), -1);
	return (0);
}

int	write_heredoc(t_data *data, t_cmd *lst, int i, char *line)
{
	lst->del_one = 0;
	while (ft_strcmp(line, lst->delimiter[i]) != 0)
	{
		if (lst->expand_heredoc == 1)
			line = dolls_expander(line, data);
		if (fill_heredoc(lst, line) == -1)
			return (-1);
		line = readline(">");
		if (line == NULL)
			if (line_eof(data, lst, i, &line) == -1)
				return (-1);
		if (g_sig)
			return (exit_status(data, 130, ""), free(line), -1);
	}
	free(line);
	return (0);
}

int	loop_heredoc(t_data *data, t_cmd *lst, char *line)
{
	int	i;

	i = 0;
	while (i < lst->nb_del && lst->heredoc == true)
	{
		line = readline(">");
		if (g_sig)
			return (exit_status(data, 130, ""), free(line), -1);
		if (line == NULL)
			if (line_eof(data, lst, i, &line) == -1)
				return (-1);
		if (ft_strcmp(line, lst->delimiter[i]) == 0)
			del_first(line, lst);
		else
			if (write_heredoc(data, lst, i, line) == -1)
				return (-1);
		i++;
		if (i < lst->nb_del)
			ft_free_heredoc(lst);
	}
	return (0);
}

int	ft_heredoc(t_data *data)
{
	char	*line;
	t_cmd	*lst;

	line = NULL;
	lst = data->cmd;
	lst->heredoc_content = NULL;
	lst->del_one = 0;
	rl_event_hook = rl_hook_function;
	while (lst)
	{
		if (loop_heredoc(data, lst, line) == -1)
			return (-1);
		lst = lst->next;
	}
	rl_callback_handler_remove();
	return (0);
}
