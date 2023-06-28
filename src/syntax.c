/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hlesny <hlesny@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/27 22:34:18 by srapin            #+#    #+#             */
/*   Updated: 2023/06/28 02:43:43 by hlesny           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
Prints the syntax error while specifying the token
Then frees all data structures and returns the 
corresponding exit status (ie 2 for syntax error)
*/

void	display_se(t_data *data, char *token)
{
	(void)data;
	printf("Syntax error near unexpected token `%s\'\n", token);
	free(token);
}

int	check_first(t_data *data, t_token_list **first)
{
	int				type;
	t_token_list	*current;

	if (!first || !*first)
		return (EXIT_OK);
	current = (*first);
	while (current && current->type == whitespace)
		current = current->next;
	if (!current)
		return (EXIT_OK);
	type = current->type;
	if (type == and_tk)
	{
		if ((*first)->length >= 2)
			display_se(data, ft_strdup("&&"));
		display_se(data, ft_strdup("&"));
		return (SYNTAX_ERROR);
	}
	else if (type == or_tk)
	{
		if ((*first)->length >= 2)
			display_se(data, ft_strdup("||"));
		display_se(data, ft_strdup("|"));
		return (SYNTAX_ERROR);
	}
	else if (type == r_parenthesis)
	{
		display_se(data, ft_strdup(")"));
		return (SYNTAX_ERROR);
	}
	return (EXIT_OK);
}

int	check_syntax(t_data *data)
{
	int	parentheses_count;

	parentheses_count = 0;
	if (!data || !(data->first) || !(*(data->first)))
		return (EXIT_OK);
	if (check_first(data, data->first))
		return (SYNTAX_ERROR);
	if (check_pipelines(data, &parentheses_count))
		return (SYNTAX_ERROR);
	if (parentheses_count)
	{
		printf("Syntax error : Missing closing parenthesis\n");
		return (SYNTAX_ERROR);
	}
	return (EXIT_SUCCESS);
}

/* int	ft_syntax(t_data *data)
{
		pid_t	pid;
		int		wstatus;

		pid = fork();
		if (pid == -1)
			perror("fork ");
		if (pid == 0)
	check_syntax(data);
		if (waitpid(pid, &wstatus, 0) == -1)
			perror("wait ");
		if (WIFEXITED(wstatus))
			return (WEXITSTATUS(wstatus));
		else if (WIFSIGNALED(wstatus))
			return (WTERMSIG(wstatus) + 128);
		else if (WIFSTOPPED(wstatus))
			return (WSTOPSIG(wstatus) + 128);
		else
	return (0);
} */
