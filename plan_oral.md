1. Définition rapide de l'émulation (1min)
* Faire fonctionner un système sur un autre système, popularité avec la montée en puissance des CPU blabla
* Différence avec virtualisation et simulation (exemple : émulateur GameBoy avec virtualisation Super Gameboy)

Introduction Chip8 et pourquoi on a fait ça (1min)
* Projet simple à implémenter, le fibonnaci dans le monde de l'émulation, 
* résultat rapide avec plein de roms et de jeux, "unit test", documenté (montré la rom de test ibm) 
* 

Démo chip8 avec explications et démo du code(2minutes)
* (slippery slope) ptite démo boum (2 niveaux)
* comment l'émulateur fonctionne : chargement du jeu (la rom) dans la mémoire vive de la ram du système, placement du cursuer au début de la rom chargée, puis début de la boucle d'exécution
* expliquer la boucle principale 
  * fetch: récupère depuis la ram le 16 bit à lire avec le program counter
  * decode: sépare les valeurs en valeurs utilisable/pertinentes
  * execute: selon la commande lue, fait ci, va là
* démonstration de la version désassemblée
  * offset sur la mémoire | contenu fetché | traduction en instruction
* montrer l'implémentation de la première commande (jump) 

Introduction Retroarch et les plateformes d'émulation en général + code de retroarch
(surement la gestion du controller, pour pouvoir montrer l'intéret)(2min)
* libretro : interface pour connecter notre émulateur à la plateforme RetroArch, l'implémenter nous donne un core
* retroarch : basiquement lecteur de core. en général, utilisé en tant que bibliothèque pour les émulateurs. user-friendly, open souce, multiplatforme, plein d'abstractions pour les dévs, etc.
* montrer le code de la manette, expliquer la fonction retro_run qui facilite les choses. 

Démonstration Retroarch en multi (1min, 1min30)
* démo à deux de pong 
* save state en live (plein de feature d'émulation) ou retour en arrière

Montrer Retroarch sur Tel(30sec)
* démo sur tél
Ouverture sur l'avenir de l'émulation (le temps qu'il reste)

En ajoutant le temps de set up des différents trucs on sera à 10 minutes.
