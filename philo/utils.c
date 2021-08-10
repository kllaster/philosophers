#include "philo.h"

void	print_status(t_philo *s_philo, char *text, char *color)
{
	pthread_mutex_lock(s_philo->print);
	printf("%llu : %d. %s%s%s\n",
		(time_unix_ms() - s_philo->s_table->start_table),
		s_philo->id, color, text, CNRM);
	pthread_mutex_unlock(s_philo->print);
}

void	free_struct(pthread_t *threads, t_monitor *s_monitor)
{
	int	i;

	i = -1;
	if (s_monitor->s_arr_philo[0].print)
	{
		pthread_mutex_destroy(s_monitor->s_arr_philo[0].print);
		free(s_monitor->s_arr_philo[0].print);
	}
	if (s_monitor->s_arr_philo)
	{
		while (++i < s_monitor->s_table->num_philo)
		{
			if (s_monitor->s_arr_philo[i].left_fork)
			{
				pthread_mutex_destroy(s_monitor->s_arr_philo[i].left_fork);
				free(s_monitor->s_arr_philo[i].left_fork);
			}
		}
	}
	free(threads);
	free(s_monitor->s_arr_philo);
	free(s_monitor);
}

int64_t	ft_atoi(const char *str)
{
	unsigned int	res;
	int				sign;

	res = 0;
	sign = 1;
	while (*str == '\t' || *str == '\n'
		|| *str == '\v' || *str == '\f'
		|| *str == '\r' || *str == ' ')
		str++;
	if (*str == '-')
		return (-1);
	if (!(*str >= '0' && *str <= '9'))
		return (-1);
	while (*str >= '0' && *str <= '9')
	{
		if ((res * 10) < res)
			return (-1);
		res = res * 10 + (*str++ - '0');
	}
	if (*str && !(*str >= '0' && *str <= '9'))
		return (-1);
	return ((int)res * sign);
}

int64_t	time_unix_ms(void)
{
	struct timeval	s_time;

	gettimeofday(&s_time, NULL);
	return (s_time.tv_sec * 1000 + (int)(s_time.tv_usec * 0.001));
}

void	timeout(int64_t to_time)
{
	int64_t	time;

	while (1)
	{
		time = time_unix_ms();
		if (to_time <= time)
			break ;
		usleep(100);
	}
}
