/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast_heredocs.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Helene <Helene@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/02 23:24:33 by Helene            #+#    #+#             */
/*   Updated: 2023/06/05 18:59:37 by Helene           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// tf do i do that in a simple way ?
char    *random_filename()
{
    char            *nb;
    char            *file_name;
    static size_t   count; // a le droit d'utiliser des variables statiques ?
    
    nb = ft_itoa(count);
    count++;
    file_name = ft_strjoin("/tmp/here_doc_tmp", nb);
    free(nb);
    return (file_name);
}

int	is_limiter(char *line, char *limiter)
{
	int	i;

	i = 0;
	while (line[i] && limiter[i] && line[i] == limiter[i])
        i++;
    if (!limiter[i]) // ie a parcouru tout le limiter
        return (1); // est le limiter
    return (0);
}

void    hd_expand(t_ht_hash_table *ht, t_token_list **t_list)
{
    char            *next_dollar_start;
    char            *dollar_start;
    size_t          next_dollar_index;
    size_t          dollar_index;
    t_token_list    *current;

    current = *t_list;
    while (current)
    {
        //printf("in the boucle\n");
        //printf("current content = %s\n", current->content);
        dollar_start = ft_strchr(current->content, '$');
        //printf("dollar start = %s\n", dollar_start);
        while (dollar_start)
        {
            //printf("in the boucle number twooo\n");
            dollar_index = current->length - ft_strlen(dollar_start);
            // printf("dollar start = %s\n", dollar_start);
            // printf("dollar index = %zu\n", dollar_index);
            next_dollar_start = ft_strchr(dollar_start + 1, '$');
            next_dollar_index = current->length - ft_strlen(next_dollar_start);
            if (next_dollar_start)
                expand(ht, &current, ft_substr(current->content, dollar_index + 1, next_dollar_index - dollar_index - 1), dollar_index);
            else if (current->type == double_quote || current->type == simple_quote)
                expand(ht, &current, ft_substr(current->content, dollar_index + 1, current->length - dollar_index - 2), dollar_index);    
            else
                expand(ht, &current, dollar_start + 1, dollar_index);
            dollar_start = next_dollar_start;
        }
        //printf("after boucle while\n");
        current = current->next;
    }
    //printf("fini les while\n");
}

/* variable expansions, reusing functions used in the main parsing but without checking syntax 
Attention ! On expand la variable meme si elle est entre single quotes */
void    hd_perform_expand(t_ht_hash_table *ht, char **str) // char * ou char ** ?
{
    size_t          len;
    char            *tmp;
    char            *tmp2;
    t_token_list    *t_list;

    // si est la chaine vide (ie l'user a juste typed un <newline>)
    if (!str || !(*str)[0]) // str existe forcément (sinon serait sorti du while (readline())) 
        return ;
    len = ft_strlen(*str);
    t_list = tokenise(ht, assign_type(*str, len), len, *str);
    
    hd_expand(ht, &t_list);
        
    /* Join all the tokens' contents*/
    tmp = NULL;
    while (t_list)
    {
        tmp2 = ft_strjoin(tmp, t_list->content);
        if (tmp)
            free(tmp);
        tmp = tmp2;
        t_list = t_list->next;
    }
    
    free(*str);
    *str = tmp;
    //printf("hd_content = %s\n", *str);
    
    // free(t_list);
}

/*
1) create a temporary file (in /tmp)
2) using readline(), rewrite the user's input in that file
3) expand variables if needed (ie if there were quotes in the limiter or not)
4) replace the initial tokens "<<" "LIM" by "<" "heredoc_file_path" in the token list   
*/
void    set_here_doc(t_ht_hash_table *ht, t_token_list *current)
{
    char            *limiter;
    char            *file_name;
    char            *hd_content;
    int             quotes;
    int             fd;
    t_word_data     *wd;

    limiter = current->next->content;
    file_name = random_filename();
    fd = open(file_name, O_WRONLY | O_TRUNC | O_CREAT, 0600); //  | O_EXCL
    if (fd == -1)
    {
        perror("open ");
        // return ?
    }

    /* Sets the quotes variable, which indicates whether or not to expand the user's input (aka the heredoc content)*/
    if (!current->next->merged_words)
        quotes = (current->next->quotes > 0);
    else 
    {
        quotes = 0;
        wd = current->next->merged_words;
        while (wd && !quotes)
        {
            if (wd->quotes > 0)
                quotes = 1;
            wd = wd->next;
        }
    }
    
    /* Give prompt back to user and write the heredoc content to the file*/
    hd_content = readline("> ");
    while (hd_content && !is_limiter(hd_content, limiter))
    {
        // cas ou hd_content == NULL ? non car sinon ne serait pas rentré dans le while...
        
        // expand les variables avant d'ecrire dans le fd 
        if (!quotes) // if quotes in limiter, don't expand user's input
            hd_perform_expand(ht, &hd_content); // vérifier que hd_content est bien modifié après l'appel !
        if (write(fd, hd_content, ft_strlen(hd_content)) == -1)
            perror ("write "); // return ? jpense pas
        if (write(fd, "\n", 1) == -1) // non ? car readline() retire le \n de fin
            perror("write ");
        free(hd_content);
        hd_content = readline("> ");
    }
    close(fd);


    // modifie les infos de current et current-> next :
    // aura maintenant '<' suivi du nom du fichier temporaire qu'a ouvert plus haut
    // les types ne changent pas (<io_red> et <word>)
    
    current->length = 1;
    
    free(current->next->content);
    current->next->content = file_name;
    current->next->length = ft_strlen(file_name);
    
    //free(file_name); 
    //red->in_fd = open(file_name, O_RDONLY); // open the file again, in read-only
    //unlink(file_name); // unlink so there's no filesystem reference. the file will be fully deleted once all fds are closed
    
    // file->name = NULL; // ?
    // file->sep = ft_strdup(current->next->content); // ? 
    // file->flag =  O_RDONLY; // ?
    
    // red->in_content = file;
    
}

/* Gives the prompt back to the user for here_doc contents, 
deleting the later from the tokens' list while doing so */
void    set_here_docs(t_ht_hash_table *ht, t_token_list **first)
{
    t_token_list    *current;

    current = *first;
    while (current)
    {
        if (current->type == l_io_redirect && current->length == 2) // ie est un here doc
            set_here_doc(ht, current);
        current = current->next;
    }
}
