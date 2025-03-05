/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   so_long.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: keanders <keanders@student.42london.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/26 17:35:40 by keanders          #+#    #+#             */
/*   Updated: 2025/03/04 15:28:42 by keanders         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SO_LONG_H
# define SO_LONG_H

# include "./mlx_linux/mlx.h"
# include <math.h>
# include <stdlib.h>
# include <time.h>

typedef struct s_vertx
{
	int		x;
	int		y;
}			t_vertx;

typedef struct s_linep2p
{
	int		step;
	int		step_y;
	int		dx;
	int		dy;
	int		i;
	int		x;
	int		y;
	int		err;
	int		e2;
}			t_linep2p;

typedef struct s_data

{
	void	*img;
	char	*addr;
	int		bits_per_pixel;
	int		line_length;
	int		endian;
}			t_data;


#endif