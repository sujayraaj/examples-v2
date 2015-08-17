#include <monkey/monkey.h>

#include "rtems_filesystem.h"
#include <monkey/monkey.h>
#include <rtems/imfs.h>


int filesystem_initialization(){
#define TARFILE_START Filesys
#define TARFILE_SIZE  Filesys_size
 printf("Untaring from memory - ");
struct stat sb;
int  sc = Untar_FromMemory((void *)TARFILE_START, TARFILE_SIZE);
  if (sc != RTEMS_SUCCESSFUL) {
    printf ("error: untar failed: %s\n", rtems_status_text (sc));
    exit(1);
  }
  printf ("successful\n");
}

int http_init()
{
mk_config = mk_server_init();
mk_config->server_conf_file = MK_DEFAULT_CONFIG_FILE;
mk_config->path_config = MK_PATH_CONF;
mk_config->sites_conf_dir = MK_DEFAULT_SITES_CONF_DIR;
mk_config->mimes_conf_file = MK_DEFAULT_MIMES_CONF_FILE;
mk_server_setup();
mk_server_loop();
return 0;
}

