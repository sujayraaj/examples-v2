/*
 * RTEMS configuration/initialization
 * 
 * This program may be distributed and used for any purpose.
 * I ask only that you:
 *    1. Leave this author information intact.
 *    2. Document any changes you make.
 *
 * W. Eric Norum
 * Saskatchewan Accelerator Laboratory
 * University of Saskatchewan
 * Saskatoon, Saskatchewan, CANADA
 * eric@skatter.usask.ca
 */

#include <bsp.h>

#include <stdio.h>
#include <stdlib.h>
#include <rtems/rtems_bsdnet.h>
#include "../networkconfig.h"

/*
 * RTEMS Startup Task
 */
static void
Init(rtems_task_argument arg)
{
        rtems_status_code sc;
        char *ifname;

        puts("*** NETDEMO TEST ***");

        on_exit(default_network_on_exit, NULL);

        /* Let other tasks run to complete background work */
        default_network_set_self_prio(RTEMS_MAXIMUM_PRIORITY - 1);

        rtems_bsd_initialize();

        ifname = NET_CFG_INTERFACE_0;

        /* Let the callout timer allocate its resources */
        sc = rtems_task_wake_after(2);
        assert(sc == RTEMS_SUCCESSFUL);

        default_network_ifconfig_lo0();
        default_network_ifconfig_hwif0(ifname);
        default_network_route_hwif0(ifname);
        doSocket();
        exit(0);
}

#include <machine/rtems-bsd-sysinit.h>

SYSINIT_NEED_NET_PF_UNIX;
SYSINIT_NEED_NET_IF_LAGG;
SYSINIT_NEED_NET_IF_VLAN;

#include <bsp/nexus-devices.h>

#define CONFIGURE_APPLICATION_NEEDS_CLOCK_DRIVER
#define CONFIGURE_APPLICATION_NEEDS_CONSOLE_DRIVER
#define CONFIGURE_APPLICATION_NEEDS_STUB_DRIVER
#define CONFIGURE_APPLICATION_NEEDS_ZERO_DRIVER
#define CONFIGURE_APPLICATION_NEEDS_LIBBLOCK

#define CONFIGURE_USE_IMFS_AS_BASE_FILESYSTEM

#define CONFIGURE_LIBIO_MAXIMUM_FILE_DESCRIPTORS 512

#define CONFIGURE_MAXIMUM_USER_EXTENSIONS 1

#define CONFIGURE_UNLIMITED_ALLOCATION_SIZE 32
#define CONFIGURE_UNLIMITED_OBJECTS
#define CONFIGURE_UNIFIED_WORK_AREAS

#define CONFIGURE_STACK_CHECKER_ENABLED

#define CONFIGURE_FIFOS_ENABLED
#define CONFIGURE_MAXIMUM_FIFOS 8
#define CONFIGURE_MAXIMUM_PIPES 8

#define CONFIGURE_RTEMS_INIT_TASKS_TABLE

#define CONFIGURE_INIT_TASK_STACK_SIZE (32 * 1024)
#define CONFIGURE_INIT_TASK_INITIAL_MODES RTEMS_DEFAULT_MODES
#define CONFIGURE_INIT_TASK_ATTRIBUTES RTEMS_FLOATING_POINT

#define CONFIGURE_INIT

#include <rtems/confdefs.h>
