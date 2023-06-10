/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax_check.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Helene <Helene@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/26 19:10:18 by Helene            #+#    #+#             */
/*   Updated: 2023/06/10 17:43:01 by Helene           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
Prints the syntax error while specifying the token
Then, frees all data structures and returns the corresponding exit status (ie 2 for syntax error)
*/
void    display_se(t_token_list **first, char *token)
{
    printf("Syntax error near unexpected token `%s\'\n", token);
    free_tokens(first);
    free(token);
    exit(SYNTAX_ERROR);
}

void    check_first(t_token_list **first)
{
    int type;
    t_token_list *current;

    if (!first || !*first)
        exit(EXIT_OK); // rien a free dans ce cas, si ?
    current = (*first);
    while (current && current->type == whitespace)
        current = current->next;
    if (!current)
        return ;
;   type = current->type;
    if (type == and_tk)
    {
        if ((*first)->length >= 2)
            display_se(first, ft_strdup("&&"));
        display_se(first, ft_strdup("&"));
    }
    else if (type == or_tk)
    {
        if ((*first)->length >= 2)
            display_se(first, ft_strdup("||"));
        display_se(first, ft_strdup("|"));
    }
    else if (type == r_parenthesis)
        display_se(first, ft_strdup(")"));
}

/* Les tokens avec des '' ou "" doivent avoir le type word, et le content "\0" !!! */
void    check_io_redirect(t_token_list **first, t_token_list **op)
{
    /*
    Un operateur de redirection est TOUJOURS suivi par un <word>
    */
    t_token_list    *current;
    
    if ((*op)->length > 2)
        display_se(first, ft_substr((*op)->content, 2, 2)); // garbage collector ? permettrait d'y ajouter le malloc de ft_substr et reglerait le probleme de ou peut le free
    current = (*op)->next;
    while (current && current->type == whitespace)
        current = current->next;
    if (!current) // cas ou rien ne suit : ie syntax error near unexpected newline
        display_se(first, ft_strdup("newline"));
    else if (current->type != word && current->type != simple_quote
        && current->type != double_quote) // le fait que la variable soit expandable ou non, ou qu'un/des fichiers resultant du wildcard expansion existe ou non, est a gerer pendant l'execution (ou du moins apres la vérification de la syntaxe)
        display_se(first, ft_substr(current->content, 0, 2));
}

void    check_simple_command(t_token_list **first, t_token_list **current, int *parentheses_count, int *s_quotes, int *d_quotes)
{
    while ((*current) && (*current)->type != and_tk && (*current)->type != or_tk)
    {
        // if ((*current)->type == whitespace)
        //     (*current) = (*current)->next;
        if ((*current)->type == l_io_redirect || (*current)->type == r_io_redirect)
            check_io_redirect(first, current);
        else if ((*current)->type == l_parenthesis)
        {
            (*parentheses_count)++; // c'est tout ?
            if ((*current)->next && (*current)->next->type == r_parenthesis)
                display_se(first, ")");
        }
        else if ((*current)->type == r_parenthesis)
        {
            if (!(*parentheses_count))
                display_se(first, ")");
            (*parentheses_count)--;
        }
        else if ((*current)->type == simple_quote 
            && ((*current)->content[(*current)->length - 1] != '\''
            || (*current)->length == 1))
            {
                printf("Syntax error : Missing closing quote\n");
                free_tokens(first);
                exit(SYNTAX_ERROR);
            }
        else if ((*current)->type == double_quote
            && ((*current)->content[(*current)->length - 1] != '\"'
            || (*current)->length == 1))
            {
                printf("Syntax error : Missing closing quote\n");
                free_tokens(first);
                exit(SYNTAX_ERROR);
            }
        *current = (*current)->next;
    }
}

void    check_control_op(t_token_list **first, t_token_list **current) // ie "||" or "&&"
{
    t_token_list    *curr;
    
    if (((*current))->length != 2)
        display_se(first, ft_substr(((*current))->content, 0, 2));
    curr = (*current)->next;
    while (curr && curr->type == whitespace)
        curr = curr->next;
    if (!curr) // faut-il redonner le prompt ?
        display_se(first, ft_strdup("newline")); // ou ft_substr((*current)->content, 0, 2) ? 
    if (curr->type == r_parenthesis)
        display_se(first, ft_strdup(")"));
    
    *current = (*current)->next; // ou simplement *current = curr ? skip les whitespaces ici comme ca c'est fait
}

void    check_pipe(t_token_list **first, t_token_list **current)
{
    t_token_list    *curr;
    
    if (!(*current)->next) // faut-il redonner le prompt ?
        display_se(first, ft_strdup("|"));
    curr = (*current)->next;
    while (curr && curr->type == whitespace)
        curr = curr->next;
    if ((*current)->next->type == r_parenthesis)
        display_se(first, ft_strdup(")"));
    if ((*current)->next->type == and_tk)
        display_se(first, ft_substr((*current)->next->content, 0, 2));
    
    (*current) = (*current)->next;
}

void    check_syntax(t_token_list **first)
{
    int parentheses_count = 0;
    int s_quotes_count = 0; // si non utilisée plus haut, à delete
    int d_quotes_count = 0; // si non utilisée plus haut, à delete
    t_token_list    *current;
    
    current = *first; // part du principe qu'à au moins un élément ; sinon aurait déjà exit
    
    check_first(first); // utile ? en a deja un dans le while 
    
    /* check pipelines one by one */
    while (current)
    {
        if (current->type == and_tk || (current->type) == or_tk && current->type > 1)
            check_control_op(first, &current);
        else
        {
            check_first(&current);
            
            /* check simple commands one by one */
            while (current && current->type != and_tk && (current->type != or_tk || current->length == 1))
            {
                if (current->type == or_tk && current->length == 1) // ie si est un pipe
                    check_pipe(first, &current);
                else
                    check_simple_command(first, &current, &parentheses_count, &s_quotes_count, &d_quotes_count);
            }
        }
    }
    if (parentheses_count) // ie toutes les parentheses ne sont pas fermees
    {
        printf("Syntax error : Missing closing parenthesis\n");
        free_tokens(first);
        exit(SYNTAX_ERROR);
    }
}

int    ft_syntax(t_token_list **first)
{
    pid_t   pid;
    int     wstatus;
    
    pid = fork();
    if (pid == -1)
        perror("fork ");
    if (pid == 0) // ie s'agit du process enfant
        check_syntax(first);
    //if (waitpid(pid, &wstatus, 0) == -1)
    if (wait(&wstatus) == -1)
        perror("wait ");
    return (wstatus);
}
