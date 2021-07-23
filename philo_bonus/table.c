#include "philo.h"

void	print_status(t_philo *s_philo, char *text, char *color)
{
	sem_wait(s_philo->print);
	printf("%llu : %d. %s%s%s\n",
		   (time_unix_ms() - s_philo->s_table->start_table),
		   s_philo->id, color, text, CNRM);
	sem_post(s_philo->print);
}

int8_t	life(t_philo *s_philo)
{
	s_philo->last_eat = time_unix_ms() + s_philo->s_table->time_eat;
	print_status(s_philo, "philo is eating", CBLU);
	timeout(s_philo->last_eat);
	sem_post(s_philo->forks);
	sem_post(s_philo->forks);
	s_philo->actual_count_eat += 1;
	s_philo->end_sleap = s_philo->s_table->time_sleep + time_unix_ms();
	if (s_philo->actual_count_eat == s_philo->s_table->num_count_eat)
	{
		print_status(s_philo, "philo is full", CYEL);
		return (1);
	}
	print_status(s_philo, "philo is sleeping", CYEL);
	timeout(s_philo->end_sleap);
	print_status(s_philo, "philo is thinking", CCYN);
	return (0);
}

void	*philo(void *data)
{
	t_philo	*s_philo;

	s_philo = (t_philo *)data;
	while (1)
	{
		if (!s_philo->s_table->start_table)
			continue ;
		if (s_philo->timeout)
		{
			s_philo->last_eat = s_philo->s_table->start_table;
			timeout(s_philo->timeout + time_unix_ms());
			s_philo->timeout = 0;
		}
		if (s_philo->time_death)
			return ((void *)"died");
		sem_wait(s_philo->forks);
		print_status(s_philo, "philo has taken fork", CCYN);
		sem_wait(s_philo->forks);
		print_status(s_philo, "philo has taken fork", CCYN);
		if (life(s_philo))
			return ((void *)"full");
	}
}

void	waiting_threads(t_monitor *s_monitor)
{
	int		id_philo;
	t_philo	*s_philo;

	id_philo = -1;
	while (++id_philo < s_monitor->s_table->num_philo)
	{
		s_philo = &(s_monitor->s_arr_philo[id_philo]);
		if (s_philo->timeout != 0)
		{
			while (1)
			{
				if (s_philo->timeout == 0)
					break ;
			}
		}
	}
}

void	*monitor(void *data)
{
	t_monitor	*s_monitor;
	t_philo		*s_philo;
	int64_t 	actual_eat;
	int64_t		time;
	int16_t 	id_philo;

	s_monitor = (t_monitor *)data;
	s_monitor->s_table->start_table = time_unix_ms();
	waiting_threads(s_monitor);
	while (1)
	{
		id_philo = -1;
		time = time_unix_ms();
		actual_eat = s_monitor->s_table->num_count_eat;
		while (++id_philo < s_monitor->s_table->num_philo)
		{
			s_philo = &(s_monitor->s_arr_philo[id_philo]);
			sem_wait(s_philo->print);
			if (s_philo->actual_count_eat != s_philo->s_table->num_count_eat)
				actual_eat = 0;
			if ((s_philo->s_table->time_life + s_philo->last_eat) <= time)
			{
				s_philo->time_death = time;
				printf("%llu : %d. %sphilo died%s\n",
					   (time - s_philo->s_table->start_table),
					   s_philo->id, CRED, CNRM);
				return ((void *)"death one philo");
			}
			sem_post(s_philo->print);
		}
		if (actual_eat != 0)
			return ((void *)"all philo are full");
	}
}