#! /usr/bin/env python

###
# Loop through BAM, search against h5 'read' column, update row
###

import pysam
import tables
import pdb


class bam_h5:
    def __init__(self, bam_fname, h5_fname):
        self.bam_fname = bam_fname
        self.bam = pysam.Samfile(bam_fname, 'r')
        self.refs = self.bam.references

        self.h5_fname = h5_fname
        self.h5 = tables.open_file(h5_fname, 'a')
        #self.table = self.h5.root.data

    def update(self):
        table = self.h5.root.data
        for read in self.bam.fetch():
            name = read.qname
            #pdb.set_trace()
        # get index
            ind = table.get_where_list('name=="' + name + '"')[0]
            table.cols.chrom[ind] = read.rname
            table.cols.position[ind] = read.pos
            #row.update()
        table.flush()
