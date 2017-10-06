#include <linux/init.h>
#include <linux/module.h>
#include <linux/timer.h>
#include <linux/kernel.h>
#include <linux/kthread.h>
#include <linux/jiffies.h>
#include <linux/time.h>
#include <linux/ktime.h>

struct task_struct *kthreadStruct;
uint32_t startTime, endTime;
struct timeval s1,s2;

int printFunction(void *unused)
{
  printk(KERN_INFO "This is in child thread\n");
  do_exit(0);
  return 0;
}

/*Initialization function sets up the timer*/
int kthreadProfile_init(void)
{
  printk(KERN_ALERT "%s loaded\n",__FUNCTION__); //__FUNCTION__ identifier holds the name of the function

  printk(KERN_INFO "Creating Thread\n");
  /* Store the time before kthread create */
  startTime = jiffies;
  do_gettimeofday(&s1);
  /* Create the kernel thread with name 'profileKthread' */
  kthreadStruct = kthread_create(printFunction, NULL, "profileKthread");
  /* Store the time after kthread create */
  endTime = jiffies;
  do_gettimeofday(&s2);
  if (kthreadStruct)
      printk(KERN_INFO "Thread Created successfully\n");
  else
      printk(KERN_ERR "Thread creation failed\n");

  printk(KERN_INFO "Time to create thread - %d jiffies\n",endTime-startTime);
  printk(KERN_INFO "Time to create thread - %ld usec",s2.tv_usec - s1.tv_usec);
  return 0;
}

/*Exit function deletes the timer*/
void kthreadProfile_exit(void)
{
  /* remove kernel thread when unloading module */
  if (kthreadStruct)
   {
       kthread_stop(kthreadStruct);
       printk(KERN_INFO "Thread stopped\n");
   }

  printk(KERN_ALERT "%s removed\n",__FUNCTION__);
}

/*Indicate the Initializing function*/
module_init(kthreadProfile_init);

/*Indicate the Exit function*/
module_exit(kthreadProfile_exit);

MODULE_LICENSE("GPL");
