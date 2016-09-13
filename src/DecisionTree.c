//
 //  DecisionTree.c
 //
 //  Created on: October 5, 2015
 //      Author: John Cabaj
//

#include <stdio.h>
#include "ARFF.h"
#include "Classes.h"
#include "Features.h"

// private functions
void init(void);                                                                                            // initialize parameters
void deinit(void);                                                                                          // deinitialize parameters        

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
                if(class_walker->num_examples == get_total_examples())
                {
                    printf("%s\n", class_walker->class_name);
                    return 0;                                                                               // return 0
                }

                class_walker = class_walker->next;                                                          // traverse Class list            
            }
        }
        printf("%d\n", get_total_examples());
    }

    // deinitialize parameters
    deinit();

    // close training and testing files
    fclose(training_file);
    fclose(testing_file);
    
    return 0;                                                                                               // return 0
}

// initialize parameters
void init(void)
{
    // initialize ARFF reader
    init_arff();

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
