#include <asm/uaccess.h>
#include <linux/types.h>
#include <linux/slab.h>
#include <cmd/def.h>
#include <cmd/protos.h>
#include <cmd/work.h>
#include <linux/module.h>
#include <linux/kobject.h>
#include <linux/pid.h>
#include <linux/sched.h>

#define TRUE 1
/**
 * gives information about a module
 */
int cmd_wait_handler(struct cmd_wait_args *args, struct cmd_wait_res *res){

        struct task_struct **tasks;
        int i = 0;
        tasks = kmalloc(sizeof(struct task_struct*),GFP_KERNEL);
        
        for(i=0;i<args->nbPid;i++){
        
               
               tasks[i] = pid_task(find_vpid(args->PIDS[i]),PIDTYPE_PID);
               if(tasks[i]==NULL){
                      return -1;
               }
        }
       
        i=0;
        while(TRUE){
                if(i == args->nbPid){
                          i = 0;
                }
                if(tasks[i]->state > 0)
                        break;
                i++;
        }
       
        res->valRet = tasks[i]->exit_code;
        res->PID = (int) tasks[i]->pid;
        kfree(tasks);
     
        return 0;
}
