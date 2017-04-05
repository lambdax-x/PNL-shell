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
        char name[255];
	char tmp[512];
	pr_debug("begin of cmd_modinfo_handler\n");
	//pr_debug("%s\n",args->name);
        strcpy(name,args->name);
	name[strlen(name)-1] = 0;
	//pr_debug("NAME : %s\n",name);
	//pr_debug("LENGTH : %d \n",(int)strlen(name));
	mod = find_module(name);
	//pr_debug("ADDR %p\n",mod);
        if ( mod != NULL ) {
                //find the parameters
                if ( mod->modinfo_attrs->attr.name != NULL ) {
                       // pr_debug("ATTR : %s\n",mod->modinfo_attrs->attr.name);
			scnprintf(tmp,512,"%s | %s | %s | %p",
			        mod->name, mod->version, mod->modinfo_attrs->attr.name, &mod);
			//pr_debug("FIN scnprintf\n");
			strcpy(res->info,tmp);
			return 0;
		}
		scnprintf(res->info, 512, "%s / %s / %p \n", mod->name,mod->version, &mod);
               // pr_debug("%s\n",res->info);
        }else{
                pr_debug("no module with this name.");
            	    return -1;
        }


        return 0;
}
