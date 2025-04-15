/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client_bonus.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: btuncer <btuncer@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/14 19:56:20 by btuncer           #+#    #+#             */
/*   Updated: 2025/04/15 19:08:15 by btuncer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <signal.h>
#include <stdbool.h>
#include <stdlib.h>
#include <unistd.h>

short	g_gate = 0;

void	messenger(int signal)
{
	int		i;

	if (signal == SIGUSR1)
		g_gate = 1;
	else if (signal == SIGUSR2)
		write(1, "OK\n", 3);
	else
	{
		i = 8;
		while (i--)
		{
			g_gate = 0;
			if (kill(signal, SIGUSR2) == -1)
				exit(1);
			while (!g_gate)
				pause();
		}
	}
}

void	send_str(char *str, int receiver_pid)
{
	int	counter;
	int	checker;

	while (*str)
	{
		counter = 0;
		while (counter < 8)
		{
			g_gate = 0;
			if (*str & (1 << counter))
				checker = kill(receiver_pid, SIGUSR1);
			else
				checker = kill(receiver_pid, SIGUSR2);
			if (checker == -1)
				exit(1);
			counter++;
			while (g_gate == 0)
				pause();
		}
		str++;
	}
}

bool	in(char *str, char c)
{
	ssize_t	counter;

	counter = 0;
	while (str[counter])
	{
		if (str[counter] == c)
			return (true);
		counter++;
	}
	return (false);
}

bool	set_pid(char *arg_pid, int *set_pid)
{
	int	counter;
	int	sign_counter;
	int	res;

	counter = 0;
	sign_counter = 0;
	res = 0;
	while (arg_pid[counter])
	{
		if (!(in("-+0123456789", arg_pid[counter])))
			return (false);
		if (in("-+", arg_pid[counter]))
			sign_counter++;
		if ((arg_pid[counter + 1] && in("0123456789", arg_pid[counter])
				&& in("+-", arg_pid[counter + 1])) || sign_counter > 1)
			return (false);
		if (in("0123456789", arg_pid[counter]))
		{
			res *= 10;
			res += arg_pid[counter] - 48;
		}
		counter++;
	}
	*set_pid = res;
	return (true);
}

int	main(int argc, char **argv)
{
	char				*string_to_send;
	int					receiver_pid;
	struct sigaction	sigact;

	if (!(argc == 3))
		return (write(1, "Usage: ./client {TARGET PID} {MESSAGE}\n", 39), 1);
	if (set_pid(argv[1], &receiver_pid) == false)
		return (write(1, "Wrong type of argument as PID.\n", 31), 1);
	if (argv[1][0] == '-' || receiver_pid < 1)
		return (write(1, "PID can not be lower than 1\n", 28), 1);
	sigact.sa_handler = messenger;
	sigact.sa_flags = 0;
	sigemptyset(&sigact.sa_mask);
	if (sigaction(SIGUSR1, &sigact, 0) == -1 || sigaction(SIGUSR2, &sigact,
			0) == -1)
		exit(1);
	string_to_send = argv[2];
	send_str(string_to_send, receiver_pid);
	messenger(receiver_pid);
}
