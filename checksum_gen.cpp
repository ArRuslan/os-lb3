#include <cstdint>
#include <cstdio>
#include <fstream>
#include <string>

uint32_t get_checksum(char* buf, uint32_t size) {
    uint32_t checksum = 0;
    auto* u32arr = (uint32_t*)buf;
    for (uint32_t i = 0; i < size / 4; ++i)
        checksum += u32arr[i];

    uint32_t left = size % 4;
    if (left) {
        buf += size;
        uint32_t left_cs = 0;
        memcpy(&left_cs, buf, left);
        checksum += left_cs;
    }

    return checksum;
}

void write_checksum(const std::string &path) {
    std::ifstream file(path, std::ios::binary);
    uint32_t checksum;

    file.seekg(0, std::ios::end);
    size_t file_size = file.tellg();
    char* buf = new char[file_size];
    file.seekg(0);
    file.read(buf, file_size);
    checksum = get_checksum(buf, file_size);
    delete[] buf;

    file.close();
    std::ofstream out_file(path, std::ios::ate | std::ios::in | std::ios::out | std::ios::binary);

    char checksumKw[] = "checksum\0\0\0\0";
    out_file.write(checksumKw, 12);

    out_file.seekp(-4, std::ios::end);
    char* checksumBuf = new char[4];
    memcpy(checksumBuf, &checksum, 4);
    out_file.write(checksumBuf, 4);

    delete[] checksumBuf;
    out_file.close();
}

int main(int argc, char** argv) {
    if(argc < 2)
        return 1;

    write_checksum(std::string(argv[argc-1]));
}
