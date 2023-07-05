/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   checks.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: facu <facu@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/21 16:34:10 by facundo           #+#    #+#             */
/*   Updated: 2023/07/05 00:14:48 by facu             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philosophers.h"

int	check_is_dead(t_philosopher *ph)
{
	pthread_mutex_lock(ph->lock);
	if (ph->is_eating)
	{
		pthread_mutex_unlock(ph->lock);
		return (0);
	}
	if (get_time() - ph->last_serving_t > ph->g_data->die_t)
		ph->is_dead = 1;
	pthread_mutex_unlock(ph->lock);
	return (ph->is_dead);
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
