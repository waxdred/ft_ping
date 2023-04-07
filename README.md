# ft_ping 42
ft_ping est une implémentation du programme de commande ping sous Linux, permettant de tester la connectivité entre deux hôtes en envoyant des paquets ICMP ECHO_REQUEST et en écoutant les réponses.
## Table
- [Installation](#Installation)
- [Fonctionnement](#Fonctionnement)
- [Documentions](#Documentions)

# Installation
```fish
git clone https://github.com/votre-nom/ft_ping.git
cd ft_ping
```

Ensuite, exécutez make pour compiler le programme:

```fish
make
```

Ensuite, exécutez ft_ping en tant que super utilisateur (root) ou avec les droits de CAP_NET_RAW:
```fish
$ sudo ./ft_ping [-h] [-v] destination
```

## Les options sont:
- ```-h```: affiche l'aide et quitte.
- ```-v```: active le mode verbeux, qui affiche les informations supplémentaires.
- ```destination```: spécifie l'adresse IP ou le nom de domaine de la destination.

## Exemple d'utilisation:

```fish
sudo ./ft_ping -v google.com
```

# Fonctionnement

Le programme ping permet de tester la connectivité réseau en envoyant des paquets ICMP Echo Request et en recevant des paquets ICMP Echo Reply en réponse. Le programme envoie une série de paquets à une adresse IP spécifiée et affiche le temps de réponse et d'autres statistiques.

Pour implémenter cette fonctionnalité, le programme utilise plusieurs fonctions importantes, notamment :

- ```socket```: crée un socket pour envoyer et recevoir des données sur le réseau.
- ```setsockopt```: configure le socket avec différentes options, telles que le temps d'attente avant expiration d'une requête ou le TTL (Time To Live) du paquet.
- ```sendto```: envoie des paquets de données sur le réseau.
- ```recvfrom```: reçoit des paquets de données du réseau.
Ces fonctions sont utilisées pour envoyer des paquets ICMP Echo Request et recevoir des paquets ICMP Echo Reply en réponse. Le programme ping mesure ensuite le temps de réponse entre l'envoi et la réception du paquet, et affiche des statistiques telles que le nombre de paquets envoyés et reçus, le temps minimum, moyen et maximum de réponse, et le pourcentage de paquets perdus.

# Documentions

## ft_ping.c

Les fonctions membres de la structure t_ping comprennent :
- ```close()```: ferme la connexion.
- ```free()```: libère la mémoire allouée à la structure t_ping.
- ```header()```: initialise l'en-tête ICMP pour le ping.
- ```help()```: affiche l'aide pour l'utilisateur.
- ```connection()```: établit la connexion avec l'hôte.
- ```getname()```: résout le nom d'hôte en adresse IP.
- ```parse()```: analyse les arguments de ligne de commande pour la configuration du ping.
- ```receive()```: reçoit les paquets de réponse pour le ping.
- ```run()```: exécute le ping.
- ```send()```: envoie les paquets de ping.

La structure t_ping utilise également une autre structure t_stat qui contient les fonctions nécessaires pour effectuer les opérations statistiques sur les données recueillies lors du ping.

- ```static void handle_signal()```: Fonction de gestion du signal qui se déclenche lorsque le programme reçoit un signal SIGINT (par exemple, lorsque l'utilisateur appuie sur Ctrl-C). Elle met simplement la variable globale signalStop à zéro.

- ```help()```: Fonction qui prend en paramètre une chaîne de caractères et affiche un message d'aide sur l'utilisation de l'application ft_ping. Cette fonction est appelée lorsque l'utilisateur utilise l'option -h.

- ```static void freePing(t_ping *ping)```: Fonction qui libère la mémoire allouée pour la structure t_ping et son membre packet.

- ```static unsigned short calculate_checksum(void *addr, size_t count)```: Fonction qui calcule le checksum d'un ensemble de données. Elle est utilisée pour vérifier l'intégrité des paquets ICMP envoyés.

- ```static int parse(t_ping *ping, int ac, char **av)```: Fonction qui prend en paramètre la structure t_ping ainsi que les arguments de ligne de commande et qui analyse les options. Elle retourne EXIT_FAILURE si elle détecte une erreur, sinon elle retourne EXIT_SUCCESS.

- ```static int ft_send(t_ping *ping)```: Fonction qui envoie un paquet ICMP ECHO_REQUEST à l'adresse IP spécifiée dans la structure t_ping. Cette fonction utilise la fonction sendto() pour envoyer le paquet et incrémente le membre seq de la structure t_ping.

- ```static int ft_receive(t_ping *ping, struct timeval dev)```: Fonction qui attend une réponse à un paquet ICMP ECHO_REQUEST envoyé précédemment. Elle utilise la fonction recvfrom() pour recevoir la réponse et affiche les informations relatives à la réponse reçue (le temps de réponse, le nombre de bytes reçus, le numéro de séquence ICMP et le TTL). Elle met également à jour les statistiques.

- ```static void fill_seq_icmp(t_ping *ping)```: Fonction qui remplit le paquet ICMP avec les données nécessaires pour envoyer un paquet ICMP ECHO_REQUEST. Elle utilise la structure icmp pour définir les paramètres du paquet (type, code, ID et numéro de séquence) et remplit les données avec le caractère '*' pour les essais de ping.

- ```static int openSocket(t_ping *ping)```: Fonction qui ouvre une socket pour envoyer et recevoir des paquets ICMP. Elle utilise la fonction socket() pour créer la socket, puis la fonction setsockopt() pour définir un temps limite de réception et d'envoi. Elle alloue également de la mémoire pour stocker le paquet ICMP dans le membre packet de la structure t_ping.

- ```static void closePing(t_ping *ping)```: cette fonction est appelée pour fermer la connexion de ping. Elle affiche des statistiques sur les pings envoyés et reçus, ainsi que la perte de paquets. Elle libère ensuite la structure t_ping, ferme la connexion et termine le programme.

- ```static int host_to_ip(t_ping *ping)```: cette fonction convertit le nom d'hôte en adresse IP en utilisant la fonction getaddrinfo. Si la conversion réussit, l'adresse IP est stockée dans la structure t_ping. Sinon, elle renvoie une erreur.

- ```int run_ping(t_ping *ping)```: cette fonction exécute le protocole ping. Elle affiche l'adresse IP de la destination, ainsi que la taille des paquets à envoyer. Elle envoie ensuite des paquets à intervalles réguliers en utilisant les fonctions send et receive définies dans la structure t_ping, et ce, tant que le signal SIGINT n'est pas capturé par la fonction handle_signal. Si l'envoi de paquets échoue, la fonction close est appelée pour fermer la connexion de ping.
- ```t_ping *initPing()```: cette fonction initialise une structure t_ping en allouant de la mémoire avec la fonction malloc. Elle initialise les différents champs de la structure t_ping avec les fonctions définies dans la structure, telles que help, free, parse, connection, etc. Elle renvoie ensuite la structure initialisée.

## stat.c
- ```static int insert_data(t_stat *stat, double data, int flag)``` : insère un nouveau point de données dans la structure t_stat. Le paramètre flag est utilisé pour indiquer si le point de données doit être ajouté à la liste chaînée de données ou à la liste chaînée de stddev.
- ```static void free_data(t_stat *stat)```: libère toute la mémoire allouée aux listes chaînées de structures t_data dans la structure t_stat.
- ```static double get_min_data(t_stat *stat, int flag)```: retourne la valeur minimale dans la liste chaînée spécifiée par le paramètre flag.
- ```static double get_max_data(t_stat *stat, int flag)```: retourne la valeur maximale dans la liste chaînée spécifiée par le paramètre flag.
- ```static double get_avg_data(t_stat *stat, int flag)```: retourne la valeur moyenne de tous les points de données dans la liste chaînée spécifiée par le paramètre flag.
- ```void initStat(t_stat *stat)``` initialise une structure t_stat et définit les pointeurs de fonction aux fonctions appropriées.
