/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quaternion.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vbleskin <vbleskin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/05 20:50:00 by vbleskin          #+#    #+#             */
/*   Updated: 2026/03/05 20:50:00 by vbleskin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fdf.h"

/**
 * Convertit les angles d'Euler en quaternion.
 * 
 * @param rx Angle de rotation autour de l'axe X en degrés.
 * @param ry Angle de rotation autour de l'axe Y en degrés.
 * @param rz Angle de rotation autour de l'axe Z en degrés.
 * @return Quaternion représentant la rotation. 
*/
t_quat	ft_euler_to_quat(double rx, double ry, double rz)
{
	t_quat	q;
	double	cr;
	double	sr;
	double	cp;
	double	sp;
	double	cy;
	double	sy;

	rx = ft_to_rad(rx);
	ry = ft_to_rad(ry);
	rz = ft_to_rad(rz);
	cr = cos(rx * 0.5);
	sr = sin(rx * 0.5);
	cp = cos(ry * 0.5);
	sp = sin(ry * 0.5);
	cy = cos(rz * 0.5);
	sy = sin(rz * 0.5);
	q.w = cr * cp * cy + sr * sp * sy;
	q.x = sr * cp * cy - cr * sp * sy;
	q.y = cr * sp * cy + sr * cp * sy;
	q.z = cr * cp * sy - sr * sp * cy;
	return (q);
}

/*
 * Convertit un quaternion en angles d'Euler.
 * 
 * @param q Quaternion à convertir.
 * @return Angles d'Euler en degrés. 
*/
t_properties	ft_quat_to_euler(t_quat q)
{
	t_properties	rot;
	double			sinr_cosp;
	double			cosr_cosp;
	double			sinp;
	double			siny_cosp;
	double			cosy_cosp;

	rot.type = 'R';
	sinr_cosp = 2.0 * (q.w * q.x + q.y * q.z);
	cosr_cosp = 1.0 - 2.0 * (q.x * q.x + q.y * q.y);
	rot.x = atan2(sinr_cosp, cosr_cosp) * 180.0 / M_PI;
	sinp = 2.0 * (q.w * q.y - q.z * q.x);
	if (fabs(sinp) >= 1.0)
	{
		if (sinp > 0.0)
			rot.y = (M_PI / 2.0) * 180.0 / M_PI;
		else
			rot.y = -(M_PI / 2.0) * 180.0 / M_PI;
	}
	else
		rot.y = asin(sinp) * 180.0 / M_PI;
	siny_cosp = 2.0 * (q.w * q.z + q.x * q.y);
	cosy_cosp = 1.0 - 2.0 * (q.y * q.y + q.z * q.z);
	rot.z = atan2(siny_cosp, cosy_cosp) * 180.0 / M_PI;
	return (rot);
}

/*
 * Interpolation sphérique linéaire entre deux quaternions.
 * 
 * @param q1 Premier quaternion.
 * @param q2 Second quaternion.
 * @param t Facteur d'interpolation (0.0 à 1.0).
 * @return Quaternion interpolé.
*/
t_quat	ft_slerp(t_quat q1, t_quat q2, double t)
{
	t_quat	res;
	double	dot;
	double	theta;
	double	sin_theta;
	double	w1;
	double	w2;

	dot = q1.x * q2.x + q1.y * q2.y + q1.z * q2.z + q1.w * q2.w;
	if (dot < 0.0)
	{
		q2.x = -q2.x;
		q2.y = -q2.y;
		q2.z = -q2.z;
		q2.w = -q2.w;
		dot = -dot;
	}
	if (dot > 0.9995)
	{
		res.x = q1.x + t * (q2.x - q1.x);
		res.y = q1.y + t * (q2.y - q1.y);
		res.z = q1.z + t * (q2.z - q1.z);
		res.w = q1.w + t * (q2.w - q1.w);
		return (res);
	}
	theta = acos(dot);
	sin_theta = sin(theta);
	w1 = sin((1.0 - t) * theta) / sin_theta;
	w2 = sin(t * theta) / sin_theta;
	res.x = q1.x * w1 + q2.x * w2;
	res.y = q1.y * w1 + q2.y * w2;
	res.z = q1.z * w1 + q2.z * w2;
	res.w = q1.w * w1 + q2.w * w2;
	return (res);
}
