/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   priv.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: srapin <srapin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/23 00:02:00 by srapin            #+#    #+#             */
/*   Updated: 2023/05/23 00:23:02 by srapin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#ifndef PRIV_H
#define PRIV_H

#include "struct.h"
#include "lib.h"

//t_cmd
void init_cmd(t_cmd *cmd, char **envp);
void add_cmdval_to_cmd(t_cmd *cmd, char *str);

//t_file_struct
void init_file_struct(t_file *file_struct);

#endif