/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_monitoring.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seonjo <seonjo@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/24 17:52:02 by seonjo            #+#    #+#             */
/*   Updated: 2023/10/31 19:57:22 by seonjo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	philo_state_check(t_philo *philo, t_arg *arg)
{
	struct timeval	tv;
	long long		now;
	long long		last_eat_time;
	long long		dead_time;

	if (gettimeofday(&tv, NULL) != 0)
		arg->error = 1;
	if (arg->error)
	{
		philo_error();
		return (1);
	}
	else
	{
		last_eat_time = philo->last_eating_time;
		now = philo_get_time(tv.tv_sec, tv.tv_usec);
		if ((now - last_eat_time) >= arg->time_to_die)
		{
			dead_time = now - arg->start_time;
			arg->dead = 1;
			printf("%lld %d died\n", dead_time, philo->philo_num);
			return (1);
		}
		return (0);
	}
}

void	philo_initiate_last_eat_time(t_philo *philos, int num_of_philo)
{
	int	i;

	pthread_mutex_lock(philos[1].arg->rsc_mutex);
	i = 1;
	while (i <= num_of_philo)
	{
		if (philos[i].last_eating_time == 0)
			philos[i].last_eating_time = philos[i].arg->start_time;
		i++;
	}
	pthread_mutex_unlock(philos[1].arg->rsc_mutex);
}

void	philo_monitoring(t_philo *philos, t_arg *arg, int num_of_philo)
{
	int	i;

	philo_initiate_last_eat_time(philos, num_of_philo);
	while (1)
	{
		i = 1;
		while (i <= num_of_philo)
		{
			pthread_mutex_lock(arg->rsc_mutex);
			if (philo_state_check(&(philos[i]), arg) == 1)
			{
				pthread_mutex_unlock(arg->rsc_mutex);
				return ;
			}
			pthread_mutex_unlock(arg->rsc_mutex);
			i++;
		}
		pthread_mutex_lock(arg->rsc_mutex);
		if (arg->end_philo == num_of_philo)
			return ;
		pthread_mutex_unlock(arg->rsc_mutex);
	}
}
