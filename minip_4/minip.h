#ifndef MINIP_H
# define MINIP_H

# include "../mlx_linux/mlx.h"
# include <X11/X.h>
# include <X11/keysym.h>
# include <math.h>
# include <stdio.h>
# include <stdlib.h>
# include <time.h>

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
# define KEY_W XK_w // Up arrow key
# define KEY_A XK_a // Left arrow key
# define KEY_S XK_s // Down arrow key
# define KEY_D XK_d // Right arrow key
# define SCALE_FACTOR 0.05  // 10% scaling

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
}				t_square;

// Function prototypes to init and draw a square.
t_square create_square(t_point top_left, t_point top_right,
	t_point bottom_right, t_point bottom_left);
void draw_square(void *mlx, void *win, t_square square);

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
	t_square	dynamic_square;
	t_square	second_dynamic_square;
	 void		*dolphin_img;
	char		*dolphin_data;
	int		dolphin_width;
	int		dolphin_height;
	int		dolphin_size_line;
	int			endian;
}				t_data;

/* Function Prototypes */
void			clear_buffer(t_data *data, void *buffer, int color);
void			render_frame(t_data *data);
int				game_loop(t_data *data);
int				key_hook(int keycode, t_data *data);
void			handle_input(t_data *data, int keycode);
void			move_square(t_square *square, int dx, int dy);
int				close_window(t_data *data);



#endif
