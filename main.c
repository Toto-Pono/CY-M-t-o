#include "ABR.h"
#define TAILLE 50                                                                                           //Taille de caracère maximum des châines de caractères récupérées

t_station recup_data_ligne(FILE* fichier, char*  caractereActuel)
{
  char      chaine[TAILLE];       //créer un tableau de cractère de taille TAILLE 
  t_station new_station;          //structure qui va contenir les infos de la ligne
  int       numero_chaine = 0;    //initialise une variable pour compter les chaines récupérées
  int       i = 1;
  char*     p;
  struct tm t;
  
  chaine[0] = *caractereActuel;

  /*on récupère les données de la ligne et on en retourne une station*/
  while(*caractereActuel!='\n'&&(*caractereActuel!=EOF)){
    /*on récupère un caractère*/
    *caractereActuel = fgetc(fichier);

    //si caractereActuel != (; ou EndOfFile ou retour à la ligne) et on ajoute caractereActuel à sa chaine, case i du tableau de caractère
    if((*caractereActuel!=';')&&(*caractereActuel!=EOF)&&(*caractereActuel!='\n')){                            
      chaine[i] = *caractereActuel; 
      i = i+1;
    }

    /*on a atteint un séparateur, on affecte la data*/
    else{
      chaine[i] = '\0';
      numero_chaine++;
      switch (numero_chaine) {
        case 1:
          /*id station*/
          new_station.id = atoi(chaine);
          break;

        case 2:  
          /*date*/
          /*Merci coco*/
          /*Bon des fois il lit la bonne valeur - 1 je ne comprend pas pk*/
          strptime(chaine, "%Y-%m-%dT%H:%M:%S+01:00", &t);
          new_station.date = mktime(&t);
          break;

        case 3:
          /*pression mer*/
          /*useless*/
          //new_station.pres_max = atof(chaine);
          break;

        case 4:
          /*orientation du vent*/
          new_station.orientation_vent = atoi(chaine);
          break;

        case 5:
          /*vitesse vent moyenne*/
          new_station.vitesse_vent = atof(chaine);
          break;
            
        case 6:
          /*humidité*/
          new_station.humidite = atoi(chaine);
          break;

        case 7:
          /*pression station*/
          new_station.pres_moy = atof(chaine);
          break;

        case 8:
          /*var pression 24h*/
          /*inutile*/
          //printf("8 :%s\n", chaine);
          break;

        case 9:
          /*précipitation 24h*/
          /*Inutile*/
          //printf("9 :%s\n", chaine);
          break;

        case 10:
          /*coordonnées*/
          p  = strchr(chaine, ',');
          *p = '\0';
          
          new_station.lattitude = atof(chaine);
          new_station.longitude = atof(p+1);
          break;

        case 11:
          /*température*/
          new_station.temp_atmos = atof(chaine);
          break;

        case 12:
          /*température min*/
          new_station.temp_min = MIN(atof(chaine), new_station.temp_atmos);

          /*check si min est bien dans le csv*/
          if(new_station.temp_min == 0)
            new_station.temp_min = new_station.temp_atmos;
          break;

        case 13:
          /*température max*/
          new_station.temp_max = MAX(atof(chaine), new_station.temp_atmos);
          break;

        case 14:
          /*altitude*/
          new_station.altitude = atoi(chaine);
          break;

        case 15:
          /*communes*/
          /*Inutile ici*/
          //printf("15 :%s\n", chaine);
          break;

        default:
          break;
      }
      i=0;
    }
  }

  return (new_station);
}


