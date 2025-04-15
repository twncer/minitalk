/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server_bonus.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: btuncer <btuncer@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/14 19:56:27 by btuncer           #+#    #+#             */
/*   Updated: 2025/04/15 18:57:23 by btuncer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <signal.h>
#include <stdlib.h>
#include <unistd.h>

void	stack(int signal, siginfo_t *sig_info, void *context)
{
	static char				temp = 0;
	static unsigned short	counter = 0;

	(void)context;
	if (signal == SIGUSR1)
		temp = temp | (1 << counter);
	counter++;
	if (counter == 8)
	{
		write(1, &temp, 1);
		if (temp == '\0')
			kill(sig_info->si_pid, SIGUSR2);
		temp = 0;
		counter = 0;
	}
	kill(sig_info->si_pid, SIGUSR1);
}

void	put_nbr(int nbr)
{
	char	to_put;

	if (nbr > 9)
	{
		put_nbr(nbr / 10);
		put_nbr(nbr % 10);
	}
	else if (nbr < 10)
	{
		to_put = nbr + 48;
		write(1, &to_put, 1);
	}
}

int	main(void)
{
	int					pid;
	struct sigaction	sigact;

	pid = getpid();
	sigact.sa_sigaction = stack;
	sigact.sa_flags = SA_SIGINFO;
	sigemptyset(&sigact.sa_mask);
	if (sigaction(SIGUSR1, &sigact, 0) == -1 || sigaction(SIGUSR2, &sigact,
			0) == -1)
		exit(1);
	write(1, "PID: ", 5);
	put_nbr(pid);
	write(1, "\n---\n", 5);
	while (1)
		pause();
}
