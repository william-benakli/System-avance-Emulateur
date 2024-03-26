1. Définition rapide de l'émulation
* Faire fonctionner un système sur un autre système
* Popularité avec la montée en puissance des CPU
* Définition de virtualisation (Super Gameboy)
* Définition de simulation (Flight Simulator)

2. Introduction Chip8 et pourquoi on a fait ça (1min)
* CHIP-8 : langage de programmation pour micro-ordinateurs
* Fibonnaci du monde de l'émulation
* Langage documenté, simple, 35 instructions
* Très testables avec pleins de ROMs, notamment de test
* Premier défi : IBM pour tester l'affichage graphique

* **Démo de Slippery Slope**
* Comment l'émulateur fonctionne :
  * chargement du jeu dans la mémoire vive du système
  * placement du cursuer au début de là où la ROM a été chargée
  * début de la boucle d'exécution
* Explication de la boucle d'exécution
  * fetch   : récupère depuis la RAM les 16 prochains bits à lire 
  * decode  : sépare ces 16 bits en valeurs utilisables/pertinentes, travail de parsing
  * execute : selon la commande lue, fait ci, va là
* Démonstration de la version désassemblée de Slippery Slope
  * offset sur la mémoire | contenu fetché à être décodé | traduction en instruction
* Montrer l'image du décodage (1 caractère = 4 bits) avec le code à côté
  * Selon la valeur fetchée, le parsing peut différer
  * démonstration de l'implémentation de la première commande, JUMP

* Volonté de déployer sur une plateforme notre émulateur
* Implémentation limitée, front-end C via SDL
* Solution : Retroarch, plateforme spécialisée pour.
* Pour comprendre ce que c'est, définissons d'abord libretro
* libretro
  * interface pour connecter notre émulateur à la plateforme RetroArch
  * l'implémenter nous donne un core. construction de pont, on communique dans un langage qu'il comprend
  * Concrètement, fichier header en C qui nous demande d'implémenter des fonctions pour des opérations variées d'un émulateur
    * charger un jeu, gérer le branchement d'un input, exécuter une frame
  * Résultat : compilation de cette implémentation vers une librairie dynamique (.so), appelé dans le jargon "core"
* RetroArch
  * Lecteur de core
  * Utiliser généralement en tant que bibliothèque pour les émulateurs
  * User-friendly, open-souce, multiplatforme, plein d'abstractions pour les dévs, etc.
* démonstration de la fonction retro_run()

* **Démo de Pong à deux joueurs**
  * démonstration des fonctionnalités de save state et de retour en arrière 

* **Démonstration de RetroArch sur Android**
