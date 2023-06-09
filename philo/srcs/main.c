/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: facundo <facundo@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/02 09:12:24 by facundo           #+#    #+#             */
/*   Updated: 2023/06/09 17:42:38 by facundo          ###   ########.fr       */
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

long get_timestamp(t_philosopher *phil)
{
	update_elapsed_total_time(phil);
	return (*(phil->total_elapsed_t));
}

void update_elapsed_last_serving_time(t_philosopher *phil)
{
	struct timeval current_time;

	gettimeofday(&current_time, NULL);
	phil->elapsed_last_serving_t = (current_time.tv_sec - phil->last_serving_t.tv_sec) * 1000L +
								   (current_time.tv_usec - phil->last_serving_t.tv_usec) / 1000L;
	// printf("Ph %d elapsed_last_serving_t: %ld\n", phil->id, phil->elapsed_last_serving_t);
}

void *check_starvation(void *arg)
{
	t_philosopher *phil;
	phil = (t_philosopher *)arg;

	while (phil->elapsed_last_serving_t <= phil->starvation_t && phil->servings != 0 && *(phil->someone_died) == 0)
	{
		usleep(100 * 1000);
		if (phil->is_eating)
			continue;
		update_elapsed_total_time(phil);
		update_elapsed_last_serving_time(phil);
	}
	if (phil->servings != 0 && *(phil->someone_died) == 0)
	{
		*(phil->someone_died) = 1;
		printf("%ld Ph %d died :(\n", get_timestamp(phil), phil->id);
	}
	return NULL;
}

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
	pthread_mutex_lock(left_fork);
	printf("%ld Ph %d has taken fork %p\n", get_timestamp(phil), phil->id, left_fork);
	pthread_mutex_lock(right_fork);
	printf("%ld Ph %d has taken fork %p\n", get_timestamp(phil), phil->id, right_fork);
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
			if (*(phil->someone_died) == 1)
			{
				pthread_mutex_unlock(left_fork);
				pthread_mutex_unlock(right_fork);
				break;
			}
			leave_forks(phil, left_fork, right_fork);

			if (phil->servings == 0 || *(phil->someone_died) == 1)
				break;

			nap(phil);
		}
	}
	return (NULL);
}

void init_philosophers(t_global_data *global_data)
{
	int i;

	i = 0;
	while (i < global_data->phil_amount)
	{
		global_data->philosophers[i].id = i + 1;
		global_data->philosophers[i].servings = global_data->servings;
		global_data->philosophers[i].left_fork = global_data->forks + i;
		global_data->philosophers[i].right_fork = global_data->forks + (i + 1) % global_data->phil_amount;
		global_data->philosophers[i].phil_amount = global_data->phil_amount;
		global_data->philosophers[i].someone_died = &global_data->someone_died;
		global_data->philosophers[i].start_time = global_data->start_time;
		global_data->philosophers[i].total_elapsed_t = &global_data->total_elapsed_t;
		global_data->philosophers[i].start_time = global_data->start_time;
		global_data->philosophers[i].eat_t = global_data->eat_t;
		global_data->philosophers[i].sleep_t = global_data->sleep_t;
		global_data->philosophers[i].starvation_t = global_data->starvation_t;
		global_data->philosophers[i].last_serving_t = global_data->start_time;
		global_data->philosophers[i].elapsed_last_serving_t = 0;
		global_data->philosophers[i].is_eating = 0;
		i++;
	}
}

void initialize_global_data(t_global_data *global_data, int argc, char **argv)
{
	global_data->phil_amount = ft_atoi(argv[1]);
	global_data->starvation_t = ft_atoi(argv[2]);
	global_data->eat_t = ft_atoi(argv[3]);
	global_data->sleep_t = ft_atoi(argv[4]);
	if (argc == 6)
		global_data->servings = ft_atoi(argv[5]);
	else
		global_data->servings = -1;
	global_data->philosophers = malloc(sizeof(t_philosopher) * global_data->phil_amount);
	global_data->forks = malloc(sizeof(pthread_mutex_t) * global_data->phil_amount);
	global_data->someone_died = 0;
	gettimeofday(&global_data->start_time, NULL);
	if (global_data->phil_amount > 200 || global_data->starvation_t < 60 || global_data->eat_t < 60 || global_data->sleep_t < 60)
	{
		printf("Error: invalid arguments\n");
		exit(1);
	}
}

int	pthread_helper(t_global_data *global_data, void*f)
{
	int	i;
	int	error;

	i = -1;
	while (i < global_data->phil_amount)
	{
		if (f == pthread_create)
	}
	return (0);
}

/* args: number_of_philosophers, time_to_die, time_to_eat, time_to_sleep,
[number_of_times_each_philosopher_must_eat] */
int main(int argc, char **argv)
{
	int i;
	t_global_data global_data;
	t_philosopher *phil;
	pthread_t *lifecycles;
	pthread_t *checkers;

	if (argc < 5 || argc > 6)
	{
		printf("Error: wrong number of arguments\n");
		return (1);
	}

	initialize_global_data(&global_data, argc, argv);
	lifecycles = malloc(sizeof(pthread_t) * global_data.phil_amount);
	checkers = malloc(sizeof(pthread_t) * global_data.phil_amount);

	i = 0;
	while (i < global_data.phil_amount)
		pthread_mutex_init(global_data.forks + i++, NULL);

	init_philosophers(&global_data);

	i = 0;
	while (i < global_data.phil_amount)
	{
		pthread_create(lifecycles + i, NULL, &routine, global_data.philosophers + i);
		pthread_create(checkers + i, NULL, &check_starvation, global_data.philosophers + i);
		i++;
	}
	i = 0;
	while (i < global_data.phil_amount)
	{
		pthread_join(lifecycles[i], NULL);
		pthread_join(checkers[i++], NULL);
	}
	return (0);
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