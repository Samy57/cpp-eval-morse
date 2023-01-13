#include <string>
#include <map>

std::map<std::string, std::string> mtoa; // morse to alphabet
std::map<std::string, std::string> atom; // alphabet to morse

void init_codex()
{
    std::array<std::pair<std::string, std::string>, 82> alphabet{ // Tableau de correspondance morse <-> alphabet
        std::pair<std::string, std::string>(".-", "a"),
        {"-...", "b"},
        {"-.-.", "c"},
        {"-..", "d"},
        {".", "e"},
        {"..-.", "f"},
        {"--.", "g"},
        {"....", "h"},
        {"..", "i"},
        {".---", "j"},
        {"-.-", "k"},
        {".-..", "l"},
        {"--", "m"},
        {"-.", "n"},
        {"---", "o"},
        {".--.", "p"},
        {"--.-", "q"},
        {".-.", "r"},
        {"...", "s"},
        {"-", "t"},
        {"..-", "u"},
        {"...-", "v"},
        {".--", "w"},
        {"-..-", "x"},
        {"-.--", "y"},
        {"--..", "z"},
        {".-", "A"},
        {"-...", "B"},
        {"-.-.", "C"},
        {"-..", "D"},
        {".", "E"},
        {"..-.", "F"},
        {"--.", "G"},
        {"....", "H"},
        {"..", "I"},
        {".---", "J"},
        {"-.-", "K"},
        {".-..", "L"},
        {"--", "M"},
        {"-.", "N"},
        {"---", "O"},
        {".--.", "P"},
        {"--.-", "Q"},
        {".-.", "R"},
        {"...", "S"},
        {"-", "T"},
        {"..-", "U"},
        {"...-", "V"},
        {".--", "W"},
        {"-..-", "X"},
        {"-.--", "Y"},
        {"--..", "Z"},
        {".----", "1"},
        {"..---", "2"},
        {"...--", "3"},
        {"....-", "4"},
        {".....", "5"},
        {"-....", "6"},
        {"--...", "7"},
        {"---..", "8"},
        {"----.", "9"},
        {"-----", "0"},
        {".-.-.-", "."},
        {"--..--", ","},
        {"..--..", "?"},
        {".----.", "'"},
        {"-.-.-----.", "!"},
        {"-..-.", "/"},
        {"-.--.", "("},
        {"-.--.-", ")"},
        {".-...", "&"},
        {"---...", ":"},
        {"-.-.-.", ";"},
        {"-...-", "="},
        {".-.-.", "+"},
        {"-....-", "-"},
        {"..--.-", "_"},
        {".-..-.", "\""},
        {"...-..-", "$"},
        {".--.-.", "@"},

        {"/", " "} // Notation de l'espace par un "/" en morse
    };

    for (auto const element : alphabet) // Initialisation des 2 dictionnaires
    {
        mtoa.insert(element);
        atom.insert(std::pair<std::string, std::string>(element.second, element.first));
    }

}

std::string codage(const std::string message){

    std::string code{""};
    for (auto a : message)
    {
        code = code + atom[std::string(1, a)] + " "; // Codage alphabet vers morse par le dict atom
    }

    return code;
}


std::string decodage(const std::string code){

    std::string decode{""};
    std::string buffer{""};
    
    for (auto a : code) // Décodage du code morse : lecture des signaux jusqu'à la fin d'une lettre (signalée par un espace) puis décodage du buffer de signaux morse
    {
        if(a == ' ')
        {
            decode = decode + mtoa[buffer];
            buffer = "";
        }
        else
        {
            buffer = buffer + a;
        }
    }

    return decode;
}
