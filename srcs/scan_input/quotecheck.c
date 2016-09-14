/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quotecheck.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tboos <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/05/11 14:28:55 by tboos             #+#    #+#             */
/*   Updated: 2016/09/13 15:35:44 by rbaran           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*ft_backslash(char **str)
{
	if (**str == '\\')
		++(*str);
	if (**str)
		++(*str);
	else
		return (BACK_ERR);
	return (NULL);
}

static char	*ft_gonext(char **str, char c)
{
	char		*test;

	++(*str);
	if ((test = ft_matchchr(str)))
	{
		if (**str == c)
			return (NULL);
		else
			return (test);
	}
	else
		return (PAR_ERR);
}

static char	*ft_gonextquote(char **str, char c)
{
	++(*str);
	while (**str && **str != c)
		if (ft_backslash(str))
			return (BACK_ERR);
	if (**str != c)
		return (QUOTE_ERR);
	++(*str);
	return (NULL);
}

char		*ft_matchchr(char **str)
{
	static char	needle[] = "]})";

	while (**str)
	{
		if (ft_strchr(needle, **str))
			return (PAR_ERR);
		if (**str == '(')
			return (ft_gonext(str, ')'));
		if (**str == '[')
			return (ft_gonext(str, ']'));
		if (**str == '{')
			return (ft_gonext(str, '}'));
		if (**str == '`')
			return (ft_gonext(str, '`'));
		if (**str == '#')
			break ;
		if (**str == '\'' && ft_gonextquote(str, **str))
			return (QUOTE_ERR);
		if (**str == '\"' && ft_gonextquote(str, **str))
			return (QUOTE_ERR);
		if (ft_backslash(str))
			return (BACK_ERR);
	}
	**str = 0;
	return (NULL);
}

int			ft_quotecheck(t_stream *stream)
{
	char		*test;

	test = stream->command;
	if ((test = ft_matchchr(&test)))
		return (ft_underline_mess(test, stream));
	return (1);
}
