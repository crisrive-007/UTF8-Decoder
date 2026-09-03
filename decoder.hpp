#include <cstdint>
#include <vector>
#include <string>

struct Error {
    std::string error;
    uint32_t offset;
};

void leerArchivoEnBufferDeBytes(const std::vector<uint8_t> buffer);
void reportarError(const std::string error, const uint32_t offset);
void guardarCodePoint(const uint32_t code_point, int bytesUsados);
void generarReporte(const std::vector<uint8_t> buffer);