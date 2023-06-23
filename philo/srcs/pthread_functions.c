/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pthread_functions.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: facundo <facundo@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/23 16:55:10 by facundo           #+#    #+#             */
/*   Updated: 2023/06/23 16:59:12 by facundo          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philosophers.h"

int	create_threads(t_philosopher *ph)
{
	return (pthread_create(&ph->lifecycle, NULL, routine, ph)
		|| pthread_create(&ph->monitoring, NULL, monitor_starvation, ph));
}

int	join_threads(t_philosopher *ph)
{
	return (pthread_join(ph->lifecycle, NULL)
		|| pthread_join(ph->monitoring, NULL));
}

int	initialize_mutexes(t_global_data *data, int i)
{
	return (pthread_mutex_init(&data->forks[i], NULL)
		|| pthread_mutex_init(&data->lock[i], NULL));
}

int	destroy_mutexes(t_global_data *data, int i)
{
	return (pthread_mutex_destroy(&data->forks[i])
		|| pthread_mutex_destroy(&data->lock[i]));
}

int	pthread_helper(t_global_data *data, void*f)
{
	int	i;
	int	error;

	i = 0;
	error = 0;
	while (i < data->phil_amount)
	{
		if (f == pthread_create)
			error = create_threads(&data->philosophers[i]);
		else if (f == pthread_join)
			error = join_threads(&data->philosophers[i]);
		else if (f == pthread_mutex_init)
			error = initialize_mutexes(data, i);
		else if (f == pthread_mutex_destroy)
			error = destroy_mutexes(data, i);
		if (error)
			return (1);
		i++;
	}
	return (0);
}
