#include <libssh/libssh.h>
#include <stdio.h>
#include <fcntl.h>
#include <string.h>

ssh_scp scp_copy_init(ssh_session session, char *remote_loc)
{
    ssh_scp scp;
    int rc;
    scp = ssh_scp_new
        (session, SSH_SCP_WRITE, remote_loc);
    if (scp == NULL)
    {
        fprintf(stderr, "Error allocating scp session: %s\n",
                ssh_get_error(session));
        // return SSH_ERROR;
    }
    rc = ssh_scp_init(scp);
    if (rc != SSH_OK)
    {
        fprintf(stderr, "Error initializing scp session: %s\n",
                ssh_get_error(session));
        ssh_scp_free(scp);
        // return rc;
    }

    return scp;
//   ssh_scp_close(scp);
//   ssh_scp_free(scp);
//   return SSH_OK;
}

unsigned long fsize(char* file)
{
    FILE * f = fopen(file, "r");
    fseek(f, 0, SEEK_END);
    unsigned long len = (unsigned long)ftell(f);
    fclose(f);
    return len;
}

char *full_file_path(char *dir, char *filename) {
    int str_length = strlen(dir) + strlen(filename) + 1;
    char *source;
    strcpy(source, dir);
    strcat(source, "/");
    strcat(source, filename);

    return source;
}

void write_file(ssh_scp scp, char *src, int file_size, int rc){
    int src_fd, n, err;
    unsigned char buffer[file_size];
    src_fd = open(src, O_RDONLY);
    while (err = read(src_fd, buffer, file_size)) {
        if (err == -1) {
            printf("Error reading file.\n");
            exit(1);
        }
        n = err;

        if (n == 0) break;

        rc += ssh_scp_write(scp, buffer, file_size);
    }

    close(src_fd);
}

int scp_copy(ssh_session session, char *src, char *dest, char *filename)
{
    int str_length = strlen(src) + strlen(filename) + 1;
    char *source = full_file_path(src, filename);
    ssh_scp scp = scp_copy_init(session, dest);

    int rc;
    int file_size = fsize(source);

    rc = ssh_scp_push_file
        (scp, filename, file_size, S_IRUSR |  S_IWUSR);
    if (rc != SSH_OK)
    {
        fprintf(stderr, "Can't open remote file: %s\n",
                ssh_get_error(session));
        return rc;
    }

    write_file(scp, source, file_size, rc);

    if (rc != SSH_OK)
    {
        fprintf(stderr, "Can't write to remote file: %s\n",
                ssh_get_error(session));
        return rc;
    }

    // free(source);
    ssh_scp_close(scp);
    ssh_scp_free(scp);
    return SSH_OK;
}