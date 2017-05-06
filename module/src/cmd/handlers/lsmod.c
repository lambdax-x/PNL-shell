#include <asm/uaccess.h>
#include <linux/types.h>
#include <linux/module.h>
#include <linux/kobject.h>
#include <common/def.h>
#include <cmd/protos.h>
#include <linux/slab.h>
#include <linux/list.h>
#include <linux/string.h>
/**
 * gives information about a module 
 */
char *tmp_enum[4] = {"LIVE","COMING","GOING","UNFORMED"};

int cmd_lsmod_handler(struct cmd_lsmod_args *args,
                                 struct cmd_lsmod_res *res){

        struct module *mod;
	struct module *tmp;

	mod = find_module("playground");
	pr_debug("Module		State	Version\n");

	pr_debug("%s	%s	%s\n",
		mod->name,
		tmp_enum[mod->state],
		mod->version);

	list_for_each_entry_reverse(tmp, &mod->list,list){
		if( &tmp->name != NULL && strlen(tmp->name) > 1 ){
			pr_debug("%s	%s	%s\n",
				tmp->name,
				tmp_enum[tmp->state],
				tmp->version);
		}
        }

        return 0;
}

