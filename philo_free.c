/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_free.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seonjo <seonjo@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/31 17:27:14 by seonjo            #+#    #+#             */
/*   Updated: 2023/10/31 18:16:56 by seonjo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	philo_free_mutex(pthread_mutex_t *mutex)
{
	pthread_mutex_destroy(mutex);
	free(mutex);
}

void	*philo_free(void *pointer1, void *pointer2, void *pointer3)
{
	if (pointer1 != NULL)
		free(pointer1);
	if (pointer2 != NULL)
		free(pointer2);
	if (pointer3 != NULL)
		free(pointer3);
	return (NULL);
}

void	philo_free_arg(t_arg *arg)
{
	philo_free_mutex(arg->start_mutex);
	philo_free_mutex(arg->rsc_mutex);
	free(arg);
}

void	*philo_free_philos(t_philo *philos, t_arg *arg, int n, int flag)
{
	int	i;

	philo_free_arg(arg);
	i = 1;
	while (i <= n)
		philo_free_mutex(philos[i++].right_fork);
	if (flag)
		free(philos[i].right_fork);
	free(philos);
	return (NULL);
}
