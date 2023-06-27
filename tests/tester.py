# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    tester.py                                          :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: srapin <srapin@student.42.fr>              +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/06/27 03:09:05 by srapin            #+#    #+#              #
#    Updated: 2023/06/27 03:22:32 by srapin           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

import pandas as pd

def main():
    err_mini = open("err_pos");
    out_mini = open("out_pos");
    err_bash = open("err_pos");
    out_bash = open("out_pos");
    
    err_bash.close();
    out_bash.close();
    err_mini.close();
    out_mini.close();
    pass

if __name__ == "__main__" :
   main()