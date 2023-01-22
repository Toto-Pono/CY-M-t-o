//librairies
#include "ABR.h"

Arbre creerNoeud(t_station val){
    Arbre new = malloc(sizeof(t_node));
    
    new->data   = val;
    new->fG     = NULL;
    new->fD     = NULL;

    return (new);
}


/*
 * mode == 2 => par vent -w
 * mode == 1 => par pression -p 1
 * mode == 0 => par température-t 1
 * 
 */
Arbre inserNoeudSPT(Arbre A, t_station val, int mode){
    if(A == NULL){
        A = creerNoeud(val);
        A->data.pres_min    = A->data.pres_moy;
        A->data.pres_max    = A->data.pres_moy;
        A->data.temp_min    = A->data.temp_atmos;
        A->data.temp_max    = A->data.temp_atmos;
        A->count = 1;
    }

    else if(A->data.id == val.id){
        
        A->count++;
        if(!mode){
            A->data.temp_max    = MAX(A->data.temp_max, val.temp_max);
            A->data.temp_atmos += val.temp_atmos;
            A->data.temp_min    = MIN(A->data.temp_min, val.temp_min); 
        }

        else if (mode == 1){
            A->data.pres_min    = MIN(A->data.pres_min, val.pres_moy);
            A->data.pres_moy   += val.pres_moy;
            A->data.pres_max    = MAX(A->data.pres_max,  val.pres_moy);
        }
        
        else if (mode == 2){
            A->data.vitesse_vent += val.vitesse_vent;
            A->data.orientation_vent += val.orientation_vent;
            A->count++;
        }
    }
        
    else if(A->data.id < val.id)
        A->fD = inserNoeudSPT(A->fD, val, mode);

    else
        A->fG = inserNoeudSPT(A->fG, val, mode);

    return (A);
}

/*
 * mode == 1 => par pression -p 2
 * mode == 0 => par température -t 2
 */
Arbre inserNoeudDPT(Arbre A,t_station val,int mode){
    if(A == NULL){
        A = creerNoeud(val);
        A->count = 1;
    }

    else if(A->data.date == val.date){
        A->count++;
        if(mode == 1){
            A->data.pres_moy   += val.pres_moy;
        }

        else if (mode == 0){
            A->data.temp_atmos += val.temp_atmos;
        }
    }
        
    else if(difftime(A->data.date, val.date) > 0){
        A->fD = inserNoeudDPT(A->fD, val, mode);}

    else{
        A->fG = inserNoeudDPT(A->fG, val, mode);
    }
    return (A);   
}


/*Insertion pour humide -m (mode != 0) et altitude -h (mode == 0) [il n'y a qu'une unique altitude donc pas besoin de faire de modif]*/
Arbre inserNoeudHU(Arbre A, t_station add, int mode){
     if(A == NULL){
        A = creerNoeud(add);
     }
        
    else if(A->data.id == add.id){   
        if(!mode)
            A->data.humidite    = MAX(A->data.humidite, add.humidite);
    }
        
    else if(A->data.humidite > add.humidite)
        A->fD = inserNoeudHU(A->fD, add, mode);

    else
        A->fG = inserNoeudHU(A->fG, add, mode);

    return (A);
}

Arbre inserNoeudAL(Arbre A, t_station add){
     if(A == NULL){
        A = creerNoeud(add);
     }
    else if (A->data.altitude > add.altitude && A->data.id != add.id)
        A->fD = inserNoeudAL(A->fD, add);

    else if (A->data.altitude <= add.altitude && A->data.id != add.id)
        {
        A->fG = inserNoeudAL(A->fG, add);
        }
    return (A);
}

/*
 *mode == 1 => par pression -p 3
 *mode == 0 => par pression -t 3
 */
