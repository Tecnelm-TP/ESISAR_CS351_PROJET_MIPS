# Projet MIPS ESISAR 2020

Ce dépot est destiné au projet de 3A-Esisar "Émulation d'un processeur MIPS"



### Prérequis

* GNU GCC for C99 or above
* GNU Make
* Bash

## Information

L'émulateur est conçu et testé sur linux. Nous ne garantissons pas son fonctionnement sur une autre platforme.
Pour compiler le programme, utilisez la commande make.

il y a deux modes d'exécutions:
* mode interactif  : Les instructions sont données au fur et à mesure par l'utilisateur. On utilise la commande : emul-mips; 
* mode non-interactif: Les instruction sont données dans un fichier au format assembleur. On utilise la commande : emul-mips -i chemin-fichier. 

Pour lancer le programme il est nécessaire de spécifier quelque paramètre:
* -o le fichier de sortie du parseur 
* -i le fichier d'entrée
* --hex pour spécifier que l'entrée est en déjà en hexadécimal 
* -h/--help pour afficher toutes les commandes 
* -p/--pas pour utiliser le mode pas à pas 

Lorsque le mode non-interactif est utilisé, il est possible de renseigner l'argument "--pas"' qui permet d'exécuter l'émulateur en mode pas à pas.


## Auteurs

* **Garrigues Clément** 
* **Moyart Alexis - [@Electroalex](https://gitlab.org/Electroalex26)**

## Licence

Licence aux fillots récursif


