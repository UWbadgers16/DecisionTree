//
 //  Classes.c
 //
 //  Created on: March 22, 2016
 //      Author: John Cabaj
//

#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include "Classes.h"

static Class* classes_head = NULL;                                                                          // initialize head of list of Classes
static Class* classes_tail = NULL;                                                                          // initialize tail of list of Classes
static int num_classes = 0;                                                                                 //  number of Classes

// initialize Class
void init_classes(void)
{
    num_classes = 0;                                                                                        // reset number of Classes
}

// deinitialize Class
void deinit_classes(void)
{
    // initialize variables
    Class* walker = classes_head;
    Class* temp;

    // loop through all Classes in list
    while(walker != NULL)
    {
        // free node
        temp = walker;
        walker = walker->next;
        free(temp->class_name);
        free(temp);            
    }

    // set head and tail to NULL
    classes_head = NULL;
    classes_tail = NULL;

    num_classes = 0;                                                                                        // reset number of Classes
}

// add a Class
void add_class(char* class_name)
{
    // if there are no Classes yet
    if(num_classes == 0)
    {
        classes_head = (Class*)malloc(sizeof(Class));                                                       // allocate a new Class
        classes_head->class_name = (char*)malloc(sizeof(char*) * (strlen(class_name)));                     // allocate a Class string
        strcpy(classes_head->class_name, class_name);                                                       // copy class name to Class
        classes_head->num_examples = 0;                                                                     // set number of examples of class to 0
        classes_head->next = NULL;                                                                          // set next Class pointer to NULL
        classes_tail = classes_head;                                                                        // set tail of Classes list to head
        num_classes++;                                                                                      // increment number of Classes
    }   
    // if a Class has been added
    else
    {
        classes_tail->next = (Class*)malloc(sizeof(Class));                                                 // allocate a new Class and attach to tail of Classes
        classes_tail = classes_tail->next;                                                                  // set tail of Classes list to new Class
        classes_tail->class_name = (char*)malloc(sizeof(char*) * (strlen(class_name)));                     // allocate a Class string
        strcpy(classes_tail->class_name, class_name);                                                       // copy class name to Class
        classes_tail->num_examples = 0;                                                                     // set number of examples of class to 0
        classes_tail->next = NULL;                                                                          // set next Class of new Class to NULL
        num_classes++;                                                                                      // increment number of Classes
    }
}

// get Classes head
Class* get_classes_head(void)
{
    return classes_head;                                                                                    // return Classes head
}

// get Classes tail
Class* get_classes_tail(void)
{
    return classes_tail;                                                                                    // return Classes tail
}

// get number of classes
int get_num_classes(void)
{
    return num_classes;                                                                                     // return number of Classes
}

// return Class matching input string
Class* find_class(char* class_name)
{
    // initialize variables
    Class* walker = classes_head;

    // loop through all Classes in list
    while(walker)
    {
        // if Class name matches
        if(strcmp(walker->class_name, class_name) == 0)
        {
            return walker;                                                                                  // return reference to Class
        }

        walker = walker->next;                                                                              // traverse Class list             
    }

    return NULL;                                                                                            // return NULL   
}

// print contents of Classes linked list
void print_classes(void)
{
    // initialize variables
    Class* walker = classes_head;

    printf("CLASSES: ");                                                                                    // print "CLASSES"

    // loop through all Classes in list
    while(walker)
    {
        printf("%s ", walker->class_name);                                                                  // print Class name
        walker = walker->next;                                                                              // traverse Class list            
    }

    printf("\n");                                                                                           // print new line
} 
