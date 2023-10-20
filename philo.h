/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seonjo <seonjo@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/20 16:44:32 by seonjo            #+#    #+#             */
/*   Updated: 2023/10/20 22:33:01 by seonjo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <sys/time.h>
# include <pthread.h>
# include <stdlib.h>

typedef struct s_arg
{
	long long	time_to_die;
	long long	time_to_eat;
	long long	time_to_sleep;
	int			eat_num_limit;
	int			is_have_eat_num_limit;
}t_arg;

typedef struct s_philo
{
	pthread_t		*thread_id;
	pthread_mutex_t	*left_fork;
	pthread_mutex_t	*right_fork;
	t_arg			*arg;
	int				eat_num;
	int				dead;
	long long		start_time;
	long long		last_eating_time;
}t_philo;

long long	philo_atoi(char *str);
void		*philo_error(t_philo *philos, t_arg *arg, int n);
char		*philo_itoa(long long n);
int			philo_strlen(char *str);

#endif
