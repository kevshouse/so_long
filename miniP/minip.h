/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minip.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: keanders <keanders@student.42london.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/06 18:06:15 by keanders          #+#    #+#             */
/*   Updated: 2025/03/06 19:04:56 by keanders         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINIP_H
# define MINIP_H

# include "../mlx_linux/mlx.h"
# include <X11/X.h>
# include <X11/keysym.h>
# include <math.h>
# include <stdio.h>
# include <stdlib.h>
# include <time.h>
# include <stdio.h>

# define WINDOW_WIDTH 600
# define WINDOW_HEIGHT 300
# define MLX_ERROR 1
# define RED_PIXEL 0xFF0000
# define GREEN_PIXEL 0xFF00
# define WHITE_PIXEL 0xFFFFFF

# define KEY_LEFT 123
# define KEY_RIGHT 124
# define KEY_UP 126
# define KEY_DOWN 125
# define KEY_ESC 53
# define KEY_SPACE 49

typedef struct s_sprite
{
	int			x;
	int			y;
	int			width;
	int			height;
	int			vx;
	int			vy;
	int			gravity;
	int			grounded;
	void		*img;
}				t_sprite;

typedef struct s_data
{
	void		*mlx;
	void		*win;
	void		*front_buffer;
	void		*back_buffer;
	char		*front_data;
	char		*back_data;
	int			bpp;
	int			size_line;
	t_sprite	player;
	t_sprite	enemy;
	t_sprite	obstacle;
	int			endian;
}				t_data;

/* Function Prototypes */
void			clear_buffer(t_data *data, void *buffer, int color);
void			render_sprite(t_data *data, t_sprite *sprite, void *buffer);
void			render_frame(t_data *data);
int				game_loop(t_data *data);
int				key_hook(int keycode, t_data *data);
void			handle_input(t_sprite *sprite, int keycode);
int				check_collision(t_sprite *a, t_sprite *b);
void			apply_gravity(t_sprite *sprite);
void			update_position(t_sprite *sprite);
void			handle_collision(t_sprite *sprite, t_sprite *obstacle);



#endif
