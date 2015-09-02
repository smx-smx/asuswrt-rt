/* nettel.c -- NETtel specific functions for the DHCP server */

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <config/autoconf.h>

int commitChanges(void) {
#ifdef CONFIG_USER_FLATFSD_FLATFSD
        char value[16];
        pid_t pid;
        FILE *in;

        /* get the pid of flatfsd */
        if ((in = fopen("/var/run/flatfsd.pid", "r")) == NULL)
                return -1;

        if (fread(value, 1, sizeof(value), in) <= 0) {
                fclose(in);
                return -1;
        }
        fclose(in);

		pid = atoi(value);

        if (pid == 0 || kill(pid, 10) == -1)
                return -1;
#endif /* CONFIG_USER_FLATFSD_FLATFSD */
        return 0;
}
