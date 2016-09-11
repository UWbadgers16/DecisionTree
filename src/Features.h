//
 //  Features.h
 //
 //  Created on: March 30, 2016
 //      Author: John Cabaj
//

#ifndef FEATURES_H
#define FEATURES_H

#include <stdint.h>
#include <string.h>
#include "Classes.h"

// defines a Value linked list node
typedef struct Value
{
	float value;																	// value
    struct Value* next;                                                        		// pointer to next node
    Class* class;																	// pointer to Class
} Value;

// defines a Feature linked list node
typedef struct Feature
{
    struct Feature* next;                                                        	// pointer to next node
    char* feature_name;																// name of Feature
    Value* values_head;																// head of Values of this Feature
    Value* values_tail;																// tail of Values of this Feature
    int num_values;																	// numer of Values of this Feature
} Feature;

void init_features(void);															// initialize Features
void deinit_features(void);															// deinitialize Features
void add_feature(char* feature_name);                                				// add a Feature
void add_value(Feature* feature, float value, Class* class);                       	// add a Value
Feature* get_features_head(void);                                               	// get Features head
Feature* get_features_tail(void);                                               	// get Features tail
Value* get_values_head(Feature* feature);                                           // get values head
Value* get_values_tail(Feature* feature);                                           // get values tail
int get_num_features(void);															// get number of Features
int get_num_values(Feature* feature);												// get number of values
void print_features(void);                                                        	// print contents of Features linked list
void print_values(Feature* feature);                                                // print contents of values linked list

#endif // FEATURES_H