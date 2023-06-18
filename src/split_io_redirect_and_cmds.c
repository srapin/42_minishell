/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   split_io_redirect_and_cmds.c                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: srapin <srapin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/26 13:06:50 by Helene            #+#    #+#             */
/*   Updated: 2023/06/18 23:29:50 by srapin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"


/*
1) io_redirect (operator + filename)
2) command (command name + options + arguments)
3) pipe
4) control opérator (&&, ||)
5) parenthesis
*/


t_here_doc    *io_hd_new(t_token_list *current)
{
    t_here_doc *new;

    new = ft_calloc(sizeof(t_here_doc), 1);
    if (!new)
        return (NULL); // perror() ?
    
    new->open_flags = O_CREAT | O_RDONLY; // ,S_IRUSR | S_IWUSR;
    new->limiter = current->next->content; // aura deja merged les mots se suivant, donc tout sera dans le next
    new->quotes = current->next->quotes;
    return (new);
}

t_simple_red    *io_simple_new(t_token_list *current)
{
    t_simple_red *new;
    
    new = ft_calloc(sizeof(t_simple_red), 1);
    if (!new)
        return (NULL); // perror() ?
    
    if (current->type == l_io_redirect) // ie '<'
    {
        new->open_flags = O_RDONLY;
        new->redirected_fd = 0;
    }
    else // ie '>' ou '>>'
    {
        new->redirected_fd = 1;
        if (current->length == 1) // ie '>'
            new->open_flags = O_WRONLY | O_CREAT | O_TRUNC; // ,S_IRWXU;
        else if (current->length == 2) // ie '>>'
            new->open_flags = O_WRONLY | O_CREAT | O_APPEND; // ,S_IRWXU;
    }
    
    // !! condition non suffisante!! il faut s'assurer que le 2 est collé à l'opérateur! (ie pas de whitespace entre les deux)
    if (current->prev && *(current->prev->content) == '2' 
        && current->prev->length == 1)
        new->redirected_fd = 2;
    new->filename = current->next->content;
    return (new);
}

void    io_add(t_io_red **first, t_token_list *current)
{
    t_io_red    *new;
    t_io_red    *curr;
    
    if (!first)
        return ;
    new = ft_calloc(sizeof(t_simple_red), 1);
    if (!new)
        return ; // perror() ?
        
    curr = (*first);
    if (current->type == l_io_redirect && current->length == 2)
        new->io_red = io_hd_new(current);
    else 
        new->io_red = io_simple_new(current);
    if (!new->io_red)
    {
        perror("malloc ");
        free(new);
        return ;
    }
    new->next = NULL;
    while (curr->next)
        curr = curr->next;
    curr->next = new;
}
