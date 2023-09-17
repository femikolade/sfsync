#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char *config(char *setting)
{
    FILE * fp;
    char * line = NULL;
    size_t len = 0;
    ssize_t read;
    char *value = (char*) malloc(100);
    char *key = (char*) malloc(100);
    strcpy(value, "error");

    fp = fopen("config", "r");
    if (fp == NULL)
        exit(EXIT_FAILURE);

    while ((read = getline(&line, &len, fp)) != -1){
        line[strcspn(line, "\n")] = '\0';
        
        key = strtok (line, " ");
        if (strcmp(key, setting) == 0){
            while(key != NULL){
                if(strcmp(key, "") != 0)
                    strcpy(value, key);
                
                key = strtok(NULL, "");
            }
        }
    }

    fclose(fp);
    if (line)
        free(line);
    
    return value;
}