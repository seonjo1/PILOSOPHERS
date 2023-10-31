/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seonjo <seonjo@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/20 16:44:32 by seonjo            #+#    #+#             */
/*   Updated: 2023/10/31 21:10:40 by seonjo           ###   ########.fr       */
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
	int				dead;
	int				error;
	int				end_philo;
	pthread_mutex_t	*start_mutex;
	pthread_mutex_t	*rsc_mutex;
}t_arg;

typedef struct s_philo
{
	pthread_t		thread_id;
	pthread_mutex_t	*left_fork;
	pthread_mutex_t	*right_fork;
	t_arg			*arg;
	int				start;
	int				philo_num;
	int				eat_num;
	long long		last_eating_time;
}t_philo;

long long	philo_atoi(char *str);
void		*philo_free_philos(t_philo *philos, t_arg *arg, int n, int flag);
void		*philo_free(void *pointer1, void *pointer2, void *pointer3);
long long	philo_get_time(int sec, int usec);
int			philo_error(void);
void		philo_join(t_philo *philos, int n);
long long	philo_print(t_philo *philo, char *str);
void		philo_monitoring(t_philo *philos, t_arg *arg, int num_of_philo);
void		philo_release_fork(t_philo *philo, int right);
void		philo_hold_fork(t_philo *philo, int right);
void		philo_eating(t_philo *philo, int sleep_time);
void		philo_sleeping(t_philo *philo, int sleep_time);
void		*philo_ready_philos(t_philo *philos, t_arg *arg, int p_num);

#endif
