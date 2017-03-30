#ifndef IOC_H
#define IOC_H

long ioc(struct file *file, unsigned int seq, unsigned long addr);

#endif
