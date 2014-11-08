/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Copyright by The HDF Group.                                               *
 * Copyright by the Board of Trustees of the University of Illinois.         *
 * All rights reserved.                                                      *
 *                                                                           *
 * This file is part of HDF5.  The full HDF5 copyright notice, including     *
 * terms governing use, modification, and redistribution, is contained in    *
 * the files COPYING and Copyright.html.  COPYING can be found at the root   *
 * of the source code distribution tree; Copyright.html can be found at the  *
 * root level of an installed copy of the electronic HDF5 document set and   *
 * is linked from the top-level documents page.  It can also be found at     *
 * http://hdfgroup.org/HDF5/doc/Copyright.html.  If you do not have          *
 * access to either file, you may request a copy from help@hdfgroup.org.     *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

/*
 *  This example writes a dataset to a new HDF5 file.
 */

#include <iostream>
#include <string>

#include "H5Cpp.h"

#ifndef H5_NO_NAMESPACE
    using namespace H5;
#endif

/*
 * libraries are organized into groups
 * each dataset contains data from one sequencing
 */

/* create class containing
 *    variables
 *        filename
 *        dataset name
 *        other attributes
 *    function
 *    constructors
 *    destructors
 *    various set/get functions
 */


/*each library has dataset
 * ONLY ONE DATATYPE PER DATASET
 * columns of hdf5 dataset
 *    unique id: long int
 *    name: string
 *    barcode: int indentifier, 0 for undefined
 *    UMI: sequence
 *    chrom: unsigned
 *    position: int
 *
 * properties
 *    aligner
 *    idlibrary
 *    idsequencing
 *    alignment date
 */
class readData {
public:
   // Variables
   string name;
   unsigned barcode;
   Seqan::DnaString umi;

   //Constructors
   readData(string name, unsigned barcode, seqan::DnaString umi) {
      name = name;
      barcode = barcode;
      umi = umi;
   }

}

class indexData {


public:
   // Variables
   H5std_string file_name;
   H5std_string dataset_name;
   H5File file;

   // Constructors
   indexData (file_name) {
      //if file exists, load hdf5 data
      //else, create file
   }
   indexData (file_name, dataset_name);
   // Destructors
   ~indexData() {
      // Release memory of variables
      // Flush h5
      // close h5
   }
   // Gettors
   string getId();
   unsigned getBarcode();
   String<Dna> getUMI();

   // format number in chrom column as 'chrN'
   string getChrom();
   unsigned getPosition();

   // Insert
   void insertReadData(readData rd);

   // Settors
   void setFileName();
   void setDatasetName();
   void setName();
   void setBarcode();
   void setUMI();

   // Output
   void groupByB
};

int main (void)
{
   /*
    * Data initialization.
    */
   int i, j;
   int data[NX][NY];          // buffer for data to write
   for (j = 0; j < NX; j++)
   {
      for (i = 0; i < NY; i++)
	 data[j][i] = i + j;
   }
   /*
    * 0 1 2 3 4 5
    * 1 2 3 4 5 6
    * 2 3 4 5 6 7
    * 3 4 5 6 7 8
    * 4 5 6 7 8 9
    */

   // Try block to detect exceptions raised by any of the calls inside it
   try
   {
      /*
       * Turn off the auto-printing when failure occurs so that we can
       * handle the errors appropriately
       */
      Exception::dontPrint();

      /*
       * Create a new file using H5F_ACC_TRUNC access,
       * default file creation properties, and default file
       * access properties.
       */
      H5File file( FILE_NAME, H5F_ACC_TRUNC );

      /*
       * Define the size of the array and create the data space for fixed
       * size dataset.
       */
      hsize_t     dimsf[2];              // dataset dimensions
      dimsf[0] = NX;
      dimsf[1] = NY;
      DataSpace dataspace( RANK, dimsf );

      /*
       * Define datatype for the data in the file.
       * We will store little endian INT numbers.
       */
      IntType datatype( PredType::NATIVE_INT );
      datatype.setOrder( H5T_ORDER_LE );

      /*
       * Create a new dataset within the file using defined dataspace and
       * datatype and default dataset creation properties.
       */
      DataSet dataset = file.createDataSet( DATASET_NAME, datatype, dataspace );

      /*
       * Write the data to the dataset using default memory space, file
       * space, and transfer properties.
       */
      dataset.write( data, PredType::NATIVE_INT );
   }  // end of try block

   // catch failure caused by the H5File operations
   catch( FileIException error )
   {
      error.printError();
      return -1;
   }

   // catch failure caused by the DataSet operations
   catch( DataSetIException error )
   {
      error.printError();
      return -1;
   }

   // catch failure caused by the DataSpace operations
   catch( DataSpaceIException error )
   {
      error.printError();
      return -1;
   }

   // catch failure caused by the DataSpace operations
   catch( DataTypeIException error )
   {
      error.printError();
      return -1;
   }

   return 0;  // successfully terminated
}

