/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: srapin <srapin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/29 00:40:45 by Helene            #+#    #+#             */
/*   Updated: 2023/06/28 13:12:46 by srapin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	merge_token_quotes(t_token_list **tk_list, char *input,
		t_token *token_stream, size_t *i)
{
	int		j;
	int		current;
	size_t	stream_len;

	stream_len = ft_strlen(input);
	current = token_stream[*i].type;
	j = *i;
	(*i)++;
	while ((*i) < stream_len && token_stream[*i].type != current)
		(*i)++;
	if ((*i) < stream_len)
		(*i)++;
	tk_add(tk_list, tk_new_elem(&input[j], (*i) - j, current,
			(current == simple_quote) + (current == double_quote) * 2));
}

void	merge_single_tokens(t_token_list **tk_list, char *input,
		t_token *token_stream, size_t stream_len)
{
	size_t	i;
	int		j;
	int		current;

	i = 0;
	while (i < stream_len)
	{
		current = token_stream[i].type;
		if (i < stream_len && (current == r_parenthesis
				|| current == l_parenthesis))
		{
			tk_add(tk_list, tk_new_elem(&input[i], 1, current, 0));
			i++;
		}
		else if (i < stream_len && (current == simple_quote
				|| current == double_quote))
			merge_token_quotes(tk_list, input, token_stream, &i);
		else if (i < stream_len)
		{
			j = i;
			while (i < stream_len && token_stream[i].type == current)
				i++;
			tk_add(tk_list, tk_new_elem(&input[j], i - j, current, 0));
		}
	}
}

t_token_list	**tokenise(t_token *token_stream, size_t stream_len,
		char *input)
{
	t_token_list	*tk_list;
	t_token_list	**first;

	if (!token_stream)
		return (NULL);
	tk_list = NULL;
	first = malloc(sizeof(t_token_list *));
	if (!first)
		return (NULL);
	merge_single_tokens(&tk_list, input, token_stream, stream_len);
	free(token_stream);
	token_stream = NULL;
	*first = tk_list;
	return (first);
}

int	set_token_operator(t_token *token, char input)
{
	if (input == '<')
		token->type = l_io_redirect;
	else if (input == '>')
		token->type = r_io_redirect;
	else if (input == '(')
		token->type = l_parenthesis;
	else if (input == ')')
		token->type = r_parenthesis;
	else if (input == '&')
		token->type = and_tk;
	else if (input == '|')
		token->type = or_tk;
	else if (input == '\'')
		token->type = simple_quote;
	else if (input == '\"')
		token->type = double_quote;
	else
		return (0);
	return (1);
}

t_token	*assign_type(char *input, size_t stream_len)
{
	size_t	i;
	t_token	*token_stream;

	i = 0;
	token_stream = ft_calloc(sizeof(t_token), stream_len + 1);
	if (!token_stream)
		return (NULL);
	while (i < stream_len)
	{
		if (ft_strchr(WHITESPACES, input[i]))
			token_stream[i].type = whitespace;
		else if (set_token_operator(&token_stream[i], input[i]))
			;
		else
			token_stream[i].type = word;
		token_stream[i].content = input[i];
		i++;
	}
	return (token_stream);
}
