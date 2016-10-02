//
 //  ARFF.c
 //
 //  Created on: September 12, 2016
 //      Author: John Cabaj
//

#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <ctype.h>
#include "Classes.h"
#include "Features.h"

char* string_uppercase(char* input);                                                                        // return uppercase of string
char* remove_whitespace(char* input);                                                                       // return string without whitespace
char* remove_leading_whitespace(char* input);                                                               // return string without leading whitespace
uint8_t check_double_quotes(char* string);                                                                  // check string for double quotes

static char* relation_name = NULL;                                                                          // name of relation
static int total_examples = 0;                                                                              // total number of examples

// init ARFF reader
void init_arff(void)
{
    total_examples = 0;                                                                                     // reset total number of examples  
}

// deinitialize ARFF
void deinit_arff(void)
{
    // free relation name
    free(relation_name);
}

// read training file
uint8_t read_training_file(FILE* training_file)
{
    // initialize variables
    char line[1024];
    char* arff_line;
    uint8_t reading_data= 0;                                                                                // flag indicating that data is being read (default to 0)
    
    // loop through all lines of training file
    while (fgets(line, 1024, training_file) != NULL)
    {
        arff_line = strtok(line, "\r\n");                                                                   // get file line

        // if line exists
        if(arff_line != NULL)
        {
            arff_line = remove_leading_whitespace(arff_line);                                               // remove leading whitespace from file line

            // if not comment
            if(arff_line[0] != '%')
            {
                // if reading header section
                if(!reading_data)
                {
                    char* header = strtok(arff_line, " ");                                                  // read header label
                    char* null_term;

                    // if line contains relation
                    if(strcmp(string_uppercase(header), "@RELATION") == 0)
                    {
                        char* relation = strtok(NULL, "\0");

                        if(check_double_quotes(relation))
                        {
                            relation = strtok(relation, "\"");                                              // get relation
                        }

                        relation_name = (char*)malloc(sizeof(char*) * (strlen(relation)));                  // allocate relation name string
                        strcpy(relation_name, relation);                                                    // copy relation to relation name
                    }
                    // if line contains an attribute
                    else if(strcmp(string_uppercase(header), "@ATTRIBUTE") == 0)
                    {
                        char* feature;                                                                      // feature name
                        char* type;                                                                         // type of feature

                        // get rest of line
                        char* attribute = strtok(NULL, "\0");

                        // if attributes has double quotes
                        if(check_double_quotes(attribute))
                        {
                            feature = strtok(attribute, "\"");                                              // get feature name
                        }
                        // if attribute doesn't have double quotes
                        else
                        {
                            feature = strtok(attribute, " ");                                               // get feature name
                        }

                        // if attribute is not class
                        if(strcmp(string_uppercase(feature), "CLASS") != 0)
                        {
                            // get attribute type
                            type = strtok(NULL, " ");

                            // if attribute is of type numeric
                            if(strcmp(string_uppercase(type), "NUMERIC") == 0)
                            {
                                // add Feature
                                add_feature(feature);
                            }
                            // if attribute is not of type numeric
                            else
                            {
                                // notify that numeric attributes must be used
                                printf("MUST USE NUMERIC ATTRIBUTES\n");
                                return 1;                                                                   // return 1
                            }
                        }
                        // if attribute is class
                        else
                        {
                            char* class_names;                                                              // class names
                            char* class_name;                                                               // class name

                            class_names = strtok(NULL, "{}");                                               // get class names
                            class_name = strtok(class_names, " ,");                                         // get first class name

                            // loop through classes
                            while(class_name)
                            {
                                // add Class
                                add_class(class_name);

                                class_name = strtok(NULL, " ,");                                            // get next class name        
                            }
                        }
                    }
                    // if line contains dattag
                    else if(strstr(string_uppercase(header), "@DATA") != NULL)
                    {
                        reading_data= 1;                                                                    // set flag indicating that data is being read
                    }
                }
                // if reading data section
                else
                {
                    char* value;                                                                            // feature value string
                    float numeric_value;                                                                    // feature numerical value
                    int value_count = 0;                                                                    // set delimiter count to 0
                    Feature* features_walker = NULL;                                                        // Feature node to walk Features list 
                    Class* class = NULL;                                                                    // Class node
                    features_walker = get_features_head();                                                  // get Features head
                    class = find_class(strrchr(remove_whitespace(strtok(line, "\r\n")), ',')+1);            // get Class
                    class->num_examples++;                                                                  // increment number of examples of class
                    total_examples++;                                                                       // increment total number of examples
                    
                    // if there is comma delimiter
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
                                features_walker = features_walker->next;                                    // traverse Feature list
                                value_count++;                                                              // increment feature value count
                            }
                            // if Feature doesn't exist
                            else
                            {
                                // notify that there are too many feature values
                                printf("TOO MANY FEATURE VALUES\n");
                                return 1;                                                                   // return 1
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
                                features_walker = features_walker->next;                                    // traverse Feature list
                                value_count++;                                                              // increment feature value count
                            }
                            // if Feature doesn't exist
                            else
                            {
                                // notify that there are too many feature values
                                printf("TOO MANY FEATURE VALUES\n");
                                return 1;                                                                   // return 1
                            }
                        }
                    }

                    // if number of feature values doesn't equal number of features
                    if(value_count  != get_num_features())
                    {
                        // notify that there are too few feature values
                        printf("TOO FEW FEATURE VALUES\n");
                        return 1;                                                                           // return 1
                    }
                }
            }
        }
    }

    return 0;                                                                                               // return 0
}

// get relation name
char* print_relation(void)
{
    printf("RELATION: %s\n", relation_name);                                                                // print relation name
}

// get total number of examples
int get_total_examples(void)
{
    return total_examples;                                                                                  // return total number of examples
}

// return uppercase of string
char* string_uppercase(char* input)
{
    // initialize variables
    int i;

    // loop through string
    for(i = 0; input[i]; i++)
    {
        input[i] = toupper(input[i]);                                                                       // switch character to uppercase
    }

    // return uppercase string
    return input;
}

// return string without whitespace
char* remove_whitespace(char* input)
{
    // initialize variables
    char* no_whitespace = input;
    int i, j = 0;

    // loop through string
    for(i = 0; input[i]; i++)
    {
        // if character is not space
        if(!isspace(input[i]))
        {
            no_whitespace[j] = input[i];                                                                    // copy character to non-whitespace string if not space
            j++;                                                                                            // increment non-whitespace string index
        }
    }

    no_whitespace[j] = '\0';                                                                                // add null terminator to non-whitespace string
    return no_whitespace;                                                                                   // return non-whitespace string
}

// return string without leading whitespace
char* remove_leading_whitespace(char* input)
{
    // initialize variables
    int i;

    // loop through string
    for(i = 0; input[i]; i++)
    {
        // if character is not space
        if(!isspace(input[i]))
        {
            input = &input[i];                                                                              // set string beginning to first occurence of non-whitespace
            return input;                                                                                   // return input if there was leading whitespace
        }
    }

    return input;                                                                                           // return input if no leading whitespace
}

// check string for double quotes
uint8_t check_double_quotes(char* string)
{
    // check for first quotation mark
    char* first_quote = strchr(string, '\"');

    // if there is one quotation mark
    if(first_quote != NULL)
    {
        // check for second quotation mark
        char* second_quote = strchr(&first_quote[1], '\"');

        // if there are two quotation marks
        if(second_quote != NULL)
        {
            return 1;                                                                                       // return double quotes
        }
    }

    return 0;                                                                                               // return no double quotes
}
