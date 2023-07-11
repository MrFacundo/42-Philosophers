/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pthread_functions.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: facundo <facundo@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/23 16:55:10 by facundo           #+#    #+#             */
/*   Updated: 2023/07/11 16:37:43 by facundo          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philosophers.h"

int	process_philosophers(t_global_data *d, void*f)
{
	int	i;
	int	error;

	error = 0;
	i = 0;
	while (i < d->phil_amount)
	{
		if (f == pthread_create)
			error = create_lifecycle_threads(&d->philosophers[i]);
		else if (f == pthread_join)
			error = pthread_join(d->philosophers[i].lifecycle, 0);
		else if (f == pthread_mutex_init)
			error = initialize_mutexes(d, i);
		else if (f == pthread_mutex_destroy)
			error = destroy_mutexes(d, i);
		if (error)
			return (1);
		i++;
	}
	return (error);
}

int	pthread_helper(t_global_data *data, void*f)
{
	int	error;

	error = 0;
	if (f == initialize_global_mutexes)
		error = initialize_global_mutexes(data);
	else if (f == pthread_create)
		error = create_monitor_thread(data);
	else if (f == pthread_join)
		error = pthread_join(data->monitoring, 0);
	if (error)
		return (1);
	error = process_philosophers(data, f);
	return (error);
}
