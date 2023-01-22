debut=$(date +%s)
type=$1
fichierE="NULL"
fichierS="meteoxxxx"
option=""

#Gnuplot
creerGraph(){
    for (( i=2; i<=$#; i++ ))
    do
        #Cas de l'option -t
        if [[ ${!i} = "-t" ]];
        then

            i=$((i+1))
            #Mode 1
            if [[ ${!i} = 1 ]];
            then
                gnuplot -e "reset;
                set terminal png;
                set autoscale fix;
                set output \"Tmode1.png\";
                set datafile separator \";\";
                set xlabel \"ID Station\";
                set ylabel \"Température\";
                set grid;
                set title \"Températures minimales, maximales et moyennes par station\";
                plot '$1' u 1:2:3 w yerror title \"Température\", ;"
            
            #Mode 2
            elif [[ ${!i} = 2 ]];
            then
                gnuplot -e "reset;
                set terminal png;
                set autoscale fix;
                set output \"Tmode2.png\";
                set datafile separator \";\";
                set xlabel \"Date\";
                set ylabel \"Température\";
                set xdata time;
                set timefmt \"%d/%m/%Y %H:%M:%S\";
                set xtics format \"%d/%m/%Y \t%H:%M:%S\";
                set term pngcairo size 1920, 1080;
                set title \"Température moyenne par date/heure\";
                plot '$1' u 1:2 w l title \"Température\";"

            #Mode 3
            elif [[ ${!i} = 3 ]];
            then
                gnuplot -e "reset;
                set terminal png;
                set autoscale fix;
                set output \"Tmode3.png\";
                set datafile separator \";\";
                set xlabel \"Date\";
                set ylabel \"Température\";
                set xdata time;
                set timefmt \"%d/%m/%Y %H:%M:%S\";
                set xtics format \"%d/%m/%Y \t%H:%M:%S\";
	            set term pngcairo size 3840, 2160;
                set title \"Température moyenne par date/heure par station\";
                plot '$1' u 0:3 w l;"
            fi

        #Cas de l'option -p
        elif [[ ${!i} = "-p" ]];
        then

            i=$((i+1))
            #Mode 1
            if [[ ${!i} = 1 ]];
            then
                gnuplot -e "reset;
                set terminal png;
                set autoscale fix;
                set output \"Pmode1.png\";
                set datafile separator \";\";
                set xlabel \"ID Station\";
                set ylabel \"Pression\";
                set grid;
                set title \"Pressions minimales, maximales et moyennes par station\";
                plot '$1' u 1:2:3 w yerror title \"Pression\", ;"
            
            #Mode 2
            elif [[ ${!i} = 2 ]];
            then
                gnuplot -e "reset;
                set terminal png;
                set autoscale fix;
                set output \"Pmode2.png\";
                set datafile separator \";\";
                set xlabel \"Date\";
                set ylabel \"Pression\";
                set xdata time;
                set timefmt \"%d/%m/%Y %H:%M:%S\";
                set xtics format \"%d/%m/%Y \t%H:%M:%S\";
                set term pngcairo size 1920, 1080;
                set title \"Pression moyenne par date/heure\";
                plot '$1' u 1:2 w l title \"Pression\";"

            #Mode 3
            elif [[ ${!i} = 3 ]];
            then
                gnuplot -e "reset;
                set terminal png;
                set autoscale fix;
                set output \"Pmode3.png\";
                set datafile separator \";\";
                set xlabel \"Date\";
                set ylabel \"Pression\";
                set xdata time;
                set timefmt \"%d/%m/%Y %H:%M:%S\";
                set xtics format \"%d/%m/%Y \t%H:%M:%S\";
	            set term pngcairo size 3840, 2160;
                set title \"Pression moyenne par date/heure par station\";
                plot '$1' u 0:3 w l;"
            fi

        #Cas de l'option -w
        elif [[ ${!i} = "-w" ]];
        then
            gnuplot -e "reset;
            set terminal png;
            set autoscale fix;
            set output \"Vent.png\";
            set datafile separator \";\";
            set term pngcairo size 3840, 2160;
            set title \"Orientation/vitesse moyenne des vents par station\";
            plot '$1' with vectors filles head lw 3;"
        
        #Cas de l'option -h
        elif [[ ${!i} = "-h" ]];
        then
            gnuplot -e "reset;
            set terminal png;
            set autoscale fix;
            set output \"Hauteur.png\";
            set datafile separator \";\";
            set term pngcairo size 3840, 2160;
            set title \"Hauteur par station\";
            plot '$1' u 1:2 w p lw 6 palette;"

        #Cas de l'option -m
        elif [[ ${!i} = "-m" ]];
        then
            gnuplot -e "reset;
            set terminal png;
            set autoscale fix;
            set output \"Humidité.png\";
            set datafile separator \";\";
            set term pngcairo size 3840, 2160;
            set title \"Humidité par station\";
            plot '$1' u 1:2 w p lw 6 palette;"
        fi
    done
}

#Vérification de l'existence de l'exécutable
if ! [[ -x exe ]];
then
    `make`
fi
#Recherce des arguments -i, -o et -d (pour -d s'il est précisé)
for (( i=2; i<=$#; i++ ))
do
    #Cas de l'argument -i
    if [[ ${!i} = "-i" ]] && [[ $i -ne $# ]];
    then
        i=$((i+1))
        fichierE=${!i}
    
    #Cas de l'argument -d, on doit vérifier que ce soit le bon format
    elif [[ ${!i} = "-d" ]] && [[ $i -ne $(($#-2)) ]];
    then
        #Vérification du format
        for (( j=$((i+1)); j<=$((i+2)); j++ ))
        do
            val=`echo $j | cut -f1 -d"-"`
            if [[ 10#$val -gt 0 && 10#$val -le 2023 ]];
            then
                val=`echo $j | cut -f2 -d"-"`
                if [[ 10#$val -ge 1 && 10#$val -le 12 ]];
                then
                    val=`echo $j | cut -f3 -d"-"`
                    if [[ 10#$val -ge 1 && 10#$val -le 31 ]];
                    then
                        verif=1
                    else
                        verif=0
                    fi
                else
                    verif=0
                fi
            else
                verif=0
            fi
        done

        #Cas où le format n'est pas valide
        if  [[ $verif = 0 ]];
        then
            echo "<ERREUR SAISIE>"
            echo "La date minimale/maximale saisie ne correspond pas au format demandé"
            echo "Veuillez consulter le manuel avec --help"
        #Cas où le format est valide
        else
            #Cas où la première date est plus petite que l'autre
            if [[ $((i+1)) -le $((i+2)) ]];
            then
                Dmin=$((i+1))
                Dmax=$((i+2))
            #Cas inverse
            else
                Dmin=$((i+2))
                Dmax=$((i+1))
            fi
        fi
        i=$((i+2))
    
    #Cas de l'argument -o
    elif [[ ${!i} = "-o" ]] && [[ $i -ne $# ]];
    then
        i=$((i+1))
        fichierS=${!i}
    
    #Tous les autres arguments en paramètres sont stockés pour les donner au .c
    else
        option+=" ${!i}"
    fi
done 

#Help
if [[ $type = "--help" ]] && [[ $# = 1 ]]; then
   echo ""
   echo "<BASH>"
   echo "bash script.sh [commande] [option1] [option2]..."
   echo "/!\\ Les commandes sont exclusives"
   echo "commande disponibles:  -F              permet de limiter les mesures à celles présentes en France métropolitaine + Corse."
   echo "                       -G              permet de limiter les mesures à celles qui sont présentes en Guyane."
   echo "                       -S              permet de limiter les mesures à celles qui sont présentes sur l’ile située à l’Est du Canada."
   echo "                       -A              permet de limiter les mesures à celles qui sont présentes aux Antilles."
   echo "                       -O              permet de limiter les mesures à celles qui sont présentes dans l’océan indien."
   echo "                       -Q              permet de limiter les mesures à celles qui sont présentes en antarctique."
   echo "option disponible:     -d <min> <max>  permet de filtrer les dates entre les valeurs moin et max, tout comme le ferait le programme C. (Format YYYY-MM-DD)"
   echo ""
   echo "`./exe --help`"

#France
elif [[ $type = "-F" ]]; then
    
    #Vérification de l'existence du .csv
    if [[ -e France.csv ]];
    then
        `rm France.csv`
        `touch France.csv`
    else
        `touch France.csv`
    fi
    echo "En cours de chargement..."
    `head -n 1 $fichierE | tail -n 1 >> France.csv`
    longueur=$(cat $fichierE | wc -l);

    #Vérification du code postal de chaque donnée
    for (( i=2; i<=$longueur; i++ ))
    do
        code=`cut -f15 -d";" $fichierE | head -n $i | tail -n 1`
        corse=$(echo "${code}" | head -c2)
        #Cas où le code postal correspond à la corse
        if [[ $corse = "2a" || $corse = "2b" ]]; 
        then
            `head -n $i $fichierE | tail -n 1 >> France.csv`
        #Ici le elif permet surtout de ne pas avoir d'erreur en comparant les codes commençant par 2a ou 2b
        elif [[ -n $code && 10#$code -ge 01000 && 10#$code -lt 96000 ]];
        then
            `head -n $i $fichierE | tail -n 1 >> France.csv`
        fi
    done
    echo "Fin de la création de France.csv"

    #Appel du c avec les autres arguments et le nom du fichier en sortie
    echo "`./exe -i France.csv -o $fichierS $option`"
    echo "$(tail -n +2 $fichierS)" > $fichierS
    longueur=$(cat $fichierS | wc -l);

    #Création des graphiques par Gnuplot
    creerGraph $fichierS $option

#Guyane
elif [[ $type = "-G" ]]; then

    #Vérification de l'existence du .csv
    if [[ -e Guyane.csv ]];
    then
        `rm Guyane.csv`
        `touch Guyane.csv`
    else
        `touch Guyane.csv`
    fi
    echo "En cours de chargement..."
    `head -n 1 $fichierE | tail -n 1 >> Guyane.csv`
    longueur=$(cat $fichierE | wc -l);

    #Vérification du code postal de chaque donnée
    for (( i=2; i<=$longueur; i++ ))
    do
        code=`cut -f15 -d";" $fichierE | head -n $i | tail -n 1`
        corse=$(echo "${code}" | head -c2)
        if ! [[ $corse = "2a" || $corse = "2b" ]];
        then
            if [[ -n $code && 10#$code -ge 97300 && 10#$code -lt 97400 ]]; 
            then
                `head -n $i $fichierE | tail -n 1 >> Guyane.csv`
            fi
        fi
    done  
    echo "Fin de la création de Guyane.csv"

    #Appel du c avec les autres arguments et le nom du fichier en sortie
    echo "`./exe -i Guyane.csv -o $fichierS $option`"
    echo "$(tail -n +2 $fichierS)" > $fichierS
    longueur=$(cat $fichierS | wc -l);

    #Création des graphiques par Gnuplot
    creerGraph $fichierS $option

#Saint Pierre et Miquelon
elif [[ $type = "-S" ]]; then

    #Vérification de l'existence du .csv
    if [[ -e SaintPierreEtMiquelon.csv ]];
    then
        `rm SaintPierreEtMiquelon.csv`
        `touch SaintPierreEtMiquelon.csv`
    else
        `touch SaintPierreEtMiquelon.csv`
    fi
    echo "En cours de chargement..."
    longueur=$(cat $fichierE | wc -l);

    #Vérification du code postal de chaque donnée
    for (( i=2; i<=$longueur; i++ ))
    do
        code=`cut -f15 -d";" $fichierE | head -n $i | tail -n 1`
        corse=$(echo "${code}" | head -c2)
        if ! [[ $corse = "2a" || $corse = "2b" ]];
        then
            if [[ -n $code && 10#$code -eq 97500 ]]; 
            then
                `head -n $i $fichierE | tail -n 1 >> SaintPierreEtMiquelon.csv`
            fi
        fi
    done
    echo "Fin de la création de SaintPierreEtMiquelon.csv"

    #Appel du c avec les autres arguments et le nom du fichier en sortie
    echo "`./exe -i SaintPierreEtMiquelon.csv -o $fichierS $option`"
    echo "$(tail -n +2 $fichierS)" > $fichierS
    longueur=$(cat $fichierS | wc -l);

    #Création des graphiques par Gnuplot
    creerGraph $fichierS $option

#Antilles
elif [[ $type = "-A" ]]; then

    #Vérification de l'existence du .csv
    if [[ -e Antilles.csv ]];
    then
        `rm Antilles.csv`
        `touch Antilles.csv`
    else
        `touch Antilles.csv`
    fi
    echo "En cours de chargement..."
    longueur=$(cat $fichierE | wc -l);

    #Vérification du code postal de chaque donnée
    for (( i=2; i<=$longueur; i++ ))
    do
        code=`cut -f15 -d";" $fichierE | head -n $i | tail -n 1`
        corse=$(echo "${code}" | head -c2)
        if ! [[ $corse = "2a" || $corse = "2b" ]];
        then
            if [[ -n $code && 10#$code -ge 97100 && 10#$code -lt 97300 ]]; 
            then
                `head -n $i $fichierE | tail -n 1 >> Antilles.csv`
            fi
        fi
    done
    echo "Fin de la création du fichier temporaire Antilles.csv"

    #Appel du c avec les autres arguments et le nom du fichier en sortie
    echo "`./exe -i Antilles.csv -o $fichierS $option`"
    echo "$(tail -n +2 $fichierS)" > $fichierS
    longueur=$(cat $fichierS | wc -l);

    #Création des graphiques par Gnuplot
    creerGraph $fichierS $option

#Ocean Indien
elif [[ $type = "-O" ]]; then

    #Vérification de l'existence du .csv
    if [[ -e OceanIndien.csv ]];
    then
        `rm OceanIndien.csv`
        `touch OceanIndien.csv`
    else
        `touch OceanIndien.csv`
    fi
    echo "En cours de chargement..."
    longueur=$(cat $fichierE | wc -l);

    #Vérification du code postal de chaque donnée
    for (( i=2; i<=$longueur; i++ ))
    do
        code=`cut -f15 -d";" $fichierE | head -n $i | tail -n 1`
        corse=$(echo "${code}" | head -c2)
        if ! [[ $corse = "2a" || $corse = "2b" ]];
        then
            if [[ -n $code && 10#$code -ge 97600 && 10#$code -lt 97700 ]]; 
            then
                `head -n $i $fichierE | tail -n 1 >> OceanIndien.csv`
            fi
        fi
    done
    echo "Fin de la création du fichier temporaire OceanIndien.csv"

    #Appel du c avec les autres arguments et le nom du fichier en sortie
    echo "`./exe -i OceanIndien.csv -o $fichierS $option`"
    echo "$(tail -n +2 $fichierS)" > $fichierS
    longueur=$(cat $fichierS | wc -l);

    #Création des graphiques par Gnuplot
    creerGraph $fichierS $option

#Antarctique
elif [[ $type = "-Q" ]]; then

    #Vérification de l'existence du .csv
    if [[ -e Antarctique.csv ]];
    then
        `rm Antarctique.csv`
        `touch Antarctique.csv`
    else
        `touch Antarctique.csv`
    fi
    echo "En cours de chargement..."
    longueur=$(cat $fichierE | wc -l);

    #Vérification du code postal de chaque donnée
    for (( i=2; i<=$longueur; i++ ))
    do
        latitude=`cut -f10 -d";" $fichierE | head -n $i | tail -n 1 | cut -f1 -d"," | cut -f1 -d"."`
        if [[ $latitude -le -60 ]];
        then
           `head -n $i $fichierE | tail -n 1 >> Antarctique.csv`
        fi
    done   
    echo "Fin de la création du fichier temporaire Antarctique.csv"

    #Appel du c avec les autres arguments et le nom du fichier en sortie
    echo "`./exe -i Antarctique.csv -o $fichierS $option`"
    echo "$(tail -n +2 $fichierS)" > $fichierS
    longueur=$(cat $fichierS | wc -l);

    #Création des graphiques par Gnuplot
    creerGraph $fichierS $option

#Mauvaise saisie   
else
    echo ""
    echo "<ERREUR SAISIE>"
    echo "Veuillez vérifier le manuel avec --help"
    echo ""
fi

#Affiche le temps d'exécution du script
fin=$(date +%s)
duree=$( echo "$fin - $debut" | bc -l )
echo "$duree"