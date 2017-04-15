#include <asm/uaccess.h>
#include <linux/types.h>
#include <linux/slab.h>
#include <cmd/def.h>
#include <cmd/protos.h>
#include <cmd/work.h>
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

	si_meminfo(args->info);
	si_swapinfo(args->info);
	
	/* convert to KB */ 
	args->info->totalram = K(args->info->totalram);
	args->info->freeram = K(args->info->freeram);
	args->info->bufferram = K(args->info->bufferram);
	args->info->totalhigh = K(args->info->totalhigh);
	args->info->freehigh = K(args->info->freehigh);
	args->info->totalswap = K(args->info->totalswap);
	args->info->freeswap = K(args->info->freeswap);
	args->info->sharedram = K(args->info->sharedram);
	args->info->mem_unit = K(args->info->mem_unit);
	
	//si_swapinfo(args->info);
	res->val = 0;
       	return 0;
}
