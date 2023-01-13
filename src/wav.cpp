#include <fstream>
#include <iostream>
#include <vector>
#define _USE_MATH_DEFINES
#include <cmath>

#include "morse.hpp"

double tick {0.1}; // Durée d'un tick en seconde

typedef struct WAV_HEADER {
  /* RIFF Chunk Descriptor */
  uint8_t RIFF[4] = {'R', 'I', 'F', 'F'}; // RIFF Header Magic header
  uint32_t ChunkSize;                     // RIFF Chunk Size
  uint8_t WAVE[4] = {'W', 'A', 'V', 'E'}; // WAVE Header
  /* "fmt" sub-chunk */
  uint8_t fmt[4] = {'f', 'm', 't', ' '}; // FMT header
  uint32_t Subchunk1Size = 16;           // Size of the fmt chunk
  uint16_t AudioFormat = 1; // Audio format 1=PCM,6=mulaw,7=alaw,     257=IBM
                            // Mu-Law, 258=IBM A-Law, 259=ADPCM
  uint16_t NumOfChan = 1;   // Number of channels 1=Mono 2=Sterio
  uint32_t SamplesPerSec = 16000;   // Sampling Frequency in Hz
  uint32_t bytesPerSec = 16000 * 2; // bytes per second
  uint16_t bytesPerBloc = 2;          // 2=16-bit mono, 4=16-bit stereo
  uint16_t bitsPerSample = 16;      // Number of bits per sample
  /* "data" sub-chunk */
  uint8_t Subchunk2ID[4] = {'d', 'a', 't', 'a'}; // "data"  string
  uint32_t Subchunk2Size;                        // Sampled data length
} wav_hdr;


int length_ticks(const std::string code){ // Calcul à partir d'un code morse le nombre de ticks nécessaires de son pour le coder

    int s {0};

    for (auto a : code){
        if( a == '.'){
            s += 2; // 1 tick de son + 1 tick blanc de fin de signal
        }
        else if( a == '-'){
            s += 4; // 3 ticks de son + 1 tick blanc de fin de signal
        }
        else if( a == ' '){
            s += 2; // 1 tick de blanc + 1 tick blanc de fin de signal
        }
        else if( a == '/'){
            s += 2; // 1 tick de blanc + 1 tick blanc de fin de signal
        }
    }

    return s;
}


void create_wav_file(const std::string file_name,const std::string message){

    wav_hdr wav;

    // CODAGE DU MESSAGE
    std::string code {codage(message)}; // message codé en morse

    // INITIALISATION DU FICHIER WAV
    
    std::ofstream file(file_name, std::ios::out | std::ios::binary);
    if(!file){
        std::cout << "File creation failed" << std::endl; 
    }

    // ECRITURE DU BLOC HEADER
    int ticks {length_ticks(code)}; // nombre de ticks à coder en audio
    int bytesPerTick {round(wav.SamplesPerSec*wav.bitsPerSample*tick/8)}; // nombre de bytes par tick codé
    wav.Subchunk2Size = ticks * bytesPerTick; // nombre de bytes d'audio dans le fichier
    wav.ChunkSize = wav.Subchunk2Size + 36; // indication longueur du fichier
    file.write(reinterpret_cast<const char *>(&wav), sizeof(wav));



    // ENCODAGE DU MORSE DANS LE FICHIER AUDIO
    int sound{0}; // False = les ticks codées doivent être du blanc, True on code du son
    int nTicks{0}; // nombre de ticks à coder (tick de blanc de fin de signal exclu)
    int i{0};
    int value{0};
    std::cout << code << std::endl;
    for (auto a : code) // on prend les signaux un par un et on les code en son
    {
        // Définition du signal à coder selon le caractère morse a
        sound = 0;
        if( a == '.'){
            sound = 1;
            nTicks = 1;
        }
        else if( a == '-'){
            sound = 1;
            nTicks = 3;
        }
        else if( a == '/'){
            nTicks = 1;
        }
        else if( a == ' '){
            nTicks = 1;
        }

        // Signal audio
        for(i = 0; i < nTicks*bytesPerTick/2; i++) // Division par 2 cf ci-dessous 1 int = 2 bytes
        {
            value = static_cast<int> (32767 * sound * sin(i * 2 * M_PI * 440 / wav.SamplesPerSec) );
            file.write(reinterpret_cast<const char *>(&value), 2); // 1 int = 2 bytes
        }

        // Signal de 1 tick blanc de fin de signal
        for(i = 0; i < bytesPerTick/2; i++) // Division par 2 cf ci-dessous 1 int = 2 bytes
        {
            value = static_cast<int> (0);
            file.write(reinterpret_cast<const char *>(&value), 2); // 1 int = 2 bytes
        }

    }

    file.close();
    std::cout << "Fichier " << file_name << " créé." << std::endl;

}



