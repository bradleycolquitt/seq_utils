#! /usr/bin/env python

# First code with no clas structures
# Online insertion of read data into h5
import sys
import os
import gzip
import bmh
import fasta
import pdb
import numpy as np
import Levenshtein as lev
import tables

BARCODES = "/home/brad/lib/barcodes/bc2.txt"

class extracter:
    def __init__(self, fastq_fname, h5_fname):
        self.fastq_fname = fastq_fname
        self.h5_fname = h5_fname
        self.h5 = 0
        # initialize h5 if does not exist
        if not os.path.exists(self.h5_fname):
            self.h5 = initialize_h5(self.h5_fname)
        else:
            self.h5 = tables.open_file(h5_fname, 'a')
        self.table = self.h5.root.data
        self.barcodes_file = open(BARCODES)
        self.barcodes = []
        self.num_barcodes = 0
        for l in self.barcodes_file:
            # expects format of name \t sequence
            self.barcodes.append(l.split()[1])
            self.num_barcodes += 1
        self.fastq_file = gzip.open(self.fastq_fname)
    def read_fastq(self):
        lineno = 0
        name = ""
        seq = ""
        qual = ""
        bc = ""
        umi = ""
        #pdb.set_trace()
        for l in self.fastq_file:
            if lineno % 4 == 0:
                name = l.strip()
            if lineno % 4 == 1:
                seq = l
            if lineno % 4 == 3:
                qual = l
                bc = self.get_barcode(seq, qual)
                umi = self.get_umi(seq, qual)
                self.insert_to_h5(name, bc, umi)
            lineno += 1
        self.h5.root.data.flush()
        self.h5.close()

    # Search sequence [5:11] for barcodes
    # Returns index of barcode with minimum distance
    def get_barcode(self, seq, qual):
        # First, check qual. If minimum PHRED is less than 20 anywhere in barcode di
        # Use Levenshtein distance to identify minimum scoring barcode

        distances = np.zeros(self.num_barcodes)
        sub = (5, 11)
        i = 0
        sseq = seq[sub[0]:sub[1]]
        for bc in self.barcodes:
            distances[i] = lev.distance(sseq, bc)
            i += 1
        #pdb.set_trace()
        distance_min = distances.min()
        if distance_min > 1: return 0
        candidates = np.where(distances == distance_min)[0]

        if len(candidates) > 1:
            candidates = self.get_barcode_multiple(candidates, sseq, qual[sub[0]:sub[1]])
        elif distance_min > 2:
            candidates = self.get_barcode_hard(candidates, sseq, qual[sub[0]:sub[1]])
        return candidates

    # Take candidate barcode from get_barcode
    # Return same barcode index if position(s) contributing to distance have low quality scores
    # Think about this more; may be better just to discard
    def get_barcode_hard(self, candidate, seq, qual):
        #ops = lev.opcodes(seq, self.barcodes[candidate])
        #ops = [op for op in ops if op[0]=='replace']
        #qual = fasta.qual_to_int(qual)
        return candidate

    # Return candidate with best quality score
    def get_barcode_multiple(self, candidates, seq, qual):
        qual = [fasta.qual_to_int(q) for q in qual]
        qual_sum = [sum(q) for q in qual]
        return np.where(qual_sum == qual_sum.min())

    def get_umi(self, seq, qual):
        sub = (0, 5)
        qual = fasta.qual_to_int(qual[sub[0]:sub[1]])
        if (min(qual) > 20):
            return seq[sub[0]:sub[1]]
        else:
            return 'N'

    def insert_to_h5(self, name, bc, umi):
        #pdb.set_trace()
        #table = self.h5.root.data
        table = self.table
        record = table.row
        record['name'] = name
        record['bc'] = bc
        record['umi'] = umi
        record.append()


class Read(tables.IsDescription):
    name = tables.StringCol(50)
    bc = tables.UInt8Col()
    umi = tables.StringCol(5)
    chrom = tables.UInt8Col()
    position = tables.UInt64Col()

# include functionality to read aligner info from MySQL db and add as properties
#    * aligner
#    * alignment date
#    * command line
#    * barcode file
def initialize_h5(h5_fname):
    h5 = tables.open_file(h5_fname, mode = "w")
    #group = h5.create_group('/', "data", 'Data table')
    table = h5.create_table('/', 'data', Read, "Reads")

    # Set attributes
    table.attrs.bc_file = BARCODES

    return h5
