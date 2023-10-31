/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_library.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seonjo <seonjo@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/20 17:51:43 by seonjo            #+#    #+#             */
/*   Updated: 2023/10/31 19:32:39 by seonjo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

long long	philo_get_time(int sec, int usec)
{
	return ((long long)sec * 1000L + (long long)usec / 1000L);
}

int	philo_error(void)
{
	printf("error\n");
	return (1);
}

void	philo_join(t_philo *philos, int n)
{
	int	i;

	i = 1;
	while (i <= n)
		pthread_join(philos[i++].thread_id, NULL);
}

long long	philo_print(t_philo *philo, char *str)
{
	struct timeval	tv;
	long long		time;

	if (gettimeofday(&tv, NULL) != 0)
	{
		philo->arg->error = 1;
		return (-1);
	}
	else if (philo->arg->error || philo->arg->dead)
		return (-1);
	time = philo_get_time(tv.tv_sec, tv.tv_usec);
	if (printf("%lld %d %s", time - philo->arg->start_time, \
		philo->philo_num, str) == -1)
		philo->arg->error = 1;
	return (time);
}