std::string decode_wav_file(const std::string file_name, const double tick)
{

    // OUVERTURE DU FICHIER WAV
    wav_hdr read;
    FILE* file;
    file = fopen(file_name.c_str(), "rb");
    if(!file){
        std::cout << "File opening failed, check that it is in the same folder." << std::endl;
    }

    // LECTURE DU HEADER DU FICHIER
    fread(&read, 44, 1, file); // 44 octets
  

    // LECTURE DES DONNEES AUDIO ET OBTENTION DU CODE MORSE

    long int data {0}; // Lecture des bytes du fichier
    int i {0}; // pour boucle for seulement
    long int blanc {0}; // Compteur de bytes blancs
    long int son {0}; // Compteur de bytes de son
    std::string buffer {""}; // buffer en morse
    std::string code {""}; // code morse
    int ticksSon {0};
    int ticksBlanc {0};

    for(i = 0; i < read.Subchunk2Size/read.bytesPerBloc; i++)
    {
        fread(&data, sizeof(read.bytesPerBloc), 1, file);

        if( data == 0 ) // c'est un blanc
        {
            blanc ++;
            if( blanc > 1) // Ce n'est pas un 0 appartenant au son (une sinusoide par exemple), on a bien fini le signal de son qui précède, on l'analyse
            {
                ticksSon = round(son*read.bytesPerBloc/(read.bytesPerSec*tick));
                if(ticksSon != 0)
                {
                    if( ticksSon == 3 ) 
                    {
                        buffer = buffer + "-";
                    }
                    else if( ticksSon == 1)
                    {
                        buffer = buffer + ".";
                    }
                    else
                    {
                        std::cout << "Erreur dans la détection des ticks à la lecture du fichier. Vérifiez le tick utilisé. " <<std::endl;
                    }
                }
                son = 0; // On prépare la lecture d'un nouveau signal de son
            }
        }
        else // c'est du son
        {
            son++;

            ticksBlanc = round(blanc*read.bytesPerBloc/(read.bytesPerSec*tick));

            if(ticksBlanc == 0 && blanc == 1) // il y a eu un zéro parmi le son, comme par exemple dans une sinusoide
            {
                son++; // on corrige le comptage des bytes de son
            }
            else if(ticksBlanc == 3) // fin de lettre
            {
                code = code + buffer + " "; // on décode la lettre

                buffer = "";
            }
            else if(ticksBlanc == 5 || ticksBlanc == 7) // Espace après une lettre
            {
                code = code + buffer + " / ";

                buffer = "";
            }
            // Si ticksBlanc == 1  c'est un blanc entre deux signaux d'une même lettre, il n'y a rien à analyser de ce blanc on continue de lire les signaux de cette lettre (donc on ne vide pas le buffer)

            blanc = 0;
        }
    }

    // Si l'audio se fini sur du son il faut récupérer la dernière lettre (code ci-dessous)
    if(blanc == 0) // Si il n'y a pas de blanc après le dernier signal on le récupère ici
    {
        ticksSon = round(son*read.bytesPerBloc/(read.bytesPerSec*tick));
        if( ticksSon = 3 ) 
        {
            buffer = buffer + "-";
        }
        else if( ticksSon = 1)
        {
            buffer = buffer + ".";
        }
        else
        {
            std::cout << "Erreur dans la détection des ticks à la lecture du fichier." << std::endl;
        }
    }
    code = code + buffer + " ";

    // AFFICHAGE DU CODE MORSE SI L'UTILISATEUR SOUHAITE LE RECUPERER
    std::cout << "\nCode morse :\n" << code << std::endl;

    // DECODAGE DU MORSE 
    std::string message {decodage(code)};

    fclose(file);
    return message;
}