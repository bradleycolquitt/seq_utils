#include <fstream>
#include <iostream>
#include <vector>
#include <boost/iostreams/filtering_streambuf.hpp>
#include <boost/iostreams/copy.hpp>
#include <boost/iostreams/filter/gzip.hpp>


int main (int argc, char** argv) {

        if(argc < 2) {
                std::cerr << "Output total amount of sequence to stdout." << std::endl;
                std::cerr << "Usage: " << argv[0] << " <gzipped FASTQ file>" << std::endl;
                return 1;
        }

        char* filename = argv[1];
        std::cout << filename << std::endl;

        std::fstream file(filename, std::fstream::binary | std::fstream::in);
        boost::iostreams::filtering_streambuf<boost::iostreams::input> in;
        in.push(boost::iostreams::gzip_decompressor());
        in.push(file);
        std::istream gzs(&in);

        std::string line;
        int pos = 1;
        unsigned long int total = 0;
        #ifdef CLOCK
        std::clock_t start;
        double duration;

        start = std::clock();
        #endif /* CLOCK */
        while(std::getline(gzs, line)) {
            #ifdef DEBUG
            std::cout << pos << std::endl;
            #endif /* DEBUG */
                        if (pos % 4 == 2) {
                                #ifdef DEBUG
                                std::cout << std::numeric_limits<unsigned long int>::max() << std::endl;
                                std::cout << line << std::endl;
                                std::cout << line.length() << std::endl;
                                if (pos == 10) return 0;
#endif /* DEBUG */

                                total += line.length();
                        }
                        ++pos;
                }
        #ifdef CLOCK
        duration = (std::clock() - start ) / (double) CLOCKS_PER_SEC;
        std::cout << "Run time: " << duration << "\t";
        #endif /* CLOCK */
        std::cout << "Total sequence in " << filename << ": " << total << std::endl;
        return 1;
}
