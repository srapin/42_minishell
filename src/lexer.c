/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hlesny <hlesny@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/29 00:40:45 by Helene            #+#    #+#             */
/*   Updated: 2023/06/21 04:26:39 by hlesny           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

/*
Breaks the input down into individual words or tokens based on whitespace separation 
and handling special characters and quotes. 
The output of this phase is a stream of tokens that represent the different 
parts of the command.
*/
// tabulation sur le terminal : ctrl + v + tab

/*
The parser scans input and breaks it down to tokens. 
A token consists of one or more characters (letters, digits, symbols),
	and represents a single unit of input. 
For example, a token can be a variable name, a keyword, a number,
	or an arithmetic operator.
The parser then takes these tokens, groups them together,
	and creates an Abstract Syntax Tree, or AST.
The parser takes the AST and passes it to the executor,
	which reads the AST and executes the parsed command.
*/

/*
Get units of different tokens, defined in tokens.h
We end up with something like :
io_redirect ('<') -> word (infile) -> word (command)
-> word (arg 1) -> '|' -> ... -> '&&' -> ...

*/
/*
Les quotes ne suppriment pas les espaces ; elles permennent, entre autres,
de considérer tout ce qui est dans les quotes comme un unique mot.
Deux mots entre quotes NON séparés par un/des whitespaces ne sont en réalité qu'un seul
mot, les whitespaces étant nécessaires pour distinguer deux mots.
Et les espaces sont ce qui permet de distinguer deux mots.
Pour ce qui est de distinguer deux opérateurs ou un mot et un opérateur,
	aucun espace 
n'est nécessaire (leurs différents types d'opérateur/mot suffisent à faire la distinction)

-----------------------------------

UN WHITESPACE DANS DES QUOTES DEVIENT UN MOT (ie token <word> et non pas <whitespace>)

Si commence par lire un single quote : double quotes are treated literally within single quotes
-> lit jusqu'au deuxieme single quote

Ne peut pas avoir une paire de single quotes dans une paire de singles quotes, 
mais peut avoir une paire de double quotes dans une paire de double quotes

!!! Double quotes can be wrapped around single or double quotes. 
Single quotes have no special meaning within double quotes.
'in file "quote"' -> in file "quote"
"in file "quote"" -> in file "quote"
not allowed : single quotes inside single quotes
    'in file 'quote'' -> will be parsed as the word <in file 'quote'>, 
    PAR CONTRE : 'in file' quote'' sera parsed as TWO words : <in file> et < quote\0>

*/

/*
Si deux mots (ou deux tokens quote) ne sont séparés par rien,
	deviennent un seul mot. 
! here_doc : le limiteur peut etre entouré de quotes,
	et il faut garder en mémoire si c'est le cas ou non
Meme si le limiteur du here_doc est entre quotes,
	ne doit PAS expandre de variables (ie le(s) $ reste(nt) tel(s) quel(s))

*/

void	merge_token_quotes(t_token_list **tk_list, char *input,
		t_token *token_stream, int *i)
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
	int	i;
	int	j;
	int	current;

	i = 0;
	j = 0;
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
	int				current;
	t_token_list	*tk_list;
	t_token_list	**first;

	if (!token_stream)
		return (NULL);
	tk_list = NULL;
	first = malloc(sizeof(t_token_list *));
	if (!first)
		return (NULL);
	merge_single_tokens(&tk_list, input, token_stream, stream_len);
		// ajouté chez oim le mardi 20 en fin d'aprem et ca leak
	/* while (i < stream_len)
    {
        
        current = token_stream[i].type;
         if (i < stream_len && (current == r_parenthesis
			|| current == l_parenthesis))
         {
             tk_add(&tk_list, tk_new_elem(&input[i], 1, current, 0));
             i++;
         }
         else if (i < stream_len && (current == simple_quote
			|| current == double_quote))
         {
             j = i;
             i++;
             while (i < stream_len && token_stream[i].type != current)
                 i++;
             if (i < stream_len)
                 i++;
             tk_add(&tk_list, tk_new_elem(&input[j], i - j, current, 
                 (current == simple_quote) + (current == double_quote) * 2));
         }
         else if (i < stream_len)
         {
             j = i;
             while (i < stream_len && token_stream[i].type == current)
                 i++;
             tk_add(&tk_list, tk_new_elem(&input[j], i - j, current, 0));
         }
    } */
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
	int		i;
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
