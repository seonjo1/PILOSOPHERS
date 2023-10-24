/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_monitoring.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seonjo <seonjo@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/24 17:52:02 by seonjo            #+#    #+#             */
/*   Updated: 2023/10/24 21:53:51 by seonjo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	philo_find_err(t_philo *philos)
{
	int	flag;
	int	i;

	i = 1;
	flag = 0;
	while (i <= philos->arg->number_of_philo)
	{
		if (philo_is_dead_n(&(philos[i]), 0))
			philo_change_dead(&(philos[i]), 1);
		else if (philo_is_dead_n(&(philos[i]), 2))
			flag = 1;
		i++;
	}
	return (flag);
}

int	philo_monitoring(t_philo *philos, t_arg *arg)
{
	struct timeval	tv;
	int				err_flag;
	int				finish_flag;
	int				i;

	if (gettimeofday(&tv, NULL) != 0)
		philo_is_dead_n(&(philos[1]), 2);
	err_flag = 0;
	while (1)
	{
		finish_flag = 1;
		i = 1;
		while (i <= arg->number_of_philo)
		{
			if (philo_is_dead_n(&(philos[i]), 0))
				finish_flag = 0;
			if (philo_is_dead_n(&(philos[i]), 1))
			{
				err_flag = 1;
				pthread_mutex_lock(philos[i].print_mutex);
				if (philo_find_err(philos) == 1)
					err_flag = 2;
				else
					philo_print(&(philos[i]), tv.tv_sec * 1000, "died\n");
			}
			else if (philo_is_dead_n(&(philos[i]), 2))
			{
				if (err_flag != 1)
					pthread_mutex_lock(philos[i].print_mutex);
				err_flag = 2;
				break ;
			}
		}
		if (err_flag != 0 || finish_flag == 1)
			break ;
	}
	err_flag = philo_find_err(philos);
	return (err_flag);
}
