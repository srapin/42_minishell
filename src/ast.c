/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: srapin <srapin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/25 21:44:19 by Helene            #+#    #+#             */
/*   Updated: 2023/06/19 03:53:221 by srapin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

/* Determine if the current token is a control operator, ie a "&&" or a "||" */
int     is_a_ctrl_op(t_token_list *current)
{
    if (current->type == and_tk)
        return (1);
    if (current->type == or_tk
        && current->length == 2)
        return (1);
    return (0);
}

void    remove_char(t_token_list *current, size_t index)
{
    char    *before;
    char    *after;

    before = ft_substr(current->content, 0, index);
    after = ft_substr(current->content, index + 1, current->length);
    if (!before || !after)
    {
        perror("malloc ");
        return ;
    }
    free(current->content);
    current->content = ft_strjoin(before, after);
    current->length--;
    free(before);
    free(after);
}

/* Update the t_redirect struct of the t_cmd structure. */
void    update_redirect(t_cmd *cmd, t_token_list *current)
{
    t_file  *file;
    
    file = create_file_struct_with_filename(current->next->content);
    if (current->type == l_io_redirect) // '<'
    {
        file->flag = O_RDONLY;
        add_in_redir_with_file_struct(cmd, file);
        
        // printf("added new infile : %s\n", current->next->content);
    }
    else
    {
        if (current->length == 1) // '>'
            file->flag = O_WRONLY | O_CREAT | O_TRUNC;
        else // '>>'
            file->flag = O_WRONLY | O_CREAT | O_APPEND;
        add_out_redir_with_file_struct(cmd, file);
        
        // printf("added new outfile : %s\n", current->next->content);
        // printf("added new outfile : %s\n", file->name);
    }
}


/* Returns the substring starting at the first encountered whitespace in the initial string.
When no whitespace is encountered, NULL is returned.
If the string is either empty or null, NULL is returned. */
int     get_whtsp_pos(char *str, int whtsp_pos)
{
    int i;
    
    if (!str)
        return (-1);
    i = whtsp_pos;
    while (str[i])
    {
        if (str[i] == ' ' || str[i] == '\t')
            return (i);
        i++;
    }
    return (-1);
}

/*
Dès que tombe sur une succession de whitespaces en parsant le token 
-> tout ce qui venait avant devient un mot (ie token) individuel 

1) des que tombe sur un mot n'étant pas entre quotes :
    s'il n'y a pas de whitespaces, le garde dans un buffer et passe au mot suivant
    sinon, cherche la position du premier whitespace, garde le sous-mot jusqu'a l'index dans un buffer,
    join ce buffer avec le buffer du/des mots précédents si ce dernier buffer est non nul,
    puis ajoute à la liste d'arguments de la commande.
    ensuite, recherche s'il y a un autre whitespace qui suit :
        si non, garde la fin du mot dans un buffer et passe au mot suivant
        si oui, répète l'opération jusqu'à arriver a la fin du mot ou jusqu'à 
        ne plus avoir de whitespaces dans le mot

*/
void    set_cmd_args(t_cmd **curr_cmd, t_token_list *curr_tk, int *i)
{
    int             j;
    int             whitespace_pos;
    int             prev_whitespace_pos;
    char            *buffer;
    char            *tmp;
    char            *substr;
    t_word_data     *wd;
    
    wd = curr_tk->merged_words;
    buffer = NULL;
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
            //////dprintf(1, "in set_cmd_args(), added new arg : %s\n", curr_tk->content);
            (*curr_cmd)->val.args[*i] = ft_strdup(curr_tk->content);
            (*i)++;
        }
        return ;
    }
    while (wd)
    {
        //////dprintf(1, "\twd = %s, quotes = %d\n", wd->content, wd->quotes);
        if (!wd->quotes)
        {
            prev_whitespace_pos = 0;
            whitespace_pos = get_whtsp_pos(wd->content, prev_whitespace_pos);
            //////dprintf(1, "\twd = %s, whitespace_pos = %d\n", wd->content, whitespace_pos);
            while (whitespace_pos != -1)
            {
                tmp = buffer;
                substr = ft_substr(wd->content, prev_whitespace_pos, whitespace_pos - prev_whitespace_pos);
                buffer = ft_strjoin(tmp, substr);
                if (*buffer) // ou if (whitespace_pos - prev_whitespace_pos) // ie si il n'est pas vide
                {
                    (*curr_cmd)->val.args[*i] = ft_strdup(buffer);
                    //////dprintf(1, "in set_cmd_args(), added new arg : %s\n", buffer);
                    (*i)++;
                    free(buffer);
                    buffer = NULL;
                }
                free(substr);
                substr = NULL;
                while (wd->content[whitespace_pos] && (wd->content[whitespace_pos] == ' ' || wd->content[whitespace_pos] == '\t'))
                    whitespace_pos++;
                prev_whitespace_pos = whitespace_pos;
                whitespace_pos = get_whtsp_pos(wd->content, prev_whitespace_pos);
                //////dprintf(1, "\twd = %s, whitespace_pos = %d\n", wd->content, whitespace_pos);
            }
            if (whitespace_pos == -1) // pas opti
            {
                tmp = buffer;
                substr = ft_substr(wd->content, prev_whitespace_pos, wd->length);
                buffer = ft_strjoin(tmp, substr);
                free(tmp);
                free(substr);
                tmp = NULL;
                substr = NULL;
            }
            //split_merged_no_quotes(curr_cmd, wd, buffer, i); // est ce que va modifier buffer ou est-ce que doit donner son adresse ?
        }
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
        //////dprintf(1, "in set_cmd_args(), added new arg : %s\n", buffer);
        (*curr_cmd)->val.args[*i] = ft_strdup(buffer);
        (*i)++;
        free(buffer);
        buffer = NULL;
    }
}

