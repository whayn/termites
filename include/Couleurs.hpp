#ifndef COULEURS_HPP
#define COULEURS_HPP

#include <string>

namespace Console {
const std::string RESET = "\033[0m";
const std::string GRAS = "\033[1m";
const std::string PALE = "\033[2m";
const std::string ITALIQUE = "\033[3m";
const std::string SOULIGNE = "\033[4m";
const std::string CLIGNOTANT = "\033[5m";
const std::string INVERSE = "\033[7m";
const std::string BARRE = "\033[9m";

// Texte
const std::string ROUGE = "\033[31m";
const std::string VERT = "\033[32m";
const std::string JAUNE = "\033[33m";
const std::string BLEU = "\033[34m";
const std::string MAGENTA = "\033[35m";
const std::string CYAN = "\033[36m";
const std::string BLANC = "\033[37m";
// Fond
const std::string FOND_ROUGE = "\033[41m";
const std::string FOND_VERT = "\033[42m";
const std::string FOND_JAUNE = "\033[43m";
const std::string FOND_BLEU = "\033[44m";
const std::string FOND_MAGENTA = "\033[45m";
const std::string FOND_CYAN = "\033[46m";
const std::string FOND_BLANC = "\033[47m";

} // namespace Console

#endif
