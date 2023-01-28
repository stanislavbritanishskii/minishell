/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sbritani <sbritani@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/22 17:53:20 by sbritani          #+#    #+#             */
/*   Updated: 2023/01/28 13:08:54 by sbritani         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char *cur_dir(void)
{
	char *res;
	res = malloc(sizeof(char) * 100);
	if (!getcwd(res, 100))
	{
		free(res);
		return (NULL);
	}
	return (res);
}

int	pwd(char **splitted_input)
{
	char	*to_print;
	if (splitted_input[1])
		printf("pwd: too many arguments\n");
	else
	{
		to_print = cur_dir();
		printf("%s\n",to_print);
		free(to_print);
	}
	return (0);
}

int cd(char **splitted_input, t_settings *settings)
{
	char *temp;
	if (!splitted_input[1])
	{
		settings->last_working_directory = cur_dir();
		chdir("/");
		return (0);
	}
	if (strings_equal(splitted_input[1], "-\0"))
	{
		temp = settings->last_working_directory;
		settings->last_working_directory = cur_dir();
		chdir(temp);
		free(temp);
		return (0);
	}
	if (!access(splitted_input[1], X_OK))
	{
		free(settings->last_working_directory);
		settings->last_working_directory = cur_dir();
		chdir(splitted_input[1]);
		return (0);
	}
	printf("cd: no such file or directory: %s\n", splitted_input[1]);
	return (1);
}

int	env(char **splitted_input, t_settings *settings)
{
	int	i;

	i = 0;
	if (splitted_input[1])
	{
		printf("sorry in current version env doesn't take any parameters\n");
		return (127);
	}
	while (settings->exported_env->keys[i])
	{
		if (settings->exported_env->values[i])
			printf("%s=%s\n", settings->exported_env->keys[i], settings->exported_env->values[i]);
		i++;
	}
	return (0);
}

int	unset(char **splitted_input, t_settings *settings)
{
	int	i;

	i = 1;
	while (splitted_input[i])
	{
		dict_delete(settings->env, splitted_input[i]);
		dict_delete(settings->exported_env, splitted_input[i]);
		i++;
	}
	return (0);
}

char **split_for_equal_sign(char *str)
{
	char	**res;
	int	i;

	res = malloc(sizeof(char *) * 3);
	res[2] = NULL;
	while (str[i] && str[i] != '=')
		i++;
	res[0] = str_copy(str, i);
	res[1] = str_copy(str + i + 1, -1);
	if (!res[1][0])
	{
		free(res[1]);
		res[1] = NULL;
	}
	return (res);
}

int	export(char **splitted_input, t_settings *settings)
{
	int		i;
	char	**temp;

	i = 1;
	while(splitted_input[i])
	{
		temp = split_for_equal_sign(splitted_input[i]);
		// print_splitted(temp);
		if (temp[1])
		{
			printf("tuta\n");
			dict_add(settings->env, temp[0], temp[1]);
			dict_add(settings->exported_env, temp[0], temp[1]);
		}
		else if (dict_get(settings->env, temp[0], NULL))
		{
			dict_add(settings->exported_env, temp[0], dict_get(settings->env, temp[0], NULL));
		}
		else
			dict_add(settings->exported_env, temp[0], NULL);
		ft_split_clear(temp);
		i++;
	}
	return (0);
}


int	deal_with_equal_sign(char **splitted_input, t_settings *settings)
{
	int	i;
	char **temp;

	while (splitted_input[i])
	{
		temp = split_for_equal_sign(splitted_input[i]);
		if (temp[1])
			dict_add(settings->env, temp[0], temp[1]);
		else
		{
			ft_split_clear(temp);
			return (127);
		}
		ft_split_clear(temp);
		i++;
	}
	return (0);
}


int parse_input(char *input, t_settings *settings)
{
	char **splitted_input;

	splitted_input = split(input, settings);
	print_splitted(splitted_input);
	// print_dict(settings->env);
	if (strings_equal(splitted_input[0], "exit"))
	{
		ft_split_clear(splitted_input);
		return (0);
	}
	if (!splitted_input[0])
	{
		ft_split_clear(splitted_input);
		return (1);
	}
	if (strings_equal(splitted_input[0], "echo\0"))
	{
		settings->last_exit_status = echo(splitted_input + 1);
		ft_split_clear(splitted_input);
		return (1);
	}
	if (strings_equal(splitted_input[0], "pwd\0"))
	{
		settings->last_exit_status = pwd(splitted_input);
		ft_split_clear(splitted_input);
		return (1);
	}
	if (strings_equal(splitted_input[0], "cd\0"))
	{
		settings->last_exit_status = cd(splitted_input, settings);
		ft_split_clear(splitted_input);
		return (1);
	}
	if (strings_equal(splitted_input[0], "env\0"))
	{
		settings->last_exit_status = env(splitted_input, settings);
		ft_split_clear(splitted_input);
		return (1);
	}
	if (strings_equal(splitted_input[0], "unset\0"))
	{
		settings->last_exit_status = unset(splitted_input, settings);
		ft_split_clear(splitted_input);
		return (1);
	}
	if (strings_equal(splitted_input[0], "export\0"))
	{
		settings->last_exit_status = export(splitted_input, settings);
		ft_split_clear(splitted_input);
		return (1);
	}
	if (ft_strchr(splitted_input[0], '='))
	{
		settings->last_exit_status = deal_with_equal_sign(splitted_input, settings);
		ft_split_clear(splitted_input);
		return (1);
	}
	else
	{
		printf("%s is an unknown command\n", splitted_input[0]);
		settings->last_exit_status = 127;
		ft_split_clear(splitted_input);
		return (1);
	}
}


void	finish(t_settings *settings, char *input)
{
	clear_settings(settings);
	free(input);
}

char	*my_readline(void)
{
	char *res;
	char *prompt;
	
	prompt = ft_str_join_free_first(cur_dir(), "> \0");
	res = readline(prompt);
	free(prompt);
	return (res);
}

void	interrupt_input(int sig)
{
	rl_redisplay();
}

void	shell(char *envp[])
{
	char *res;
	t_settings *settings;

	settings = create_setttings(envp);
	// signal(SIGINT, interrupt_input);
	settings->last_working_directory = cur_dir();
	res = my_readline();
	if (!parse_input(res, settings))
		return (finish(settings, res));
	while (res)
	{
		add_history(res);
		free(res);
		res = my_readline();
		if (!parse_input(res, settings))
			return (finish(settings, res));
	}
	finish(settings, res);
}

int main(int argc, char **argv, char **envp)
{
	if (argc && argv)
	shell(envp);
	system("leaks shell");
}