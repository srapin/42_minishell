/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: srapin <srapin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/03 02:12:27 by Helene            #+#    #+#             */
/*   Updated: 2023/06/28 13:12:46 by srapin           ###   ########.fr       */
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
	int	wstatus;

	wstatus = check_syntax(data);
	if (wstatus)
	{
		g_exit_status = wstatus;
		free_tokens(data->first);
		data->first = NULL;
		return (0);
	}
	return (1);
}
/* void	print_ast(t_cmd *ast)
{
	t_cmd	*current_pipeline;
	t_cmd	*current_simple_cmd;

	current_pipeline = ast;
	while (current_pipeline)
	{
		current_simple_cmd = current_pipeline;
		while (current_simple_cmd)
		{
			printf("command : \n");
			printf("\tname : %s. empty string ? %d\n\targs : \n",
					current_simple_cmd->val.value,
					*current_simple_cmd->val.value == '\0');
			for (int i = 0; current_simple_cmd->val.args[i]; i++)
				printf("\t\targ %d : %s\n", i, current_simple_cmd->val.args[i]);
			printf("Pipe ? ");
			if (current_simple_cmd->red.next_cmd)
				printf("Yes\n");
			else
				printf("No\n");
			current_simple_cmd = current_simple_cmd->red.next_cmd;
		}
		if (current_pipeline->ctrl == and)
			printf("control_op = '&&' \n");
		else if (current_pipeline->ctrl == or)
			printf("control_op = '||' \n");
		else
			printf("control_op = ';' \n");
		current_pipeline = current_pipeline->next;
	}
} */

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
