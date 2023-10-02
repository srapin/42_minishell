/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast_simple_command.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: srapin <srapin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/23 15:02:23 by Helene            #+#    #+#             */
/*   Updated: 2023/06/28 13:12:46 by srapin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* Update the t_redirect struct of the t_cmd structure. */
void	update_redirect(t_cmd *cmd, t_token_list *current)
{
	t_file	*file;

	file = create_file_struct_with_filename(current->next->content);
	if (current->type == l_io_redirect)
	{
		file->flag = O_RDONLY;
		file->out = false;
	}
	else
	{
		if (current->length == 1)
			file->flag = O_WRONLY | O_CREAT | O_TRUNC;
		else
			file->flag = O_WRONLY | O_CREAT | O_APPEND;
		file->out = true;
	}
	add_redir_with_file_struct(cmd, file);
}

void	get_attributs(t_cmd *current_cmd, t_token_list **curr_tk,
		int *args_count, int *subshell)
{
	t_token_list	*current_tk;

	current_tk = *curr_tk;
	while (current_tk && current_tk->type != and_tk
		&& current_tk->type != or_tk)
	{
		if (current_tk->type == l_io_redirect
			|| current_tk->type == r_io_redirect)
		{
			update_redirect(current_cmd, current_tk);
			current_tk = current_tk->next->next;
		}
		else if (current_tk->type == l_parenthesis)
		{
			*subshell = 1;
			set_subshell(current_cmd, &current_tk);
		}
		else
		{
			*args_count += get_words_count(current_tk);
			current_tk = current_tk->next;
		}
	}
	*curr_tk = current_tk;
}

void	set_simple_command(t_cmd *current_cmd, t_token_list **curr_tk)
{
	int				subshell;
	int				args_count;
	t_token_list	*current_tk;
	t_token_list	*cmd_start_tk;

	subshell = 0;
	args_count = 0;
	current_tk = *curr_tk;
	cmd_start_tk = *curr_tk;
	get_attributs(current_cmd, &current_tk, &args_count, &subshell);
	current_cmd->red.next_cmd = NULL;
	if (!subshell)
		set_command_attributs(&current_cmd, cmd_start_tk, args_count);
	*curr_tk = current_tk;
}
