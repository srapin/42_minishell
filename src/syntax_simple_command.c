/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax_simple_command.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hlesny <hlesny@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/23 15:17:34 by Helene            #+#    #+#             */
/*   Updated: 2023/06/27 17:58:32 by hlesny           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"


/* Les tokens avec des '' ou "" doivent avoir le type word,
	et le content "\0" !!! */
void	check_io_redirect(t_data *data, t_token_list **op)
{
	t_token_list	*current;

	if ((*op)->length > 2)
		display_se(data, ft_substr((*op)->content, 2, 2));
	// garbage collector ? permettrait d'y ajouter le malloc de ft_substr et reglerait le probleme de ou peut le free
	current = (*op)->next;
	while (current && current->type == whitespace)
		current = current->next;
	if (!current)
		// cas ou rien ne suit : ie syntax error near unexpected newline
		display_se(data, ft_strdup("newline"));
	else if (current->type != word && current->type != simple_quote
			&& current->type != double_quote)
		display_se(data, ft_substr(current->content, 0, 2));
}

void	sc_check_quotes(t_data *data, t_token_list **current)
{
	if ((*current)->type == simple_quote
		&& ((*current)->content[(*current)->length - 1] != '\''
			|| (*current)->length == 1))
	{
		printf("Syntax error : Missing closing quote\n");
		free_parsing_data(data);
		exit(SYNTAX_ERROR);
	}
	else if ((*current)->type == double_quote
			&& ((*current)->content[(*current)->length - 1] != '\"'
				|| (*current)->length == 1))
	{
		printf("Syntax error : Missing closing quote\n");
		free_parsing_data(data);
		exit(SYNTAX_ERROR);
	}
}

void	sc_check_parentheses(t_data *data, t_token_list **current,
		int *parentheses_count)
{
	if ((*current)->type == l_parenthesis)
	{
		(*parentheses_count)++;
		*current = (*current)->next;
		while (*current && (*current)->type == whitespace)
			*current = (*current)->next;
		if (*current && (*current)->type == r_parenthesis)
			display_se(data, ft_strdup(")"));
		else if (*current && (*current)->type == l_parenthesis)
			(*parentheses_count)++;
		else if (!(*current))
		{
			printf("Syntax error : Missing closing parenthesis\n");
			free_parsing_data(data);
			exit(SYNTAX_ERROR);	
		}
	}
	else if ((*current)->type == r_parenthesis)
	{
		if (!(*parentheses_count))
			display_se(data, ft_strdup(")"));
		(*parentheses_count)--;
	}
}

void	check_simple_command(t_data *data, t_token_list **current,
		int *parentheses_count)
{
	while ((*current) && (*current)->type != and_tk
		&& (*current)->type != or_tk)
	{
		if ((*current)->type == l_io_redirect
			|| (*current)->type == r_io_redirect)
			check_io_redirect(data, current);
		else if ((*current)->type == l_parenthesis
				|| (*current)->type == r_parenthesis)
			sc_check_parentheses(data, current, parentheses_count);
		else if ((*current)->type == simple_quote
				|| (*current)->type == double_quote)
			sc_check_quotes(data, current);
		*current = (*current)->next;
	}
}