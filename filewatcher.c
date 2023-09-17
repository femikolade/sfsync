#include <libssh/libssh.h>

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/inotify.h>
#include <unistd.h>
#include <string.h>
#include "connect.c"
#include "auth.c"
#include "upload-file.c" 
#include "file-path.c"

#define EVENT_SIZE  ( sizeof (struct inotify_event) )
#define BUF_LEN     ( 1024 * ( EVENT_SIZE + 16 ) )

int filewatcher(char *local_dir, char *remote_dir)
{
    int length, i = 0;
    int fd;
    int wd;
    char buffer[BUF_LEN];
    
    fd = inotify_init();

    if ( fd < 0 ) {
        perror( "inotify_init" );
    }

    wd = inotify_add_watch( fd, local_dir, 
                            IN_MODIFY | IN_CREATE | IN_DELETE );
    length = read( fd, buffer, BUF_LEN );

    if ( length < 0 ) {
        perror( "read" );
    }

    while(true){
        while ( i < length ) {
            struct inotify_event *event = ( struct inotify_event * ) &buffer[ i ];
            if ( event->len ) {
                char *local_file_path = file_path(local_dir, event->name, "/");
                char *remote_file_path = file_path(remote_dir, event->name, "/");

                ssh_session session = connect();
                auth(session);
                
                if ( event->mask & IN_MODIFY ) {
                    printf( "modified: upload '%s'.\n", event->name );
                    scp_copy(session, local_dir, remote_dir, event->name);
                }
                // free(local_file_path);
                // free(remote_file_path);

            }
            i += EVENT_SIZE + event->len;
        }
        i = 0;
        length = read( fd, buffer, BUF_LEN );

        // ( void ) inotify_rm_watch( fd, wd );
        // ( void ) close( fd );

    // exit( 0 );
    }
}