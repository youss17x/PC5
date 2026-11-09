#include "pc5_library.h"
#include "pc5_logs.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
int main(void) {
 char root[]="/tmp/pc5-diagnostics-test-XXXXXX", path[PC5_PATH];
 assert(mkdtemp(root));
 assert(!pc5_diagnostics_enabled(root,"sample"));
 assert(!pc5_diagnostics_set(root,"sample",1));
 assert(pc5_diagnostics_enabled(root,"sample"));
 assert(!pc5_diagnostics_enabled(root,"other"));
 assert(pc5_diagnostics_set(root,"../escape",1)==-1);
 assert(pc5_diagnostics_set(root,"",1)==-1);
 assert(!pc5_diagnostics_set(root,"sample",0));
 assert(!pc5_diagnostics_enabled(root,"sample"));
 assert(!pc5_diagnostics_set(root,"sample",0));
 snprintf(path,sizeof(path),"%s/prefixes/sample/pc5/diagnostics.txt",root);
 assert(!pc5_write_file(path,"PC5_DIAGNOSTICS_2\n",18));
 assert(!pc5_diagnostics_enabled(root,"sample"));
 Pc5Buf notes={0},input={0};
 for(int i=0;i<100;i++) pc5_buf_puts(&input,"REC WINE wine-ps5: exception registers rbx=6fffffbc003c address=6fffffbc003c\n");
 pc5_log_notes((char*)input.data,input.len,&notes);assert(!pc5_buf_terminate(&notes));
 assert(strstr((char*)notes.data,"not a proven root cause"));
 char *p=(char*)notes.data;int n=0;while((p=strstr(p,"registers rbx="))){n++;p++;}assert(n==8);
 assert(notes.len<8192);
 pc5_buf_free(&notes);pc5_buf_free(&input);
 unlink(path);snprintf(path,sizeof(path),"%s/prefixes/sample/pc5",root);rmdir(path);
 snprintf(path,sizeof(path),"%s/prefixes/sample",root);rmdir(path);
 snprintf(path,sizeof(path),"%s/prefixes",root);rmdir(path);rmdir(root);
 puts("PASS per-game opt-in, disable, isolation, invalid IDs, version validation, bounded diagnostic notes");
}
