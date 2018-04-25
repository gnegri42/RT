/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shadows.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bmuselet <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/03/21 16:13:41 by bmuselet          #+#    #+#             */
/*   Updated: 2018/04/12 10:43:19 by bmuselet         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

static void	calc_shadow_pt2(t_env *e, t_ray *light_ray)
{
	t_vec	temp;

	if (((t_obj*)e->objs->content) != light_ray->hit_obj)
	{
		temp = vector_substraction(light_ray->pos,
				((t_obj*)e->objs->content)->pos);
		if (((t_obj*)e->objs->content)->type == SPH)
			light_ray->length = sphere_inter(e, light_ray, temp);
		if (((t_obj*)e->objs->content)->type == CYL)
			light_ray->length = cylindre_inter(e, light_ray, temp);
		if (((t_obj*)e->objs->content)->type == CON)
			light_ray->length = cone_inter(e, light_ray, temp);
		if (((t_obj*)e->objs->content)->type == PLA)
			light_ray->length = plan_inter(e, light_ray);
	}
}

int			calc_shadow(t_env *e, t_ray light_ray)
{
	double	dist_obj_to_light;
	t_list	*tmp;

	dist_obj_to_light = length_between_vectors(light_ray.hit_pos,
		((t_obj*)e->lights->content)->pos);
	light_ray.length = INFINITE;
	light_ray.pos = light_ray.hit_pos;
	light_ray.dir = light_ray.hit_dir;
	tmp = e->objs;
	while (e->objs != NULL)
	{
		calc_shadow_pt2(e, &light_ray);
		if (light_ray.length < dist_obj_to_light)
		{
			e->objs = tmp;
			return (1);
		}
		e->objs = e->objs->next;
	}
	e->objs = tmp;
	return (0);
}
