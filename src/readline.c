/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   readline.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: srapin <srapin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/26 22:15:52 by Helene            #+#    #+#             */
/*   Updated: 2023/06/24 12:24:18 by srapin           ###   ########.fr       */
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

void 	lexe_parse_execute(t_data *data, char *input)
{
	size_t stream_len;
	t_cmd 	*cmd;

	stream_len = ft_strlen(input);
	data->first = tokenise(assign_type(input, stream_len), stream_len,
			input);
	signal(SIGINT, sigint_during_cmd_exec);
	cmd = parse_and_execute(data);
	signal(SIGINT, sigint_next_prompt);
	if (input)
	{
		free(input);
		input = NULL;
	}
	free_cmds(&cmd, false);
}

void	read_lines(t_data *data)
{
	char			*input;
	//size_t			stream_len;
	t_cmd			*cmd;
	struct termios	termios_p;

	tcgetattr(STDIN_FILENO, &termios_p);
	input = readline("$ ");
	cmd = NULL;
	while (input)
	{
		tcsetattr(STDIN_FILENO, TCSAFLUSH,&termios_p);
		//stream_len = ft_strlen(input);
		if (!ft_strlen(input))
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
		
		lexe_parse_execute(data, input);
		
		/*data->first = tokenise(assign_type(input, stream_len), stream_len,
				input);
		signal(SIGINT, sigint_during_cmd_exec);
		cmd = parse(data);
		signal(SIGINT, sigint_next_prompt);
		if (input)
		{
			free(input);
			input = NULL;
		}
		free_cmds(&cmd, false);*/
		input = readline("$ ");
	}
	if (!cmd) //todo c est moche
		free_parsing_data(data);
	free_cmds(&cmd, true);
	free(input);
	write(STDERR_FILENO, "exit\n", 1);
	// printf("exit\n");
}
