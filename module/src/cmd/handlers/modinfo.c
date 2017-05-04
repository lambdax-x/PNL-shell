#include <asm/uaccess.h>
#include <linux/types.h>
#include <linux/module.h>
#include <linux/kobject.h>
#include <common/def.h>
#include <cmd/protos.h>
#include <linux/slab.h>
/**
 * gives information about a module 
 */
int cmd_modinfo_handler(struct cmd_modinfo_args *args,
                                 struct cmd_modinfo_res *res){

        struct module *mod;
        char *name_of_module;
	char tmp[512];
	
	name_of_module = kmalloc(sizeof(char)*(args->sizeName), GFP_KERNEL);
	if (copy_from_user(name_of_module, args->name, args->sizeName) != 0)
                return -EFAULT;
                
	name_of_module[args->sizeName] = 0;
	
	mod = find_module(name_of_module);

        if ( mod != NULL ) {
                //find the parameters
                if ( mod->modinfo_attrs->attr.name != NULL ) {
                       
		       scnprintf(tmp,512,"name           : %s\n",mod->name);
		       scnprintf(tmp,512,"%sversion        : %s\n",
		                 tmp,mod->version);
		       scnprintf(tmp,512,"%sload address   : %p\n",
		                 tmp,&mod);
		       scnprintf(tmp,512,"%snumber param   : %d\n",
		                 tmp,mod->num_kp);
			       
			for(int i = 0; i< mod->num_kp; i++){
			        scnprintf(tmp,512,
			                "%s"
			                "param %d        : %s\n"
			                ,tmp
			                ,i
			                ,mod->kp[i].name);
			              
			}
			
					
			if(args->sizeRes <= strlen(tmp)){
			        pr_debug("ERROR TAILLE, %d, %d",args->sizeRes,
			                        (int)strlen(tmp));
			        goto error;
			}else{
			        if(copy_to_user(args->result,tmp
			                ,args->sizeRes) != 0){
			                 goto error;
			        }
			}
			return 0;
		}
        }else{
                goto error;
        }
  
error:
        kfree(name_of_module);
        return -1;
}
