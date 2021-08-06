#include "fractol.h"

//#include	"mlx/mlx.h"
//#include	<stdio.h>
//#include	<stdlib.h>
//#include <math.h>
//#define		KEY_EXIT		17
//#define		KEY_ESC			53
//#define		KEY_PRESS		2
//#define		SCROLL_UP		4
//#define		SCROLL_DOWN		5
//
//#define		C				8
//
//#define		LEFT			123
//#define		RIGHT			124
//#define		DOWN			125
//#define		UP				126
//
//#define		WIDTH			800
//#define		HEIGHT			600
//
//typedef struct	s_complex
//{
//	double		re;
//	double		im;
//}				t_complex;
//
//typedef	struct	s_img{
//	void		*ptr;
//	char		*data;
//	int			size_l;
//	int			bpp;
//	int			endian;
//}				t_img;
//
//typedef	struct	s_f{
//	void		*ptr;
//	void		*win;
//	int			color_style;
//	int			max_iter;
//	int			(*func)(struct s_f*);
//	t_img		img;
//	t_complex	c;
//	t_complex	min;
//	t_complex	max;
//	t_complex	factor;
//}				t_f;
//
//int					ft_strcmp(const char *s1, const char *s2);
//
//t_complex	init_complex(double re, double im)
//{
//	t_complex	complex;
//
//	complex.re = re;
//	complex.im = im;
//	return (complex);
//}
//
//int	color_set(int iter, t_f *f)
//{
//	double	t;
//	int		red;
//	int		green;
//	int		blue;
//
//	if (iter == f->max_iter)
//		return (0x00000000);
//	if (f->color_style == 0)
//	{
//		t = (double)iter / (double)f->max_iter;
//		red = (int)(9 * (1 - t) * pow (t, 3) * 255);
//		green = (int)(15 * pow((1 - t), 2) * pow(t, 2) * 255);
//		blue = (int)(8.5 * pow((1 - t), 3) * t * 255);
//	}
//	else if (f->color_style == 1)
//	{
//		red = sin(0.3 * (double)iter);
//		green = sin(0.3 * (double)iter + 3) * 127 + 128;
//		blue = sin(0.3 * (double)iter + 3) * 127 + 128;
//	}
//	else if (f->color_style == 2)
//	{
//		red = (((double)iter + 1) * 25) / 2;
//		green = 0;
//		blue = (((double)iter + 1) * 25) / 2;
//	}
//	return ((red << 16) + (green << 8) + blue);
//}
//
//int	mandelbrot(t_f *f)
//{
//	int			iter;
//	double		x;
//	double		x2;
//	double		y;
//	double		y2;
//
//	iter = 0;
//	x = 0;
//	y = 0;
//	x2 = 0;
//	y2 = 0;
//	while (x2 + y2 <= 4 && (iter < f->max_iter))
//	{
//		y = 2 * x * y + f->c.im;
//		x = x2 - y2 + f->c.re;
//		x2 = x * x;
//		y2 = y * y;
//		iter++;
//	}
//	return (iter);
//}
//
//int	tricorn(t_f *f)
//{
//	int			iter;
//	t_complex	z;
//	t_complex	z2;
//
//	iter = 0;
//	z = init_complex(f->c.re, f->c.im);
//	z2 = init_complex(z.re * z.re, z.im * z.im);
//	while (z2.re + z2.im <= 4 && iter < f->max_iter)
//	{
//		z.im = -2.0 * z.re * z.im + f->c.im;
//		z.re = z2.re - z2.im + f->c.re;
//		z2.re = z.re * z.re;
//		z2.im = z.im * z.im;
//		iter++;
//	}
//	return (iter);
//}
//
//int	julia(t_f *f)
//{
//	int			iter;
//	t_complex	z;
//	t_complex	z2;
//	t_complex	k;
//
//	iter = 0;
//	z = init_complex(f->c.re, f->c.im);
//	z2 = init_complex(z.re * z.re, z.im * z.im);
//	k = init_complex(-0.701756, -0.3842);
//	while (z2.re + z2.im <= 4 && iter < f->max_iter)
//	{
//		z.im = 2.0 * z.re * z.im + k.im;
//		z.re = z2.re - z2.im + k.re;
//		z2.re = z.re * z.re;
//		z2.im = z.im * z.im;
//		iter++;
//	}
//	return (iter);
//}
//
//int	init_window(t_f *f)
//{
//	f->ptr = mlx_init();
//	if (!f->ptr)
//		return (0);
//	f->win = mlx_new_window(f->ptr, WIDTH, HEIGHT, "fractol");
//	if (!f->win)
//		return (0);
//	f->img.ptr = mlx_new_image(f->ptr, WIDTH, HEIGHT);
//	if (!f->img.ptr)
//		return (0);
//	f->img.data = mlx_get_data_addr(f->img.ptr, &f->img.bpp,\
//	&f->img.size_l, &f->img.endian);
//	if (!f->img.data)
//		return (0);
//	mlx_string_put(f->ptr, f->win, 100, 100, 0x00FFFFFF, "C : change color");
//	return (1);
//}
//
//void	init_fractal(t_f *f)
//{
//	f->max_iter = 100;
//	f->color_style = 0;
//	f->min = init_complex(-2.0, -1.0);
//	f->max = init_complex(1.0, 1.0);
//}
//
//void	put_pixel(int x, int y, int color, t_f *f)
//{
//	char	*dst;
//
//	dst = f->img.data + (y * f->img.size_l) + (x * f->img.bpp / 8);
//	*(unsigned int *)dst = color;
//}
//
//void	draw(t_f *f)
//{
//	int		iter;
//	int		color;
//	int		x;
//	int		y;
//
//	f->factor = init_complex(
//			(f->max.re - f->min.re) / WIDTH,
//			(f->max.im - f->min.im) / HEIGHT);
//	y = 0;
//	while (y <= HEIGHT)
//	{
//		f->c.im = f->min.im + y * f->factor.im;
//		x = 0;
//		while (x <= WIDTH)
//		{
//			f->c.re = f->min.re + x * f->factor.re;
//			put_pixel(x, y, color_set(f->func(f), f), f);
//			x++;
//		}
//		y++;
//	}
//	mlx_put_image_to_window(f->ptr, f->win, f->img.ptr, 0, 0);
//}
//
//void	interpolate(double zoom_rate, t_complex pos, t_f *f)
//{
//	f->max.re = pos.re + (f->max.re - pos.re) * zoom_rate;
//	f->min.re = pos.re + (f->min.re - pos.re) * zoom_rate;
//	f->max.im = pos.im + (f->max.im - pos.im) * zoom_rate;
//	f->min.im = pos.im + (f->min.im - pos.im) * zoom_rate;
//}
//
//int	zoom(int key, int x, int y, t_f *f)
//{
//	t_complex	pos;
//	double		zoom_rate;	
//
//	if (key == SCROLL_UP || key == SCROLL_DOWN)
//	{
//		pos = init_complex(
//				f->min.re + (double)x * f->factor.re,
//				f->min.im + (double)y * f->factor.im);
//		if (key == SCROLL_UP)
//			zoom_rate = 1.2;
//		else
//		{
//			zoom_rate = 0.8;
//			f->max_iter += 20;
//		}
//		interpolate(zoom_rate, pos, f);
//		mlx_clear_window(f->ptr, f->win);
//		printf("max_iter :%d\n", f->max_iter);
//		draw(f);
//	}
//	return (0);
//}
//
//void move(int key, t_f *f)
//{
//	t_complex	factor;
//
//	factor = init_complex(f->max.re - f->min.re, f->max.im - f->min.im);
//	if (key == LEFT)
//	{
//		f->max.re -= factor.re * 0.05;
//		f->min.re -= factor.re * 0.05;
//	}
//	else if (key == RIGHT)
//	{
//		f->max.re += factor.re * 0.05;
//		f->min.re += factor.re * 0.05;
//	}
//	else if (key == DOWN)
//	{
//		f->max.im += factor.im * 0.05;
//		f->min.im += factor.im * 0.05;
//	}	
//	else if (key == UP)
//	{
//		f->max.im -= factor.im * 0.05;
//		f->min.im -= factor.im * 0.05;
//	}
//	draw(f);
//}
//
//int	key_press(int key, t_f *f)
//{
//	if (key == KEY_ESC)
//		exit(0);
//	else if (key == C)
//	{
//		f->color_style = (f->color_style + 1) % 3;
//		draw(f);
//	}
//	else if (key == LEFT || key == RIGHT || key == DOWN || key == UP)
//		move(key, f);
//	return (0);
//}
//
//int	close_window(int key)
//{
//	exit(0);
//}
//
//int	check_argv(t_f *f, int argc, char **argv)
//{
//	if (argc == 2)
//	{
//		if (!ft_strcmp(argv[1], "mandelbrot"))
//		{
//			f->func = &mandelbrot;
//			return (1);
//		}
//		else if (!ft_strcmp(argv[1], "julia"))
//		{
//			f->func = &julia;
//			return (1);
//		}
//		else if (!ft_strcmp(argv[1], "tricorn"))
//		{
//			f->func = &tricorn;
//			return (1);
//		}
//	}
//	printf("Choose fractal \n* mandelbrot \n* julia \n* tricorn\n");
//	return (0);
//}
//
//void hooks_loop(t_f *f)
//{
//	mlx_hook(f->win, KEY_PRESS, 0, key_press, f);
//	mlx_hook(f->win, SCROLL_UP, 0, zoom, f);
//	mlx_hook(f->win, SCROLL_DOWN, 0, zoom, f);
//	mlx_hook(f->win, KEY_EXIT, 0, close_window, 0);
//	mlx_loop(f->ptr);
//}

int	main(int argc, char **argv)
{
	t_f	f;

	if (!init_window(&f) || !check_argv(&f, argc, argv))
		return (0);
	init_fractal(&f);
	draw(&f);
	hooks_loop(&f);
}
