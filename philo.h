/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seonjo <seonjo@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/20 16:44:32 by seonjo            #+#    #+#             */
/*   Updated: 2023/10/30 18:28:47 by seonjo           ###   ########.fr       */
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
	int				number_of_philo;
	long long		time_to_die;
	long long		time_to_eat;
	long long		time_to_sleep;
	long long		start_time;
	int				eat_num_limit;
	int				is_have_eat_num_limit;
	pthread_mutex_t	*start_mutex;
}t_arg;

typedef struct s_philo
{
	pthread_t		thread_id;
	pthread_mutex_t	*left_fork;
	pthread_mutex_t	*right_fork;
	pthread_mutex_t	*print_mutex;
	pthread_mutex_t	*resouce_mutex;
	t_arg			*arg;
	int				philo_num;
	int				eat_num;
	int				dead;
	long long		last_eating_time;
}t_philo;

int			philo_error(void);
long long	philo_atoi(char *str);
char		*philo_itoa(long long n);
void		*philo_free(t_philo *philos, t_arg *arg, int n, int flag);
int			philo_strlen(char *str);
void		philo_action(t_philo *philo);
int			philo_print(t_philo *philo, long long time, char *str, int die);
long long	philo_print_mutex(t_philo *philo, char *str);
int			philo_monitoring(t_philo *philos, int num_of_philo);
void		philo_join(t_philo *philos, t_arg *arg);
void		philo_change_dead(t_philo *philo, int n);
int			philo_is_dead_n(t_philo *philo, int n);
long long	philo_get_time(int sec, int usec);
void		philo_chage_last_eat_time(t_philo *philo, long long time);
long long	philo_get_last_eat_time(t_philo *philo);

#endif
