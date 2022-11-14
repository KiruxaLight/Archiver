#include "archiver.h"
#include "compressor.h"
#include "decompressor.h"

void Archiver::Compress(const std::string& name, Writer& writer, bool end_archive) {
    Compressor compressor;
    compressor.Compress(name, writer, end_archive);
}

void Archiver::Decompress(const std::string& name) {
    Decompressor decompressor;
    decompressor.Decompress(name);
}

void Archiver::OutHelp() {
    std::cout << "Archive file: archiver -c archive_name file1 file2 ...\n";
    std::cout << "Dearchive file: archiver -d archive_name\n";
    std::cout << "Get a help: archiver -h\n";
}