/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   var_expansion.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hlesny <hlesny@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/26 19:12:06 by Helene            #+#    #+#             */
/*   Updated: 2023/06/23 01:24:54 by hlesny           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

/*
The parameter name or symbol can be enclosed in braces, 
which are optional except for positional parameters with more than one digit 
or when parameter is a name and is followed by a character that could be 
interpreted as part of the name. 
The matching closing brace shall be determined by counting brace levels, 
skipping over enclosed quoted strings, and command substitutions.

If the parameter is not enclosed in braces, and is a name,
	the expansion shall use
the longest valid name (see XBD Name), whether or not the variable represented 
by that name exists. Otherwise, the parameter is a single-character symbol, 
and behavior is unspecified if that character is neither a digit nor one of 
the special parameters (see Special Parameters).

If a parameter expansion occurs inside double-quotes:
    Pathname expansion shall not be performed on the results of the expansion.
    Field splitting shall not be performed on the results of the expansion.
*/

void	update_tk_content(t_token_list **current, char *before_key, char *value,
		char *after_value)
{
	char	*tmp;

	free((*current)->content);
	tmp = ft_strjoin(before_key, value);
	(*current)->content = ft_strjoin(tmp, after_value);
	(*current)->length = ft_strlen((*current)->content);
	free(before_key);
	free(after_value);
	free(tmp);
	before_key = NULL;
	after_value = NULL;
	tmp = NULL;
}

void	search_and_expand(t_ht_hash_table *ht, t_token_list **current,
		char *var, size_t dollar_index)
{
	char	*before_key;
	char	*after_value;
	char	*tmp;

	char *value; /* ne pas le free ! car est malloc dans la hash_map,
		et peut en re avoir besoin dans une autre commande*/
	before_key = ft_substr((*current)->content, 0, dollar_index);
	/* if (*var == '?') // expand_last_exit_status(current)
	{
		value = ft_itoa(g_exit_status);
		after_value = ft_substr((*current)->content, dollar_index + 2,
				(*current)->length);
	} */
	//else
	//{
		value = ht_search(ht, var);
		after_value = ft_substr((*current)->content, dollar_index
				+ ft_strlen(var) + 1, (*current)->length);
	//}
	update_tk_content(current, before_key, value, after_value);
	/* if (*var == '?')
		free(value); */
}

/*
Dans le cas export a="coucou $b", export b=test, echo "$a",
	ne doit expand que a lors du parsing
-> en effet, lors d'un appel a export,
	l'expansion de variables est egalement faite 
-> dans l'exemple ci dessus, si tape dans cet ordre, alors a="coucou ",
	car b n'est au moment la
pas une variable d'env, et donc $b=NULL.
si b etait exporte avant a, alors on aurait a="coucou test"

-> dans tous les cas, n'a qu'a expand qu'au "premier niveau" lors du parsing, 
du fait du fonctionnement du builtin export
*/

void	expand(t_ht_hash_table *ht, t_token_list **current, char *var,
		size_t dollar_index)
{
	if (!var)
		return ;
	if (!(*var))
	{
		if ((*current)->next && ((*current)->next->type == simple_quote
				|| (*current)->next->type == double_quote))
			remove_char(*current, dollar_index);
		else
			free(var);
		return ;
	}
	search_and_expand(ht, current, var, dollar_index);
}

char	*set_var_name(t_token_list *current, size_t dollar_index,
		size_t next_dollar_index, int exp_case)
{
	int		k;
	char	*var_name;

	k = 1;
	if (exp_case == 1)
		k = 2;
	/* var_name = set_var_name(current, dollar_index,
			next_dollar_index,exp_case); */
	if (exp_case == 0)
		var_name = ft_substr(current->content, dollar_index + 1,
				next_dollar_index - dollar_index - 1);
	else
		var_name = ft_substr(current->content, dollar_index + 1, current->length
				- dollar_index - k);
	return (var_name);
}

/* Returns NULL in case no valid identifier follows the '$' at the given <dollar_index> index
Case 0 : is followed by another dollar sign (ie next_dollar_index != -1)
Case 1 : is inside double quotes
Case 2 : not followed by another dollar sign, and not in quotes */
char	*get_valid_id(t_token_list *current, size_t dollar_index,
		size_t next_dollar_index, int exp_case)
{
	int		k;
	char	*var_name;

	k = 1;
	if (exp_case == 1)
		k = 2;
	var_name = set_var_name(current, dollar_index, next_dollar_index, exp_case);
	/* if (exp_case == 0)
		var_name = ft_substr(current->content, dollar_index + 1,
				next_dollar_index + 1);
	else
		var_name = ft_substr(current->content, dollar_index + 1, current->length
				- dollar_index - k); */
	while (k + dollar_index <= current->length && !valid_name(var_name))
	{
		free(var_name);
		k++;
		if (exp_case == 0)
			var_name = ft_substr(current->content, dollar_index + 1,
					next_dollar_index - dollar_index - k);
		else
			var_name = ft_substr(current->content, dollar_index + 1,
					current->length - dollar_index - k);
	}
	if (k + dollar_index <= current->length)
		return (var_name);
	free(var_name);
	return (NULL);
}

