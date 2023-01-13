#include <iostream>
#include <string>
#include <map>
#include <fstream>

#include "morse.hpp"
#include "wav.hpp"

int main()
{
    init_codex(); // initialisation des dictionnaires globaux au fichier morse.cpp

    std::string message;
    std::string fileName;
    int choix {0};
    double tick {0};

    while(choix != 3)
    {
        std::cout << "Que souhaitez-vous faire :\n1. Coder un message\n2. Décoder un message\n3. Fermer le programme.\n";
        std::cin >> choix;
        std::cin.get();

        if( choix == 1)
        {
            std::cout << "Entrez le message : " << std::endl;
            std::getline(std::cin , message);
            std::cout << "Entrez le nom du fichier wav à créer: " << std::endl;
            std::cin >> fileName;
            create_wav_file(fileName, message);
        }
        else if(choix == 2)
        {
            std::cout << "Entrez le nom du fichier wav à décoder : " << std::endl;
            std::cin >> fileName;
            std::cout << "Entrez le tick utilisé (en seconde) : " << std::endl;
            std::cin >> tick;
            message = decode_wav_file(fileName, tick);

            std::cout << "Message décodé : \n" << message << std::endl << std::endl ;
        }
    }



    return EXIT_SUCCESS;
}