/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Helene <Helene@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/03 02:12:27 by Helene            #+#    #+#             */
/*   Updated: 2023/06/08 23:34:51 by Helene           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
Parser : part of our Command Line Interpreter that calls the lexical scanner to retrieve tokens.
It then constructs an Abstract Syntax Tree, or AST, out of these tokens. 
This AST is what we’ll pass to the executor to be executed.
*/


// ((cat out) && ls | grep "c" | (wc -l)) || echo "hello world" 
// env | grep "a"$test | wc -l && ls


/*
Ensures the command abides to the following syntax rules :
    - No '|' at the very beginning or end of the command line, and of each command list (ie check between () as well)
    - No "&&" or "||" at the very beginning or end of the command line, and of each command list (ie check between () as well)
    - No more than two consecutives '<' or '>'
    - For '<' and '>' symbols : if what follows isn't a word, a dollar or an asterisk : syntax error
    
Check for unclosed parentheses
Check for unclosed quotes. 

((cat out) && ls | (wc -l)) || echo "hello world" 
*/


void    print_type(t_token_list *current)
{
        if (current->type == word)
            printf("current->type = word \n");
        else if (current->type == simple_quote)
            printf("current->type = simple quote\n");
        else if (current->type == double_quote)
            printf("current->type = double quote \n");
        else if (current->type == r_parenthesis)
            printf("current->type = )\n");
        else if (current->type == l_parenthesis)
            printf("current->type = (\n");
        else if (current->type == r_io_redirect)
            printf("current->type = >\n");
        else if (current->type == l_io_redirect)
            printf("current->type = <\n");
        else if (current->type == and)
            printf("current->type = &\n");
        else if (current->type == or)
            printf("current->type = |\n");
        else if (current->type == whitespace)
            printf("current->type = whitespace\n");
        current = current->next;
}

void print_tokens(t_token_list *first)
{
    t_token_list *current;

    current = first;
    while (current)
    {
        print_type(current);
        // print_merged_words(current);
        if (!*(current->content))
            printf("current->content = \'0\'\n\n");
        else
            printf("current->content = %s\n\n", current->content);
        current = current->next;
    }
    printf("----------------------\n\n");
}

void    print_ast(t_cmd *ast)
{
    t_cmd *current_pipeline;
    t_cmd *current_simple_cmd;

    current_pipeline = ast;
    while (current_pipeline)
    {
        current_simple_cmd = current_pipeline;
        while (current_simple_cmd)
        {
            // print command's name and arguments/options
            printf("command : \n");
            printf("\tname : %s\n\targs : \n", current_simple_cmd->val.value);
            for (int i = 0; current_simple_cmd->val.args[i]; i++)
                printf("\t\targ %d : %s\n", i, current_simple_cmd->val.args[i]);
        
            // print in and out streams 
            // printf("instream :\n");
            // for (t_list *curr_in = current_simple_cmd->red.in_list; curr_in; curr_in = curr_in->next)
            //     printf("flag : %d\nfile name : %s\n", curr_in->content->flag, curr_in->content->name);
            // printf("outstream : \n");
            // for (t_list *curr_out = current_simple_cmd->red.out_list; curr_out; curr_out = curr_out->next)
            //     printf("flag : %d\nfile name : %s\n", curr_out->content->flag, curr_out->content->name);
            
            // print if a pipe follows this command
            printf("Pipe ? ");
            if (current_simple_cmd->red.next_cmd)
                printf("Yes\n");
            else 
                printf("No\n");
            
            current_simple_cmd = current_simple_cmd->red.next_cmd;
        }
        if (current_pipeline->ctrl == and)
            printf("control_op = '&&' \n");
        else if (current_pipeline->ctrl == or)
            printf("control_op = '||' \n");
        else
            printf("control_op = ';' \n");
        current_pipeline = current_pipeline->next;
    }
}

void    parse(t_ht_hash_table *ht, t_token_list *first)
{    
    //print_tokens(first);
    check_pipeline_list(&first);
    
    perform_variable_exp(ht, &first);
    //print_tokens(first);
    
    // tej les quotes en regroupant les <word> qui se suivent et ne sont pas 
    // séparés par des whitespaces ou des operateurs de controle autres que des quotes.
    // Cela ne veut pas dire que cela enleve les possibles nouveaux whitespaces 
    // dans un <word>, apparus suite à l'expansion de variables.
    // On ne fait cela (traites les chaines de caractères  qu'après avoir géré les wildcards et avoir
    // séparé les tokens entre redirections et commandes
    delete_quotes(&first);
    group_words(&first);
    //print_tokens(first);

    perform_wildcard_exp(ht, &first);
    //print_tokens(first);
    
    
    set_here_docs(ht, &first);
    //print_tokens(first);

    t_cmd *ast = get_ast(ht, &first);
    print_ast(ast);
    
    // for (t_token_list *current = first; current; current = current->next)
    // {
    //     if (current->type == l_io_redirect && current->length == 2)
    //     {
    //         int fd = open(current->next->content, O_RDONLY);
    //         if (fd == -1)
    //             perror("open ");
    //         char *line = get_next_line(fd);
    //         while (line)
    //         {
    //             printf("%s\n", line);
    //             free(line);
    //             line = get_next_line(fd);
    //         }
    //         close(fd);
    //     }
    //     printf("\n\n");
    // }
}