void
Ecrire_fichier(const char* fic_name, Arbre a, int tri, int type)
{
  FILE* fichier = NULL;

  /*on ouvre le fichier en mode ecriture*/
  fichier = fopen(fic_name, "w");
  if (fichier == NULL){
    fprintf(stderr, "Erreur écriture\n");
    exit(-1);
  }

  /*en fonction du tri choisie*/
  switch (tri)
  {
  /*-t*/
  case 0:
  if(type == 1){//fprintf(fichier, "id station;Température min;Température max;Température moy\n");
      ParcoursInfixe(a, fichier, 1);
    }
    else if (type == 2){
      ParcoursInfixe(a, fichier, 6);
    }
    else{
      ParcoursInfixe(a, fichier, 8);
    }
    break;

  /*-p*/
  case 1:
    if(type == 1){
      ParcoursInfixe(a, fichier, 2);
    }
    else if (type == 2){
      ParcoursInfixe(a, fichier, 7);
    }
    else{
      ParcoursInfixe(a, fichier, 9);
    }
    break;
  /*-w*/
  case 2:
    ParcoursInfixe(a, fichier, 3);
    break;
  
  /*-h*/
  case 3:
    ParcoursInfixe(a, fichier, 4);
    break;
  
  /*-m*/
  case 4:
    ParcoursInfixe(a, fichier, 5);
    break;

  default:
    fprintf(stderr, "mauvais argument dans Ecrire fichier\n");
    break;
  }

  fclose(fichier);

  printf("\t%s a été crée avec succès!\n", fic_name);
}



