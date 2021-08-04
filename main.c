#include "mlx/mlx.h"
#include	<stdio.h>
#include	<stdlib.h>
#include	<math.h>

#define		X_EVENT_KEY_PRESS		2
#define		X_EVENT_KEY_EXIT		17
#define		X_EVENT_SCROLL_UP		4
#define		X_EVENT_SCROLL_DOWN		5
#define		WIDTH					800
#define		HEIGHT					600
#define		KEY_ESC					53
#define		ITER_MAX				30

typedef struct	s_complex
{
	double		re;
	double		im;
}				t_complex;

typedef	struct	s_img{
	void		*img_ptr;
	char		*data;
	int			size_l;
	int			bpp;
	int			endian;
}				t_img;

typedef	struct	s_mlx{
	void		*mlx_ptr;
	void		*win;
	t_img		img;
	t_complex	c;
	t_complex	min;
	t_complex	max;
	t_complex	factor;
}				t_mlx;

t_complex	init_complex(double re, double im)
{
	t_complex complex;

	complex.re = re;
	complex.im = im;
	return (complex);
}

int		color_set(int iter)
{
	double	r;
	double	b;

	if (iter == ITER_MAX)
		return (0);
	r = (((double)iter + 1) * 25) / 2;
	b = (((double)iter + 1) * 25) / 2;
	return (((int)(r) << 16) + (int)(b));
}

int		get_mandelbrot_iter(t_mlx *mlx)
{
	int			iter;
	t_complex	z;

	iter = 0;
	z = init_complex(mlx->c.re, mlx->c.im);
	while ((pow(z.re, 2.0) + pow(z.im, 2.0) <= 4) && (iter < ITER_MAX))
	{
		z = init_complex(
				pow(z.re, 2.0) - pow(z.im, 2.0) + mlx->c.re,
				2.0 * z.re * z.im + mlx->c.im);
		iter++;
	}
	return (iter);
}

int		get_julia_iter(int pos_w, int pos_h, int iter, t_mlx *mlx)
{
	double	c_re;
	double	c_im;
	double	x;
	double	x_new;
	double	y;

	c_re = -0.7;
	c_im = 0;
	x = ((pos_w - WIDTH / 2.0) * 4.0 / WIDTH);
	y = ((HEIGHT / 2.0) - pos_h) * 4.0 / HEIGHT;
	while ((pow(x, 2.0) + pow(y, 2.0) < 4) && (iter < ITER_MAX))
	{
		x_new = pow(x, 2.0) - pow(y, 2.0) + mlx->c.re;
		y = 2 * x * y + mlx->c.im;
		x = x_new;
		iter++;
	}
	return (iter);
}

int		window_init(t_mlx *mlx)
{
	mlx->mlx_ptr = mlx_init();
	if (!mlx->mlx_ptr)
		return (0);
	mlx->win = mlx_new_window(mlx->mlx_ptr, WIDTH, HEIGHT, "fractol");
	if (!mlx->win)
		return (0);
	mlx->img.img_ptr = mlx_new_image(mlx->mlx_ptr, WIDTH, HEIGHT);
	if (!mlx->img.img_ptr)
		return (0);
	mlx->img.data = mlx_get_data_addr(mlx->img.img_ptr, &mlx->img.bpp, &mlx->img.size_l, &mlx->img.endian);
	if (!mlx->img.data)
		return (0);
	return (1);
}

int		fractol_init(t_mlx *mlx)
{
	mlx->min = init_complex(-2.0, -1.0);
	mlx->max = init_complex(1.0, 1.0);
	return (1);
}

void	put_pixel(t_img *img, int x, int y, int color)
{
	char	*dst;

	dst = img->data + (y * img->size_l) + (x * img->bpp / 8);
	*(unsigned int *)dst = color;
}

void	draw(t_mlx *mlx)
{
	int		iter;
	int		color;
	int		pos_w;
	int		pos_h;

	mlx->factor = init_complex(
			(mlx->max.re - mlx->min.re) / WIDTH,
			(mlx->max.im - mlx->min.im) / HEIGHT);
	pos_h = mlx->min.im;
	while (pos_h <= HEIGHT)
	{
		mlx->c.im = mlx->max.im - pos_h * mlx->factor.im;
		pos_w = mlx->min.re;
		while (pos_w <= WIDTH)
		{
			mlx->c.re = mlx->min.re + pos_w * mlx->factor.re;
			iter = get_mandelbrot_iter(mlx);
			//iter = get_julia_iter(pos_h, pos_w, 0, mlx);
			put_pixel(&(mlx->img), pos_w, pos_h, color_set(iter));
			pos_w++;
		}
		pos_h++;
	}
	mlx_put_image_to_window(mlx->mlx_ptr, mlx->win, mlx->img.img_ptr, 0, 0);
}

void	reset_min_max(double zoom_rate, t_complex pos, t_mlx *mlx)
{
	mlx->max.re = pos.re + (mlx->max.re - pos.re) * zoom_rate;
	mlx->min.re = pos.re + (mlx->min.re - pos.re) * zoom_rate;
	mlx->max.im = pos.im + (mlx->max.im - pos.im) * zoom_rate;
	mlx->min.im = pos.im + (mlx->min.im - pos.im) * zoom_rate;
}

int		mouse_event(int keycode,int x, int y, t_mlx *mlx)
{
	t_complex pos;
	double zoom_rate;

	if (keycode == X_EVENT_SCROLL_UP || keycode == X_EVENT_SCROLL_DOWN)
	{
		pos = init_complex(
				mlx->min.re + (double)x * mlx->factor.re,
				mlx->max.im - (double)y * mlx->factor.im);
		if (keycode == X_EVENT_SCROLL_UP)
			zoom_rate = 1.3;
		else
			zoom_rate = 0.7;
		reset_min_max(zoom_rate, pos, mlx);
//		mlx_clear_window(mlx->mlx_ptr, mlx->win);
//		printf("mouse wheel %lf\n", zoom_rate);
		draw(mlx);
	}
	return (0);
}

int		key_press(int keycode)
{
	if (keycode == KEY_ESC)
		exit(0);
	return(0);
}

int		close_window(int keycode)
{
	exit(0);
}

int		main(void)
{
	t_mlx	mlx;

	if (!window_init(&mlx))
		return (0);
	fractol_init(&mlx);
	draw(&mlx);
	mlx_hook(mlx.win, X_EVENT_KEY_PRESS, 0, key_press, 0);
	mlx_hook(mlx.win, X_EVENT_KEY_EXIT, 0, close_window, 0);
	mlx_hook(mlx.win, X_EVENT_SCROLL_UP, 0, mouse_event, &mlx);
	mlx_hook(mlx.win, X_EVENT_SCROLL_DOWN, 0, mouse_event, &mlx);
	mlx_loop(mlx.mlx_ptr);
	return (0);
}
