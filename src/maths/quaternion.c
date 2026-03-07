/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quaternion.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vbleskin <vbleskin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/05 20:50:00 by vbleskin          #+#    #+#             */
/*   Updated: 2026/03/07 12:09:32 by vbleskin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fdf.h"

/**
 * Convertit les angles d'Euler en quaternion.
 * 
 * @param rot_x Angle de rotation autour de l'axe X en degrés.
 * @param rot_y Angle de rotation autour de l'axe Y en degrés.
 * @param rot_z Angle de rotation autour de l'axe Z en degrés.
 * @return Quaternion représentant la rotation. 
 */
t_quat	ft_euler_to_quat(double rot_x, double rot_y, double rot_z)
{
	t_quat			quaternion;
	t_quat_maths	q_maths;

	rot_x = ft_to_rad(rot_x);
	rot_y = ft_to_rad(rot_y);
	rot_z = ft_to_rad(rot_z);
	q_maths.cr = cos(rot_x * 0.5);
	q_maths.sr = sin(rot_x * 0.5);
	q_maths.cp = cos(rot_y * 0.5);
	q_maths.sp = sin(rot_y * 0.5);
	q_maths.cy = cos(rot_z * 0.5);
	q_maths.sy = sin(rot_z * 0.5);
	quaternion.w = q_maths.cr * q_maths.cp * q_maths.cy
		+ q_maths.sr * q_maths.sp * q_maths.sy;
	quaternion.x = q_maths.sr * q_maths.cp * q_maths.cy
		- q_maths.cr * q_maths.sp * q_maths.sy;
	quaternion.y = q_maths.cr * q_maths.sp * q_maths.cy
		+ q_maths.sr * q_maths.cp * q_maths.sy;
	quaternion.z = q_maths.cr * q_maths.cp * q_maths.sy
		- q_maths.sr * q_maths.sp * q_maths.cy;
	return (quaternion);
}

/**
 * Convertit un quaternion en angles d'Euler.
 * 
 * @param quat Quaternion à convertir.
 * @return Angles d'Euler en degrés. 
 */
t_properties	ft_quat_to_euler(t_quat q)
{
	t_properties	rot;
	double			sinp;

	rot.type = 'R';
	rot.x = atan2(2.0 * (q.w * q.x + q.y * q.z),
			1.0 - 2.0 * (q.x * q.x + q.y * q.y)) * 180.0 / M_PI;
	sinp = 2.0 * (q.w * q.y - q.z * q.x);
	if (ft_abs_double(sinp) >= 1.0)
	{
		if (sinp > 0.0)
			rot.y = 90.0;
		else
			rot.y = -90.0;
	}
	else
		rot.y = asin(sinp) * 180.0 / M_PI;
	rot.z = atan2(2.0 * (q.w * q.z + q.x * q.y),
			1.0 - 2.0 * (q.y * q.y + q.z * q.z)) * 180.0 / M_PI;
	return (rot);
}

/**
 * Interpolation linéaire entre deux quaternions.
 * 
 * @param quat1 Premier quaternion.
 * @param quat2 Second quaternion.
 * @param factor Facteur d'interpolation (0.0 à 1.0).
 * @return Quaternion interpolé.
*/
static t_quat	ft_lerp_quat(t_quat quat1, t_quat quat2, double factor)
{
	t_quat	interpolated_quat;

	interpolated_quat.x = quat1.x + factor * (quat2.x - quat1.x);
	interpolated_quat.y = quat1.y + factor * (quat2.y - quat1.y);
	interpolated_quat.z = quat1.z + factor * (quat2.z - quat1.z);
	interpolated_quat.w = quat1.w + factor * (quat2.w - quat1.w);
	return (interpolated_quat);
}

/**
 * Interpolation sphérique linéaire entre deux quaternions.
 * 
 * @param quat1 Premier quaternion.
 * @param quat2 Second quaternion.
 * @param factor Facteur d'interpolation (0.0 à 1.0).
 * @return Quaternion interpolé.
*/
t_quat	ft_slerp(t_quat quat1, t_quat quat2, double factor)
{
	t_quat	inter_quat;
	double	v[4];
	double	dot;

	dot = quat1.x * quat2.x + quat1.y * quat2.y
		+ quat1.z * quat2.z + quat1.w * quat2.w;
	if (dot < 0.0)
	{
		quat2.x = -quat2.x;
		quat2.y = -quat2.y;
		quat2.z = -quat2.z;
		quat2.w = -quat2.w;
		dot = -dot;
	}
	if (dot > 0.9995)
		return (ft_lerp_quat(quat1, quat2, factor));
	v[0] = acos(dot);
	v[1] = sin(v[0]);
	v[2] = sin((1.0 - factor) * v[0]) / v[1];
	v[3] = sin(factor * v[0]) / v[1];
	inter_quat.x = quat1.x * v[2] + quat2.x * v[3];
	inter_quat.y = quat1.y * v[2] + quat2.y * v[3];
	inter_quat.z = quat1.z * v[2] + quat2.z * v[3];
	inter_quat.w = quat1.w * v[2] + quat2.w * v[3];
	return (inter_quat);
}
