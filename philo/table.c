#include "philo.h"

void	check_death(t_philo *s_philo)
{
	pthread_mutex_lock(s_philo->mutex_death);
	pthread_mutex_unlock(s_philo->mutex_death);
}

void	sleaping(t_philo *s_philo)
{
	int64_t	time;

	while (1)
	{
		time = time_unix_ms();
		if (s_philo->end_sleap != 0 && s_philo->end_sleap <= time)
		{
			s_philo->end_sleap = 0;
			check_death(s_philo);
			printf("%llu : %d. %sphilo thinking%s\n", (time - s_philo->s_table->start_table), s_philo->id, CCYN, CNRM);
			break ;
		}
	}
}

void	eating(t_philo *s_philo)
{
	int64_t	time;

	time = time_unix_ms();
	s_philo->last_eat = s_philo->s_table->time_eat + time;
	printf("%llu : %d. %sphilo eating%s\n", (time - s_philo->s_table->start_table), s_philo->id, CBLU, CNRM);
	while (1)
	{
		time = time_unix_ms();
		if (s_philo->last_eat <= time)
			break ;
	}
	pthread_mutex_unlock(s_philo->left_fork);
	pthread_mutex_unlock(s_philo->right_fork);
	s_philo->actual_count_eat += 1;
	s_philo->end_sleap = s_philo->s_table->time_sleep + time;
	if (s_philo->actual_count_eat == s_philo->s_table->num_count_eat)
	{
		printf("%llu : %d. %sphilo is full%s\n", (time - s_philo->s_table->start_table), s_philo->id, CYEL, CNRM);
		return ;
	}
	check_death(s_philo);
	printf("%llu : %d. %sphilo sleaping%s\n", (time - s_philo->s_table->start_table), s_philo->id, CYEL, CNRM);
	sleaping(s_philo);
}

void	get_forks(t_philo	*s_philo)
{
	int64_t	time;

	check_death(s_philo);
	pthread_mutex_lock(s_philo->right_fork);
	check_death(s_philo);
	time = time_unix_ms();
	printf("%llu : %d. %sphilo has taken a right fork%s\n", (time - s_philo->s_table->start_table), s_philo->id, CCYN, CNRM);
	pthread_mutex_lock(s_philo->left_fork);
	check_death(s_philo);
	time = time_unix_ms();
	printf("%llu : %d. %sphilo has taken a left fork%s\n", (time - s_philo->s_table->start_table), s_philo->id, CCYN, CNRM);
}

void	*philo(void *data)
{
	t_philo	*s_philo;

	s_philo = (t_philo *)data;
	while (1)
	{
		if (!s_philo->s_table->start_table)
			continue ;
		if (s_philo->time_death
			|| (s_philo->s_table->num_count_eat != 0
				&& s_philo->actual_count_eat == s_philo->s_table->num_count_eat))
			return ((void *)"death or full");
		get_forks(s_philo);
		check_death(s_philo);
		eating(s_philo);
	}
}

void	*monitor(void *data)
{
	int16_t 	id_philo;
	int16_t 	actual_eat;
	t_monitor	*s_monitor;
	t_philo		*s_philo;
	int64_t		time;

	s_monitor = (t_monitor *)data;
	id_philo = -1;
	while (++id_philo < s_monitor->s_table->num_philo)
	{
		s_philo = &(s_monitor->s_arr_philo[id_philo]);
		s_philo->last_eat = time_unix_ms();
	}
	s_monitor->s_table->start_table = time_unix_ms();
	while (1)
	{
		id_philo = -1;
		time = time_unix_ms();
		actual_eat = s_philo->s_table->num_count_eat;
		while (++id_philo < s_monitor->s_table->num_philo)
		{
			s_philo = &(s_monitor->s_arr_philo[id_philo]);
			if (s_philo->actual_count_eat != s_philo->s_table->num_count_eat)
			{
				actual_eat = 0;
				pthread_mutex_lock(s_philo->mutex_death);
				if ((s_philo->s_table->time_life + s_philo->last_eat) <= time)
				{
					s_philo->time_death = time;
					printf("%llu : %d. %sphilo death%s\n", (s_philo->time_death - s_philo->s_table->start_table), s_philo->id, CRED, CNRM);
					return ((void *)"death one philo");
				}
				pthread_mutex_unlock(s_philo->mutex_death);
			}
		}
		if (s_philo->s_table->num_count_eat != 0 &&
			actual_eat == s_philo->s_table->num_count_eat)
			return ((void *)"all philo are full");
	}
}