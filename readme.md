Bonchouran c'est ici la dok

# ling_project
## 1. Introduction

le *project ling* est, comme son nom l'indique, le futur de l'équipement tactique connecté. Il est composé de plusieurs projets, chacun ayant une fonctionnalité bien précise, et un nom encore plus poussé que ses fonctionnalités.

Ces projets sont les suivants:
- **ling**: (concurrent direct de [Gate](https://www.gatee.eu/titan), dont l'[ETU Titan](https://www.gatee.eu/titan) me fascine) Ce module représente le coeur de toute réplique dans lequel il serait équipé. C'est un ETU (Electronic Trigger Unit) qui offre un contrôle sans précédent des modes de tirs, de la cadence, ainsi que moultes autres paramètres (certains prêts, d'autres encore à venir c'est quand même un **$énorme$** buget).
Il est doté d'une interface personnalisable avec autant de modules que de billes dans le chargeur (non, il y en a 4 pour l'instant mais ca viendra).
Les modules déjà présent sont un **affichage du mode de tir sélectionné**, un **compteur de munitions**, un **gyroscope** ainsi qu'un mode **viseur holographique**.

- **bbuddy**: (raccourci de *battle buddy*) C'est le module servant de traceur, permettant de suivre les frags et la position du joueur l'ayant équipé. Il fournit aussi une interface visuelle, pouvant afficher diverses infos comme le temps restant avant la fin de la partie, le nombre de frags restants, etc...
Il aurait besoin d'une page à lui tout seul, si on prend en compte le Notion qui lui est dédié. (oui ca viendra)

- **brotherb**: (inspiré de Big Brother) Un des projets le plus théorique du lot. Il s'agit d'un module fixé sur le casque, plastron ou canon transmettant un flux vidéo constant du joeur l'ayant équipé. Il permettrait de suivre le joueur en temps réel, et de le localiser sur une carte. (oui ca viendra aussi). En communiquant en temps réel avec les projets **ling** et **bbuddy**, les informations concernant la réplique (munitions, mode de tir, etc) et le joueur (position, frags, etc) pourraient être affichées sur l'écran du joueur, et sur l'écran du traceur. Ce projet cache 2 objectifs distincts. L'un est de transmettre ces informations tel un HUD de Call of Duty sur des lunettes connectées directement sur le casque.
L'autre est plus loin dans la doc

- **battlemommy**: (l'équivalent du vaisseau mère du *project ling*) A l'origine, c'est le module fournissant une connexion centralisé de tous les projets, faisant office d'un point accès afin de faciliter les communications. Ce serait théoriquement le seul module connecté à Internet, pour permettre un contrôle de flux avancé si le besoin s'en fait sentir. 
En récupérant les flux vidéos de **brotherb**, et en y ajoutant une couche avec les informations de **bbuddy** et **ling**, il serait possible de créer une interface de contrôle de la partie, permettant de suivre les joueurs, de les localiser et de les observer, en retransmettant en direct les parties sur des plateformes de streaming. De plus, il serait possible de contrôler les modes de tir, les munitions, etc... de chaque joueur, afin de créer des parties plus équilibrées, ou de faire des parties plus funs.
Une touche d'algo serait potentiellement ajoutable pour caster dynamiquement les joueurs ayant le plus de frags, ou les joueurs ayant le plus de morts, ou quand 2 joueurs sont susceptibles de se rencontrer, etc...
En analysant les données de tous les modules, il serait possible de déterminer des comportements, des tendances, des statistiques, etc.. 

## 2. Frameworks utilisés

- **oled.ino**: C'est le coeur de l'affichage, il est utilisé par la grande majorité modules, avec quelques modifications en fonction des besoins, et permet d'afficher des informations sur l'écran OLED.
Il possède différentes méthodes d'affichages, dont les 3 principaux; *modules*, *nav*, et *viseur holographique*. Le code est plus ou moins différent en fonction des projets, mais le principe reste le même. Un axe d'amélioration serait de créer une classe *Oled* qui serait utilisée par tous les modules, et qui permettrait de centraliser les méthodes d'affichage, et de les rendre plus facilement modifiables.


## 3. Ressources externes
- **Tacti Connect**: (*Notion*) La version regroupant l'origine du projet, dont les premiers outils de QOL Airsoft (A²B pour regrouper les dépenses, BBGear pour l'équipement, etc..). Fait aussi office de suivi des différents projets mais la timeline n'est plus vraiment à jour.
[C'est ici !](https://pyctus.notion.site/pyctus/Tacti-Connect-b880c0bb861442b192c1bbdf104d08c2) 

## x. Annexe

### x.0 Les projets
[Les démos, c'est ici !](https://www.notion.so/pyctus/D-mos-35428c046b044c6d8d4b4104f5a96d3e?pvs=4)
### x.1 Les modules
### x.2 Les méthodes d'affichage
### x.3 Les méthodes de communication
### x.4 Les méthodes de contrôle
### x.5 Les méthodes de localisation
### x.6 Les méthodes de détection