ESISAR PROJET 3A - EMULATION D'UN PROCESSEUR MIPS
license fillot récursif. 

L'objectif de ce projet est l'émulation d'un processeur MIPS en langage C. 
L'émulateur a été dévelopé sous linux et pourra ne pas fonctionner en l'état sous windows.

Pour éxécuter l'émulateur 


# Projet MIPS ESISAR 2020

Ce dépot est destiné au projet de 3A-Esisar "Émulation d'un processeur MIPS"



### Prérequis

* GNU GCC for C99 or above
* GNU Make
* Bash

## Information

L'émulateur et conçus et testé sur linux, nous ne garantissons pas son fonctionnement sur une autre platforme.
Pour compiler le programme utiliser la commande make.

il y a deux modes d'éxécutions:
* mode interactif  : Les instructions sont données au fur et à mesure par l'utilisateur on utilise la commande emul-mips; 
* mode non-interactif: Les instruction sont données dans un fichier au format assembleur, emul-mips nom-fichier 

pour le mode non-interactif il est possible de renseigner l'argument -pas qui permet d'éxécuter l'émulateur en mode pas à pas


## Authors

* **Garrigues Clément** 
* **Moyart Alexis** 

## License

Licence au fillot récursif


