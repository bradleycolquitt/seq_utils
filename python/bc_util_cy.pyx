import Levenshtein as lev

cpdef int min_barcode(list barcodes, char* sseq):
    cdef int val = 0
    cdef int distance_min = 100
    cdef int i = 0
    cdef int ind = 0
    
    for bc in barcodes:
        val = lev.distance(sseq, bc)
        if val == 0:
            ind = i
            
            return ind
        if val < distance_min:
            distance_min = val
            ind = i
        i += 1

    if distance_min > 0: ind = -1
    return ind