/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   readline.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hlesny <hlesny@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/26 22:15:52 by Helene            #+#    #+#             */
/*   Updated: 2023/06/21 06:41:26 by hlesny           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

/*

Function: int rl_on_new_line ()

Tell the update routines that we have moved onto a new (empty) line, 
usually after ouputting a newline.

-------------------------

Function: void rl_replace_line (const char *text, int clear_undo)

Replace the contents of rl_line_buffer with text. 
The point and mark are preserved, if possible. 
If clear_undo is non-zero, the undo list associated with the current line 
is cleared.

-------------------------

Function: int rl_redisplay ()

Change what's displayed on the screen to reflect the current contents 
of rl_line_buffer.

--------------------------

Function: void rl_clear_history (void)

Clear the history list by deleting all of the entries, 
in the same manner as the History library’s clear_history() function. 
This differs from clear_history because it frees private data Readline saves 
in the history list.
*/

int	only_whitespaces(char *str)
{
	int	i;

	if (!str)
		return (-1);
	i = 0;
	while (str[i] && (str[i] == ' ' || str[i] == '\t'))
		i++;
	if (str[i])
		return (0);
	return (1);
}

/*
readline returns the text of the line read. 
A blank line returns the empty string. 
If EOF is encountered while reading a line, and the line is empty, 
NULL is returned. 
If an EOF is read with a non-empty line, it is treated as a newline.
*/
void	read_lines(t_data *data)
{
	char			*input;
	size_t			stream_len;
	t_token_list	*tk_list;
	t_cmd			*cmd;
	struct termios	termios_p;

	tcgetattr(STDIN_FILENO, &termios_p);
	input = readline("$ ");
	cmd = NULL;
	tk_list = NULL;
	while (input)
	{
		// printf("ret %d\n", tcsetattr(STDIN_FILENO, TCSAFLUSH,&termios_p));
		stream_len = ft_strlen(input);
		if (!stream_len)
		{
			input = readline("$ ");
			continue ;
		}
		if (input && input[1] == 4)
			ft_exit(NULL, NULL);
		add_history(input);
		if (only_whitespaces(input))
		{
			input = readline("$ ");
			continue ;
		}
		data->first = tokenise(assign_type(input, stream_len), stream_len,
				input);
		signal(SIGINT, sigint_during_cmd_exec);
		cmd = parse(data);
		signal(SIGINT, sigint_next_prompt);
		if (input)
		{
			free(input);
			input = NULL;
		}
		free_cmds(&cmd, false);
			// vérifier que ca couille pas si cmd est NULL 
            // (quand a une erreur de syntaxe, parse retourne NULL)
		input = readline("$ ");
	}
	if (!cmd) //todo c est moche
		free_parsing_data(data);
	free_cmds(&cmd, true);
	free(input);
	printf("exit\n");
}
