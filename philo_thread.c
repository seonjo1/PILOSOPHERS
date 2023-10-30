/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_thread.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seonjo <seonjo@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/24 17:32:04 by seonjo            #+#    #+#             */
/*   Updated: 2023/10/30 20:07:16 by seonjo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	philo_sleeping(t_philo *philo, int sleep_time)
{
	struct timeval	tv;
	long long		limit;
	long long		time;

	if (gettimeofday(&tv, NULL) != 0)
		philo_change_dead(philo, 2);
	time = philo_print_mutex(philo, "is sleeping\n");
	if (time > 0)
	{
		limit = time + philo->arg->time_to_sleep;
		while (philo_is_dead_n(philo, 0) && limit > time)
		{
			usleep(sleep_time);
			if (gettimeofday(&tv, NULL) != 0)
				philo_change_dead(philo, 2);
			time = philo_get_time(tv.tv_sec, tv.tv_usec);
		}
		philo_print_mutex(philo, "is thinking\n");
		usleep(200);
	}
}

void	philo_eating(t_philo *philo, int sleep_time)
{
	struct timeval	tv;
	long long		limit;
	long long		time;

	if (gettimeofday(&tv, NULL) != 0)
		philo_change_dead(philo, 2);
	time = philo_print_mutex(philo, "is eating\n");
	if (time > 0)
	{
		philo_chage_last_eat_time(philo, time);
		limit = time + philo->arg->time_to_eat;
		while (philo_is_dead_n(philo, 0) && limit > time)
		{
			usleep(sleep_time);
			if (gettimeofday(&tv, NULL) != 0)
				philo_change_dead(philo, 2);
			time = philo_get_time(tv.tv_sec, tv.tv_usec);
		}
		if (philo->arg->is_have_eat_num_limit == 1)
			philo->eat_num++;
	}
}

void	philo_hold_fork(t_philo *philo, int right)
{
	if (philo->arg->number_of_philo == 1)
		right = !right;
	if (right)
	{
		pthread_mutex_lock(philo->right_fork);
		philo_print_mutex(philo, "has taken a fork\n");
	}
	else
	{
		pthread_mutex_lock(philo->left_fork);
		philo_print_mutex(philo, "has taken a fork\n");
	}
}

void	philo_release_fork(t_philo *philo, int right)
{
	if (philo->arg->number_of_philo == 1)
		right = !right;
	if (right)
		pthread_mutex_unlock(philo->right_fork);
	else
		pthread_mutex_unlock(philo->left_fork);
}

void	philo_action(t_philo *philo)
{
	pthread_mutex_lock(philo->arg->start_mutex);
	pthread_mutex_unlock(philo->arg->start_mutex);
	if (philo->philo_num % 2 == 0)
	{
		philo_print_mutex(philo, "is thinking\n");
		usleep((philo->arg->time_to_eat / 3) * 1000);
	}
	philo->last_eating_time = philo->arg->start_time;
	while (philo_is_dead_n(philo, 0))
	{
		philo_hold_fork(philo, philo->philo_num % 2);
		philo_hold_fork(philo, !(philo->philo_num % 2));
		philo_eating(philo, philo->arg->number_of_philo);
		philo_release_fork(philo, !(philo->philo_num % 2));
		philo_release_fork(philo, philo->philo_num % 2);
		if (philo->arg->is_have_eat_num_limit == 1 && \
			philo->arg->eat_num_limit == philo->eat_num)
			break ;
		philo_sleeping(philo, philo->arg->number_of_philo);
	}
	if (philo_is_dead_n(philo, 0))
		philo_change_dead(philo, 3);
}
