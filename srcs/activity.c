/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   activity.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vintran <vintran@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/02 16:32:34 by vintran           #+#    #+#             */
/*   Updated: 2021/10/05 16:54:47 by vintran          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	print_msg(t_p *p, char *msg)
{
	pthread_mutex_lock(&p->info->m_msg);
	pthread_mutex_lock(&p->info->m_stop);
	if (!p->info->stop)
		printf("%7dms  %3d    %s\n", get_time() - p->info->t_start, p->id, msg);
	pthread_mutex_unlock(&p->info->m_msg);
	pthread_mutex_unlock(&p->info->m_stop);
}

void	droitier(t_p *p)
{
	pthread_mutex_lock(p->r_fork);
	print_status(p, "has taken a fork");
	if (p->n == 1)
	{
		pthread_mutex_unlock(p->r_fork);
		ft_usleep(p->t_die);
		return ;
	}
	pthread_mutex_lock(p->l_fork);
	print_status(p, "has taken a fork");
	print_status(p, "is eating");
	pthread_mutex_lock(&p->info->m_last_eat);
	p->t_last_eat = get_time();
	pthread_mutex_unlock(&p->info->m_last_eat);
	ft_usleep(p->t_eat);
	pthread_mutex_unlock(p->r_fork);
	pthread_mutex_unlock(p->l_fork);
}

void	gaucher(t_p *p)
{
	pthread_mutex_lock(p->l_fork);
	print_status(p, "has taken a fork");
	if (p->n == 1)
	{
		pthread_mutex_unlock(p->l_fork);
		ft_usleep(p->t_die);
		return ;
	}
	pthread_mutex_lock(p->r_fork);
	print_status(p, "has taken a fork");
	print_status(p, "is eating");
	pthread_mutex_lock(&p->info->m_last_eat);
	p->t_last_eat = get_time();
	pthread_mutex_unlock(&p->info->m_last_eat);
	ft_usleep(p->t_eat);
	pthread_mutex_unlock(p->l_fork);
	pthread_mutex_unlock(p->r_fork);
}

void	eat_activity(t_p *p)
{
	if (p->id % 2 == 0)
		gaucher(p);
	else
		droitier(p);
	pthread_mutex_lock(&p->info->m_meals);
	p->info->meals[p->id - 1]++;
	pthread_mutex_unlock(&p->info->m_meals);
}

void	activity(t_p *p)
{
	int	stop;

	pthread_mutex_lock(&p->info->m_stop);
	stop = p->info->stop;
	pthread_mutex_unlock(&p->info->m_stop);
	if (!stop)
		eat_activity(p);
	pthread_mutex_lock(&p->info->m_stop);
	stop = p->info->stop;
	pthread_mutex_unlock(&p->info->m_stop);
	if (!stop)
	{
		print_status(p, "is sleeping");
		ft_usleep(p->t_sleep);
	}
	print_status(p, "is thinking");
}