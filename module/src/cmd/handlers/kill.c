#include <linux/uaccess.h>
#include <linux/types.h>
#include <linux/pid.h>
#include <cmd/def.h>
#include <cmd/protos.h>
#include <cmd/work.h>

/**
 * kill a process
 */
int cmd_kill_handler(struct cmd_kill_args *args, struct cmd_kill_res *res){

        struct pid *p;
        pr_debug("begin of cmd_modinfo_handler\n");
        p = find_get_pid(args->pid);
        if (!p) {
                pr_debug("[KILL] PID NOT FOUND \n");
                goto ending;
        }
        kill_pid(p, args->sig, 1);
        put_pid(p);
        res->status = 0;
        return 0;
ending:
        res->status = -1;
        return -1;
}

