/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_thread_fuc.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seonjo <seonjo@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/31 17:29:26 by seonjo            #+#    #+#             */
/*   Updated: 2023/10/31 20:47:21 by seonjo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <philo.h>

int	philo_timer(t_arg *arg, long long limit, long long time, int sleep_time)
{	
	struct timeval	tv;

	pthread_mutex_lock(arg->rsc_mutex);
	while (!arg->dead && limit > time)
	{
		pthread_mutex_unlock(arg->rsc_mutex);
		usleep(sleep_time);
		if (gettimeofday(&tv, NULL) != 0)
		{
			pthread_mutex_lock(arg->rsc_mutex);
			arg->error = 1;
			pthread_mutex_unlock(arg->rsc_mutex);
			return (-1);
		}
		time = philo_get_time(tv.tv_sec, tv.tv_usec);
		pthread_mutex_lock(arg->rsc_mutex);
	}
	pthread_mutex_unlock(arg->rsc_mutex);
	return (1);
}

void	philo_sleeping(t_philo *philo, int sleep_time)
{
	long long	limit;
	long long	time;

	pthread_mutex_lock(philo->arg->rsc_mutex);
	time = philo_print(philo, "is sleeping\n");
	if (time != -1)
	{
		limit = time + philo->arg->time_to_sleep;
		pthread_mutex_unlock(philo->arg->rsc_mutex);
		if (philo_timer(philo->arg, limit, time, sleep_time) == -1)
			return ;
		pthread_mutex_lock(philo->arg->rsc_mutex);
		philo_print(philo, "is thinking\n");
		pthread_mutex_unlock(philo->arg->rsc_mutex);
		usleep(200);
	}
	else
		pthread_mutex_unlock(philo->arg->rsc_mutex);
}

void	philo_eating(t_philo *philo, int sleep_time)
{
	long long	limit;
	long long	time;

	pthread_mutex_lock(philo->arg->rsc_mutex);
	time = philo_print(philo, "is eating\n");
	if (time != -1)
	{
		philo->last_eating_time = time;
		limit = time + philo->arg->time_to_eat;
		pthread_mutex_unlock(philo->arg->rsc_mutex);
		if (philo_timer(philo->arg, limit, time, sleep_time) == -1)
			return ;
		if (philo->arg->is_have_eat_num_limit == 1)
			philo->eat_num++;
	}
	else
		pthread_mutex_unlock(philo->arg->rsc_mutex);
}

void	philo_hold_fork(t_philo *philo, int right)
{
	if (right)
	{
		pthread_mutex_lock(philo->right_fork);
		pthread_mutex_lock(philo->arg->rsc_mutex);
		philo_print(philo, "has taken a fork\n");
		pthread_mutex_unlock(philo->arg->rsc_mutex);
	}
	else
	{
		pthread_mutex_lock(philo->left_fork);
		pthread_mutex_lock(philo->arg->rsc_mutex);
		philo_print(philo, "has taken a fork\n");
		pthread_mutex_unlock(philo->arg->rsc_mutex);
	}
}

void	philo_release_fork(t_philo *philo, int right)
{
	if (right)
		pthread_mutex_unlock(philo->right_fork);
	else
		pthread_mutex_unlock(philo->left_fork);
}
