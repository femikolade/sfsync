#include <libssh/libssh.h>
#include <string.h>

char *file_path(char *dir, char *file, char *separator) {

    char *full_path;
    
    int str_length = strlen(dir) + strlen(file) + 3;
    char dest[str_length];
    strcpy(dest, dir);
    strcat(dest, separator);
    strcat(dest, file);
    
    return full_path;
}

char *config_values(char *setting)
{
    FILE * fp;
    char * line = NULL;
    size_t len = 0;
    ssize_t read;
    char *value = (char*) malloc(100);
    char *key = (char*) malloc(100);
    strcpy(value, "error");

    fp = fopen("config.conf", "r");
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