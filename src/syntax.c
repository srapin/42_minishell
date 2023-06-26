/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hlesny <hlesny@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: Invalid date        by                   #+#    #+#             */
/*   Updated: 2023/06/26 18:50:04 by hlesny           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "../inc/minishell.h"

/*
Prints the syntax error while specifying the token
Then frees all data structures and returns the 
corresponding exit status (ie 2 for syntax error)
*/
void	display_se(t_data *data, char *token)
{
	printf("Syntax error near unexpected token `%s\'\n", token);
	free_parsing_data(data);
	free(token);
	exit(SYNTAX_ERROR);
}

void	check_first(t_data *data, t_token_list **first)
{
	int				type;
	t_token_list	*current;

	if (!first || !*first)
		exit(EXIT_OK);
	current = (*first);
	while (current && current->type == whitespace)
		current = current->next;
	if (!current)
		return ;
	type = current->type;
	if (type == and_tk)
	{
		if ((*first)->length >= 2)
			display_se(data, ft_strdup("&&"));
		display_se(data, ft_strdup("&"));
	}
	else if (type == or_tk)
	{
		if ((*first)->length >= 2)
			display_se(data, ft_strdup("||"));
		display_se(data, ft_strdup("|"));
	}
	else if (type == r_parenthesis)
		display_se(data, ft_strdup(")"));
}

void	check_syntax(t_data *data)
{
	int	parentheses_count;

	/* dprintf(1, "syntax\n"); */
	parentheses_count = 0;
	if (!data || !(data->first) || !(*(data->first)))
		return ;
	check_first(data, data->first);
	check_pipelines(data, &parentheses_count);
	if (parentheses_count)
	{
		printf("Syntax error : Missing closing parenthesis\n");
		free_parsing_data(data);
		exit(SYNTAX_ERROR);
	}
	free_parsing_data(data);
	//dprintf(1, "syntax\n pid = %d\n", getpid());
	exit(EXIT_SUCCESS);
}

int	ft_syntax(t_data *data)
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
		return (0); // ? b
//	return (WIFEXITED(wstatus) && WEXITSTATUS(wstatus)); // marche pas ienb a modifier
//	return (wstatus);
}
