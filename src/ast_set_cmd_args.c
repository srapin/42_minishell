/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast_set_cmd_args.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: srapin <srapin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/23 15:03:43 by Helene            #+#    #+#             */
/*   Updated: 2023/06/28 13:12:46 by srapin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	split_not_merged_no_quotes(t_cmd **curr_cmd, t_token_list *curr_tk,
		int *i)
{
	int		wht_pos;
	int		p_wht_pos;
	char	*substr;
	char	*buffer;

	p_wht_pos = 0;
	wht_pos = get_whtsp_pos(curr_tk->content, p_wht_pos);
	buffer = NULL;
	while (wht_pos != -1)
	{
		substr = ft_substr(curr_tk->content, p_wht_pos, wht_pos - p_wht_pos);
		set_buffer(&buffer, substr);
		reassess_buffer(curr_cmd, &buffer, i);
		skip_whitespaces_reassess_indexes(curr_tk->content,
			&wht_pos, &p_wht_pos);
	}
	if ((size_t) p_wht_pos < curr_tk->length)
	{
		(*curr_cmd)->val.args[*i] = ft_substr(curr_tk->content,
				p_wht_pos, curr_tk->length);
		(*i)++;
	}
}

void	split_merged_no_quotes(t_cmd **curr_cmd, t_word_data *wd, char **buffer,
		int *i)
{
	int		wht_pos;
	int		p_wht_pos;
	char	*substr;

	p_wht_pos = 0;
	wht_pos = get_whtsp_pos(wd->content, p_wht_pos);
	while (wht_pos != -1)
	{
		substr = ft_substr(wd->content, p_wht_pos, wht_pos - p_wht_pos);
		set_buffer(buffer, substr);
		reassess_buffer(curr_cmd, buffer, i);
		skip_whitespaces_reassess_indexes(wd->content,
			&wht_pos, &p_wht_pos);
	}
	if (wht_pos == -1)
	{
		substr = ft_substr(wd->content, p_wht_pos, wd->length);
		set_buffer(buffer, substr);
	}
}

void	get_args_from_merged_words(t_cmd **curr_cmd, t_token_list *curr_tk,
		int *i)
{
	char		*buffer;
	char		*tmp;
	t_word_data	*wd;

	wd = curr_tk->merged_words;
	buffer = NULL;
	while (wd)
	{
		if (!wd->quotes)
			split_merged_no_quotes(curr_cmd, wd, &buffer, i);
		else
		{
			tmp = buffer;
			buffer = ft_strjoin(tmp, wd->content);
			free(tmp);
		}
		wd = wd->next;
	}
	if (buffer && *buffer)
	{
		(*curr_cmd)->val.args[*i] = ft_strdup(buffer);
		(*i)++;
		free(buffer);
		buffer = NULL;
	}
}

/*
Dès que tombe sur une succession de whitespaces en parsant le token 
-> tout ce qui venait avant devient un mot (ie token) individuel 

1) des que tombe sur un mot n'étant pas entre quotes :
    s'il n'y a pas de whitespaces,
	le garde dans un buffer et passe au mot suivant
    sinon, cherche la position du premier whitespace,
	garde le sous-mot jusqu'a l'index dans un buffer,
    join ce buffer avec le buffer du/des mots précédents
	si ce dernier buffer est non nul,
    puis ajoute à la liste d'arguments de la commande.
    ensuite, recherche s'il y a un autre whitespace qui suit :
        si non, garde la fin du mot dans un buffer et passe au mot suivant
        si oui, répète l'opération jusqu'à arriver a la fin du mot ou jusqu'à 
        ne plus avoir de whitespaces dans le mot

*/
void	set_cmd_args(t_cmd **curr_cmd, t_token_list *curr_tk, int *i)
{
	if (!curr_tk->content[0])
	{
		(*curr_cmd)->val.args[*i] = ft_strdup("\0");
		(*i)++;
		return ;
	}
	if (!curr_tk->merged_words)
	{
		if (!curr_tk->quotes)
			split_not_merged_no_quotes(curr_cmd, curr_tk, i);
		else
		{
			(*curr_cmd)->val.args[*i] = ft_strdup(curr_tk->content);
			(*i)++;
		}
		return ;
	}
	get_args_from_merged_words(curr_cmd, curr_tk, i);
}

void	set_command_attributs(t_cmd **current,
		t_token_list *current_tk, int args_count)
{
	int	i;

	i = 0;
	if (!args_count)
		return ;
	(*current)->val.args = malloc(sizeof(char *) * (args_count + 1));
	if (!(*current)->val.args)
	{
		perror("malloc ");
		return ;
	}
	while (current_tk && i < args_count)
	{
		if (current_tk->type == l_io_redirect
			|| current_tk->type == r_io_redirect)
			current_tk = current_tk->next->next;
		else
		{
			set_cmd_args(current, current_tk, &i);
			current_tk = current_tk->next;
		}
	}
	(*current)->val.args[i] = NULL;
	if (args_count)
		(*current)->val.value = ft_strdup((*current)->val.args[0]);
}
