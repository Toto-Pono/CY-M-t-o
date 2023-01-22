# README
## Project : CPI2-CY-METEO 
### Table des matières
    1. [Comprendre le sujet](#comprendre)
    2. [Compiler](#compiler)
       1. [Lancer l'exécutable](#exe)
       2. [Supprimer les fichiers en .exe et .o](#suppr)

# Etape 1: Bien comprendre le sujet
Objectif : Trier les données du csv de manière la plus optimisé et rapide d'accès possible

- Utilisation d'un ABR: recherche, insertion et suppression en O(log(n))
- Il faut etre capable de créer un ABR selon différente variable
- Pour créer un ABR il faut etre capable de comparer 2 noeuds selon la dite variable
- Sortir les données triés pour une utilisation dans un script shell

./exe [OPTION]

#2. Compiler
***
Pour compiler :
```sh
make
```
Cela créera aussi l'éxecutable.
## Lancer le programme
***
Pour lancer le programme :
```sh
bash script.sh [COMMANDE] [OPTION1]...
```

### Commande pris par le shell
```sh	
	-F
```
		traite uniquement les données de France métropolitaine et Corse.
```sh	
	-G
```
		traite uniquement les données de Guyane.
```sh	
	-A
```
		traite uniquement les données des Antilles.
```sh	
	-S
```
		traite uniquement les données de St Pierre et Miquelon.
```sh	
	-O
```
		traite uniquement les données de l’Océan indien.
```sh	
	-Q
```
		traite uniquement les données de l’Antarctique

### Commande pris par le C
Options obligatoire:

```sh	
	-i "fichier"
```
		"fichier" correspond au fichier d'entrée en csv.
```sh	
	-o "fichier"
```
		Permet de choisir le nom du fichier ou le préfixe du nom dans le cas où plusieurs fichiers sont attendu en sortie.

Options obligatoires exclusives, une seul autorisé par exécution:

```sh	
	-t <mode>
```
traite la température, mode est compris entre 1 et 3 comme expliqué ci-dessous.
```sh	
	-p <mode>
```
 traite la pression atmosphériques, mode est compris entre 1 et 3 comme expliqué ci-dessous.
	
Pour les modes voici les possibilités:
  mode=1:
		renvoie les températures ou pressions minimals, maximals et moyennes par stations, triées par numéro de station croissant.
	mode=2:
		renvoie les températures ou pressions moyennes par date, triées dans l’ordre chronologique sur l’ensemble des stations.
	mode=3:
		renvoie les températures ou pression par station, triées par dates puis par ordre croissant de numéro de station.
	
```sh	
	-w
```
 traite le vent en renvoyant l’orientation ainsi que la vitesse moyenne du vent.
```sh	
	-h
```
		renvoie la hauteur de chaque stations triées par ordre décroissant.
```sh	
	-m
```
		renvoie l’humidité maximal pour chaque stations triées par ordre décroissant.
```sh	
	--help
```
		affiche l’aide

Options facultatives:
	
```sh	
	-g <min> <max>
```
		permet de garder uniquement les stations comprises entre la longitude min et max, min et max sont des entiers.	
```sh	
	-a <min> <max>
```
		permet de garder uniquement les stations comprises entre la latitude min et max, min et max sont des entiers.	
```sh	
	-d <min> <max>
```
		permet de garder uniquement les données comprises entre la date min et max, min et max sont au format  YYYY-MM-DD.

## Supprimer les .o et l'exécutable
***
Pour supprimer les .o et l'exécutable
```sh
make clean
```

### Crédits
***
Auteur : PHILIPPONNEAU Thomas <philipponn@cy-tech.fr>
