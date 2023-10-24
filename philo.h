/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seonjo <seonjo@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/20 16:44:32 by seonjo            #+#    #+#             */
/*   Updated: 2023/10/24 22:12:10 by seonjo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <unistd.h>
# include <sys/time.h>
# include <pthread.h>
# include <stdlib.h>
# include <stdio.h>

typedef struct s_arg
{
	long long	time_to_die;
	long long	time_to_eat;
	long long	time_to_sleep;
	int			number_of_philo;
	int			eat_num_limit;
	int			is_have_eat_num_limit;
}t_arg;

typedef struct s_philo
{
	pthread_t		thread_id;
	pthread_mutex_t	*left_fork;
	pthread_mutex_t	*right_fork;
	pthread_mutex_t	*dead_mutex;
	pthread_mutex_t	*print_mutex;
	t_arg			*arg;
	int				philo_num;
	int				eat_num;
	int				dead;
	long long		start_time;
	long long		last_eating_time;
}t_philo;

int			philo_error(void);
long long	philo_atoi(char *str);
char		*philo_itoa(long long n);
void		*philo_free(t_philo *philos, t_arg *arg, int n, int flag);
int			philo_strlen(char *str);
void		philo_action(t_philo *philo);
int			philo_print(t_philo *philo, long long time, char *str);
void		philo_print_mutex(t_philo *philo, long long time, char *str);
int			philo_monitoring(t_philo *philos, t_arg *arg);
void		philo_join(t_philo *philos);
void		philo_change_dead(t_philo *philo, int n);
int			philo_is_dead_n(t_philo *philo, int n);

#endif
