/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quotes.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Helene <Helene@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/26 19:13:44 by Helene            #+#    #+#             */
/*   Updated: 2023/06/14 17:01:24 by Helene           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
Après l'expansion de variables. 
Enleve les quotes de début et de fin, et remplace le token <quote> par <word>
*/
void    delete_quotes(t_token_list **first)
{
    char *content_tmp;
    t_token_list *current;
    t_token_list *current_prev;

    if (!first || !(*first))
        return ;
    current = *first;
    if (current->type == simple_quote || current->type == double_quote)
    {
        content_tmp = ft_substr(current->content, 1, current->length - 2);
        if (!content_tmp) // && current->length - 2 > 0 // si le malloc ne fonctionne pas
            return ; // que faire ?
        free(current->content);
        current->content = content_tmp;
        current->type = word;
    }
    current_prev = current;
    current = current->next;
    while (current)
    {
        if (current->type == l_parenthesis)
        {
            while (current && current->type != r_parenthesis)
                current = current->next;
            if (current)
                current = current->next;
        }
        else 
        {
            if (current->type == simple_quote || current->type == double_quote)
            {
                if (current_prev->type != l_io_redirect || current_prev->length == 1) // si le token n'est pas le delimiteur d'un here_doc
                {
                    content_tmp = ft_substr(current->content, 1, current->length - 2);
                    if (!content_tmp) // si le malloc ne fonctionne pas
                        return ; // que faire ?
                    free(current->content);
                    current->content = content_tmp;
                    current->type = word;
                    current->length -= 2;
                }
            }
            current = current->next;
        }
    }
}

t_word_data     *new_word_data(t_token_list *token)
{
    t_word_data *wd;

    wd = ft_calloc(sizeof(t_word_data), 1);
    if (!wd)
        return (NULL);
    wd->content = ft_strdup(token->content);
    wd->quotes = token->quotes;
    wd->length = token->length;
    wd->quotes = 0;
    wd->next = NULL;
    return (wd);
}

void    add_word_data(t_word_data **first, t_word_data *to_add)
{
    size_t      word_end_index;
    t_word_data *current;

    if (!first)
        return ;
    word_end_index = 0;
    if (!(*first))
        *first = to_add;
    else
    {
        current = (*first);
        while (current->next)
        {
            word_end_index += current->length;
            current = current->next;
        }
        word_end_index += current->length;
        to_add->word_start_index = word_end_index;
        current->next = to_add;
    }
}

void    set_merged_words(t_token_list **curr)
{
    char            *content_tmp;
    t_token_list    *current;
    t_token_list    *tmp;

    current = *curr;
    if (!current->merged_words) // ie le <word> n'a pas encore été merged avec un autre <word>
        add_word_data(&current->merged_words, new_word_data(current));
    add_word_data(&current->merged_words, new_word_data(current->next));
    
    content_tmp = ft_strjoin(current->content, current->next->content);
    free(current->content);
    current->content = content_tmp;
    current->length = ft_strlen(content_tmp);
    
    tmp = current->next; // the one to delete
    tmp->prev->next = tmp->next; // fait pointer le précédent sur l'élément suivant celui qui va etre supprimé
    if (tmp->next)
        tmp->next->prev = tmp->prev; // idem mais dans l'autre sens
    free(tmp->content);
    free(tmp);
    tmp = NULL;

    *curr = current;
}

/*
Apres avoir enlevé les quotes, groupe deux <word> qui se suivent 
(comprendre ne sont pas séparés par des whitespaces, ou un opérateur de controle autre que des quotes)
-> il est alors obligatoire d'avoir gardé les tokens <whitespace> jusque là
*/
void    group_words(t_token_list **first)
{
    t_token_list    *current;
    t_token_list    *current2;
    char *content_tmp;

    if (!first)
        return ; // ?
    current = *first;
    while (current)
    {
        if (current->type == l_parenthesis)
        {
            while (current && current->type != r_parenthesis)
                current = current->next;
            if (current)
                current = current->next;
        }
        else if (current->type == word && current->next && current->next->type == word)
        {
            set_merged_words(&current);
            
            // if (!current->merged_words) // ie le <word> n'a pas encore été merged avec un autre <word>
            //     add_word_data(&current->merged_words, new_word_data(current));
            // add_word_data(&current->merged_words, new_word_data(current->next));
            
            // content_tmp = ft_strjoin(current->content, current->next->content);
            // free(current->content);
            // current->content = content_tmp;
            // current->length = ft_strlen(content_tmp);
            
            // current2 = current->next; // the one to delete
            // current2->prev->next = current2->next; // fait pointer le précédent sur l'élément suivant celui qui va etre supprimé
            // if (current2->next)
            //     current2->next->prev = current2->prev; // idem mais dans l'autre sens
            // free(current2->content);
            // free(current2);
            // current2 = NULL;
        }
        else if (current->type == whitespace) // deletes the token
        {
            if (current == *first)
                *first = current->next;
            current2 = current; 
            current = current->next;
            if (current2->prev) // vérifie qu'il ne s'agit pas du premier élément
                current2->prev->next = current2->next; // fait pointer le précédent sur l'élément suivant celui qui va etre supprimé
            if (current2->next) // vérifie qu'il ne s'agit pas du dernier élément
                current2->next->prev = current2->prev; // idem mais dans l'autre sens
            
            free(current2->content);
            free(current2);

            //current = current->next;
            //tk_del_one(&current);
        }
        else
            current = current->next;
    }
}
