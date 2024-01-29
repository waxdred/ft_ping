#include "../includes/ft_ping.h"

void help(char *s) {
  fprintf(stderr, "Usage: sudo %s [OPTIONS] ip_adress\n\n", s);
  fprintf(stderr, "Options:\n");
  fprintf(stderr, "  -h\t\tAfficher cette aide et quitter.\n");
  fprintf(stderr, "  -s size\tTaille du paquet ECHO_REQUEST (en octets)\n");
  fprintf(
      stderr,
      "  -c count\tNombre de paquets ECHO_REQUEST à envoyer (puis arrêter).\n");
  fprintf(stderr,
          "  -t ttl\tDéfinir la valeur TTL (Time to Live) des paquets IP.\n");
  fprintf(stderr, "  -v\t\tActiver le mode verbeux (affiche des informations "
                  "supplémentaires).\n");
  fprintf(stderr, "  -W timeout\tSpécifier un délai maximal (en secondes) pour "
                  "attendre une réponse.\n");
  fprintf(stderr, "  -w deadline\tDéfinir une échéance (en secondes) après "
                  "laquelle ft_ping s'arrête,\n");
  fprintf(
      stderr,
      "         \tpeu importe combien de paquets ont été envoyés ou reçus.\n");
  fprintf(stderr, "  -q\t\tMode silencieux. Affiche uniquement les résumés de "
                  "début et de fin.\n");
}
