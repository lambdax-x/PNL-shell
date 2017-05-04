#include <asm/uaccess.h>
#include <linux/types.h>
#include <linux/slab.h>
#include <common/def.h>
#include <cmd/protos.h>
#include <linux/module.h>
#include <linux/kobject.h>
#include <linux/mm.h>
#include <linux/swap.h>

#define K(x) ((x) << (PAGE_SHIFT - 10))
#define si_swapinfo(val) \
 do { (val)->freeswap = (val)->totalswap = 0; } while (0)
/**
 * information about system
 */
int cmd_meminfo_handler(struct cmd_meminfo_args *args, struct cmd_meminfo_res *res){

        struct sysinfo *tmp;
        tmp = kmalloc(sizeof(struct sysinfo),GFP_KERNEL);
        //args->info
	si_meminfo(tmp);
	si_swapinfo(tmp);
	
	/* convert to KB*/ 
	tmp->totalram = K(tmp->totalram);
	tmp->freeram = K(tmp->freeram);
	tmp->bufferram = K(tmp->bufferram);
	tmp->totalhigh = K(tmp->totalhigh);
	tmp->freehigh = K(tmp->freehigh);
	tmp->totalswap = K(tmp->totalswap);
	tmp->freeswap = K(tmp->freeswap);
	tmp->sharedram = K(tmp->sharedram);
	tmp->mem_unit = K(tmp->mem_unit);
	
	//si_swapinfo(args->info);
	res->val = 0;
	
	if(copy_to_user(args->info,tmp,sizeof(struct sysinfo)) != 0){
		pr_debug("ERROR COPY");
		return EFAULT;
	}
	
       	return 0;
}
