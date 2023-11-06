/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_thread_start.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seonjo <seonjo@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/24 17:32:04 by seonjo            #+#    #+#             */
/*   Updated: 2023/11/06 11:21:31 by seonjo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	philo_cycle(t_philo *philo)
{
	pthread_mutex_lock(philo->arg->rsc_mutex);
	while (!philo->arg->dead && !philo->arg->error)
	{
		pthread_mutex_unlock(philo->arg->rsc_mutex);
		philo_hold_fork(philo, philo->philo_num % 2);
		philo_hold_fork(philo, !(philo->philo_num % 2));
		philo_eating(philo, philo->arg->number_of_philo);
		philo_release_fork(philo, !(philo->philo_num % 2));
		philo_release_fork(philo, philo->philo_num % 2);
		if (philo->arg->is_have_eat_num_limit == 1 && \
			philo->arg->eat_num_limit == philo->eat_num)
		{
			pthread_mutex_lock(philo->arg->rsc_mutex);
			philo->arg->end_philo++;
			break ;
		}
		philo_sleeping(philo, philo->arg->number_of_philo);
		pthread_mutex_lock(philo->arg->rsc_mutex);
	}
	pthread_mutex_unlock(philo->arg->rsc_mutex);
}

void	philo_exception_case(t_philo *philo)
{
	pthread_mutex_lock(philo->arg->rsc_mutex);
	philo_print(philo, "is thinking\n");
	pthread_mutex_unlock(philo->arg->rsc_mutex);
	philo_hold_fork(philo, philo->philo_num % 2);
	usleep((philo->arg->time_to_die) * 1000);
	philo_release_fork(philo, philo->philo_num % 2);
}

void	philo_action(t_philo *philo)
{
	pthread_mutex_lock(philo->arg->start_mutex);
	pthread_mutex_lock(philo->arg->rsc_mutex);
	philo->last_eating_time = philo->arg->start_time;
	philo->start = 1;
	pthread_mutex_unlock(philo->arg->rsc_mutex);
	pthread_mutex_unlock(philo->arg->start_mutex);
	if (philo->arg->number_of_philo == 1)
	{
		philo_exception_case(philo);
		return ;
	}
	if (philo->philo_num % 2 == 0 || (philo->arg->number_of_philo % 2 == 1 && \
		philo->philo_num == philo->arg->number_of_philo))
	{
		pthread_mutex_lock(philo->arg->rsc_mutex);
		philo_print(philo, "is thinking\n");
		pthread_mutex_unlock(philo->arg->rsc_mutex);
		if (philo->arg->time_to_die > philo->arg->time_to_eat)
			usleep(philo->arg->time_to_eat * 200);
		else
			usleep((philo->arg->time_to_die) * 1000);
	}
	philo_cycle(philo);
}

void	philo_start_philos(t_arg *arg)
{
	struct timeval	tv;

	if (gettimeofday(&tv, NULL) != 0)
	{
		arg->start_time = 0;
		arg->error = 1;
	}
	else
		arg->start_time = philo_get_time(tv.tv_sec, tv.tv_usec);
}

void	*philo_ready_philos(t_philo *philos, t_arg *arg, int p_num)
{
	int	i;

	pthread_mutex_lock(arg->start_mutex);
	i = 1;
	while (i <= p_num)
	{
		if (pthread_create(&(philos[i].thread_id), NULL, (void *)philo_action, \
		&philos[i]) != 0)
		{
			pthread_mutex_unlock(arg->start_mutex);
			philo_join(philos, i - 1);
			return (philo_free_philos(philos, arg, p_num, 0));
		}
		i++;
	}
	philo_start_philos(arg);
	pthread_mutex_unlock(arg->start_mutex);
	return (philos);
}
