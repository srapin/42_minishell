/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hlesny <hlesny@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/25 21:44:19 by Helene            #+#    #+#             */
/*   Updated: 2023/06/17 22:36:46 by hlesny           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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
    
    // removes the whitespaces that aren't in quotes in the current token 
    //  (whitespaces that could for example be a result of a variable expansion)
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
        {
            //dprintf(1, "in set_cmd_args(), current token = %s\n", curr_tk->content);
            prev_whitespace_pos = 0;
            whitespace_pos = get_whtsp_pos(curr_tk->content, prev_whitespace_pos);
            //dprintf(1, "\twd = %s, whitespace_pos = %d\n", curr_tk->content, whitespace_pos);
            while (whitespace_pos != -1)
            {
                tmp = buffer;
                substr = ft_substr(curr_tk->content, prev_whitespace_pos, whitespace_pos - prev_whitespace_pos);
                buffer = ft_strjoin(tmp, substr);
                //dprintf(1, "in set_cmd_args(), in while (whitespace_pos != -1)\n");
                if (*buffer) // ie si il n'est pas vide
                {
                    (*curr_cmd)->val.args[*i] = ft_strdup(buffer);
                    //dprintf(1, "in set_cmd_args(), added new arg : %s\n", buffer);
                    (*i)++;
                    free(buffer);
                    buffer = NULL;
                }
                free(substr);
                substr = NULL;
                //j = whitespace_pos;
                while (curr_tk->content[whitespace_pos] && (curr_tk->content[whitespace_pos] == ' ' || curr_tk->content[whitespace_pos] == '\t'))
                    whitespace_pos++;
                prev_whitespace_pos = whitespace_pos;
                whitespace_pos = get_whtsp_pos(curr_tk->content, prev_whitespace_pos);
                //dprintf(1, "\twd = %s, whitespace_pos = %d\n", curr_tk->content, whitespace_pos);
            }
            if (prev_whitespace_pos < curr_tk->length)
            {
                (*curr_cmd)->val.args[*i] = ft_substr(curr_tk->content, prev_whitespace_pos, curr_tk->length);
                (*i)++;
            }
            
        }
        else
        {
            //dprintf(1, "in set_cmd_args(), added new arg : %s\n", curr_tk->content);
            (*curr_cmd)->val.args[*i] = ft_strdup(curr_tk->content);
            (*i)++;
        }
        return ;
    }
    
    //dprintf(1, "in set_cmd_args()\n");
    // 16/06, 20h : rentre pas dans la boucle ?
    while (wd)
    {
        //dprintf(1, "\twd = %s, quotes = %d\n", wd->content, wd->quotes);
        if (!wd->quotes)
        {
            prev_whitespace_pos = 0;
            whitespace_pos = get_whtsp_pos(wd->content, prev_whitespace_pos);
            //dprintf(1, "\twd = %s, whitespace_pos = %d\n", wd->content, whitespace_pos);
            while (whitespace_pos != -1)
            {
                tmp = buffer;
                substr = ft_substr(wd->content, prev_whitespace_pos, whitespace_pos - prev_whitespace_pos);
                buffer = ft_strjoin(tmp, substr);
                if (*buffer) // ou if (whitespace_pos - prev_whitespace_pos) // ie si il n'est pas vide
                {
                    (*curr_cmd)->val.args[*i] = ft_strdup(buffer);
                    //dprintf(1, "in set_cmd_args(), added new arg : %s\n", buffer);
                    (*i)++;
                    free(buffer);
                    buffer = NULL;
                }
                free(substr);
                substr = NULL;
                j = whitespace_pos;
                while (wd->content[j] && (wd->content[j] == ' ' || wd->content[j] == '\t'))
                    j++;
                prev_whitespace_pos = j;
                whitespace_pos = get_whtsp_pos(wd->content, prev_whitespace_pos);
                //dprintf(1, "\twd = %s, whitespace_pos = %d\n", wd->content, whitespace_pos);
            }
            if (whitespace_pos == -1) // pas opti
            {
                tmp = buffer;
                substr = ft_substr(wd->content, prev_whitespace_pos, wd->length);
                buffer = ft_strjoin(tmp, substr);
            }
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
        //dprintf(1, "in set_cmd_args(), added new arg : %s\n", buffer);
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
    //dprintf(1, "in set_command_attributs\n");
    (*current)->val.args = malloc(sizeof(char *) * (args_count + 1));
    if (!(*current)->val.args)
    {
        perror("malloc ");
        return ;
    }
    while (current_tk && i < args_count) //while (current_tk && current_tk->type != and && current_tk->type != or)
    {
        if (current_tk->type == l_io_redirect || current_tk->type == r_io_redirect)
            current_tk = current_tk->next->next;
        else 
        {
            //dprintf(1, "before set_cmd_args()\n");
            //dprintf(1, "args_count = %d\n", args_count);
            set_cmd_args(current, current_tk, &i);
            //dprintf(1, "after set_cmd_args()\n");
            //tk_del_one(first_tk, current_tk); // est-ce que current_tk pointe encore au bon endroit après ça ?
            current_tk = current_tk->next;
        }
    }
    (*current)->val.args[i] = NULL;
    if (args_count) // le if est-il obligatoire ?
        (*current)->val.value = ft_strdup((*current)->val.args[0]);
}

int get_words_count(t_token_list *current)
{
    int             i;
    int             count;
    int             buffer; // 0 when empty, 1 when not
    int             whitespace_pos;
    int             prev_whitespace_pos;
    t_word_data     *wd;

    i = 0;
    buffer = 0;
    count = 0; // 0 ou 1 ?
    if (!current->content[0]) // si avait juste "" ou ''
        return (1);
    wd = current->merged_words;
    if (!wd)
    {
        if (!current->quotes)
        {
            //dprintf(1, "in get_words_count(), current token = %s\n", current->content);
            prev_whitespace_pos = 0;
            whitespace_pos = get_whtsp_pos(current->content, prev_whitespace_pos);
            //dprintf(1, "\twd = %s, whitespace_pos = %d\n", current->content, whitespace_pos);
            while (whitespace_pos != -1)
            {
                // tmp = buffer;
                // substr = ft_substr(current->content, prev_whitespace_pos, whitespace_pos - prev_whitespace_pos);
                // buffer = ft_strjoin(tmp, substr);
                if (whitespace_pos - prev_whitespace_pos) // ie si il n'est pas vide
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
        //dprintf(1, "in get_words_count(), count = %d\n", count);
        return (count);
    }
    while (wd)
    {
        //dprintf(1, "in get_words_count(), wd = %s\n", wd->content);
        if (!wd->quotes)
        {
            prev_whitespace_pos = 0;
            whitespace_pos = get_whtsp_pos(wd->content, prev_whitespace_pos);
            while (whitespace_pos != -1)
            {
                //dprintf(1, "in while (whitespace_pos != -1)\n");
                if (whitespace_pos - prev_whitespace_pos) // ie si le buffer n'est pas vide
                {
                    count++;
                    buffer = 0;
                }
                
                while (wd->content[whitespace_pos] && (wd->content[whitespace_pos] == ' ' || wd->content[whitespace_pos] == '\t'))
                    whitespace_pos++;
                //dprintf(1, "whitespace_pos = %d, prev_shitespace_pos = %d\n", whitespace_pos, prev_whitespace_pos);
                prev_whitespace_pos = whitespace_pos;
                //dprintf(1, "\twd = %s, prev_whitespace_pos = %d\n", wd->content, prev_whitespace_pos);
                whitespace_pos = get_whtsp_pos(wd->content, prev_whitespace_pos); // ou prev_whitespace_pos + 1 ?
                //dprintf(1, "\twd = %s, whitespace_pos = %d\n", wd->content, whitespace_pos);
            }
            if (whitespace_pos == -1) // pas opti
                buffer = 1;
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
    if (!(*last)) // si la liste est vide
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
        files_count++;
        count = ft_itoa(files_count);
        filename = ft_strjoin("/tmp/subshell_args_", count);
    }
    free(count);
    return (filename);
}

t_cmd *init_new_cmd(t_ht_hash_table *ht, t_list *exp_hist)
{
    t_cmd *cmd;
    cmd = ft_calloc(sizeof(t_cmd), 1);
    if (!cmd)
    {
        perror("malloc ");
        return (NULL);
    }
    cmd->env = ht;
    cmd->export_history = exp_hist;
    init_redirections(&(cmd->red));
    //init_cmd(cmd, envp); envp est en fait ht
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



void    set_subshell_attributs(t_cmd *current_cmd, t_token_list **curr_tk)
{
    int             i;
    int             open_parentheses;
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
    i = 0;
    open_parentheses = 1;
    current_tk = *curr_tk;
    //cmd_start_tk = current_tk;
    current_tk = current_tk->next;
    while (current_tk) // && current_tk->type != r_parenthesis
    {
        if (current_tk->type == l_parenthesis)
            open_parentheses++;
        if (current_tk->type == r_parenthesis)
            open_parentheses--;
        //dprintf(1, "in set_subshell(), current_tk = %s\n", current_tk->content);
        if (!open_parentheses)
            break;
        if (write(fd_subshell, current_tk->content, current_tk->length) == -1) // vérifier si current_tk->length est bien à jour !
            perror ("write ");
        current_tk = current_tk->next;
    }
    if (current_tk)
        current_tk = current_tk->next;
    *curr_tk = current_tk;
    //add_val_to_cmd(current_cmd, ft_strjoin("minishell ", subshell_filename));
    
    current_cmd->val.value = ft_strdup("minishell");
    
    current_cmd->val.args = malloc(sizeof(char *) * 3);
    if (!current_cmd->val.args)
    {
        perror("malloc ");
        // return ?
    }
    current_cmd->val.args[0] = ft_strdup("minishell");
    current_cmd->val.args[1] = ft_strdup(subshell_filename);
    current_cmd->val.args[2] = NULL;
    
    free(subshell_filename);
    subshell_filename = NULL;
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
        //dprintf(1, "in set_simple_command(), current_tk = %s\n", current_tk->content);
        
        if (current_tk->type == l_io_redirect || current_tk->type == r_io_redirect)
        {
            update_redirect(current_cmd, current_tk);
            current_tk = current_tk->next->next;
        }
        else if (current_tk->type == l_parenthesis)
        {
            subshell = 1;
            set_subshell_attributs(current_cmd, &current_tk);
        }
        else
        {
            //dprintf(1, "in set_simple_command(), before calling get_words_count()\n");
            args_count += get_words_count(current_tk);
            //dprintf(1, "in set_simple_command(), after calling get_words_count()\n");
            current_tk = current_tk->next;
        }
    }
    current_cmd->red.next_cmd = NULL;
    //dprintf(1, "in set_simple_command(), just before calling set_command_attributs()\n");
    if (!subshell)
        set_command_attributs(&current_cmd, first_tk, *cmd_start_tk, args_count);
    *curr_tk = current_tk;
}

t_cmd   *get_ast(t_ht_hash_table *ht, t_token_list **first_tk, t_list *exp_hist)
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
    *ast = init_new_cmd(ht, exp_hist);
    if (!*ast)
        return (NULL);
    subshell = 0;
    pipeline_start_tk = *first_tk; 
    pipeline_start_cmd = *ast;

    //dprintf(1, "in get_ast(), before the while(pipeline_start_tk)\n");
    
    while (pipeline_start_tk)
    {
        //dprintf(1, "in get_ast(), in the while(pipeline_start_tk)\n");
        current_tk = pipeline_start_tk;
        current_cmd = pipeline_start_cmd;
        
        // tant que n'est ni un '&&' ni un '||'
        // ie peut avoir des pipes, mais ne touche ici pas à la variable next de t_cmd
        while (current_tk && current_tk->type != and_tk && (current_tk->type != or_tk || current_tk->length == 1))
        {
            //dprintf(1, "in get_ast(), in the  while(not && and not ||), before calling set_simple_command()\n");
            cmd_start_tk = current_tk;
            set_simple_command(current_cmd, first_tk, &cmd_start_tk, &current_tk);
            
            if (current_tk && current_tk->type == or_tk && current_tk->length == 1)
            {
                current_cmd->red.next_cmd = init_new_cmd(ht, exp_hist);
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
            pipeline_start_cmd->next = init_new_cmd(ht, exp_hist);
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
    free_tokens(first_tk);
    t_cmd * test = *ast;
    free(ast);
    return (test);
}
