/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   subroutines.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: facundo <facundo@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/12 14:28:30 by facundo           #+#    #+#             */
/*   Updated: 2023/06/12 15:56:43 by facundo          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philosophers.h"

void assign_forks(t_philosopher *phil, pthread_mutex_t **left_fork, pthread_mutex_t **right_fork)
{
	if (phil->id % 2 == 0)
	{
		*left_fork = phil->right_fork;
		*right_fork = phil->left_fork;
	}
	else
	{
		*left_fork = phil->left_fork;
		*right_fork = phil->right_fork;
	}
}

void	eat(t_philosopher *phil, pthread_mutex_t *left_fork, pthread_mutex_t *right_fork)
{
	if (check_someone_died(phil))
		return ;
	pthread_mutex_lock(left_fork);
	printf("%ld Ph %d has taken fork %p\n", get_timestamp(phil), phil->id, left_fork);
	if (check_someone_died(phil))
		return ;
	pthread_mutex_lock(right_fork);
	printf("%ld Ph %d has taken fork %p\n", get_timestamp(phil), phil->id, right_fork);
	if (check_someone_died(phil))
		return ;
	phil->is_eating = 1;
	printf("%ld Ph %d is eating\n", get_timestamp(phil), phil->id);
	usleep(phil->eat_t * 1000);
}

void	leave_forks(t_philosopher *phil, pthread_mutex_t *left_fork, pthread_mutex_t *right_fork)
{
	pthread_mutex_unlock(left_fork);
	pthread_mutex_unlock(right_fork);
	phil->is_eating = 0;
	printf("%ld Ph %d has finished eating\n\n", get_timestamp(phil), phil->id);
	gettimeofday(&phil->last_serving_t, NULL);
	phil->servings--;
	printf("%ld Ph %d has %d servings left\n", get_timestamp(phil), phil->id, phil->servings);
}

void	nap(t_philosopher *phil)
{
	printf("%ld Ph %d has started sleeping\n", get_timestamp(phil), phil->id);
	usleep(phil->sleep_t * 1000);
}