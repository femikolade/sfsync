#include <libssh/libssh.h>
#include <stdlib.h>
#include <string.h>
#include "verify_knownhost.c"

int auth(ssh_session my_ssh_session) {

    char *password;
    int rc;

    // Verify the server's identity
    // For the source code of verify_knownhost(), check previous example
    if (verify_knownhost(my_ssh_session) < 0)
    {
        ssh_disconnect(my_ssh_session);
        ssh_free(my_ssh_session);
        exit(-1);
    }

    // Authenticate ourselves
    password = getpass("Password: ");
    rc = ssh_userauth_password(my_ssh_session, NULL, password);
    if (rc != SSH_AUTH_SUCCESS)
    {
        fprintf(stderr, "Error authenticating with password: %s\n",
                ssh_get_error(my_ssh_session));
        ssh_disconnect(my_ssh_session);
        ssh_free(my_ssh_session);
        exit(-1);
    }

    return 0;
}