/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sbritani <sbritani@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/22 17:52:49 by sbritani          #+#    #+#             */
/*   Updated: 2023/01/25 19:13:35 by sbritani         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

#include <stdlib.h>
#include <stdio.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <unistd.h>
#include "libft/libft.h"


typedef struct dict_s
{
	char	**keys;
	char	**values;
	int		len;
}	t_dict;

typedef struct	settings_s
{
	int		last_exit_status;
	t_dict	*env;
	t_dict	*exported_env;
	char	*last_working_directory;
}	t_settings;

char		*ft_str_join_free_first(char *str1, char *str2);
int			strings_equal(char *str1, char *str2);
t_settings	*create_setttings(char **env);
void		clear_settings(t_settings *settings);
char		**add_string_to_string_arr(char *str, char **arr, int n);
char		**remove_string_from_str_arr(char **arr, int n);
char		*str_copy(char *str, int n);
char	*ft_str_join_free_both(char *str1, char *str2);

// dict funcitons
t_dict	*init_dict(void);
void	dict_add(t_dict *dict, char *key, char *value);
char	*dict_get(t_dict *dict, char *key, char *default_return);
void	dict_delete(t_dict *dict, char *key);
void	free_dict(t_dict *dict);

// printing some stuff functions
void	print_dict(t_dict *dict);
void	print_splitted(char **splitted);

//echo functions
int	echo(char **args);

// split functions
typedef struct next_arg_return_s
{
	char	*arg;
	int		last_index;
}	t_next_arg_return;

int	is_bash_special_char(char c);
t_next_arg_return *get_next_arg(char *input, t_settings *settings);
char **split(char *input, t_settings *settings);
//split utils functions
t_next_arg_return	*init_next_arg(void);
void	free_next_arg_return(t_next_arg_return *next_arg);


#endif