/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_monitoring.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seonjo <seonjo@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/24 17:52:02 by seonjo            #+#    #+#             */
/*   Updated: 2023/10/30 18:28:35 by seonjo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	philo_find_err(t_philo *philos, int num)
{
	int	flag;
	int	i;

	i = 1;
	flag = 0;
	while (i <= num)
	{
		if (philo_is_dead_n(&(philos[i]), 0))
		{
			philo_change_dead(&(philos[i]), 1);
			// pthread_mutex_lock(philos[1].print_mutex);
			// write(1, "philo ", 6);
			// char *str = philo_itoa(i);
			// write(1, str, philo_strlen(str));
			// write(1, " is dead\n", 9);
			// pthread_mutex_unlock(philos[1].print_mutex);
		}
		else if (philo_is_dead_n(&(philos[i]), 2))
			flag = 1;
		i++;
	}
	return (flag);
}

long long	philo_die_check(t_philo *philo)
{
	struct timeval	tv;
	long long		now;
	long long		last_eat_time;

	if (gettimeofday(&tv, NULL) != 0)
		philo_change_dead(philo, 2);
	else
	{
		last_eat_time = philo_get_last_eat_time(philo);
		now = philo_get_time(tv.tv_sec, tv.tv_usec);
		if ((now - last_eat_time) >= philo->arg->time_to_die)
			philo_change_dead(philo, 1);
		return (now);
	}
	return (1);
}

int	philo_check_one_cycle(t_philo *philos, int num_of_philo, int i, int *flag)
{
	int			finish_flag;
	int			exit_flag;
	long long	time;

	finish_flag = 1;
	exit_flag = 0;
	while (i <= num_of_philo)
	{
		time = philo_die_check(&(philos[i]));
		if (philo_is_dead_n(&(philos[i]), 0))
			finish_flag = 0;
		else if (philo_is_dead_n(&(philos[i]), 3) == 0)
		{
			*flag = philo_find_err(philos, num_of_philo);
			pthread_mutex_lock(philos[i].print_mutex);
			exit_flag = 1;
			if (*flag == 0)
				philo_print(&(philos[i]), time, "died\n", 1);
			pthread_mutex_unlock(philos[i].print_mutex);
			break ;
		}
		i++;
	}
	return (finish_flag | exit_flag);
}

int	philo_monitoring(t_philo *philos, int num_of_philo)
{
	int	flag;
	int	i;

	i = 1;
	while (i <= num_of_philo)
	{
		philos[i].last_eating_time = philos[i].arg->start_time;
		i++;
	}
	flag = 0;
	while (1)
	{
		if (philo_check_one_cycle(philos, num_of_philo, 1, &flag) == 1)
		{
			pthread_mutex_destroy(philos[1].arg->start_mutex);
			free(philos[1].arg->start_mutex);
			return (flag);
		}
	}
}
