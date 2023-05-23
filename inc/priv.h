/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   priv.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: srapin <srapin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/23 00:02:00 by srapin            #+#    #+#             */
/*   Updated: 2023/05/23 02:23:28 by srapin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#ifndef PRIV_H
#define PRIV_H

#include "struct.h"
#include "lib.h"

//t_cmd
void init_cmd(t_cmd *cmd, char **envp);
void add_cmdval_to_cmd(t_cmd *cmd, char *str);
void add_args_to_cmd(t_cmd *cmd, char *args);//do not use!
void add_value_to_cmd(t_cmd *cmd, char *val); //do not use!

//t_file_struct
void init_file_struct(t_file *file_struct);

#endif