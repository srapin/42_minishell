/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   readline.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Helene <Helene@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/26 22:15:52 by Helene            #+#    #+#             */
/*   Updated: 2023/06/05 16:08:37 by Helene           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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

/*
readline returns the text of the line read. 
A blank line returns the empty string. 
If EOF is encountered while reading a line, and the line is empty, 
NULL is returned. 
If an EOF is read with a non-empty line, it is treated as a newline.
*/
void    read_lines(t_ht_hash_table *ht)
{
    char *input;
    size_t stream_len;
    t_token_list  *tk_list;

    input = NULL;
    while (1) // ou while (true) 
    {
        if (input)
        {
            free(input);
            input = NULL;
        }
        input = readline("$ "); // "> " is when entering a multi line command : le print uniquement pour le here_doc
        if (!input)
            continue;
        stream_len = ft_strlen(input);
        if (!stream_len) // ie si input_str = '\0'.
            continue;
        tk_list = tokenise(ht, assign_type(input, stream_len), stream_len, input);
        parse(ht, tk_list); // l'a mis ici et plus dans tokenise(). vérifier si marche encore !!
        add_history(input);   
        
    }
}