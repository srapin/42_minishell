/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: srapin <srapin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/27 22:34:18 by srapin            #+#    #+#             */
/*   Updated: 2023/06/28 13:12:46 by srapin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
Prints the syntax error while specifying the token
Then frees all data structures and returns the 
corresponding exit status (ie 2 for syntax error)
*/

void	display_custom_se(bool quotes)
{
	if (quotes == false)
	{
		if (write(STDERR_FILENO,
				"Syntax error : Missing closing parenthesis\n", 43) == -1)
			perror("write");
	}
	else
	{
		if (write(STDERR_FILENO,
				"Syntax error : Missing closing quote\n", 37) == -1)
			perror("write");
	}
}

void	display_se(t_data *data, char *token)
{
	char	*mess;
	char	*tmp;

	(void)data;
	tmp = ft_strjoin("Syntax error near unexpected token `", token);
	mess = ft_strjoin(tmp, "\'\n");
	if (write(STDERR_FILENO, mess, ft_strlen(mess)) == -1)
		perror("write");
	free(tmp);
	free(mess);
	free(token);
}

int	check_first_and_or(t_data *data, t_token_list *current)
{
	int				type;

	type = current->type;
	if (type == and_tk)
	{
		if (current->length >= 2)
			display_se(data, ft_strdup("&&"));
		else
			display_se(data, ft_strdup("&"));
		return (SYNTAX_ERROR);
	}
	else if (type == or_tk)
	{
		if (current->length >= 2)
			display_se(data, ft_strdup("||"));
		else
			display_se(data, ft_strdup("|"));
		return (SYNTAX_ERROR);
	}
	return (0);
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
	if (check_first_and_or(data, current))
		return (SYNTAX_ERROR);
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
		display_custom_se(false);
		return (SYNTAX_ERROR);
	}
	return (EXIT_SUCCESS);
}
