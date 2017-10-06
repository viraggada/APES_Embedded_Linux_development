#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/sched.h>
#include <linux/sched/signal.h>
#include <linux/signal.h>

struct task_struct *task, *child, *task_list;
struct list_head *list;
uint32_t child_count;
/*Initialization function sets up the timer*/
int kernelLL_init(void)
{
  printk(KERN_ALERT "%s loaded\n",__FUNCTION__); //__FUNCTION__ identifier holds the name of the function
  task = current;

  while(task->pid!=0)
  {
    printk(KERN_INFO "Process ID - %d , Process State %ld , Process Name - %s\n", task->pid,task->state,task->comm);
    child_count = 0;
    list_for_each(list,&task->children){
      child = list_entry(list, struct task_struct, sibling);
      printk(KERN_INFO "Child ID - %d , Child State %ld , Child Name - %s\n",child->pid,child->state,child->comm);
      child_count++;
    }
    printk(KERN_INFO "Number of Children for PID %d are %d",task->pid,child_count);
    task = task->parent;
  }

  for_each_process(task_list)
  {
    printk(KERN_INFO "Process ID - %d , Process State %ld , Process Name - %s\n",task_list->pid,task_list->state,task_list->comm);
  }
  return 0;
}

/*Exit function deletes the timer*/
void kernelLL_exit(void)
{
  printk(KERN_ALERT "%s removed\n",__FUNCTION__);
}

/*Indicate the Initializing function*/
module_init(kernelLL_init);

/*Indicate the Exit function*/
module_exit(kernelLL_exit);

MODULE_LICENSE("GPL");
