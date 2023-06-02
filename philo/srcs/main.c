/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: facundo <facundo@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/02 09:12:24 by facundo           #+#    #+#             */
/*   Updated: 2023/06/02 16:21:14 by facundo          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philosophers.h"

void	*routine(void *arg)
{
	t_phil	*phil;

	phil = (t_phil *)arg;
	while (phil->meals < MEALS)
	{
		pthread_mutex_lock(phil->left_fork);
		pthread_mutex_lock(phil->right_fork);
		printf("Thread %d: started\n", phil->id);
		printf("Philosopher %d: has started eating\n", phil->id);
		usleep(EAT_T * 1000);
		printf("Philosopher %d: has finished eating\n", phil->id);
		phil->meals++;
		printf("Philosopher has had %d meals\n", phil->meals);
		pthread_mutex_unlock(phil->left_fork);
		pthread_mutex_unlock(phil->right_fork);
		printf("Philosopher %d: has started sleeping\n", phil->id);
		usleep(SLEEP_T * 1000);
		printf("Philosopher %d: has started thinking\n", phil->id);
	}
	printf("Thread %d: finished\n", phil->id);
	return (NULL);
}

void	initialize_phil(t_phil *phil)
{
	int	i;
	pthread_mutex_t *forks[PHIL_AMOUNT];
	i = 0;
	while (i < PHIL_AMOUNT)
	{
		phil[i].id = i;
		phil[i].meals = 0;
		phil[i].left_fork = pthread_mutex_init(forks + i, NULL);
		phil[i].right_fork = pthread_mutex_init(forks + (i + 1) % PHIL_AMOUNT, NULL);
		i++;
	}
}

int	main(int argc, char **argv)
{
	pthread_t	th[PHIL_AMOUNT];
	t_phil		phil[PHIL_AMOUNT];

	initialize_phil(phil);
	int			i;
	i = 0;
	while(i < PHIL_AMOUNT)
		pthread_create(th + i, NULL, &routine, phil + i);
	return (0);
}
