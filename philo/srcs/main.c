/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: facundo <facundo@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/02 09:12:24 by facundo           #+#    #+#             */
/*   Updated: 2023/06/12 15:42:40 by facundo          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philosophers.h"

void *check_starvation(void *arg)
{
	t_philosopher *phil;
	phil = (t_philosopher *)arg;

	while (phil->elapsed_last_serving_t <= phil->starvation_t && phil->servings != 0)
	{
		if (check_someone_died(phil))
			break;
		usleep(100 * 1000);
		if (phil->is_eating)
			continue;
		update_elapsed_total_time(phil);
		update_elapsed_last_serving_time(phil);
		if (check_someone_died(phil))
			break;
	}
	pthread_mutex_lock(phil->someone_died_mutex);
	if (phil->servings != 0 && *(phil->someone_died) == 0)
	{
		*(phil->someone_died) = 1;
		printf("Ph %d phil->someone_died = %d\n", phil->id, *(phil->someone_died)); 
		printf("%ld Ph %d died :(\n", get_timestamp(phil), phil->id);
	}
	pthread_mutex_unlock(phil->someone_died_mutex);
	return NULL;
}

void *routine(void *arg)
{
	t_philosopher *phil;
	pthread_mutex_t *left_fork;
	pthread_mutex_t *right_fork;

	phil = (t_philosopher *)arg;
	assign_forks(phil, &left_fork, &right_fork);

	while (phil->servings != 0 && *(phil->someone_died) == 0)
	{
		printf("%ld Ph %d is thinking\n", get_timestamp(phil), phil->id);

		if (phil->phil_amount == 1)
		{
			pthread_mutex_lock(left_fork);
			printf("%ld Ph %d has taken fork %p\n", get_timestamp(phil), phil->id, left_fork);
			while (*(phil->someone_died) == 0)
				usleep(100 * 1000);
			pthread_mutex_unlock(left_fork);
			break;
		}
		else
		{
			eat(phil, left_fork, right_fork);
			if (check_someone_died(phil))
				break;
			leave_forks(phil, left_fork, right_fork);
			
			if (phil->servings == 0 || *(phil->someone_died) == 1)
				break;

			nap(phil);
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
	t_philosopher *phil;

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