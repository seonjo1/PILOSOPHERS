# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: seonjo <seonjo@student.42.fr>              +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/10/19 17:46:49 by seonjo            #+#    #+#              #
#    Updated: 2023/10/30 20:05:06 by seonjo           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = philo
CC = cc
CFLAGS = -Wall -Wextra -Werror -g -fsanitize=thread
SRCS = philo_main.c philo_atoi.c philo_itoa.c philo_library.c philo_monitoring.c philo_thread.c
# SRCS2 = ft_printf_number_bonus.c freeing_bonus.c ft_printf_pointer_bonus.c ft_printf_bonus.c ft_split_bonus.c main_bonus.c get_next_line_utils_bonus.c exe_cmd_bonus.c get_next_line_bonus.c library_bonus.c here_doc_bonus.c
OBJS = $(SRCS:.c=.o)
# OBJS2 = $(SRCS2:.c=.o)

all : $(NAME)

$(NAME) : $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(NAME)
#	 rm -f $(OBJS2)

# bonus : $(OBJS2)
# 	$(CC) $(CFLAGS) $(OBJS2) -o $(NAME)
# 	rm -f $(OBJS)
# 	@touch bonus

%.o : %.c
	$(CC) $(CFLAGS) -I . -c $<

clean :
	rm -f $(OBJS)
#	rm -f $(OBJS2)
# 	rm -f bonus

fclean : clean
	rm -f $(NAME)

re : fclean all

.PHONY : all clean fclean re