/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   var_expansion.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Helene <Helene@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/26 19:12:06 by Helene            #+#    #+#             */
/*   Updated: 2023/06/10 20:21:01 by Helene           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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

/*
Dans le cas export a="coucou $b", export b=test, echo "$a", ne doit expand que a lors du parsing
-> en effet, lors d'un appel a export, l'expansion de variables est egalement faite 
-> dans l'exemple ci dessus, si tape dans cet ordre, alors a="coucou ", car b n'est au moment la
pas une variable d'env, et donc $b=NULL.
si b etait exporte avant a, alors on aurait a="coucou test"

-> dans tous les cas, n'a qu'a expand qu'au "premier niveau" lors du parsing, 
du fait du fonctionnement du builtin export
*/

char    *get_last_exit_status(void)
{
    return (ft_itoa(55)); // juste pour que ca compile
}

void    expand(t_ht_hash_table *ht, t_token_list **current, char *var, size_t dollar_index) 
{
    char *value; // ne pas le free ! car est malloc dans la hash_map, et peut en re avoir besoin dans une autre commande
    char *before_key;
    char *after_value;
    
    if (!var) // le malloc de ft_substr() n'a pas fonctionné
        return ;
    
    /* Pas sure à 100% de ça, à revérifier */
    if (!(*var)) // ie rien ne suit le '$'
    {
        if ((*current)->next && (*current)->next->type == simple_quote 
            || (*current)->next->type == double_quote)
            remove_char(*current, dollar_index);
        return ;
    }
    
    //printf("var = %s\ndollar index = %zu\n", var, dollar_index);

    before_key = ft_substr((*current)->content, 0, dollar_index);
    if (*var == '?') // expand_last_exit_status(current)
    {
        value = get_last_exit_status(); // le mettre direct dans l'env ?
        after_value = ft_substr((*current)->content, dollar_index + 2, (*current)->length);
    }
    else
    {
        value = ht_search(ht, var);
        // if (!value)
        //     printf("ht_search returned NULL\n\n");
        after_value = ft_substr((*current)->content, dollar_index + ft_strlen(var) + 1, 
            (*current)->length);
    }
    
    
    //printf("before key = %s\n", before_key);
    //printf("after key = %s\n", after_value);
    
    
    
    free((*current)->content);
    (*current)->content = ft_strjoin(ft_strjoin(before_key, value), after_value);
    (*current)->length = ft_strlen((*current)->content);
    free(var);
    free(before_key);
    free(after_value);
}



void    perform_variable_exp(t_ht_hash_table *ht, t_token_list **first)
{
    char            *next_dollar_start;
    char            *dollar_start;
    size_t          next_dollar_index;
    size_t          dollar_index;
    t_token_list    *current;
    
    current = *first;
    while (current)
    {
        //printf("current->content = %s\n", current->content);
        //printf("current->length = %zu\n", current->length);
        if (current->type != simple_quote && (!current->prev 
            || current->prev->type != l_io_redirect || current->prev->length == 1)) // ie n'est pas dans le limiteur d'un here_doc
        {
            dollar_start = ft_strchr(current->content, '$');
            while (dollar_start)
            {
                dollar_index = current->length - ft_strlen(dollar_start);
                //printf("dollar start = %s\n", dollar_start);
                //printf("dollar index = %zu\n", dollar_index);
                // ne delete les quotes et merge les mots qu'apres l'expansion de variables
                next_dollar_start = ft_strchr(dollar_start + 1, '$');
                next_dollar_index = current->length - ft_strlen(next_dollar_start);
                if (next_dollar_start)
                    expand(ht, &current, ft_substr(current->content, dollar_index + 1, next_dollar_index - dollar_index - 1), dollar_index);
                else if (current->type == double_quote)
                    expand(ht, &current, ft_substr(current->content, dollar_index + 1, current->length - dollar_index - 2), dollar_index);    
                else
                    expand(ht, &current, ft_strdup(dollar_start + 1), dollar_index);
                dollar_start = next_dollar_start;
            }
        }
        current = current->next;
    }
}