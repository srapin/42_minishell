/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hlesny <hlesny@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/03 02:12:27 by Helene            #+#    #+#             */
/*   Updated: 2023/06/27 22:08:08 by hlesny           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
Parser : part of our Command Line Interpreter that calls 
the lexical scanner to retrieve tokens.
It then constructs an Abstract Syntax Tree, or AST, out of these tokens. 
This AST is what we’ll pass to the executor to be executed.
*/

// ((cat out) && ls | grep "c" | (wc -l)) || echo "hello world"
// env | grep "a"$test | wc -l && ls

/*
Ensures the command abides to the following syntax rules :
    - No '|' at the very beginning or end of the command line,
	and of each command list (ie check between () as well)
    - No "&&" or "||" at the very beginning or end of the command line,
	and of each command list (ie check between () as well)
    - No more than two consecutives '<' or '>'
    - For '<' and '>' symbols : if what follows isn't a word,
	a dollar or an asterisk : syntax error
    
Check for unclosed parentheses
Check for unclosed quotes. 

((cat out) && ls | (wc -l)) || echo "hello world" 
*/

void	unlink_files(t_data *data)
{
	t_list	*lst;

	lst = data->files;
	while (lst)
	{
		unlink(lst->content);
		lst = lst->next;
	}
	ft_lstfree(&(data->files), free);
}

int	check_for_syntax(t_data *data)
{
	int		wstatus;

	wstatus = ft_syntax(data);
	if (wstatus)
	{
		g_exit_status = wstatus % 256;
		free_tokens(data->first);
		data->first = NULL;
		return (0);
	}
	return (1);
}

t_cmd	*parse_and_execute(t_data *data)
{
	t_cmd	*ast;

	ast = NULL;
	if (!check_for_syntax(data))
		return (NULL);
	perform_variable_exp(data);
	if (!(*data->first))
	{
		free(data->first);
		data->first = NULL;
		return (NULL);
	}
	delete_quotes(data);
	group_words(data);
	perform_wildcard_exp(data);
	if (set_here_docs(data))
		ast = get_ast(data);
	free_tokens(data->first);
	data->first = NULL;
	exec_cmds(ast);
	unlink_files(data);
	return (ast);
}
