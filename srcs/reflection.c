/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   reflection.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgreil <mgreil@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/03/01 16:51:50 by mgreil            #+#    #+#             */
/*   Updated: 2018/04/23 14:50:47 by mgreil           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

t_vec			get_reflection(t_vec ray_dir, t_vec normal)
{
	t_vec	reflection;

	reflection = vector_double_product(normal,
		vector_dot_product(ray_dir, normal) * 2);
	reflection = vector_normalize(
		vector_substraction(ray_dir, reflection));
	return (reflection);
}

static void		init_ray(t_ray *to_init, t_ray ray)
{
	to_init->length = INFINITE;
	to_init->hit_obj = NULL;
	to_init->pos = ray.hit_pos;
	to_init->dir = get_reflection(ray.dir, get_normal(ray.hit_pos, ray));
}

static t_color	color_ray(t_ray ray, t_ray new_ray)
{
	if (new_ray.hit_obj)
		new_ray.color = color_balanced(ray.color, new_ray.hit_obj->color,
			1 - ray.hit_obj->refl, ray.hit_obj->refl);
	else
		new_ray.color = color_balanced(ray.color, (t_color){0, 0, 0},
			1 - ray.hit_obj->refl, ray.hit_obj->refl);
	return (new_ray.color);
}

void			ray_refl(t_env *e, t_ray *ray, int nb_rebond)
{
	t_list	*tmp;
	t_ray	new_ray;

	init_ray(&new_ray, *ray);
	tmp = e->objs;
	while (e->objs != NULL)
	{
		if (((t_obj*)e->objs->content) != ray->hit_obj)
			check_inter_objects(e, &new_ray);
		e->objs = e->objs->next;
	}
	e->objs = tmp;
	new_ray.color = color_ray(*ray, new_ray);
	if (new_ray.hit_obj && new_ray.hit_obj->refr > 0 && nb_rebond < NB_MIRROR)
		ray_refr(e, &new_ray, nb_rebond + 1);
	else if (new_ray.hit_obj && new_ray.hit_obj->refl > 0
		&& nb_rebond < NB_MIRROR)
		ray_refl(e, &new_ray, nb_rebond + 1);
	else if (new_ray.hit_obj && new_ray.hit_obj->refl > 0)
		new_ray.hit_obj = NULL;
	*ray = new_ray;
}