int	is_only_dollars(char *str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		if (str[i] != '$')
			return (0);
		i++;
	}
	return (1);
}

void	check_next_token(t_token_list **curr, size_t dollar_index)
{
	int				dollars_count;
	t_token_list	*current;

	current = *curr;
	dollars_count = 0;
	while (current && (current->type == word || current->type == simple_quote
			|| current->type == double_quote)
		&& is_only_dollars(current->content))
	{
		if (current->next && (current->next->type == simple_quote
			|| current->next->type == double_quote))
			{
				dollars_count = ft_strlen(current->content);
				if (dollars_count && dollars_count % 2)
					remove_char(current, 0);
			}
		current = current->next;
	}
	//*curr = current;
	
	// if ((*current)->next && ((*current)->next->type == simple_quote
	// 		|| (*current)->next->type == double_quote))
	// 	remove_char(*current, dollar_index);
}

/* 
$$$$?
dollar_start = $$$$?
i = 4
dollar_start[i] = '?'
dollar_index = 5 - 
*/
int	is_exit_status(t_ht_hash_table *ht, t_token_list *current,
		char *dollar_start)
{
	int		i;
	char	*value;
	char	*after_value;
	char	*before_key;
	size_t	dollar_index;

	i = 0;
	if (!(*dollar_start))
	{
		free(dollar_start);
		return (1); // ou 0, change r car va rien expand dans tous les cas
	}
	while (dollar_start[i] && dollar_start[i] == '$')
		i++;
	if (dollar_start[i] && dollar_start[i] == '?')
	{
		dollar_index = current->length - ft_strlen(dollar_start + i) - 1;
		before_key = ft_substr(current->content, 0, dollar_index);
		value = ft_itoa(g_exit_status);
		after_value = ft_substr(current->content, dollar_index + 2,
				current->length);
		update_tk_content(&current, before_key, value, after_value);
		check_next_token(&current, current->length - ft_strlen(dollar_start)
				- 1);
		free(dollar_start);
		return (1);
	}
	return (0);
}

char	*get_next_d_start(char *d_start)
{
	int		i;
	char	*next_d_start;

	i = 0;
	while (d_start[i] && d_start[i] == '$')
		i++;
	next_d_start = ft_strdup(ft_strchr(d_start + i, '$')); // malloc a proteger
	return (next_d_start);
}

char	*get_var_name(t_token_list *current, char *next_d_start, size_t d_index,
		size_t next_d_index)
{
	char	*var_name;

	if (next_d_start && *next_d_start)
		var_name = get_valid_id(current, d_index, next_d_index, 0);
	else if (current->type == double_quote)
		var_name = get_valid_id(current, d_index, next_d_index, 1);
	else
		var_name = get_valid_id(current, d_index, next_d_index, 2);
	return (var_name);
}

void	parse_current_tk(t_ht_hash_table *ht, t_token_list *current)
{
	char	*next_d_start;
	char	*d_start;
	char	*var_name;
	size_t	next_d_index;
	size_t	d_index;

	d_start = ft_strdup(ft_strchr(current->content, '$'));
	if (is_exit_status(ht, current, d_start))
		return ;
	while (d_start && *d_start)
	{
		d_index = current->length - ft_strlen(d_start);
		next_d_start = ft_strdup(ft_strchr(d_start + 1, '$'));
		next_d_index = current->length - ft_strlen(next_d_start);
		var_name = get_var_name(current, next_d_start, d_index, next_d_index);
		/* if (next_d_start && *next_d_start)
			var_name = get_valid_id(current, d_index, next_d_index, 0);
		else if (current->type == double_quote)
			var_name = get_valid_id(current, d_index, next_d_index, 1);
		else
			var_name = get_valid_id(current, d_index, next_d_index, 2); */
		if (var_name)
			search_and_expand(ht, &current, var_name, d_index);
		else
			check_next_token(&current, d_index);
		free(d_start);
		d_start = next_d_start;
		free(var_name);
	}
	free(d_start);
	d_start = NULL;
}

void	perform_variable_exp(t_data *data)
{
	t_token_list *current;

	current = *(data->first);
	while (current)
	{
		while (current && current->type == whitespace)
			current = current->next;
		if (!current)
			break ;
		if (current->type == l_parenthesis)
		{
			while (current && current->type != r_parenthesis)
				current = current->next;
			if (current)
				current = current->next;
			continue ;
		}
		else if (current->type != simple_quote && (!current->prev
					|| current->prev->type != l_io_redirect
					|| current->prev->length == 1))
			// ie n'est pas dans le limiteur d'un here_doc
			parse_current_tk(data->env, current);
		current = current->next;
	}
}