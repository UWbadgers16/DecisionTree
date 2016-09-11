//
 //  Classes.h
 //
 //  Created on: March 22, 2016
 //      Author: John Cabaj
//

#ifndef CLASSES_H
#define CLASSES_H

#include <stdint.h>
#include <string.h>

// defines a Class linked list node
typedef struct Class
{
    struct Class* next;                                                             // pointer to next node
    char* class_name;                                                               // name of Class
    int num_examples;																// number of examples of Class
} Class;

void init_classes(void);															// initialize Classes
void deinit_classes(void);															// deinitialize Classes
void add_class(char* class_name);                                                   // add a Class
Class* get_classes_head(void);                                                      // get Classes head
Class* get_classes_tail(void);                                                      // get Casses tail
int get_num_classes(void);															// get number of Classes
Class* find_class(char* class_name);												// return Class matching input string
void print_classes(void);                                                           // print contents of Classes linked list

#endif // CLASSES_H