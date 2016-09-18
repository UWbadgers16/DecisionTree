//
 //  Features.c
 //
 //  Created on: March 22, 2016
 //      Author: John Cabaj
//

#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include "Features.h"

static Feature* features_head = NULL;                                                                       // initialize head of list of Features
static Feature* features_tail = NULL;                                                                       // initialize tail of list of Features
static int num_features = 0;                                                                                // number of Features

// initialize Features
void init_features(void)
{
    num_features = 0;                                                                                       // reset number of Features
}

// deinitialize Features
void deinit_features(void)
{
    // initialize variables
    Feature* feature_walker = features_head;
    Feature* feature_temp;
    Value* value_walker = NULL;
    Value* value_temp;

    // loop through all Features in list
    while(feature_walker != NULL)
    {
        feature_temp = feature_walker;                                                                      // alias current Feature
        feature_walker = feature_walker->next;                                                              // traverse Feature list
        value_walker = feature_temp->values_head;                                                           // find Value head of Feature

        // loop through all Features in list
        while(value_walker != NULL)
        {
            value_temp = value_walker;                                                                      // alias current Value
            value_walker = value_walker->next;                                                              // traverse Value list

            // free Value
            free(value_temp);
        }

        // set head and tail to of Values of Feature to NULL
        feature_temp->values_head = NULL;
        feature_temp->values_tail = NULL;

        // free Feature name
        free(feature_temp->feature_name);

        // free Feature
        free(feature_temp);            
    }

    // set head and tail to NULL
    features_head = NULL;
    features_tail = NULL;

    num_features = 0;                                                                                       // reset number of Features
}

// add Feature
void add_feature(char* feature_name)
{
    // if there are no Features yet
    if(num_features == 0)
    {
        features_head = (Feature*)malloc(sizeof(Feature));                                                  // allocate new Feature
        features_head->feature_name = (char*)malloc(sizeof(char*) * (strlen(feature_name)));                // allocate Feature string
        strcpy(features_head->feature_name, feature_name);                                                  // copy feature name to Feature
        features_head->num_values = 0;                                                                      // initialize number of Values of this Feature
        features_head->next = NULL;                                                                         // set next Feature pointer to NULL
        features_tail = features_head;                                                                      // set tail of Features list to head
        num_features++;                                                                                     // increment number of Features
    }   
    // if Feature has been added
    else
    {
        features_tail->next = (Feature*)malloc(sizeof(Feature));                                            // allocate new Feature and attach to tail of Features
        features_tail = features_tail->next;                                                                // set tail of Features list to new Feature
        features_tail->feature_name = (char*)malloc(sizeof(char*) * strlen(feature_name));                  // allocate Feature string
        strcpy(features_tail->feature_name, feature_name);                                                  // copy feature name to Feature
        features_tail->num_values = 0;                                                                      // initialize number of Values of this Feature
        features_tail->next = NULL;                                                                         // set next Feature of new Feature to NULL
        num_features++;                                                                                     // increment number of Features
    }
}

// add Value
void add_value(Feature* feature, float value, Class* class)
{
    // if are are no Values yet
    if(feature->num_values == 0)
    {
        feature->values_head = (Value*)malloc(sizeof(Value));                                               // allocate new Value
        feature->values_head->value = value;                                                                // set value
        feature->values_head->class = class;                                                                // set Class
        feature->values_head->next = NULL;                                                                  // set next Value pointer to NULL
        feature->values_tail = feature->values_head;                                                        // set tail of Values list to head
        feature->num_values++;                                                                              // increment number of Values
    }
    // if Value has been added
    else
    {
        feature->values_tail->next = (Value*)malloc(sizeof(Value));                                         // allocate new Value and attach to tail of Values
        feature->values_tail = feature->values_tail->next;                                                  // set tail of Values list to new Value
        feature->values_tail->value = value;                                                                // set value
        feature->values_tail->class = class;                                                                // set Class
        feature->values_tail->next = NULL;                                                                  // set next Value of new Value to NULL
        feature->num_values++;                                                                              // increment number of Values
    }
}

// get Features head
Feature* get_features_head(void)
{
    return features_head;                                                                                   // return Features head
}

// get Features tail
Feature* get_features_tail(void)
{
    return features_tail;                                                                                   // return Features tail
}

// get Values head
Value* get_values_head(Feature* feature)
{
    return feature->values_head;                                                                            // return Values head
}

// get Values tail
Value* get_values_tail(Feature* feature)
{
    return feature->values_tail;                                                                            // return Values tail
}

// get number of Features
int get_num_features(void)
{
    return num_features;                                                                                    // return number of Features
}

// get number of Values
int get_num_values(Feature* feature)
{
    return feature->num_values;                                                                             // return number of Values
}

// print contents of Features linked list
void print_features(void)
{
    // initialize variables
    Feature* walker = features_head;

    printf("FEATURES: ");                                                                                   // print "FEATURES: "

    // loop through all Features in list
    while(walker->next)
    {
        printf("%s,", walker->feature_name);                                                                // print Feature name
        walker = walker->next;                                                                              // traverse Feature list                
    }

    printf("%s\n", walker->feature_name);                                                                   // print last Feature name
}

// print contents of Values linked list
void print_values(Feature* feature)
{
    // initialize variables
    Value* walker = feature->values_head;

    printf("VALUES: ");                                                                                     // print "VALUES: "

    // loop through Features list
    while(walker->next)
    {
        printf("%f,", walker->value);                                                                       // print Value
        walker = walker->next;                                                                              // traverse Value list                
    }

    printf("%f\n", walker->value);                                                                          // print last Value
} 
