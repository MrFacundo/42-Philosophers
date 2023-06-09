/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: facundo <facundo@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/02 09:12:51 by facundo           #+#    #+#             */
/*   Updated: 2023/06/09 16:31:26 by facundo          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILOSOPHERS_H
# define PHILOSOPHERS_H

# include <pthread.h>
# include <time.h>
# include <stdio.h>
# include <stdarg.h>
# include <stdlib.h>
# include <unistd.h>
#include <sys/time.h>

# define NUM_PHILOSOPHERS 20
# define SERVINGS 4
# define EAT_T 500
# define SLEEP_T 500

typedef struct philosophers
{
	int				id;
	int				servings;
	pthread_mutex_t	*left_fork;
	pthread_mutex_t	*right_fork;
	int				phil_amount;
	int				*someone_died;
	struct timeval	start_time;
	long			*total_elapsed_t;
	struct timeval	last_serving_t;
	long			elapsed_last_serving_t;
	int				eat_t;
	int				sleep_t;
	int				starvation_t;
	int				is_eating;
}	t_philosopher;

typedef struct global_data
{
	int				phil_amount;
	int				someone_died;
	t_philosopher	*philosophers;
	pthread_mutex_t	*forks;
	long			total_elapsed_t;
	struct timeval	start_time;
	int				eat_t;
	int				sleep_t;
	int				starvation_t;
	int				servings;
}	t_global_data;

int	ft_atoi(const char *nptr);

# endif