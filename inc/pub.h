/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pub.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: srapin <srapin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/23 00:15:39 by srapin            #+#    #+#             */
/*   Updated: 2023/05/23 01:55:25 by srapin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PUB_H
#define PUB_H
#include "struct.h"

//file_struct
void init_file_struct_with_filename(t_file *file_struct, char *filename);
void init_file_struct_with_sep(t_file *file_struct, char *sep);
void init_file_struct_with_fd(t_file *file_struct, int fd);
t_file *create_file_struct_with_filename(char *filename);
t_file * create_file_struct_with_sep(char *sep);
t_file * create_file_struct_with_fd(int fd);
void set_fd(t_file * f_struct, int *fd, int flag);

//red
void add_in_redir_with_file_struct(t_cmd *cmd, t_file *file_struct);
void add_out_redir_with_file_struct(t_cmd *cmd, t_file *file_struct);
void add_err_redir_with_file_struct(t_cmd *cmd, t_file *file_struct);
void init_redirections(t_redirect *red);
t_redirect *create_redir();

//cmd
void init_cmd_and_add_val(t_cmd *cmd, char **envp, char *args);
t_cmd *create_cmd_with_val(char **envp, char *args);

//to_del
int count_cmds(t_cmd *first_cmd);

#endif