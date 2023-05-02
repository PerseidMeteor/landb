#include "iostream"
#include "thrid_party/clipp/include/clipp.h"
#include "replxx.h"

int main(int argc, char **argv)
{
    // parse command line
    std::string dbpath;
    bool check_crc = false;
    size_t dead_bytes_threshold = 128 * 1024 * 1024;

    auto cli = (clipp::value("db path", dbpath),
                clipp::option("-c", "--checksum")
                    .set(check_crc)
                    .doc("Enable CRC verification"),
                clipp::option("-d", "--dead_bytes_threshold")
                    .set(dead_bytes_threshold)
                    .doc("maximum single log file size"));
    if (!clipp::parse(argc, argv, cli))
    {
        std::cerr << clipp::make_man_page(cli, argv[0]);
        return 0;
    };

    return 0;
}