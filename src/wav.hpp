#pragma once

int length_ticks(const std::string code);
void create_wav_file(const std::string fileName, const std::string message);
std::string decode_wav_file(const std::string fileName, const double tick); // tick = pas de temps du morse en seconde