void    set_command_attributs(t_cmd **current, t_token_list **first_tk, t_token_list *current_tk, int args_count)
{
    int i;

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
        if (current_tk->type == l_io_redirect || current_tk->type == r_io_redirect)
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

int     no_merged_words_count(t_token_list *current)
{

    int     count;
    int     whitespace_pos;
    int     prev_whitespace_pos;

    count = 0;
    if (!current->quotes)
    {
        prev_whitespace_pos = 0;
        whitespace_pos = get_whtsp_pos(current->content, prev_whitespace_pos);
        while (whitespace_pos != -1)
        {
            if (whitespace_pos - prev_whitespace_pos)
                count++;
            while (current->content[whitespace_pos] && (current->content[whitespace_pos] == ' ' || current->content[whitespace_pos] == '\t'))
                whitespace_pos++;
            prev_whitespace_pos = whitespace_pos;
            whitespace_pos = get_whtsp_pos(current->content, prev_whitespace_pos);
        }
        if (prev_whitespace_pos < current->length)
            count++;
    }
    else
        count++;
    return (count);
}

void    wd_no_quotes(char *content, int *buffer, int *count)
{
    int     whitespace_pos;
    int     prev_whitespace_pos;

    prev_whitespace_pos = 0;
    whitespace_pos = get_whtsp_pos(content, prev_whitespace_pos);
    while (whitespace_pos != -1)
    {
        if (whitespace_pos - prev_whitespace_pos)
        {
            (*count)++;
            *buffer = 0;
        }
        while (content[whitespace_pos] && (content[whitespace_pos] == ' ' || content[whitespace_pos] == '\t'))
            whitespace_pos++;
        prev_whitespace_pos = whitespace_pos;
        whitespace_pos = get_whtsp_pos(content, prev_whitespace_pos);
    }
    if (whitespace_pos == -1)
        *buffer = 1;
}

int     get_words_count(t_token_list *current)
{
    int             count;
    int             buffer;
    t_word_data     *wd;

    buffer = 0;
    count = 0;
    if (!current->content[0]) // si avait juste "" ou ''
        return (1);
    wd = current->merged_words;
    if (!wd)
        return (no_merged_words_count(current));
    while (wd)
    {
        if (!wd->quotes)
        {
            wd_no_quotes(wd->content, &buffer, &count);
        }
        else
            buffer = 1;
        wd = wd->next;
    }
    if (buffer)
        count++;
    return (count);
}

void    add_to_cmd_list(t_cmd **last, t_cmd *new)
{
    if (!last)
        return ;
    if (!(*last))
        *last = new;
    else
    {
        (*last)->next = new;
        *last = new;
    }  
}

// peut regrouper cette fonction et random_filename() utilisée dans les here docs en une seule,
// en passant comme argument a la fonction si veut ecrire "/tmp/subshell_args_" ou "/tmp/here_doc_tmp_"
char    *random_subshell_fname(void)
{
    static size_t   files_count;
    char            *count;
    char            *filename;

    count = ft_itoa(files_count);
    filename = ft_strjoin("/tmp/subshell_args_", count);
    while (access(filename, F_OK | R_OK | W_OK) == 0)
    {
        free(filename);
        free(count);
        files_count++;
        count = ft_itoa(files_count);
        filename = ft_strjoin("/tmp/subshell_args_", count);
    }
    free(count);
    return (filename);
}

t_cmd *init_new_cmd(t_data *data)
{
    t_cmd *cmd;
    cmd = ft_calloc(sizeof(t_cmd), 1);
    if (!cmd)
    {
        perror("malloc ");
        return (NULL);
    }
    cmd->env = data->env;
    cmd->export_history = data->exp_history;
    init_redirections(&(cmd->red));
    return (cmd);
}


/* Retourne une liste chainée de pipelines, avec chaque {commande + redirections} 
regroupées dans un élément de type t_cmd.

Parse le flux de tokens :

Déclare et initialise une variable t_cmd.
Tant que ne tombe pas sur un opérateur de controle (ie un && ou un ||), 
met à jour cette commande avec les tokens que l'on lit :
    -> Dès que tombe sur un opérateur de redirection, met à jour l'attribut t_redirect de t_cmd
    -> Sinon, met à jour le nom et les arguments de la commande.
Dès que tombe sur un opérateur de controle :
    -> Met à jour l'attribut ctrl_op de la variable t_cmd actuelle.
    -> Ajoute la variable t_cmd actuelle à la liste de commandes.
    -> Déclare et initialise une nouvelle variable t_cmd.
    -> Répète les opérations ci-dessus jusqu'à tomber à nouveau sur un opérateur de 
       contrôle, ou jusqu'à être arrivé à la fin de la liste de tokens.


(echo un | (echo deux && echo trois))
*/

void    write_in_file(t_token_list **curr_tk, int fd_subshell)
{
    int             open_parentheses;
    t_token_list    *current_tk;

    open_parentheses = 1;
    current_tk = (*curr_tk)->next;
    while (current_tk)
    {
        if (current_tk->type == l_parenthesis)
            open_parentheses++;
        if (current_tk->type == r_parenthesis)
            open_parentheses--;
        if (!open_parentheses)
            break;
        if (write(fd_subshell, current_tk->content, current_tk->length) == -1)
            perror ("write ");
        current_tk = current_tk->next;
    }
    if (current_tk)
        current_tk = current_tk->next;
    *curr_tk = current_tk;   
}

void    set_subshell_attributs(t_cmd *current_cmd, char *subshell_filename)
{
    current_cmd->val.value = ft_strdup("minishell");
    current_cmd->val.args = malloc(sizeof(char *) * 3);
    if (!current_cmd->val.args)
    {
        perror("malloc ");
        return ;
    }
    current_cmd->val.args[0] = ft_strdup("minishell");
    current_cmd->val.args[1] = ft_strdup(subshell_filename);
    current_cmd->val.args[2] = NULL;
    free(subshell_filename);
    subshell_filename = NULL;
}

void    set_subshell(t_cmd *current_cmd, t_token_list **curr_tk)
{
    int             fd_subshell;
    char            *subshell_filename;
    t_token_list    *current_tk;
    
    if (!current_cmd || !curr_tk)
        return ;
    subshell_filename = random_subshell_fname();
    fd_subshell = open(subshell_filename, O_CREAT | O_WRONLY, 00700);
    if (fd_subshell == -1)
    {
        perror("open ");
        // free everything and return
    }
    write_in_file(curr_tk, fd_subshell);
    set_subshell_attributs(current_cmd, subshell_filename);
}

void    get_attribut(t_cmd *current_cmd, t_token_list **curr_tk, int *args_count, int *subshell)
{
    t_token_list    *current_tk;

    current_tk = *curr_tk;
    if (current_tk->type == l_io_redirect || current_tk->type == r_io_redirect)
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
        args_count += get_words_count(current_tk);
        current_tk = current_tk->next;
    }
    *curr_tk = current_tk;
}

