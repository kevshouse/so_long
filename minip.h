/* ************************************************************************** */
/*																			*/
/*														:::	  ::::::::   */
/*   minip.h											:+:	  :+:	:+:   */
/*													+:+ +:+		 +:+	 */
/*   By: keanders <keanders@student.42london.com	+#+  +:+	   +#+		*/
/*												+#+#+#+#+#+   +#+		   */
/*   Created: 2025/03/06 18:06:15 by keanders		  #+#	#+#			 */
/*   Updated: 2025/03/06 19:04:56 by keanders		 ###   ########.fr	   */
/*																			*/
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

# define KEY_LEFT 65361
# define KEY_RIGHT 65363
# define KEY_UP 65362
# define KEY_DOWN 65364
# define KEY_ESC 65307
# define KEY_SPACE 32
# define MOVEMENT_STEP 5

typedef struct s_point
{
	int			x;
	int			y;
}				t_point;

typedef struct s_square
{
	t_point			top_left;
	t_point			top_right;
	t_point			bottom_right;
	t_point			bottom_left;
	//t_point			rotation_centre;
}				t_square;
// Function prototypes to init aand draw a square.
t_square create_square(t_point top_left, t_point top_right, t_point bottom_right, t_point bottom_left);
void draw_square(void *mlx, void *win, t_square square);

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
	t_square	dynamic_square;
	int			endian;
}				t_data;

/* Function Prototypes */
void			clear_buffer(t_data *data, void *buffer, int color);
void			render_sprite(t_data *data, t_sprite *sprite, void *buffer);
void			render_frame(t_data *data);
int				game_loop(t_data *data);
int				key_hook(int keycode, t_data *data);
void			handle_input(t_data *data, int keycode);
int				check_collision(t_sprite *a, t_sprite *b);
void move_square(t_square *square, int dx, int dy); // Ensure this prototype is present
int			close_window(t_data *data);
void			apply_gravity(t_sprite *sprite);
void			update_position(t_sprite *sprite);
void			handle_collision(t_sprite *sprite, t_sprite *obstacle);
void			move_square(t_square *square, int dx, int dy);



#endif
