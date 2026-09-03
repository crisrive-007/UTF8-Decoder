#include <cstddef>
#include <cstdint>
#include <fstream>
#include <iostream>
#include <vector>

#include "decoder.hpp"

int main(int argc, char* argv[]) {
    std::string ruta;

    if(argc < 2) {
        std::cout << "ERROR: No se ingreso ninguna ruta de archivo." << std::endl;
        return 1;
    } else if(argc > 2) {
        std::cout << "ERROR: Formato de argumentos invalido." << std::endl;
        return 1;
    } else {
        ruta = argv[1];
    }

    std::ifstream in(ruta, std::ios::binary);

    if(!in.is_open()) {
        std::cout << "ERROR: No se pudo abrir el archivo." << std::endl;
        return 1;
    }

    std::vector<uint8_t> buffer;

    in.seekg(0, std::ios::end);
    size_t size = in.tellg();
    in.seekg(0, std::ios::beg);

    buffer.resize(size);
    
    in.read(reinterpret_cast<char*>(buffer.data()), size);

    leerArchivoEnBufferDeBytes(buffer);
}