/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: facundo <facundo@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/02 09:12:24 by facundo           #+#    #+#             */
/*   Updated: 2023/06/19 16:48:13 by facundo          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philosophers.h"

int	check_is_dead(t_philosopher *ph)
{
	int ret;

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

void *check_starvation(void *arg)
{
	t_philosopher *ph;
	ph = (t_philosopher *)arg;

	while (!check_someone_died(ph) && ph->servings != 0)
	{
		if (check_is_dead(ph))
		{
			pthread_mutex_lock(&ph->g_data->someone_died_mutex);
			ph->g_data->someone_died = 1;
			pthread_mutex_unlock(&ph->g_data->someone_died_mutex);
			print_status(ph, "died");
		}
	}
	return NULL;
}

void table_for_one(t_philosopher *ph, pthread_mutex_t *left_fork)
{
	pthread_mutex_lock(left_fork);
	print_status(ph, "has taken a fork");
	while (ph->is_dead == 0)
		ft_usleep(ph->g_data->eat_t);
	pthread_mutex_unlock(left_fork);
}

// modifies left and right fork pointers to avoid deadlocks
void assign_forks(t_philosopher *ph, pthread_mutex_t **left_fork, pthread_mutex_t **right_fork)
{
	if (ph->id % 2 == 0)
	{
		*left_fork = ph->right_fork;
		*right_fork = ph->left_fork;
	}
	else
	{
		*left_fork = ph->left_fork;
		*right_fork = ph->right_fork;
	}
}

int	check_servings(t_philosopher *ph)
{
	int ret;

	pthread_mutex_lock(ph->lock);
	ret = ph->servings == 0;
	pthread_mutex_unlock(ph->lock);
	return (ret);
}

void *routine(void *arg)
{
	t_philosopher *ph;
	pthread_mutex_t *left_fork;
	pthread_mutex_t *right_fork;

	ph = (t_philosopher *)arg;
	assign_forks(ph, &left_fork, &right_fork);

	while (!check_servings(ph) && !check_someone_died(ph))
	{
		print_status(ph, "is thinking");
		if (ph->g_data->phil_amount == 1)
		{
			table_for_one(ph, left_fork);
			break;
		}
		else
		{
			eat(ph, left_fork, right_fork);
			leave_forks(ph, left_fork, right_fork);
			if (check_servings(ph))
				break;
			nap(ph);
		}
	}
	return (NULL);
}

/* args: number_of_philosophers, time_to_die, time_to_eat, time_to_sleep,
[number_of_times_each_philosopher_must_eat] */
int main(int argc, char **argv)
{
	int i;
	t_global_data global_data;
	t_philosopher *ph;

	if (argc < 5 || argc > 6)
	{
		printf("Error: wrong number of arguments\n");
		return (1);
	}

	if (initialize_global_data(&global_data, argc, argv))
		return (1);
	pthread_helper(&global_data, pthread_mutex_init);
	pthread_mutex_init(&global_data.someone_died_mutex, NULL);
	init_philosophers(&global_data);
	pthread_helper(&global_data, pthread_create);
	pthread_helper(&global_data, pthread_join);
	print_results(&global_data);
	return (0);
}