#include <linux/init.h>
#include <linux/module.h>
#include <linux/timer.h>
#include <linux/kernel.h>


static struct timer_list ktimer;

/*Callback function for our timer*/
void ktimer_callback(unsigned long data)
{
  static int count = 1;
  printk(KERN_INFO "Number of times callback called - %d\n",count);
  count++;
  /*Set the timer again*/
  mod_timer(&ktimer, jiffies + msecs_to_jiffies(500));
}

/*Initialization function sets up the timer*/
int hw2_kernel_module_init(void)
{ 
  printk(KERN_ALERT "%s loaded\n",__FUNCTION__); //__FUNCTION__ identifier holds the name of the function 
  /* setup your timer to call my_timer_callback */
  setup_timer(&ktimer, ktimer_callback, 0);
  /* setup timer interval to 500 msecs */
  mod_timer(&ktimer, jiffies + msecs_to_jiffies(500));

  return 0;
}

/*Exit function deletes the timer*/
void hw2_kernel_module_exit(void)
{
  int status;
  /* remove kernel timer when unloading module */
  if((status = del_timer(&ktimer))== 0)
  {
    printk(KERN_ALERT "Timer inactive\n");
  }
  printk(KERN_ALERT "%s removed\n",__FUNCTION__);
}

/*Indicate the Initializing function*/
module_init(hw2_kernel_module_init);

/*Indicate the Exit function*/
module_exit(hw2_kernel_module_exit);

MODULE_LICENSE("GPL");
