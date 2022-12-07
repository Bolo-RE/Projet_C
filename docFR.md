# Documentation Projet C

## Principe
Le but de l'application est simple gerer un reseau electrique. Pour ce faire nous avons 3 types de structures a gerer : les centrales, les villes et les lignes qui permettent de relier ces entites

Le logiciel permet donc de creer et de supprimer les differents composants du reseau. En plus de cela, il possible de sauvegarder et de charger des reseaux. Enfin, l'energie recues par les villes et generee par les centrales est egalement disponible.  

## Utilisation
En lancant l'application avec l'option `-h`, on accede a l'aide integree dans le logiciel. Avec un nom de fichier, il est possible de charger son contenu en tant que reseau.

L'application se decompose en deux parties : la vue principale toujours visible et les fonctions de gestion invisible pour l'utilisateur

Ces fonctions sont appellees en pressant des touches dans la fenetre principale qui vont transmettre les informations requises pour chacune de ces fonctions a partir des entrees utilisateur.

Bien que toutes les fonctions soient reliees a une touche, la naviguation est simplifiee par l'utilisation de la toolbar. 

Avec les touches 'a' et 'b', il est possible de faire defiler l'ensemble des fonctions disponibles dans le logiciel. Pour activer l'une de ces fonctions, il suffit d'utiliser les touches de 1-9. 
Lorsque l'utilisateur demande a voir des donnees hors du reseau electrique, qui est toujours affiche, elles sont affichees a droite dans un cadre reserve. Par exemple, il possible d'utiliser les touches 
'o' et 'p' pour afficher les informations liees aux centrales et aux villes.

## Mapping des touches
La configuration par defaut des touches est la suivante :

`a` -> augmente de 1 vers la droite la position de la toolbar 

`b` -> augmente de 1 vers la gauche la position de la toolbar 

`c` -> affiche la puissance recue par une ville

`d` -> affiche la puissance generee par une centrale

`f` -> change la puissance contenue dans les lignes d`une centrale donnee

`g` -> cree une ville

`h` -> cree une centrale

`i` -> cree une ligne

`j` -> supprimer une ville

`k` -> supprimer une centrale

`l` -> supprimer une ligne

`m` -> affiche l`aide ; egalment visible en lancant le programe avec l`option `-h`

`n` -> permet de cacher la toolbar

`o` -> affiche les informations liees aux villes : ID et nom

`p` -> affiche les informations liees aux centrales et leurs lignes : ID, ID de la ligne, ID de la ville liee et la puissance contenue dans la ligne

`q` -> sauvegarder le reseau

`r` -> charger un fichier dans le reseau

`t` -> fait defiler le contenu du panneau de droite vers le bas

`u` -> fait defiler le conteu du panneau de droite vers le haut