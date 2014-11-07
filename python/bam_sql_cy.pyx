#! /usr/bin/env python

import os
import pysam
import pdb
import sqlite3

cdef class bam_db:
    cdef str bam_fname
    cdef object bam
    cdef tuple refs
    cdef long bam_counts
    cdef str dest_fname
    cdef object conn
    cdef object c

    def __init__(self, bam_fname, dest_fname):
        self.bam_fname = bam_fname
        self.bam = pysam.Samfile(bam_fname, 'r')
        self.refs = self.bam.references
        self.bam_counts = self.bam.count()

        self.dest_fname = dest_fname

        self.conn = sqlite3.connect(self.dest_fname, isolation_level=None)
        self.c = self.conn.cursor()

    def fill_join(self):
        self.fill_db()
        self.join_bc_align()

    cpdef fill_db(self):
        self.c.execute('''CREATE TABLE IF NOT EXISTS align (name text, chrom int, position int)''')

        cdef long readno = 0
        chunk_size = self.bam_counts

        cdef object read
        self.c.execute("BEGIN TRANSACTION")
        for read in self.bam.fetch():
            if not read.is_unmapped:
                self.c.execute('''INSERT INTO align VALUES ('{0}', {1}, {2})'''.format(read.qname, read.rname, read.pos))
                readno += 1
            if readno == chunk_size:
                self.c.execute("COMMIT")
                self.c.execute("BEGIN TRANSACTION")
                readno = 0
        self.c.execute("COMMIT")

    # modify to joint to db
    cpdef join_bc_align(self):
        self.c.execute('''select data.name,
                          data.bc,
                          data.umi,
                          align.chrom,
                          align.position
                          FROM data INNER JOIN align where align.name = data.name''')

        out = open(self.dest_fname + "_out.txt", 'w')
        cdef tuple entry
        for entry in self.c.fetchall():
            out.write("\t".join([str(e) for e in entry]) + "\n")