/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_main.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seonjo <seonjo@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/20 16:43:58 by seonjo            #+#    #+#             */
/*   Updated: 2023/11/01 11:13:19 by seonjo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	*philo_arg_check(t_arg *arg)
{
	if (arg->number_of_philo <= 0 || arg->time_to_die <= 0 || \
		arg->time_to_eat <= 0 || arg->time_to_sleep <= 0 || \
		(arg->is_have_eat_num_limit && arg->eat_num_limit <= 0))
	{
		philo_free(arg->start_mutex, arg->rsc_mutex, arg);
		return (NULL);
	}
	arg->dead = 0;
	arg->error = 0;
	arg->end_philo = 0;
	return (arg);
}

void	*philo_arg_setting(int argc, char **argv)
{
	t_arg	*arg;

	arg = malloc(sizeof(t_arg));
	if (arg == NULL)
		return (NULL);
	arg->start_mutex = malloc(sizeof(pthread_mutex_t));
	if (arg->start_mutex == NULL)
		return (philo_free(arg, NULL, NULL));
	arg->rsc_mutex = malloc(sizeof(pthread_mutex_t));
	if (arg->rsc_mutex == NULL)
		return (philo_free(arg->start_mutex, arg, NULL));
	arg->number_of_philo = philo_atoi(argv[1]);
	arg->time_to_die = philo_atoi(argv[2]);
	arg->time_to_eat = philo_atoi(argv[3]);
	arg->time_to_sleep = philo_atoi(argv[4]);
	if (argc == 6)
	{
		arg->eat_num_limit = philo_atoi(argv[5]);
		arg->is_have_eat_num_limit = 1;
	}
	else
		arg->is_have_eat_num_limit = 0;
	return (philo_arg_check(arg));
}

void	*philo_setting_philo(t_philo *philos, t_arg *arg, int i)
{
	philos[i].start = 0;
	philos[i].last_eating_time = 0;
	philos[i].philo_num = i;
	philos[i].arg = arg;
	philos[i].eat_num = 0;
	philos[i].right_fork = malloc(sizeof(pthread_mutex_t));
	if (philos[i].right_fork == NULL)
		return (philo_free_philos(philos, arg, i - 1, 0));
	if (pthread_mutex_init(philos[i].right_fork, NULL) != 0)
		return (philo_free_philos(philos, arg, i - 1, 1));
	return (philos);
}

void	*philo_make_philos(t_philo *philos, t_arg *arg)
{
	int	i;

	if (pthread_mutex_init(arg->start_mutex, NULL) != 0)
		return (philo_free(arg->start_mutex, arg->rsc_mutex, arg));
	if (pthread_mutex_init(arg->rsc_mutex, NULL) != 0)
	{
		pthread_mutex_destroy(arg->start_mutex);
		return (philo_free(arg->start_mutex, arg->rsc_mutex, arg));
	}
	i = 1;
	while (i <= arg->number_of_philo)
		if (philo_setting_philo(philos, arg, i++) == NULL)
			return (NULL);
	philos[1].left_fork = philos[arg->number_of_philo].right_fork;
	i = 2;
	while (i <= arg->number_of_philo)
	{
		philos[i].left_fork = philos[i - 1].right_fork;
		i++;
	}
	return (philos);
}

int	main(int argc, char **argv)
{
	t_philo	*philos;
	t_arg	*arg;
	int		errflag;

	if (!(argc == 5 || argc == 6))
		return (philo_error());
	arg = philo_arg_setting(argc, argv);
	if (arg == NULL)
		return (philo_error());
	philos = malloc(sizeof(t_philo) * (arg->number_of_philo + 1));
	if (philos == NULL)
	{
		philo_free(arg->start_mutex, arg->rsc_mutex, arg);
		return (philo_error());
	}
	if (philo_make_philos(philos, arg) == NULL)
		return (philo_error());
	if (philo_ready_philos(philos, arg, arg->number_of_philo) == NULL)
		return (philo_error());
	philo_monitoring(philos, arg, arg->number_of_philo);
	errflag = arg->error;
	philo_join(philos, arg->number_of_philo);
	philo_free_philos(philos, arg, arg->number_of_philo, 0);
	return (errflag);
}
