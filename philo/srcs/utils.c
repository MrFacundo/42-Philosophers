/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: facundo <facundo@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/12 14:14:44 by facundo           #+#    #+#             */
/*   Updated: 2023/06/19 15:27:29 by facundo          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philosophers.h"

void	print_status(t_philosopher *ph, char *status)
{
	long	timestamp;

	timestamp = get_time() - ph->g_data->start_time;
	if ((ph->is_dead && !ft_strncmp(status, "died", 5)) || !check_someone_died(ph))
		printf("%ld %d %s\n", timestamp, ph->id, status);
}

void	ft_usleep(long time_in_ms)
{
	long	start_time;

	start_time = 0;
	start_time = get_time();
	while ((get_time() - start_time) < time_in_ms)
		usleep(time_in_ms / 10);
}

long	get_time(void)
{
	long			timestamp;
	struct timeval	current_time;

	gettimeofday(&current_time, 0);
	timestamp = current_time.tv_sec * 1000 + current_time.tv_usec / 1000;
	return (timestamp);
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
			error = pthread_mutex_init(global_data->forks + i, NULL)
				|| pthread_mutex_init(global_data->lock + i, NULL);
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

int check_someone_died(t_philosopher *ph)
{
    int ret;

	ret = 0;
    pthread_mutex_lock(&ph->g_data->someone_died_mutex);
    if (ph->g_data->someone_died == 1)
		ret = 1;
    pthread_mutex_unlock(&ph->g_data->someone_died_mutex);
    return ret;
}
