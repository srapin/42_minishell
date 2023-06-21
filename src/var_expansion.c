/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   var_expansion.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hlesny <hlesny@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/26 19:12:06 by Helene            #+#    #+#             */
/*   Updated: 2023/06/21 03:30:11 by hlesny           ###   ########.fr       */
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

If the parameter is not enclosed in braces, and is a name, the expansion shall use
the longest valid name (see XBD Name), whether or not the variable represented 
by that name exists. Otherwise, the parameter is a single-character symbol, 
and behavior is unspecified if that character is neither a digit nor one of 
the special parameters (see Special Parameters).

If a parameter expansion occurs inside double-quotes:
    Pathname expansion shall not be performed on the results of the expansion.
    Field splitting shall not be performed on the results of the expansion.
*/

void    update_tk_content()
{
    
}

void    search_and_expand()
{
    
    
    update_tk_content();
}

/*
Dans le cas export a="coucou $b", export b=test, echo "$a", ne doit expand que a lors du parsing
-> en effet, lors d'un appel a export, l'expansion de variables est egalement faite 
-> dans l'exemple ci dessus, si tape dans cet ordre, alors a="coucou ", car b n'est au moment la
pas une variable d'env, et donc $b=NULL.
si b etait exporte avant a, alors on aurait a="coucou test"

-> dans tous les cas, n'a qu'a expand qu'au "premier niveau" lors du parsing, 
du fait du fonctionnement du builtin export
*/

void    expand(t_ht_hash_table *ht, t_token_list **current, char *var, size_t dollar_index) 
{
    char *value; // ne pas le free ! car est malloc dans la hash_map, et peut en re avoir besoin dans une autre commande
    char *before_key;
    char *after_value;
    char *tmp;
    
    if (!var) // le malloc de ft_substr() n'a pas fonctionné
        return ;
    if (!(*var)) // ie rien ne suit le '$', et le prochain token n'est pas des quotes
    {
        if ((*current)->next && ((*current)->next->type == simple_quote 
            || (*current)->next->type == double_quote))
            remove_char(*current, dollar_index);
        else
            free(var);
        return ;
    }

    //search_and_expand(current, var, dollar_index);
    
    before_key = ft_substr((*current)->content, 0, dollar_index);
    if (*var == '?') // expand_last_exit_status(current)
    {
        value = ft_itoa(g_exit_status);
        after_value = ft_substr((*current)->content, dollar_index + 2, (*current)->length);
    }
    else
    {
        value = ht_search(ht, var);
        after_value = ft_substr((*current)->content, dollar_index + ft_strlen(var) + 1, 
            (*current)->length);
    }
    
    
    free((*current)->content);
    tmp = ft_strjoin(before_key, value);
    (*current)->content = ft_strjoin(tmp, after_value);
    (*current)->length = ft_strlen((*current)->content);
    if (*var == '?')
        free(value);
    free(var);
    free(before_key);
    free(after_value);
    free(tmp);
    // tout mettre a NULL apres aussi 
}



void    perform_variable_exp(t_data *data)
{
    char            *next_dollar_start;
    char            *dollar_start;
    char            *var_name;
    size_t          next_dollar_index;
    size_t          dollar_index;
    t_token_list    *current;
    
    current = *(data->first);
    while (current)
    {
        while (current && current->type == whitespace)
            current = current->next;
        if (!current)
            break;
        if (current->type == l_parenthesis)
        {
            while (current && current->type != r_parenthesis)
                current = current->next;
            if (current)
                current = current->next;
            continue ;
        }
        else if (current->type != simple_quote && (!current->prev 
            || current->prev->type != l_io_redirect || current->prev->length == 1)) // ie n'est pas dans le limiteur d'un here_doc
        {
            dollar_start = ft_strdup(ft_strchr(current->content, '$'));
            while (dollar_start && *dollar_start)
            {
                dollar_index = current->length - ft_strlen(dollar_start);
                next_dollar_start = ft_strdup(ft_strchr(dollar_start + 1, '$'));
                next_dollar_index = current->length - ft_strlen(next_dollar_start);
                if (next_dollar_start && *next_dollar_start)
                {
                    int k = 1;
                    char *var_name;
                    var_name = ft_substr(current->content, dollar_index + 1, next_dollar_index + 1);
                    while (k + dollar_index <= current->length && !valid_name(var_name))
                    {
                        free(var_name);
                        k++;
                        var_name = ft_substr(current->content, dollar_index + 1, current->length - dollar_index - k);
                    }
                    if (k + dollar_index <= current->length) // ie valid_name() == 1
                        expand(data->env, &current, var_name, dollar_index);  
                        /* expand(data->env, &current, ft_substr(current->content, dollar_index + 1, next_dollar_index - dollar_index - 1), dollar_index); */
                }
                else if (current->type == double_quote)
                {
                    int k = 1;
                    char *var_name;
                    var_name = ft_substr(current->content, dollar_index + 1, current->length - dollar_index - k);
                    while (k + dollar_index <= current->length && !valid_name(var_name))
                    {
                        free(var_name);
                        k++;
                        var_name = ft_substr(current->content, dollar_index + 1, current->length - dollar_index - k);
                    }
                    if (k + dollar_index <= current->length) // ie valid_name() == 1
                        expand(data->env, &current, var_name, dollar_index);    
                }
                else
                {
                    int k = 1;
                    char *var_name;
                    var_name = ft_substr(current->content, dollar_index + 1, current->length - dollar_index - k);
                    while (k + dollar_index <= current->length && !valid_name(var_name))
                    {
                        free(var_name);
                        k++;
                        var_name = ft_substr(current->content, dollar_index + 1, current->length - dollar_index - k);
                    }
                    if (k + dollar_index <= current->length) // ie valid_name() == 1
                        expand(data->env, &current, var_name, dollar_index);  
                        //expand(data->env, &current, ft_strdup(dollar_start + 1), dollar_index);
                }
                free(dollar_start);
                dollar_start = NULL;
                dollar_start = next_dollar_start;
            }
            free(dollar_start);
            dollar_start = NULL;
        }
        current = current->next;
    }
}