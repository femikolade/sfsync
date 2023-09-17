#include <libssh/libssh.h>
#include <stdlib.h>
#include <string.h>
#include "config.c"

ssh_session connect() {

    char *h = config("host");
    char host[strlen(h)];
    strcpy(host, h);
    char * p = config("port");
    int port = atoi(p);
    int rc;

    ssh_session my_ssh_session = ssh_new();
    int verbosity = SSH_LOG_PROTOCOL;
    if (my_ssh_session == NULL)
        exit(-1);
    
    ssh_options_set(my_ssh_session, SSH_OPTIONS_HOST, host);
    ssh_options_set(my_ssh_session, SSH_OPTIONS_LOG_VERBOSITY, &verbosity);
    ssh_options_set(my_ssh_session, SSH_OPTIONS_PORT, &port);

    rc = ssh_connect(my_ssh_session);
    if (rc != SSH_OK)
    {
        fprintf(stderr, "Error connecting to %s: %s\n",
            host, ssh_get_error(my_ssh_session));
        exit(-1);
    }

    return my_ssh_session;
}
