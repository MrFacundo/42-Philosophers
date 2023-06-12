/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: facundo <facundo@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/12 14:14:44 by facundo           #+#    #+#             */
/*   Updated: 2023/06/12 16:19:07 by facundo          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philosophers.h"

void update_elapsed_total_time(t_philosopher *phil)
{
	struct timeval current_time;

	gettimeofday(&current_time, NULL);
	*(phil->total_elapsed_t) = (current_time.tv_sec - phil->start_time.tv_sec) * 1000L +
							   (current_time.tv_usec - phil->start_time.tv_usec) / 1000L;
}

void update_elapsed_last_serving_time(t_philosopher *phil)
{
	struct timeval current_time;

	gettimeofday(&current_time, NULL);
	phil->elapsed_last_serving_t = (current_time.tv_sec - phil->last_serving_t.tv_sec) * 1000L +
								   (current_time.tv_usec - phil->last_serving_t.tv_usec) / 1000L;

}
long get_timestamp(t_philosopher *phil)
{
	update_elapsed_total_time(phil);
	return (*(phil->total_elapsed_t));
}

int	pthread_helper(t_global_data *global_data, void*f)
{
	int	i;
	int	error;

	i = 0;
	error = 0;
	while (i < global_data->phil_amount)
	{
		if (f == pthread_create)
		{
			error = pthread_create(&global_data->philosophers[i].lifecycle, NULL, routine, &global_data->philosophers[i])
				|| pthread_create(&global_data->philosophers[i].monitoring, NULL, check_starvation, &global_data->philosophers[i]);
		}
		else if (f == pthread_join)
		{
			error = pthread_join(global_data->philosophers[i].lifecycle, NULL)
				|| pthread_join(global_data->philosophers[i].monitoring, NULL);
		}
		else if (f == pthread_mutex_init)
		{
			error = pthread_mutex_init(global_data->forks + i, NULL);
		}
		else if (f == pthread_mutex_destroy)
		{
			error = pthread_mutex_destroy(global_data->forks + i);
		}
		if (error)
			return (1);
		i++;
	}
	return (0);
}

int check_someone_died(t_philosopher *phil)
{
    int someone_died = 0;

    pthread_mutex_lock(phil->someone_died_mutex);
    if (*(phil->someone_died) == 1) {
        someone_died = 1;
		pthread_mutex_unlock(phil->left_fork);
		pthread_mutex_unlock(phil->right_fork);	
    }
    pthread_mutex_unlock(phil->someone_died_mutex);

    return someone_died;
}
/*
void	free_all(t_global_data *global_data)
{
	int	i;

	i = 0;
	while (i < global_data->phil_amount)
		pthread_mutex_destroy(global_data->forks + i++);
	free(global_data->forks);
	free(global_data->philosophers);
} */