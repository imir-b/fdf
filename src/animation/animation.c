/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   animation.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vlad <vlad@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/31 21:27:49 by vlad              #+#    #+#             */
/*   Updated: 2026/02/01 22:31:46 by vlad             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fdf.h"

// User Interface :
// - FPS :fait
// - menu de selection d'animations / poses (= parser et stocker les noms) :parsing fait

// Appliquer dans cet ordre : 
// 1 - scale
// 2 - rotate
// 3 - translate

/**
 * logique transition entre 2 frames :
 * 1 - trouver/stocker la clé précédente
 * 2 - trouver la clé suivante
 * 3 - calculer le % d'avancement entre les deux
 * 4 - calculer la valeur pondéré <-- (A * (1 - t) + (B * t))
 * t = 0 <-- 100% A + 0% B
 * t = 0.5 <-- 50% A + 50% B
 * t = 0.9 <-- 10% A + 90% B
 */