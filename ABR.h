/* **************************************************************************** */
/*                                                                              */
/*                                                       ::::::::  :::   :::    */
/*   CY-Meteo                                           :+:    :+: :+:   :+:    */
/*                                                    +:+         +:+ +:+       */
/*   By: Durandnico <durandnico@cy-tech.fr>          +#+          +#++:         */
/*                                                 +#+           +#+            */
/*   Created: 17/01/2023 17:31:54 by Durandnico   #+#    #+#    #+#             */
/*   Updated: None                                ########     ###              */
/*                                                                              */
/* **************************************************************************** */


/*! 
 *  \file ABR.h
 *  \author DURAND Nicolas Erich Pierre <nicolas.durand@cy-tech.fr>
 *  \version 1.0
 *  \date Tue 17 January 2023 - 17:31:54
 *
 *  \brief Headers of ABR.c
 *
 *
 */

#ifndef __ABR_H
#define __ABR_H
#define _XOPEN_SOURCE //nécessaire pour strptime car ??? personne ne sait 

// librairies
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

#define MAX(a,b)(a<b) ? b : a
#define MIN(a,b)(a>b) ? b : a

#define ERREUR_SAISIE -1
#define HELP 0

/*!
 *  \struct t_station
 *  \author DURAND Nicolas Erich Pierre <nicolas.durand@cy-tech.fr>
 *  \version 1.0
 *  \date Wed 18 January 2023 - 00:13:44
 *  \brief 
 *  \param 
 */

typedef struct  s_station
{
    int     id;
    double  temp_atmos;
    double  temp_min;
    double  temp_max;
    double  pres_max;
    double  pres_moy;
    double  pres_min;
    double  vitesse_vent;
    int     orientation_vent;
    int     altitude;
    int     humidite;

    float     longitude;
    float     lattitude;
    time_t  date;
    int     heurs;
}               t_station;

typedef struct      s_node
{
    t_station       data;
    int count;
    /*Main tree (X axe)*/
    struct s_node*  fG;
    struct s_node*  fD;

}                   t_node;

typedef t_node* Arbre;

Arbre creerNoeud(t_station value);

/*
 * mode == 2 => par vent -w
 * mode == 1 => par pression -p 1
 * mode == 0 => par température-t 1
 * 
 */
Arbre inserNoeudSPT(Arbre A, t_station add, int mode);

/*
 * mode == 1 => par pression -p 2
 * mode == 0 => par température -t 2
 */
Arbre inserNoeudDPT(Arbre A,t_station val,int tri);

/*
 * mode == 0 => par humidité -m
 */
Arbre inserNoeudHU(Arbre A, t_station add, int mode);

/*
 * mode != 0 => par altitude -h
 */
Arbre inserNoeudAL(Arbre A, t_station add);

/*
 *mode == 1 => par pression -p 3
 *mode == 0 => par pression -t 3
 */
Arbre inserNoeudDPS(Arbre A, t_station add, int mode);

/*
 *
 * mode ==  1 : -t 1
 *          2 : -p 1
 *          3 : -w
 *          4 : -h
 *          5 : -m
 *          6 : -t 2
 *          7 : -p 2
 * 
 */
void ParcoursInfixe(Arbre A, FILE* fichier, int mode);
#endif  