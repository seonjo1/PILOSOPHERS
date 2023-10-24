/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_thread.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seonjo <seonjo@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/24 17:32:04 by seonjo            #+#    #+#             */
/*   Updated: 2023/10/24 21:55:28 by seonjo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	philo_sleeping(t_philo *philo, int sleep_time)
{
	struct timeval	now;
	long long		limit;
	long long		time;

	if (gettimeofday(&now, NULL) != 0)
		philo_change_dead(philo, 2);
	if (philo_is_dead_n(philo, 0))
	{
		time = now.tv_sec * 1000;
		philo_print_mutex(philo, time, "is sleeping\n");
		limit = time + philo->arg->time_to_sleep;
		while (philo_is_dead_n(philo, 0) && limit > time)
		{
			usleep(sleep_time);
			if (gettimeofday(&now, NULL) != 0)
				philo_change_dead(philo, 2);
			time = now.tv_sec * 1000;
		}
	}
}

void	philo_eating(t_philo *philo, int sleep_time)
{
	struct timeval	now;
	long long		limit;
	long long		time;

	if (gettimeofday(&now, NULL) != 0)
		philo_change_dead(philo, 2);
	if (philo_is_dead_n(philo, 0))
	{
		time = now.tv_sec * 1000;
		philo_print_mutex(philo, time, "is eating\n");
		limit = time + philo->arg->time_to_eat;
		while (philo_is_dead_n(philo, 0) && limit > time)
		{
			usleep(sleep_time);
			if (gettimeofday(&now, NULL) != 0)
				philo_change_dead(philo, 2);
			time = now.tv_sec * 1000;
		}
		if (philo->arg->is_have_eat_num_limit == 1)
			philo->eat_num++;
	}
}

void	philo_hold_fork(t_philo *philo, int right)
{
	struct timeval	tv;

	if (gettimeofday(&tv, NULL) != 0)
		philo_change_dead(philo, 2);
	if (right)
	{
		pthread_mutex_lock(philo->right_fork);
		if (philo_is_dead_n(philo, 0))
			philo_print_mutex(philo, (long long)(tv.tv_sec * 1000), \
						"has taken a fork\n");
	}
	else
	{
		pthread_mutex_lock(philo->left_fork);
		if (philo_is_dead_n(philo, 0))
			philo_print_mutex(philo, (long long)(tv.tv_sec * 1000), \
						"has taken a fork\n");
	}
}

void	philo_release_fork(t_philo *philo, int right)
{
	if (right)
		pthread_mutex_unlock(philo->right_fork);
	else
		pthread_mutex_lock(philo->left_fork);
}

void	philo_action(t_philo *philo)
{
	struct timeval	tv;

	while (philo_is_dead_n(philo, 0))
	{
		if (gettimeofday(&tv, NULL) != 0)
			philo_change_dead(philo, 2);
		else
			philo_print_mutex(philo, tv.tv_sec * 1000, "is thinking\n");
		philo_hold_fork(philo, philo->philo_num % 2);
		philo_hold_fork(philo, philo->philo_num % 2 + 1);
		philo_eating(philo, philo->arg->number_of_philo);
		philo_release_fork(philo, philo->philo_num % 2 + 1);
		philo_release_fork(philo, philo->philo_num % 2);
		philo_sleeping(philo, philo->arg->number_of_philo);
		if (philo->arg->is_have_eat_num_limit == 1 && \
			philo->arg->eat_num_limit == philo->eat_num)
			break ;
	}
	if (philo_is_dead_n(philo, 0))
		philo_change_dead(philo, 3);
}
