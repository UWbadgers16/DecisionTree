//
 //  DecisionTree.c
 //
 //  Created on: October 5, 2015
 //      Author: John Cabaj
//

#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "Classes.h"
#include "Features.h"

// private functions
uint8_t read_training_file(FILE* training_file);                                                            // read training file
void init(void);                                                                                            // initialize parameters
void deinit(void);                                                                                          // deinitialize parameters
char* string_uppercase(char* input);                                                                        // return uppercase of a string

static int total_examples = 0;                                                                              // total number of examples            

int main(int argc, char** argv) 
{
    // initialize variables
    FILE* training_file;
    FILE* testing_file;
    Class* class_walker = NULL;
    int total_instances = 0;

    // if there aren't enough input arguments
    if(argc < 2)
    {
        printf("usage: decision_tree training_file testing_file\n");                                        // print usage statement
        return 1;                                                                                           // return 1
    }
    // if there are enough input arguments
    else
    {
        // initialize parameters
        init();

        // open training file
        training_file = fopen(argv[1], "r");

        // if no training file was found
        if (!training_file)
        {
            // notify that training file was not found and return
            printf("TRAINING FILE NOT FOUND\n");
            return 1;                                                                                       // return 1
        }

        // open testing file
        testing_file = fopen(argv[2], "r");

        // if no testing file was found
        if (!testing_file)
        {
            // notify that testing file was not found and return
            printf("TESTING FILE NOT FOUND\n");
            return 1;                                                                                       // return 1
        }

        // if reading training file returned error
        if(read_training_file(training_file) == 1)
        {
            return 1;                                                                                       // return 1
        }
        // if there are no classes
        else if(get_num_classes() == 0)
        {
            // notify no classes
            printf("NO CLASSES\n");
            return 1;                                                                                       // return 1
        }
        else
        {
            // print list of classes
            print_classes();

            class_walker = get_classes_head();                                                              // get Classes list head

            // loop through all Classes in list
            while(class_walker)
            {
                // if one Class contains all examples
                if(class_walker->num_examples == total_examples)
                {
                    printf("%s\n", class_walker->class_name);
                    return 0;                                                                               // return 0
                }

                class_walker = class_walker->next;                                                          // traverse Class list            
            }
        }
    }

    // deinitialize parameters
    deinit();

    // close training and testing files
    fclose(training_file);
    fclose(testing_file);
    
    return 0;                                                                                               // return 0
}

// read training file
uint8_t read_training_file(FILE* training_file)
{
    // initialize variables
    char line[1024];
    char* header;
    char* attribute;
    char* type;
    char* class_name;
    char* value;
    float numeric_value;
    int i, value_count = 0;
    uint8_t reading_data = 0;                                                                               // flag indicating that data is being read (default to 0)
    Feature* features_walker = NULL;
    Class* class = NULL;
    
    // loop through all lines of training file
    while (fgets(line, 1024, training_file) != NULL)
    {
        // if not an empty line
        if(strcmp(line, "\n") != 0)
        {
            // if reading header section
            if(!reading_data)
            {
                header = strtok(line, " ");                                                                 // read header label

                // if line contains an attribute
                if(strcmp(string_uppercase(header), "@ATTRIBUTE") == 0)
                {
                    // get attribute
                    attribute = strtok(NULL, " ");

                    // if attribute is not class
                    if(strcmp(string_uppercase(attribute), "CLASS") != 0)
                    {
                        // get attribute type
                        type = strtok(NULL, " \n");

                        // if attribute is of type numeric
                        if(strcmp(string_uppercase(type), "NUMERIC") == 0)
                        {
                            // add Feature
                            add_feature(attribute);
                        }
                        // if attribute is not of type numeric
                        else
                        {
                            // notify that numeric attributes must be used
                            printf("MUST USE NUMERIC ATTRIBUTES\n");
                            return 1;                                                                       // return 1
                        }
                    }
                    // if attribute is class
                    else
                    {
                        // loop through classes
                        while(class_name = strtok(NULL, "{,}\n"))
                        {
                            // add Class
                            add_class(class_name);
                        }
                    }
                }
                // if line contains data tag
                else if(strstr(string_uppercase(header), "@DATA\n") != NULL)
                {
                    reading_data = 1;                                                                       // set flag indicating that data is being read
                }
            }
            // if reading data section
            else
            {
                value_count = 0;                                                                            // set delimiter count to 0
                features_walker = get_features_head();                                                      // get Features head
                class = find_class(strrchr(strtok(line, "\n"), ',')+1);                                     // get Class
                class->num_examples++;                                                                      // increment number of examples of class
                total_examples++;                                                                           // increment total number of examples
                
                // if there is a comma delimiter
                if(value = strtok(line, ","))
                {
                    // if value numeric  
                    if(sscanf(value, "%f", &numeric_value))
                    {
                        // if Feature is exists
                        if(features_walker)
                        {
                            // add feature value
                            add_value(features_walker, numeric_value, class);
                            features_walker = features_walker->next;                                        // traverse Feature list
                            value_count++;                                                                  // increment feature value count
                        }
                        // if Feature doesn't exist
                        else
                        {
                            // notify that there are too many feature values
                            printf("TOO MANY FEATURE VALUES\n");
                            return 1;                                                                       // return 1
                        } 
                    }                                       
                }

                // loop through features
                while(value = strtok(NULL, ","))
                {
                    // if value numeric  
                    if(sscanf(value, "%f", &numeric_value))
                    {
                        // if Feature exists
                        if(features_walker)
                        {
                            // add feature value
                            add_value(features_walker, numeric_value, class);
                            features_walker = features_walker->next;                                        // traverse Feature list
                            value_count++;                                                                  // increment feature value count
                        }
                        // if Feature doesn't exist
                        else
                        {
                            // notify that there are too many feature values
                            printf("TOO MANY FEATURE VALUES\n");
                            return 1;                                                                       // return 1
                        }
                    }
                }

                // if number of feature values doesn't equal number of features
                if(value_count  != get_num_features())
                {
                    // notify that there are too few feature values
                    printf("TOO FEW FEATURE VALUES\n");
                    return 1;                                                                               // return 1
                }
            }
        }
    }

    return 0;                                                                                               // return 0
}

// initialize parameters
void init(void)
{
    total_examples = 0;                                                                                     // reset total number of examples

    // initialize Classes
    init_classes();

    // initialize Features
    init_features();
}

// deinitialize parameters
void deinit(void)
{
    // deinitialize Classes
    deinit_classes();

    // deinitialize Features
    deinit_features();
}

// return uppercase of a string
char* string_uppercase(char* input)
{
    // initialize variables
    int i;

    // loop through string
    for(i = 0; input[i]; i++)
    {
        input[i] = toupper(input[i]);                                                                       // switch character to lowercase
    }

    // return lowercase string
    return input;
}
