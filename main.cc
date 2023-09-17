#include <libssh/libssh.h>

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/inotify.h>
#include <unistd.h>
#include "filewatcher.c"
// #include "config.c"

#define EVENT_SIZE  ( sizeof (struct inotify_event) )
#define BUF_LEN     ( 1024 * ( EVENT_SIZE + 16 ) )

int main( int argc, char **argv )
{
    char *remote_dir = config("remote_dir");
    char *local_dir = config("local_dir");

    filewatcher(local_dir, remote_dir);

    return 0;
}
