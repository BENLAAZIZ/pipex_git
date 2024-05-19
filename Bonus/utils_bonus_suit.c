/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_bonus_suit.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hben-laz <hben-laz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/16 21:35:08 by hben-laz          #+#    #+#             */
/*   Updated: 2024/05/19 17:48:30 by hben-laz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex_bonus.h"

char	*ft_strchr(const char *s, int c)
{
	int	i;

	i = 0;
	if ((unsigned char)c == '\0')
		return ((char *)s + ft_strlen(s));
	while (s[i] != '\0')
	{
		if (s[i] == (unsigned char)c)
			return ((char *)(s + i));
		i++;
	}
	return (NULL);
}

char	*find_path(char **env, char *str)
{
	int	i;

	i = 0;
	if (!env || !env[0] || !str)
		return (NULL);
	while (env[i])
	{
		if (ft_strncmp(env[i], str, ft_strlen(str)) == 0)
			return (env[i]);
		i++;
	}
	return (NULL);
}

void	exec_cmd(char **cmd, char **cmd_find)
{
	char	*s;
	char	*comand;
	int		i;

	i = -1;
	s = NULL;
	comand = NULL;
	s = ft_strjoin("/", cmd[0]);
	if (!s)
		return (free_t_split(cmd));
	while (cmd_find[++i])
	{
		comand = ft_strjoin(cmd_find[i], s);
		if (!comand)
			return (free(s));
		if (access(comand, X_OK) != 0)
			free(comand);
		else
			break ;
	}
	free(s);
	if (execve(comand, cmd, NULL) == -1)
		ft_error("command not found: ", cmd[0], 0);
}

void	handle_file_operations(char **av, int ac, int *fd, int nc)
{
	int	fd_file;

	if (nc == 1)
	{
		fd_file = open(av[1], O_RDONLY);
		if (fd_file == -1)
			perror(av[1]);
		if (dup2(fd[1], 1) == -1)
			perror("dup2 ");
		if (dup2(fd_file, 0) == -1)
			perror("dup2 ");
	}
	else
	{
		if (ft_strncmp(av[1], "here_doc", 8) == 0)
			fd_file = open(av[ac - 1], O_RDWR | O_CREAT | O_APPEND, 0777);
		else
			fd_file = open(av[ac - 1], O_RDWR | O_CREAT | O_TRUNC, 0777);
		if (fd_file == -1)
			perror(av[ac - 1]);
		if (dup2(fd_file, 1) == -1)
			perror("dup2 ");
	}
	close_fd(fd);
	close(fd_file);
}
