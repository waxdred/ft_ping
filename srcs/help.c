#include "../includes/ft_ping.h"

static void help1() {
  fprintf(
      stderr,
      "ft_ping [-h help] [-c count] [-t ttl] [-v verbose] [-W timeout] [-w ");
  fprintf(stderr, "deadline]");
  fprintf(stderr, "[-w deadline] [-q silence]\n\n");
  fprintf(stderr, "OPTIONS\n\n\t");
  fprintf(stderr, "-c count\n\t\tS'arrêter après l'envoi de nombre paquets ");
  fprintf(stderr, "ECHO_REQUEST.\n\t\t");
  fprintf(stderr, "Combiné");
  fprintf(stderr,
          "à l'option  -w deadline,  ping  doit recevoir ses nombre paquets ");
  fprintf(stderr, "ECHO_REPLY\n\t\t");
  fprintf(stderr, "avant que la temporisation n'expire.\n\n\t");
  fprintf(stderr, "-t ttl\n\t\tSpécifie le champ IP « Time to Live ».\n\n\t");
  fprintf(stderr, "-v verbose\n\t\tSortie verbeuse.\n\n\t");
  fprintf(
      stderr,
      "-W deadline \n\t\tFixe un délai (en secondes),  avant que  ping ne ");
}

void help(char *s) {
  fprintf(stderr, "Usage: sudo %s ip_adress\n\n", s);
  help1();
  fprintf(stderr, "mette fin à son");
  fprintf(stderr, "reçus.Note ");
  fprintf(stderr, ": ici,");
  fprintf(stderr,
          "ping ne s 'arrêtera pas  après l' envoi de quelques paquets,\n\t\t");
  fprintf(
      stderr,
      "mais attendra que le délai expire ou que nombre sondes aient reçu une");
  fprintf(stderr,
          "ou encore qu'une notification d'erreur provienne du réseau. \n\n\t");
  fprintf(stderr, "\n\n\t -q silence \n\t\tSortie silencieuse. Rien n'est "
                  "affiché sauf les ");
  fprintf(stderr,
          "lignes \n\t\t de résumé au démarrage et à la fin de l'exécution. ");
  fprintf(stderr, "?)\n\n\t");
}
