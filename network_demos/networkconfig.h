#ifndef __NETWORK_CONFIG_H__
#define __NETWORK_CONFIG_H__

#include <rtems/malloc.h>
#include <rtems/libcsupport.h>


#include <net/if.h>

#include <assert.h>
#include <fcntl.h>
#include <ifaddrs.h>
#include <stdio.h>
#include <stdlib.h>
#include <sysexits.h>


#include <machine/rtems-bsd-commands.h>

#include <rtems.h>
#include <rtems/stackchk.h>
#include <rtems/bsd/bsd.h>



#include <bsp.h>

#if defined(LIBBSP_ARM_ALTERA_CYCLONE_V_BSP_H)
  #define NET_CFG_INTERFACE_0 "dwc0"
#elif defined(LIBBSP_ARM_REALVIEW_PBX_A9_BSP_H)
  #define NET_CFG_INTERFACE_0 "smc0"
#elif defined(LIBBSP_ARM_XILINX_ZYNQ_BSP_H)
  #define NET_CFG_INTERFACE_0 "cgem0"
#elif defined(__GENMCF548X_BSP_H)
  #define NET_CFG_INTERFACE_0 "fec0"
#else
  #define NET_CFG_INTERFACE_0 "lo0"
#endif

#define NET_CFG_SELF_IP "10.1.2.5"
#define NET_CFG_NETMASK "255.255.255.0"
#define DNET_CFG_PEER_IP "10.66.22.175"
#define NET_CFG_GATEWAY_IP "10.1.1.1"

static void
default_network_set_self_prio(rtems_task_priority prio)
{
	rtems_status_code sc;

	sc = rtems_task_set_priority(RTEMS_SELF, prio, &prio);
	assert(sc == RTEMS_SUCCESSFUL);
}

static void
default_network_ifconfig_lo0(void)
{
	int exit_code;
	char *lo0[] = {
		"ifconfig",
		"lo0",
		"inet",
		"127.0.0.1",
		"netmask",
		"255.255.255.0",
		NULL
	};
	char *lo0_inet6[] = {
		"ifconfig",
		"lo0",
		"inet6",
		"::1",
		"prefixlen",
		"128",
		"alias",
		NULL
	};

	exit_code = rtems_bsd_command_ifconfig(RTEMS_BSD_ARGC(lo0), lo0);
	assert(exit_code == EX_OK);

	exit_code = rtems_bsd_command_ifconfig(RTEMS_BSD_ARGC(lo0_inet6), lo0_inet6);
	assert(exit_code == EX_OK);
}

static void
default_network_ifconfig_hwif0(char *ifname)
{
	int exit_code;
	char *ifcfg[] = {
		"ifconfig",
		ifname,
		"inet",
		NET_CFG_SELF_IP,
		"netmask",
		NET_CFG_NETMASK,
		NULL
	};

	exit_code = rtems_bsd_command_ifconfig(RTEMS_BSD_ARGC(ifcfg), ifcfg);
	assert(exit_code == EX_OK);
}

static void
default_network_route_hwif0(char *ifname)
{
	int exit_code;
	char *dflt_route[] = {
		"route",
		"add",
		"-host",
		NET_CFG_GATEWAY_IP,
		"-iface",
		ifname,
		NULL
	};
	char *dflt_route2[] = {
		"route",
		"add",
		"default",
		NET_CFG_GATEWAY_IP,
		NULL
	};

	exit_code = rtems_bsd_command_route(RTEMS_BSD_ARGC(dflt_route), dflt_route);
	assert(exit_code == EXIT_SUCCESS);

	exit_code = rtems_bsd_command_route(RTEMS_BSD_ARGC(dflt_route2), dflt_route2);
	assert(exit_code == EXIT_SUCCESS);
}

static void
default_network_on_exit(int exit_code, void *arg)
{
	rtems_stack_checker_report_usage_with_plugin(NULL,
	    rtems_printf_plugin);

	if (exit_code == 0) {
		puts("*** END OF SIMULATION ***");
	}
}


#endif
