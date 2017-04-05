#include <asm/uaccess.h>
#include <linux/types.h>
#include <linux/slab.h>
#include <cmd/def.h>
#include <cmd/protos.h>
#include <cmd/work.h>
#include <linux/module.h>
#include <linux/kobject.h>
/**
 * gives information about a module
 */
int cmd_modinfo_handler(struct cmd_modinfo_args *args, struct cmd_modinfo_res *res){

        struct module *mod;
        printk("begin of cmd_modinfo_handler\n");

        mod = find_module(args->name);
        if( mod != NULL ){
                //find the parameters
                if( mod->modinfo_attrs->attr.name != NULL ){
                        printk("ATTR : %s\n",mod->modinfo_attrs->attr.name);
			scnprintf(res->info,256,"%s | %s | %s | %p",
			        mod->name, mod->version, mod->modinfo_attrs->attr.name, &mod);
		}
		scnprintf(res->info, 512, "%s / %s / %p \n", mod->name,mod->version, &mod);
                printk("%s\n",res->info);

        }else{
                printk("no module with this name.");
                return -1;
        }


        return 0;
}
