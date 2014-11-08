/*
  Set up some data structure (hdf5, sqlite) that will allow fast random access
  Stream in gzipped fastq file
  For each record,
      store readid to data structure
      approximate search against indexed set of barcodes
         infix region [5,12], extended by 1 nt to account for indels
         update data structure with barcode
         store begin/end positions
      if barcode start position != 6, discard read
      extract 5 nt upstream of barcode begin position
         update data structure with UMI
 */