Arbre inserNoeudDPS(Arbre A, t_station val, int mode){
    if(A == NULL){
        A = creerNoeud(val);
    }

    else if(difftime(A->data.date, val.date) == 0){
        
        if(A->data.id < val.id)
            A->fD = inserNoeudDPS(A->fD, val, mode);

        else if (A->data.id > val.id)
        {
            A->fG = inserNoeudDPS(A->fG, val, mode);
        }
    }
        
    else if(difftime(A->data.date, val.date) > 0){
        A->fD = inserNoeudDPS(A->fD, val, mode);}

    else{
        A->fG = inserNoeudDPS(A->fG, val, mode);
    }
    return (A);   
}


/*
 *
 * mode ==  1 : -t 1
 *          2 : -p 1
 *          3 : -w
 *          4 : -h
 *          5 : -m
 *          6 : -t 2
 *          7 : -p 2
 *          8 : -t 3
 *          9 : -p 3
 */
void ParcoursInfixe(Arbre A, FILE* fichier, int mode){

    if(A != NULL){
        ParcoursInfixe(A->fG,fichier, mode);

        struct tm * pTime;
        char buffer[ 1024 ];

        switch (mode)
        {
        /*Température: id|min|max|moy*/
        case 1:
            /*donné qui vont être écrite dans le csv*/
            fprintf(fichier, "%d;%lf;%lf;%lf\n", A->data.id, A->data.temp_min, A->data.temp_max , A->data.temp_atmos / A->count);
            break;

        /*Pression: id|min|max|moy*/
        case 2:
            fprintf(fichier, "%d;%lf;%lf;%lf\n", A->data.id, A->data.pres_min, A->data.pres_max , A->data.pres_moy / A->count);
            break;

        /*Vent: id|orientation moy|Vitesse moy*/
        case 3:
            fprintf(fichier, "%d;%d;%lf\n", A->data.id, A->data.orientation_vent / A->count, A->data.vitesse_vent / A->count);
            break;
        
        /*Altitude: id|altitude*/
        /*C'est pas vraiment pas ordre décroissant :c*/
        case 4:
            fprintf(fichier, "%d;%d\n", A->data.id, A->data.altitude);
            break;
            
        /*Humidité: id|humidité*/
        case 5:
            fprintf(fichier, "%d;%d\n", A->data.id, A->data.humidite);
            break;
        
        /*Température 2 : id|moy*/
        case 6:
            /*transforme time_t en date DD/MM/YYYY:H:M:S*/
            pTime = localtime( & A->data.date );
            strftime( buffer, 1024, "%d/%m/%Y %H:%M:%S", pTime );
    
            fprintf(fichier, "%s;%lf\n", buffer , A->data.temp_atmos / A->count);
            break;

        /*pression 2 : id|moy*/
        case 7:
            /*transforme time_t en date DD/MM/YYYY H:M:S*/
            pTime = localtime( & A->data.date );
            strftime( buffer, 1024, "%d/%m/%Y %H:%M:%S", pTime );
    
            fprintf(fichier, "%s;%lf\n", buffer , A->data.pres_moy / A->count);
            break;

        /*Température 3 : date/id/tmp*/
        case 8:
            /*transforme time_t en date DD/MM/YYYY H:M:S*/
            pTime = localtime( & A->data.date );
            strftime( buffer, 1024, "%d/%m/%Y %H:%M:%S", pTime );
            
            fprintf(fichier, "%s;%d;%lf\n", buffer , A->data.id, A->data.temp_atmos);
            break;

        /*Pression 3 : date/id/pre*/
        case 9:
            /*transforme time_t en date DD/MM/YYYY H:M:S*/
            pTime = localtime( & A->data.date );
            strftime( buffer, 1024, "%d/%m/%Y %H:%M:%S", pTime );
            
            fprintf(fichier, "%s;%d;%lf\n", buffer , A->data.id, A->data.pres_moy);
            break;

        default:
            break;
        }

        ParcoursInfixe(A->fD, fichier, mode);
    }
}
