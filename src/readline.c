/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   readline.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: srapin <srapin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/26 22:15:52 by Helene            #+#    #+#             */
/*   Updated: 2023/06/28 13:12:46 by srapin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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

void	lexe_parse_execute(t_data *data, char *input)
{
	size_t	stream_len;
	t_cmd	*cmd;

	stream_len = ft_strlen(input);
	data->first = tokenise(assign_type(input, stream_len), stream_len, input);
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
	t_cmd			*cmd;
	struct termios	termios_p;

	tcgetattr(STDIN_FILENO, &termios_p);
	input = readline("$ ");
	cmd = NULL;
	while (input)
	{
		tcsetattr(STDIN_FILENO, TCSAFLUSH, &termios_p);
		if (!ft_strlen(input))
		{
			input = readline("$ ");
			continue ;
		}
		add_history(input);
		if (!only_whitespaces(input))
			lexe_parse_execute(data, input);
		input = readline("$ ");
	}
	if (!cmd)
		free_parsing_data(data);
	free_cmds(&cmd, true);
	free(input);
	write(STDERR_FILENO, "exit\n", ft_strlen("exit\n"));
}