int main(int argc, char** argv)
{
  FILE* fichier = NULL;       //flux de lecture
  char caractereActuel = 0;   //initialise le caractère récupéré                                                                                    
  Arbre a = NULL;             //ABR pour ranger les donnés rapidement
  t_station tmp;              //temp station qui va etre add
  int boucle;
  int indiceMode;
  int verif;
  int mode;
  int tri;
  float Gmin = -__FLT_MAX__ - 1;  //valeur minimun  
  float Gmax = __FLT_MAX__;     //valeur max 
  float Amin = -__FLT_MAX__-1;
  float Amax = __FLT_MAX__;
  time_t Dmin = -__LONG_MAX__-1; //time_t est un typedef d'un long
  time_t Dmax = __LONG_MAX__;
  char* nomF;
  char* nomS;
  struct tm t;

  //Initialisation
  indiceMode = 0;
  verif = 0;
  boucle = 0;
  tri = -1;
  mode = -1;
  nomS = "meteo0000.csv";




  if (argc == 2 && !(strcmp(argv[1], "--help")))
    {
      printf("\n<C>\n");
      printf("Pour utiliser le programme correctement, veuillez saisir l'un des arguments suivants :\n\n");
      printf("-t <mode> : (t)emperatures.\n");
      printf("-p <mode> : (p)ressions atmosphériques.\n");
      printf("Pour ces 2 options, il faut indiquer la valeur de <mode> :\n");
      printf("\t◦ 1 : produit en sortie les températures (ou pressions) minimales, maximales et moyennes par station dans l’ordre\n");
      printf("\tcroissant du numéro de station.\n");
      printf("\t◦ 2 : produit en sortie les températures (ou pressions) moyennes par date/heure, triées dans l’ordre\n");
      printf("\tchronologique. La moyenne se fait sur toutes les stations.\n");
      printf("\t◦ 3 : produit en sortie les températures (ou pressions) par date/heure par station. Elles seront triées d’abord par ordre\n");
      printf("\tchronologique, puis par ordre croissant de l’identifiant de lastation.\n");
      printf("-w : vent ( (w)ind ). Produit en sortie l’orientation moyenne et la vitesse moyenne des\n");
      printf("\tvents pour chaque station. Quand on parle de moyenne, il s’agira\n");
      printf("\tde faire la somme pour chaque composante du vecteur, et d’en\n");
      printf("\tfaire la moyenne une fois tous les vecteurs traités. On aura donc\n");
      printf("\tune moyenne sur l’axe X et une moyenne sur l’axe Y : les 2 résultats\n");
      printf("\tfournissant le module et l’orientation moyens demandés. Les\n");
      printf("\tdonnées seront triées par identifiant croissant de la station.\n");
      printf("-h : (h)auteur. Produit en sortie la hauteur pour chaque station. Les hauteurs seront triées par ordre décroissant.\n");
      printf("-m : humidité ( (m)oisture ). Produit en sortie l’humidité maximale pour chaque station.\n");
      printf("\tLes valeurs d’humidités seront triées par ordre décroissant.\n");
      printf("\nVeuillez saisir obligatoirement ces deux options :\n");
      printf("-i \"nom_fichier.csv\" : (f)ichier d’entrée. Permet de spécifier le chemin du fichier CSV d’entrée (fichier fourni). (Mettre les guillemets /!\\) \n");
      printf("-o \"nom_fichier.csv\" : (f)ichier de sortie. Permet de donner un nom au fichier de sortie contenant les données. (Mettre les guillemets /!\\) \n");
      printf("\n<OPTIONS>\n");
      printf("Pour restreindre le nombre de données, vous pouvez utilisez ces options :\n\n");
      printf("-g <min> <max> : lon(g)itude. Permet de filtrer les données de sortie en ne gardant que les\n");
      printf("\tdonnées qui sont dans l’intervalle de longitudes [<min>..<max>]\n");
      printf("\tincluses. Le format des longitudes est un nombre réel.\n");
      printf("-a <min> <max> : l(a)titude. Permet de filtrer les données de sortie en ne gardant que les\n");
      printf("\trelevés qui sont dans l’intervalle de latitudes [<min>..<max>]\n");
      printf("\tincluses. Le format des latitudes est un nombre réel.\n");
      printf("-d <min> <max> : (d)ates. Permet de filtrer les données de sortie en ne gardant que les\n");
      printf("\trelevés qui sont dans l’intervalle de dates [<min>..<max>] incluses.\n");
      printf("\tLe format des dates est une chaine de type YYYY-MM-DD (année-mois-jour).\n");
      exit(HELP);
    }
  else if (argc >= 6)
  {
  
    while ((boucle < argc-1) && (verif == 0))
    {
      if (!(strcmp(argv[boucle], "-p")))
      {
        //On retient ici où est saisie la méthode demandée
        indiceMode = boucle+1;
        verif = 1;
        tri = 1;
        mode = atoi(argv[indiceMode]);
        printf("%d\n", mode);
        if (mode < 1 || mode > 3)
        {
          printf("\n<ERREUR SAISIE>\n");
          printf("Mode pas bien saisi\n");
          printf("Veuillez consulter le manuel avec l'option --help.\n\n");
          exit(ERREUR_SAISIE);
        }
      }
      if (!(strcmp(argv[boucle], "-t")))
      {
        //On retient ici où est saisie la méthode demandée
        indiceMode = boucle+1;
        verif = 1;
        tri = 0;
        
        mode = atoi(argv[indiceMode]);
        printf("%d\n", mode);
        if (mode < 1 || mode > 3)
        {
          printf("\n<ERREUR SAISIE>\n");
          printf("Mode pas bien saisi\n");
          printf("Veuillez consulter le manuel avec l'option --help.\n\n");
          exit(ERREUR_SAISIE);
        }
      }
      if (!(strcmp(argv[boucle], "-w")))
      {
        verif = 1;
        tri = 2;
      }
      if (!(strcmp(argv[boucle], "-h")))
      {
        verif = 1;
        tri = 3;
      }
      if (!(strcmp(argv[boucle], "-m")))
      {
        verif = 1;
        tri = 4;
      }
      if (!(strcmp(argv[boucle], "-i")))
      {
        nomF = strdup(argv[boucle+1]);
      }
      if (!(strcmp(argv[boucle], "-o")))
      {
        nomS = strdup(argv[boucle+1]);
      }
      boucle++;  
    }

    if (boucle < argc)
    {
      while (boucle < argc)
      {
        if (!(strcmp(argv[boucle], "-t")) || !(strcmp(argv[boucle], "-p")) || !(strcmp(argv[boucle], "-h")) || !(strcmp(argv[boucle], "-w")) || !(strcmp(argv[boucle], "-m")))
        {
          printf("\n<ERREUR SAISIE>\n");
          printf("Trop d'arguments incompatibles\n");
          printf("Veuillez consulter le manuel avec l'option --help.\n\n");
          exit(ERREUR_SAISIE);
        }
        if (!(strcmp(argv[boucle], "-i")))
        {
          nomF = strdup(argv[boucle+1]);
        }
        if (!(strcmp(argv[boucle], "-o")))
        {
          nomS = strdup(argv[boucle+1]);
        }
      boucle++;
      }
    }

    boucle = 0;
    while (boucle < argc-2)
    {
      if (!(strcmp(argv[boucle], "-g")))
      {
        Gmin = atof(argv[boucle+1]);
        Gmax = atof(argv[boucle+2]);
        if (Gmin > Gmax)
        {
          printf("\n<ERREUR SAISIE>\n");
          printf("Veuillez consulter le manuel avec l'option --help.\n\n");
          exit(ERREUR_SAISIE);
        }
      }
      if (!(strcmp(argv[boucle], "-a")))
      {
        Amin = atof(argv[boucle+1]);
        Amax = atof(argv[boucle+2]);
        if (Amin > Amax)
        {
          printf("\n<ERREUR SAISIE>\n");
          printf("Veuillez consulter le manuel avec l'option --help.\n\n");
          exit(ERREUR_SAISIE);
        }
      }
      if (!(strcmp(argv[boucle], "-d")))
      {
        strptime(argv[boucle+1], "%Y-%m-%d", &t);
        Dmin = mktime(&t);
        strptime(argv[boucle+2], "%Y-%m-%d", &t);
        Dmax = mktime(&t);

        /*car c'est cassé en time_t*/
        if (Dmin > Dmax)
        {
          printf("\n<ERREUR SAISIE>\n");
          printf("Veuillez consulter le manuel avec l'option --help.\n\n");
          exit(ERREUR_SAISIE);
        }
      }
      boucle++;
    }
    
  }
  else {
      printf("\n<ERREUR SAISIE>\n");
      printf("Veuillez consulter le manuel avec l'option --help.\n\n");
      exit(ERREUR_SAISIE);
  }

  fichier = fopen(nomF, "r");   //ouvre le document "test.csv" en mode lecture
  
  /*on skip la 1ère ligne*/
  while((caractereActuel!=EOF)&&(caractereActuel!='\n')){
    caractereActuel=fgetc(fichier);
  }

  while(caractereActuel!=EOF){
    if(caractereActuel!='\n'){ 
    
    /*on récupère les infos de la lignes*/
    tmp = recup_data_ligne(fichier, &caractereActuel);
    
      if(tmp.lattitude >= Amin && tmp.lattitude <= Amax && tmp.longitude <= Gmax && tmp.longitude >= Gmin && tmp.date >= Dmin && tmp.date <= Dmax){ 
      
        switch (tri)
        {
        /*-p et -t utilise les même fonctions */
        case 0: // -t
        case 1: // -p
          switch (mode)
          {
          case 1:
            a = inserNoeudSPT(a, tmp, tri);
            break;
          
          case 2:
            a = inserNoeudDPT(a, tmp, tri);
            break;
          
          case 3:
            a = inserNoeudDPS(a, tmp, tri);
            break;
  
          default:
            fprintf(stderr, "mauvais mode");
            break;
          }
          break;
  
        /*-w*/
        case 2:
          a = inserNoeudSPT(a, tmp, tri);
          break;
        
        case 3: // -h
          a = inserNoeudAL(a, tmp);
          break;
        case 4: // -m
          a = inserNoeudHU(a, tmp, tri - 3);
          break;
  
        default:
          break;
        }
      }
    }
    else{
      caractereActuel=fgetc(fichier);
    }
  }
  fclose(fichier);

  /*mettre argument du -o | a | type de tri (même que si dessus) | mode pour -t -p sinon random(1 / 2 / 3)*/
  Ecrire_fichier(nomS, a, tri , mode);
}