void    set_simple_command(t_cmd *current_cmd, t_token_list **first_tk, t_token_list **cmd_start_tk, t_token_list **curr_tk)
{
    int             subshell;
    int             args_count;
    t_token_list    *current_tk;

    subshell = 0;
    args_count = 0;
    current_tk = *curr_tk;
    while (current_tk && current_tk->type != and_tk && current_tk->type != or_tk)
    {
        if (current_tk->type == l_io_redirect || current_tk->type == r_io_redirect)
        {
            update_redirect(current_cmd, current_tk);
            current_tk = current_tk->next->next;
        }
        else if (current_tk->type == l_parenthesis)
        {
            subshell = 1;
            set_subshell(current_cmd, &current_tk);
        }
        else
        {
            args_count += get_words_count(current_tk);
            current_tk = current_tk->next;
        }
        //get_attribut(current_cmd, &current_tk, &args_count, &subshell);
    }
    current_cmd->red.next_cmd = NULL;
    if (!subshell)
        set_command_attributs(&current_cmd, first_tk, *cmd_start_tk, args_count);
    *curr_tk = current_tk;
}

void    malloc_error()
{

}

t_cmd   *get_ast(t_data *data)
{
    int             subshell;
    int             args_count;
    t_token_list    *pipeline_start_tk;
    t_token_list    *cmd_start_tk;
    t_token_list    *current_tk;
    t_cmd           **ast;
    t_cmd           *pipeline_start_cmd;
    t_cmd           *current_cmd;
    
    ast = malloc(sizeof(t_cmd *));
    if (!ast)
    {
        malloc_error();
        return NULL;
    }
    *ast = init_new_cmd(data);
    if (!*ast)
        return (NULL);
    subshell = 0;
    pipeline_start_tk = *(data->first); 
    pipeline_start_cmd = *ast;

    //////dprintf(1, "in get_ast(), before the while(pipeline_start_tk)\n");
    
    while (pipeline_start_tk)
    {
        //////dprintf(1, "in get_ast(), in the while(pipeline_start_tk)\n");
        current_tk = pipeline_start_tk;
        current_cmd = pipeline_start_cmd;
        
        // tant que n'est ni un '&&' ni un '||'
        // ie peut avoir des pipes, mais ne touche ici pas à la variable next de t_cmd
        while (current_tk && current_tk->type != and_tk && (current_tk->type != or_tk || current_tk->length == 1))
        {
            //////dprintf(1, "in get_ast(), in the  while(not && and not ||), before calling set_simple_command()\n");
            cmd_start_tk = current_tk;
            set_simple_command(current_cmd, data->first, &cmd_start_tk, &current_tk);
            
            if (current_tk && current_tk->type == or_tk && current_tk->length == 1)
            {
                current_cmd->red.next_cmd = init_new_cmd(data);
                if (!current_cmd->red.next_cmd)
                {
                    perror("malloc ");
                    // free and return. Quel exit status ?
                }
                current_cmd = current_cmd->red.next_cmd;
                current_tk = current_tk->next;
            }
        }
        if (current_tk && is_a_ctrl_op(current_tk))
        {
            pipeline_start_cmd->ctrl = (current_tk->type == and_tk) * and + (current_tk->type == or_tk) * or;
            pipeline_start_cmd->next = init_new_cmd(data);
            pipeline_start_cmd = pipeline_start_cmd->next;
            if (!pipeline_start_cmd)
            {
                perror("malloc ");
                // free en cascade. Quel exit status ?
            }
            pipeline_start_tk = current_tk->next;
        }
        else // ie est arrivé à la fin de la liste de tokens -> ctrl_op = ';'
        {
            current_cmd->ctrl = pointvirgule;
            pipeline_start_tk = NULL;
        }
    }
    /* free_tokens(data->first); */
    t_cmd * test = *ast;
    free(ast);
    return (test);
}
