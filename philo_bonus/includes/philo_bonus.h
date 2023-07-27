/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_bonus.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: facundo <facundo@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/27 12:12:01 by facundo           #+#    #+#             */
/*   Updated: 2023/07/27 16:54:15 by facundo          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_BONUS_H

# define PHILO_BONUS_H

# include <pthread.h>
# include <semaphore.h>
# include <sys/stat.h>
# include <fcntl.h>
#include <sys/time.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
# include <unistd.h>

// limits
# define MIN_PHIL_AMOUNT 1
# define MAX_PHIL_AMOUNT 200
# define MIN_T 60
# define MAX_T 60000

// messages
# define S_FORK "has taken a fork"
# define S_EAT "is eating"
# define S_SLEEP "is sleeping"
# define S_THINK "is thinking"
# define S_DIED "died"
# define E_MALLOC "Error: malloc failed\n"
# define E_THREAD "Error: pthread function failed\n"
# define E_ARGS "Error: bad arguments\n"
# define E_SEM "Error: semaphore function failed\n"

typedef struct s_philo
{
	int						id;
	int						pid;
	long					last_serving_t;
	sem_t					*printf_sem;
	sem_t					*forks;
	pthread_t				monitoring;
	pthread_t				lifecycle;
	struct s_global_data	*g_data;
}	t_philo;

typedef struct s_global_data
{
	int						phil_amount;
	int						die_t;
	int						eat_t;
	int						sleep_t;
	int						servings;
	sem_t					*printf_sem;
	sem_t					*forks;
	t_philo					*philosophers;
	long					start_time;
}	t_global_data;

/* utils.c */
long	get_time(void);
void	*monitor_starvation(void *arg);
void	*routine(void *arg);

/* libft_utils.c */
int		ft_atoi(char *str);
int		ft_strlen(char *s);
int		ft_strncmp(char *str_1, char *str_2, int n);
void	ft_usleep(unsigned int n);

#endif
