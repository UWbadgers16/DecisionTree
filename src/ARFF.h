//
 //  ARFF.h
 //
 //  Created on: September 12, 2016
 //      Author: John Cabaj
//

#ifndef ARFF_H
#define ARFF_H

#include <stdint.h>

void init_arff(void);																// initialize ARFF reader
void deinit_arff(void);																// deinitialize ARFF
uint8_t read_training_file(FILE* training_file);									// read training file
char* print_relation(void);															// print relation name
int get_total_examples(void);														// get total number of examples

#endif // ARFF_H