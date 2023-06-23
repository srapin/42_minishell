/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: srapin <srapin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/21 04:36:57 by hlesny            #+#    #+#             */
/*   Updated: 2023/06/23 22:38:10 by srapin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

/*
Prints the syntax error while specifying the token
Then,
	frees all data structures and returns the corresponding exit status (ie 2 for syntax error)
*/
void	display_se(t_data *data, char *token)
{
	printf("Syntax error near unexpected token `%s\'\n", token);
	free_parsing_data(data);
	/* free_tokens(first); */
	free(token);
	exit(SYNTAX_ERROR);
}

void	check_first(t_data *data, t_token_list **first)
{
	int				type;
	t_token_list	*current;

	if (!first || !*first)
		exit(EXIT_OK); // rien a free dans ce cas, si ?
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

	// //dprintf(1, "syntax\n");
	parentheses_count = 0;
	if (!data || !(data->first) || !(*(data->first)))
		return ;
	check_first(data, data->first);
	/* check pipelines one by one */
	check_pipelines(data, &parentheses_count);
	if (parentheses_count)
	{
		printf("Syntax error : Missing closing parenthesis\n");
		free_parsing_data(data);
		exit(SYNTAX_ERROR);
	}
	free_parsing_data(data);
	exit(EXIT_SUCCESS);
}

int	ft_syntax(t_data *data)
{
	pid_t	pid;
	int		wstatus;

	pid = fork();
	if (pid == -1)
		perror("fork ");
	if (pid == 0) // ie s'agit du process enfant
		check_syntax(data);
	if (waitpid(pid, &wstatus, 0) == -1)
		perror("wait ");
	return (wstatus);
}
