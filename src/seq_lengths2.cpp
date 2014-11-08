/*
  Sum of sequence lengths in a fastq file. Using Heng Li' kseq.h library for gzipped fastq reading
  Slightly faster than boost version (seq_lengths)
 */

#include <fstream>
#include <iostream>
#include <zlib.h>

extern "C"
{
#include <kseq/kseq.h>
}

KSEQ_INIT(gzFile, gzread)

int main (int argc, char** argv) {

        if(argc < 2) {
                std::cerr << "Output total amount of sequence to stdout." << std::endl;
                std::cerr << "Usage: " << argv[0] << " <gzipped FASTQ file>" << std::endl;
                return 1;
        }
        gzFile fp;
        kseq_t *seq;
        int l;

        fp = gzopen(argv[1], "r"); // STEP 2: open the file handler
        seq = kseq_init(fp); // STEP 3: initialize seq
        int pos = 1;
        unsigned long int total = 0;

        #ifdef CLOCK
        std::clock_t start;
        double duration;
        start = std::clock();
        #endif /* CLOCK */

        while ((l = kseq_read(seq)) >= 0) { // STEP 4: read sequence
            #ifdef DEBUG
            std::cout << seq->seq.s << std::endl;
            std::cout << seq->seq.l << std::endl;
            printf("name: %s\n", seq->name.s);
            if (seq->comment.l) printf("comment: %s\n", seq->comment.s);
            printf("seq: %s\n", seq->seq.s);
            if (seq->qual.l) printf("qual: %s\n", seq->qual.s);
            if (pos == 10) return 0;
            #endif /* DEBUG */

            total += seq->seq.l;
            ++pos;
        }

        #ifdef CLOCK
        duration = (std::clock() - start ) / (double) CLOCKS_PER_SEC;
        std::cout << "Run time: " << duration << "\t";
        #endif /* CLOCK */

        kseq_destroy(seq); // STEP 5: destroy seq
        gzclose(fp); // STEP 6: close the file handler
        std::cout << "Total sequence in " << argv[1] << ": " << total << std::endl;
        return 0;
}
