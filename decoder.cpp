#include "decoder.hpp"
#include <cstdint>
#include <iostream>
#include <string>
#include <format>

uint32_t offset;
uint8_t b1, b2, b3, b4;
uint32_t code_point;
std::vector<uint32_t> code_points;
int bytesUsados;
int cantidadBytes[4];
std::vector<Error> errores;

void leerArchivoEnBufferDeBytes(const std::vector<uint8_t> buffer) {
    offset = 0;

    if (buffer.size() >= 3 && buffer[0] == 0xEF && buffer[1] == 0xBB && buffer[2] == 0xBF) {
        offset = 3;
    }

    while(offset < buffer.size()) {
        b1 = buffer[offset];

        if((b1 & 0x80) == 0x00) {
            code_point = b1;
            bytesUsados = 1;
        }
        else if((b1 & 0xE0) == 0xC0) {
            bytesUsados = 2;

            if((offset + 1) >= buffer.size()) {
                reportarError("Secuencia incompleta", offset);
                offset++;
                continue;
            }

            b2 = buffer[offset + 1];

            if((b2 & 0xC0) != 0x80) {
                reportarError("Byte de continuacion esperado, no encontrado", offset);
                offset++;
                continue;
            }

            code_point = (((b1 & 0x1F) << 6) | (b2 & 0x3f));

            if(code_point < 0x80) {
                reportarError("Codificacion sobrelarga", offset);
                offset++;
                continue;
            }
        }
        else if((b1 & 0xF0) == 0xE0) {
            bytesUsados = 3;

            if((offset + 2) >= buffer.size()) {
                reportarError("Secuencia incompleta", offset);
                offset++;
                continue;
            }

            b2 = buffer[offset + 1];
            b3 = buffer[offset + 2];

            if((b2 & 0xC0) != 0x80 || (b3 & 0xC0) != 0x80) {
                reportarError("Byte de continuacion esperado, no encontrado", offset);
                offset++;
                continue;
            }

            code_point = (((b1 & 0x0F) << 12) | ((b2 & 0x3F) << 6) | (b3 & 0x3F));

            if(code_point < 0x800) {
                reportarError("Codifiacion sobrelarga", offset);
                offset++;
                continue;
            }
        }
        else if((b1 & 0xF8) == 0xF0) {
            bytesUsados = 4;

            if((offset + 3) >= buffer.size()) {
                reportarError("Secuencia incompleta", offset);
                offset++;
                continue;
            }

            b2 = buffer[offset + 1];
            b3 = buffer[offset + 2];
            b4 = buffer[offset + 3];

            if((b2 & 0xC0) != 0x80 || (b3 & 0xC0) != 0x80 || (b4 & 0xC0) != 0x80) {
                reportarError("Byte de continuacion esperado, no encontrado", offset);
                offset++;
                continue;
            }

            code_point = (((b1 & 0x07) << 18) | ((b2 & 0x3f) << 12) | ((b3 & 0x3F) << 6) | ((b4 & 0x3F)));

            if(code_point <0x10000) {
                reportarError("Codificacion sobrelarga", offset);
                offset++;
                continue;
            }
        }
        else if((b1 & 0xC0) == 0x80) {
            reportarError("Byte de continuacion inesperado", offset);
            offset++;
            continue;
        }
        else {
            reportarError("Byte lider invalido", offset);
            offset++;
            continue;
        }
        
        guardarCodePoint(code_point, bytesUsados);

        offset += bytesUsados;

        cantidadBytes[bytesUsados - 1]++;
    }

    generarReporte(buffer);
}

void reportarError(const std::string error, const uint32_t offset) {
    Error er;
    er.error = error;
    er.offset = offset;

    errores.push_back(er);
}

void guardarCodePoint(const uint32_t code_point, int bytesUsados) {
    code_points.push_back(code_point);
}

void generarReporte(const std::vector<uint8_t> buffer) {
    std::cout <<  "=== Contenido decodificado ===" << std::endl;

    for(int i = 0; i < code_points.size(); i++) {
        if(code_points[i] >= 0x20 && code_points[i] <= 0x7E) {
            std::cout << static_cast<uint8_t>(code_points[i]) << std::endl;
        } else {
            std::cout << std::format("U+{:04X}", code_points[i]) << std::endl;
        }
    }

    std::cout << std::endl;
    std::cout << "=== Errores detectados ===" << std::endl;

    for(Error er : errores) {
        std::cout << "[offset " << er.offset << "] " << er.error << std::endl;
    }

    std::cout << std::endl;
    std::cout << "=== Resumen ===" << std::endl;

    std::cout << "Bytes totales: " << buffer.size() << std::endl;

    std::cout << "Code Points validos: " << cantidadBytes[0] + cantidadBytes[1] + cantidadBytes[2] + cantidadBytes[3] << std::endl;
    
    for (int i = 0; i < 4; i++) {
        std::cout << " - " <<  i+1 << " byte: " << cantidadBytes[i] << std::endl;
    }

    std::cout << "Errores detectados: " << errores.size() << std::endl;
}