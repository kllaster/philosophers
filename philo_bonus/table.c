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
		if (s_philo->time_death)
			exit(1);
		sem_wait(s_philo->forks);
		print_status(s_philo, "philo has taken fork", CCYN);
		sem_wait(s_philo->forks);
		print_status(s_philo, "philo has taken fork", CCYN);
		if ((s_philo->s_table->time_life + s_philo->last_eat) <= time_unix_ms())
		{
			sem_wait(s_philo->print);
			s_philo->time_death = time_unix_ms();
			printf("%llu : %d. %sphilo died%s\n",
				   (s_philo->time_death - s_philo->s_table->start_table),
				   s_philo->id, CRED, CNRM);
			exit(2);
		}
		if (life(s_philo))
			exit(3);
	}
}