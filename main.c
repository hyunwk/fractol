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
	void		*ptr;
	void		*win;
	double		zoom;
	int			max_iter;
	int			(*func)(struct s_mlx*);
	t_img		img;
	t_complex	c;
	t_complex	min;
	t_complex	max;
	t_complex	factor;
}				t_mlx;

int					ft_strcmp(const char *s1, const char *s2);

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

int		get_tricorn_iter(t_mlx *mlx)
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

//t_mlx	*window_init()
int		window_init(t_mlx *mlx)
{
	mlx->ptr = mlx_init();
	if (!mlx->ptr)
		return (0);
	mlx->win = mlx_new_window(mlx->ptr, WIDTH, HEIGHT, "fractol");
	if (!mlx->win)
		return (0);
	mlx->img.ptr = mlx_new_image(mlx->ptr, WIDTH, HEIGHT);
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
	y = mlx->min.im - 1;
	while (++y <= HEIGHT)
	{
		mlx->c.im = mlx->min.im + y * mlx->factor.im;
		x = mlx->min.re - 1;
		while (++x <= WIDTH)
		{
			mlx->c.re = mlx->min.re + x * mlx->factor.re;
			iter = mlx->func(mlx);
			put_pixel(&(mlx->img), x, y, color_set(iter, mlx));
		}
	}
	mlx_put_image_to_window(mlx->ptr, mlx->win, mlx->img.ptr, 0, 0);
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
	double		zoom_rate;	

	if (keycode == X_EVENT_SCROLL_UP || keycode == X_EVENT_SCROLL_DOWN)
	{
		pos = init_complex(
				mlx->min.re + (double)x * mlx->factor.re,
				mlx->min.im + (double)y * mlx->factor.im);
		if (keycode == X_EVENT_SCROLL_UP)
		{
			if (mlx->zoom >= 2)
				return (0);
			mlx->zoom *= 1.2;
			zoom_rate = 1.2;
		}
		else
		{
			mlx->zoom *= 0.8;
			zoom_rate = 0.8;
			mlx->max_iter += 20;
		}
		reset_min_max(zoom_rate, pos, mlx);
		mlx_clear_window(mlx->ptr, mlx->win);
		printf("zoom rate : %f max_iter :%d\n", mlx->zoom, mlx->max_iter);
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

int		check_argv(t_mlx *mlx, int argc, char **argv)
{
	if (argc == 2)
	{
		if (!ft_strcmp(argv[1], "mandelbrot"))
		{
			mlx->func = &get_mandelbrot_iter;
			return (1);
		}
		else if (!ft_strcmp(argv[1], "julia"))
		{
			mlx->func = &get_julia_iter;
			return (1);
		}
		else if (!ft_strcmp(argv[1], "tricorn"))
		{
			mlx->func = &get_tricorn_iter;
			return (1);
		}
	}
	printf("Choose fractal \n* mandelbrot \n* julia \n* tricorn\n");
	return (0);
}

void	hooks(t_mlx *mlx)
{
	mlx_hook(mlx->win, X_EVENT_KEY_PRESS, 0, key_press, 0);
	mlx_hook(mlx->win, X_EVENT_KEY_EXIT, 0, close_window, 0);
	mlx_hook(mlx->win, X_EVENT_SCROLL_UP, 0, mouse_event, mlx);
	mlx_hook(mlx->win, X_EVENT_SCROLL_DOWN, 0, mouse_event, mlx);
	mlx_loop(mlx->ptr);
}

int		main(int argc, char **argv)
{
	t_mlx	mlx;

	if (!window_init(&mlx) || !check_argv(&mlx, argc, argv))
		return (0);
	fractol_init(&mlx);
	draw(&mlx);
	hooks(&mlx);
}
