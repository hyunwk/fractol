#include "mlx/mlx.h"
#include	<stdio.h>
#include	<stdlib.h>
#include	<math.h>

#define		X_EVENT_KEY_PRESS		2
#define		X_EVENT_KEY_EXIT		17
#define		X_EVENT_SCROLL_UP		4
#define		X_EVENT_SCROLL_DOWN		5
#define		WIN_WIDTH				800
#define		WIN_HEIGHT				600
#define		KEY_ESC					53
#define		ITER_MAX				30
//#define		ITER_MAX				100

typedef	struct	s_img{
	void		*img_ptr;
	char		*data;
	int			size_l;
	int			bpp;
	int			endian;
	int			zoom;
	double		min_re;
	double		max_re;
	double		min_im;
	double		max_im;
	double		factor_re;
	double		factor_im;
}				t_img;

typedef	struct	s_mlx{
	void		*mlx_ptr;
	void		*win;
	int			zoom;
	double		c_re;
	double		c_im;

	t_img		img;
}				t_mlx;

int		color_set(int iter)
{
	double	r;
	double	g;
	double	b;
	int		color;

	r = sin(0.3 * (double)iter);
	//g = sin(0.3 * (double)iter + 3) * 127 + 128;
	//b = sin(0.3 * (double)iter + 3) * 127 + 128;
	g = sin(0.3 * (double)iter + 10) * 127 + 12;
	b = sin(0.3 * (double)iter + 10) * 127 + 12;
	color = ((int)(255.999 * r) << 16) + ((int)(255.999 * g) << 8) + ((int)(255.999 * b));
	return (color);
}


//int		get_mandelbrot_iter(int location_w, int location_h, int iter)
int		get_mandelbrot_iter(int location_w, int location_h, int iter, t_img *img)
{
	double c_re;
	double c_im;
	double x;
	double x_new;
	double y;

	//c_re = ((location_w - WIN_WIDTH / 2) * 3.0 / WIN_WIDTH) - 0.5;
	c_re = img->min_re + location_w * img->factor_re; 
	//c_im = ((WIN_HEIGHT / 2) - location_h) * 2.0 / WIN_HEIGHT;
	c_im = img->min_im + location_h * img->factor_im; 
//	printf("fat_re : %lf fat_im : %lf\n",img->factor_re, img->factor_im);
//	printf("c_re : %lf c+im : %lf\n",c_re, c_im);
	//x = 0;
	x = c_re;
	//y = 0;
	y = c_im;
	while ((pow(x, 2.0) + pow(y, 2.0) < 4) && (iter < ITER_MAX))
	{
		x_new = pow(x, 2.0) - pow(y, 2.0) + c_re;
		y = 2 * x * y + c_im;
		x = x_new;
		iter++;
	}
	return (iter);
}

int		get_julia_iter(int location_w, int location_h, int iter, t_img *img)
{
	double	c_re;
	double	c_im;
	double	x;
	double	x_new;
	double	y;

	c_re = -0.7;
	c_im = 0;
	x = ((location_w - WIN_WIDTH / 2) * 4.0 / WIN_WIDTH);
	y = ((WIN_HEIGHT / 2) - location_h) * 4.0 / WIN_HEIGHT;
	while ((pow(x, 2.0) + pow(y, 2.0) < 4) && (iter < ITER_MAX))
	{
		x_new = pow(x, 2.0) - pow(y, 2.0) + c_re;
		y = 2 * x * y + c_im;
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
	mlx->win = mlx_new_window(mlx->mlx_ptr, WIN_WIDTH, WIN_HEIGHT, "fractol");
	if (!mlx->win)
		return (0);
	mlx->img.img_ptr = mlx_new_image(mlx->mlx_ptr, WIN_WIDTH, WIN_HEIGHT);
	if (!mlx->img.img_ptr)
		return (0);
	mlx->img.data = mlx_get_data_addr(mlx->img.img_ptr, &mlx->img.bpp, &mlx->img.size_l, &mlx->img.endian);
	if (!mlx->img.data)
		return (0);
	mlx->img.zoom = 1.0;
	mlx->img.max_im = 1.0;
	mlx->img.min_im = -1.0;
	mlx->img.max_re = 1.0;
	mlx->img.min_re = -2.0;
	return (1);
}



void	put_pixel(t_img *img, int x, int y, int color)
{
	char	*dst;

	dst = img->data + (y * img->size_l) + (x * img->bpp / 8);
	*(unsigned int *)dst = color;
}


void	start_draw(t_img *img, t_mlx *mlx)
{
	int		iter;
	int		color;
	int		location_w;
	int		location_h;

	mlx_clear_window (mlx->mlx_ptr, mlx->win);
	img->factor_re = (img->max_re - img->min_re) / WIN_WIDTH;
	img->factor_im = (img->max_im - img->min_im) / WIN_HEIGHT;
	//location_h = 0;
	location_h = mlx->img.min_im;
	while (location_h <= WIN_HEIGHT)
	//while (location_h <= mlx->img.max_im)
	{
		//location_w = 0;
		location_w = mlx->img.min_re;
		while (location_w <= WIN_WIDTH)
		//while (location_w <= mlx->img.max_re)
		{
			iter = get_mandelbrot_iter(location_w, location_h, 0, img);

			if (iter < ITER_MAX)
			{
				put_pixel(img, location_w, location_h, color);
				color = color_set(iter);
			}
			else
				put_pixel(img, location_w, location_h, 0);

			//location_w++;
			location_w += img->factor_re;
		}
		//location_h++;
		location_h += img->factor_im;
	}
}
int		mouse_event(int keycode,int x, int y, t_mlx *mlx)
{
	int temp;
	double zoom_rate;

	temp = 0;
	if (keycode == X_EVENT_SCROLL_UP)
	{
		temp++;
		zoom_rate = 1.5;
	}
	else if (keycode == X_EVENT_SCROLL_DOWN)
	{
		temp--;
		zoom_rate = 0.5;
	}
	else
		return (0);
	//mlx->img.zoom *= zoom_rate;
//	mlx->img.max_re *= mlx->img.zoom;
//	mlx->img.min_re *= mlx->img.zoom;
//	mlx->img.max_im *= mlx->img.zoom;
//	mlx->img.min_im *= mlx->img.zoom;
	mlx->img.max_re *= zoom_rate;
	mlx->img.min_re *= zoom_rate;
	mlx->img.max_im *= zoom_rate;
	mlx->img.min_im *= zoom_rate;

	printf("temp %d\n", temp);
	start_draw(&(mlx->img), mlx);
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
	start_draw(&mlx.img, &mlx);
	mlx_put_image_to_window(mlx.mlx_ptr, mlx.win, mlx.img.img_ptr, 0, 0);
	mlx_hook(mlx.win, X_EVENT_KEY_PRESS, 0, key_press, 0);
	mlx_hook(mlx.win, X_EVENT_KEY_EXIT, 0, close_window, 0);
	mlx_hook(mlx.win, X_EVENT_SCROLL_UP, 0, mouse_event, &mlx);
	mlx_hook(mlx.win, X_EVENT_SCROLL_DOWN, 0, mouse_event, &mlx);
	mlx_loop(mlx.mlx_ptr);
	return (0);
}
