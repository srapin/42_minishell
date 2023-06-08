/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   defines.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Helene <Helene@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/28 17:25:15 by Helene            #+#    #+#             */
/*   Updated: 2023/06/08 15:38:40 by Helene           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef DEFINES_H
# define DEFINES_H

// ?
# define MAX_CHAR                   1000 /* Max number of characters to be supported */
# define MAX_CMD                    100 /* Max number of commands to be supported */

# define GETCWD_INITIAL_BUF_SIZE    50 /* Initial buffer size for getcwd()'s call*/

# define EXIT_OK                    0
# define NO_FILE_OR_DIR             1 // "no such file or directory" error's exit status
# define INVALID_VAR_ID             1 // "not a valid identifier"'s export error message
# define SYNTAX_ERROR               2
//# define BUILTIN_INCORRECT_USAGE    2
# define CMD_NOT_EXECUTABLE         126 // cannot execute the command (ex : `./` : is a directory)
# define CMD_NOT_FOUND              127


#endif