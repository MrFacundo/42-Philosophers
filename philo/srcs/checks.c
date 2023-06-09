/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   checks.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: facundo <facundo@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/21 16:34:10 by facundo           #+#    #+#             */
/*   Updated: 2023/07/12 15:49:45 by facundo          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philosophers.h"

int	check_is_dead(t_philosopher *ph)
{
	int	ret;

	pthread_mutex_lock(ph->lock);
	if (ph->is_eating)
	{
		pthread_mutex_unlock(ph->lock);
		return (0);
	}
	if (get_time() - ph->last_serving_t > ph->g_data->die_t)
		ph->is_dead = 1;
	ret = ph->is_dead;
	pthread_mutex_unlock(ph->lock);
	return (ret);
}

int	check_servings(t_philosopher *ph)
{
	int	ret;

	pthread_mutex_lock(ph->lock);
	ret = ph->servings == 0;
	pthread_mutex_unlock(ph->lock);
	return (ret);
}

int	check_someone_died(t_global_data *data)
{
	int	ret;

	pthread_mutex_lock(&data->someone_died_mutex);
	ret = data->someone_died == 1;
	pthread_mutex_unlock(&data->someone_died_mutex);
	return (ret);
}

int	check_argc(int argc)
{
	if (argc < 5 || argc > 6)
	{
		printf(E_ARGS);
		return (1);
	}
	return (0);
}
