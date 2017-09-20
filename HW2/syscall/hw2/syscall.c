// Printk and KERN_* macros
#include <linux/kernel.h>
#include <linux/syscalls.h>
#include <linux/slab.h>
#include <uapi/asm-generic/errno-base.h>
#include <asm/types.h>

// the system call
//asmlinkage long sys_hw2(int32_t *src, size_t length, int32_t *dst)
SYSCALL_DEFINE3(hw2,
                int32_t *, src,
                size_t, length,
                int32_t *, dst)
{ 
  int32_t *kmem = NULL;
  size_t i = 0, j =0;
  int32_t temp;

  printk(KERN_INFO "Entered Syscall");

  /*Check for invalid conditions before doing anything*/
  if(src == NULL)
    return EFAULT;

  if(dst == NULL)
    return EFAULT;
  
  if(length == 0)
    return EINVAL;
  
  /*Allocate kernel memory for our purpose*/
  if((kmem = (int32_t *)kmalloc(sizeof(int32_t)*length,GFP_KERNEL)) == NULL)
    return ENOMEM;
  
  printk(KERN_INFO "Buffer size allocated - %ld",sizeof(int32_t)*length);
  /* copy src, which is in the user’s address space, into kmem */
  if (copy_from_user(kmem, src, length*sizeof(int32_t)))
    return EFAULT;
  
  printk(KERN_INFO "Sorting Data");
  for(i = 0 ; i < length ; i++)
  {
    for(j = i +1 ; j < length ; j++)
    {
      if((*(kmem+i))<(*(kmem+j)))
      {
        temp = *(kmem+i);
        *(kmem+i) = *(kmem+j);
        *(kmem+j) = temp;
      }
    }
  }
  printk(KERN_INFO "Sorting completed");
  /* copy kmem into dst, which is in the user’s address space */
  if (copy_to_user(dst, kmem, length*sizeof(int32_t)))
    return EFAULT;
  
  /*Free the allocated kernel memory*/
  kfree((void *)kmem);
  
  printk(KERN_INFO "Exited from Syscall");
  // 0 for success
  return 0;
}
