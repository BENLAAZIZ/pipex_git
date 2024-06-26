/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_suit_bonus.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hben-laz <hben-laz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/20 16:12:00 by hben-laz          #+#    #+#             */
/*   Updated: 2024/05/22 21:59:40 by hben-laz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex_bonus.h"

void	ft_initialis_data(t_data *data, int ac, char **av, char **env)
{
	char	*str;

	data->ac = ac;
	data->av = av;
	str = NULL;
	str = find_path(env, "PATH=");
	if (str == NULL)
	{
		data->path = NULL;
		return ;
	}
	data->path = ft_split(str + 5, ':');
}

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
	if (!env || !*env || !str)
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
		return (free_t_split(cmd), free_t_split(cmd_find));
	while (cmd_find[++i])
	{
		comand = ft_strjoin(cmd_find[i], s);
		if (!comand)
			return (free(s), free_t_split(cmd), free_t_split(cmd_find));
		if (access(comand, X_OK) != 0)
			free(comand);
		else
			break ;
	}
	free(s);
	free_t_split(cmd_find);
	if (execve(comand, cmd, NULL) == -1)
		ft_error("command not found: ", cmd[0], 0, 0);
}

void	handle_file_operations(t_data *data, int nc)
{
	if (nc == 1)
	{
		data->fd_file = open(data->av[1], O_RDONLY);
		if (data->fd_file == -1)
			perror(data->av[1]);
		dup2(data->fd[1], 1);
		dup2(data->fd_file, 0);
	}
	else
	{
		if (ft_strncmp(data->av[1], "here_doc", 8) == 0)
			data->fd_file = open(data->av[data->ac - 1],
					O_RDWR | O_CREAT | O_APPEND, 0644);
		else
			data->fd_file = open(data->av[data->ac - 1],
					O_RDWR | O_CREAT | O_TRUNC, 0644);
		if (data->fd_file == -1)
			perror(data->av[data->ac - 1]);
		dup2(data->fd_file, 1);
	}
	close_fd(data->fd);
	close(data->fd_file);
}
