#include "mlx/mlx.h"
#include	<stdio.h>
#include	<stdlib.h>

#define		X_EVENT_KEY_PRESS		2
#define		X_EVENT_KEY_EXIT		17
#define		X_EVENT_SCROLL_UP		4
#define		X_EVENT_SCROLL_DOWN		5
#define		WIDTH					800
#define		HEIGHT					600
#define		KEY_ESC					53

typedef struct	s_complex
{
	double		re;
	double		im;
}				t_complex;

typedef	struct	s_img{
	void		*ptr;
	char		*data;
	int			size_l;
	int			bpp;
	int			endian;
}				t_img;

typedef	struct	s_mlx{
	void		*mlx_ptr;
	void		*win;
	double		zoom;
	int			max_iter;
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

int		color_set(int iter, t_mlx *mlx)
{
	double	r;
	double	b;

	if (iter == mlx->max_iter)
		return (0);
	r = (((double)iter + 1) * 25) / 2;
	b = (((double)iter + 1) * 25) / 2;
	return (((int)(r) << 16) + (int)(b));
}

int		get_mandelbrot_iter(t_mlx *mlx)
{
	int			iter;
	double		x;
	double		x2;
	double		y;
	double		y2;

	iter = 0;
	x = 0;
	y = 0;
	x2 = 0;
	y2 = 0;
	while (x2 + y2 <= 4 && (iter < mlx->max_iter))
	{
		y = 2 * x * y + mlx->c.im;
		x = x2 - y2 + mlx->c.re;
		x2 = x * x;
		y2 = y * y;
		iter++;
	}
	return (iter);
}

int		get_mandelbar_iter(t_mlx *mlx)
{
	int			iter;
	t_complex	z;
	t_complex	z2;

	iter = 0;
	z = init_complex(mlx->c.re, mlx->c.im);
	z2 = init_complex(z.re * z.re, z.im * z.im);
	while (z2.re + z2.im <= 4 && iter < mlx->max_iter)
	{
		z.im = -2.0 * z.re * z.im + mlx->c.im;
		z.re = z2.re - z2.im + mlx->c.re;
		z2.re = z.re * z.re;
		z2.im = z.im * z.im;
		iter++;
	}
	return (iter);
}

int		get_julia_iter(t_mlx *mlx)
{
	int			iter;
	t_complex	z;
	t_complex	z2;
	t_complex	k;

	iter = 0;
	z = init_complex(mlx->c.re, mlx->c.im);
	z2 = init_complex(z.re * z.re, z.im * z.im);
	k = init_complex(-0.701756, -0.3842);
	while (z2.re + z2.im <= 4 && iter < mlx->max_iter)
	{
		z.im = 2.0 * z.re * z.im + k.im;
		z.re = z2.re - z2.im + k.re;
		z2.re = z.re * z.re;
		z2.im = z.im * z.im;
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
	mlx->img.ptr = mlx_new_image(mlx->mlx_ptr, WIDTH, HEIGHT);
	if (!mlx->img.ptr)
		return (0);
	mlx->img.data = mlx_get_data_addr(mlx->img.ptr, &mlx->img.bpp, &mlx->img.size_l, &mlx->img.endian);
	if (!mlx->img.data)
		return (0);
	return (1);
}

int		fractol_init(t_mlx *mlx)
{
	mlx->zoom = 1.0;
	mlx->max_iter = 100;
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
	int		x;
	int		y;

	mlx->factor = init_complex(
			(mlx->max.re - mlx->min.re) / WIDTH,
			(mlx->max.im - mlx->min.im) / HEIGHT);
	y = mlx->min.im;
	while (y <= HEIGHT)
	{
		mlx->c.im = mlx->min.im + y * mlx->factor.im;
		x = mlx->min.re;
		while (x <= WIDTH)
		{
			mlx->c.re = mlx->min.re + x * mlx->factor.re;
			//iter = get_mandelbrot_iter(mlx);
			//iter = get_julia_iter(mlx);
			iter = get_mandelbar_iter(mlx);
			put_pixel(&(mlx->img), x, y, color_set(iter, mlx));
			x++;
		}
		y++;
	}
	mlx_put_image_to_window(mlx->mlx_ptr, mlx->win, mlx->img.ptr, 0, 0);
}

void	reset_min_max(t_complex pos, t_mlx *mlx)
{
	mlx->max.re = pos.re + (mlx->max.re - pos.re) * mlx->zoom;
	mlx->min.re = pos.re + (mlx->min.re - pos.re) * mlx->zoom;
	mlx->max.im = pos.im + (mlx->max.im - pos.im) * mlx->zoom;
	mlx->min.im = pos.im + (mlx->min.im - pos.im) * mlx->zoom;
}

int		mouse_event(int keycode,int x, int y, t_mlx *mlx)
{
	t_complex pos;
	
	if (keycode == X_EVENT_SCROLL_UP || keycode == X_EVENT_SCROLL_DOWN)
	{
		pos = init_complex(
				mlx->min.re + (double)x * mlx->factor.re,
				mlx->min.im + (double)y * mlx->factor.im);
		if (keycode == X_EVENT_SCROLL_UP)
			//mlx->zoom *= 1.02;
			mlx->zoom = 1.2;
		else
		{
			//mlx->zoom *= 0.98;
			mlx->zoom = 0.8;
			mlx->max_iter += 10;
		}
		if (mlx->zoom >= 2)
			return (0);
		reset_min_max(pos, mlx);
		mlx_clear_window(mlx->mlx_ptr, mlx->win);
		printf("zoom rate : %lf max_iter :%d\n", mlx->zoom, mlx->max_iter);
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
