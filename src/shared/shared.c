#ifndef SHARED
#define SHARED
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdio.h>
//#include "sys/ioctl.h"
//#include "unistd.h"
#include "shared.h"

void print_greeting(void) {
        //struct winsize w;
        //w.ws_col
        //ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
        char *title = " mint REPL ";
        printf("%s", title);
        char *banner = "_";
        char *flavor_text = "enter \"exit\" without quotes to exit the REPL\n";
        int len = (strlen(flavor_text) - strlen(title)) / strlen(banner); 
        for (int i = 0; i < len; i++) {
                printf("%s", banner);
        }
        printf("]\n");
        printf("%s", flavor_text);
}

float lookup_var(VarMap **map, const char* varName) {
        VarMap *temp = (*map);
        while (temp->next != NULL) {
                if (strcmp(temp->varName, varName) == 0) {
                        return temp->value;
                }
                temp = temp->next;
        }

        fprintf(stderr, "Variable not found: %s\n", varName);
        exit(EXIT_FAILURE);
}

void update_var_map(VarMap **map, const char* varName, float value) {
    VarMap *temp = (*map);
    while (temp != NULL) {
        if (strcmp(temp->varName, varName) == 0) {
            temp->value = value;
            return;
        }
        temp = temp->next;
    }
    // If not found, add new variable.
    VarMap* newVar = (VarMap*)malloc(sizeof(VarMap));
    if (!newVar) return; // Handle allocation failure gracefully.
    strcpy(newVar->varName, varName);
    newVar->value = value;
    newVar->next = *map;
    *map = newVar;
}

void printVarMap(VarMap **map) {
    printf("printing var map\n");
    VarMap *temp = (*map);
    while (temp != NULL) {
        printf("varName: %s, value: %f\n", temp->varName, temp->value);
        temp = temp->next;
    }
}

void free_var_map(VarMap **map) {
    VarMap *temp = (*map);
    while (temp != NULL) {
        VarMap *next = temp->next;
        free(temp);
        temp = next;
    }
    *map = NULL;
}

void add_default_vars(VarMap **map) {
        update_var_map(map, "pi", 3.14159);
        update_var_map(map, "e", 2.71828);
        update_var_map(map, "phi", 1.61803);
        update_var_map(map, "tau", 6.28318);
        update_var_map(map, "c", 299792458);
        update_var_map(map, "h", 6.62607015);
        update_var_map(map, "g", 9.80665);
        update_var_map(map, "G", 6.67430);
        update_var_map(map, "N", 6.02214076);
        update_var_map(map, "R", 8.314462618);
        update_var_map(map, "k", 1.380649);
        update_var_map(map, "sigma", 5.670374419);
        update_var_map(map, "epsilon", 8.8541878128);
        update_var_map(map, "mu", 1.2566370614);
        update_var_map(map, "q", 1.602176634);
        update_var_map(map, "m_e", 9.10938356);
        update_var_map(map, "m_p", 1.672621923);
        update_var_map(map, "m_n", 1.674927471);
}

#endif
