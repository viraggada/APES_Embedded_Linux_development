#include <linux/init.h>
#include <linux/module.h>
#include <linux/timer.h>
#include <linux/kernel.h>
#include <linux/jiffies.h>
#include <linux/time.h>
#include <linux/ktime.h>
#include <linux/cdev.h>
#include <linux/fs.h>
#include <linux/gpio.h>
#include <linux/leds.h>
#include <linux/string.h>
#include <linux/types.h>
#include <linux/slab.h>
#include <linux/uaccess.h>


#define DEV_NAME       "ledDriver"
#define CLASS_NAME     "ledClass"
#define DELIMITER      " "
#define LED_GPIO       (53) //USR0 LED - GPIO1_21 = 32*1 + 21 = 53
#define DATA_SIZE      (60)
#define NUM_OF_COMMAND (4)
#define REQUEST_SIZE   (10)

static dev_t deviceNum; /* Struct to hold the device number */
static int majorNum;    /* Vriable to hold the major number */
struct cdev * my_cdev;  /* cdev object */
uint8_t return_val;
uint8_t command_num;
uint8_t i;

/* Set default values */
static bool ledState = 0;
static bool lightState = 0;
static long int ledPeriod = 2000;
static long int ledDutyCycle = 50;
long int dutyOn = 1000;
long int dutyOff = 1000;
long int ledTime = 1000;

static struct timer_list freqTimer;

char * dataBuff;
char responseBuff[DATA_SIZE] = {(int)'\0'};
char charReq[REQUEST_SIZE] = {(int)'\0'};
char * command_list[NUM_OF_COMMAND]= {"led","period","duty","read"};
char * command;
char * request;
int request_val;

/* Prototypes for callback functions */
static int dev_open(struct inode * inode, struct file *file);
static int dev_release(struct inode * inode, struct file * file);
static ssize_t dev_read(struct file * file, char * buff, size_t len, loff_t * offset);
static ssize_t dev_write(struct file * file, const char * buff, size_t len, loff_t * offset);
int ledCharDriver_init(void);
void ledCharDriver_exit(void);

/* Structure containing the callbacks */
static struct file_operations fops =
{
  .owner   = THIS_MODULE,
  .open    = dev_open,
  .release = dev_release,
  .write   = dev_write,
  .read    = dev_read,
};

/*Callback function for our frequency timer*/
void freqtimer_callback(unsigned long data)
{

  /* Change time expire value based on provious state */
  if(lightState == 1){
    lightState = 0;
    ledTime = dutyOff;
  }else if(lightState == 0){
    lightState = 1;
    ledTime = dutyOn;
  }

  gpio_set_value(LED_GPIO,lightState);

  printk(KERN_INFO "Setting led state to - %d, led time - %ld\n",lightState,ledTime);

  /*Set the timer again*/
  mod_timer(&freqTimer, jiffies + msecs_to_jiffies(ledTime));
}


/*Initialization function */
int ledCharDriver_init(void)
{
  printk(KERN_ALERT "%s loaded\n",__FUNCTION__); //__FUNCTION__ identifier holds the name of the function

  /* Get a Major number dynamically */
  return_val= alloc_chrdev_region(&deviceNum, 0, 1, DEV_NAME);
  if(return_val< 0)
  {
    printk(KERN_ALERT "Failed to allocate a major number for the device!\n");
    return return_val;
  }

  majorNum = MAJOR(deviceNum);
  printk(KERN_INFO "The major number driver %s is: %d\n", DEV_NAME, majorNum);

  /* Create our cdev structure */
  my_cdev= cdev_alloc();
  /* Connect the file operations with cdev */
  cdev_init(my_cdev, &fops);
  /* Connect Major number to cdev */
  return_val = cdev_add(my_cdev,deviceNum,1);
  if(return_val< 0)
  {
    unregister_chrdev_region(deviceNum, 1);
    printk(KERN_ALERT "Unable to add cdev to kernel!\n");
    return return_val;
  }

  /* Check if the given pin number is valid */
  if (!gpio_is_valid(LED_GPIO)){
    cdev_del(my_cdev);
    unregister_chrdev_region(deviceNum, 1);
    printk(KERN_INFO "Invalid LED GPIO\n");
    return -ENODEV;
  }

  /* Request control over this pin */
  return_val = gpio_request(LED_GPIO,"usr_led_0");
  if(return_val< 0)
  {
    cdev_del(my_cdev);
    unregister_chrdev_region(deviceNum, 1);
    printk(KERN_ALERT "Failed to request GPIO %d\n",LED_GPIO);
    return return_val;
  }

  /* Set the pin as as output and initial value */
  gpio_direction_output(LED_GPIO, lightState);   // Set the gpio to be in output mode and on



  printk(KERN_INFO "sudo mknod -m 666 /dev/ledDriver c %d 0\n",majorNum);
  return 0;
}

/*Exit function */
void ledCharDriver_exit(void)
{
  /* remove kernel timer when unloading module */
  if((return_val = del_timer(&freqTimer))== 0)
  {
    printk(KERN_ALERT "Timer inactive\n");
  }

  /* Clear the led value */
  gpio_set_value(LED_GPIO,false);

  /* Release control of the pin */
  gpio_free(LED_GPIO);

  /* Remove the cdev */
  cdev_del(my_cdev);

  /* Release the major number */
  unregister_chrdev_region(deviceNum, 1);
  printk(KERN_ALERT "%s removed\n",__FUNCTION__);
}

static int dev_open(struct inode * inode, struct file *file){
  printk(KERN_INFO "Device Opened\n");
  return 0;
}

static int dev_release(struct inode * inode, struct file * file){
  printk(KERN_ALERT "Device Closed\n");
  return 0;
}

static ssize_t dev_read(struct file * file, char * buff, size_t len, loff_t * offset){
  //gpio_set_value(LED_GPIO,false);
  printk(KERN_INFO "Reading from device\n");
  printk(KERN_INFO "Response - %s\n",responseBuff);
  /* copy responseBuff into buff, which is in the user’s address space */
  if((return_val = copy_to_user(buff,responseBuff,strlen(responseBuff)))!=0){
    memset(responseBuff,(int)'\0',DATA_SIZE);
    return EFAULT;
  }

  memset(responseBuff,(int)'\0',DATA_SIZE);

  return 0;
}

static ssize_t dev_write(struct file * file, const char * buff, size_t len, loff_t * offset){
  /*Allocate kernel memory for our purpose*/
  if((dataBuff = (char *)kmalloc(len*sizeof(char),GFP_KERNEL)) == NULL)
    return ENOMEM;

  memset(dataBuff,(int)'\0',len*sizeof(char));
  return_val = copy_from_user(dataBuff,buff,len);

  /* Seperate command and request */
  command = strsep(&dataBuff,DELIMITER);
  if(command == NULL){
    printk(KERN_ALERT "Missing 2 arguments");
    return 0;
  }

  request = strsep(&dataBuff,DELIMITER);
  if(request == NULL){
    printk(KERN_ALERT "Missing argument");
    return 0;
  }

  /* Perform actions based on commands */
  for(i = 0; i<NUM_OF_COMMAND; i++) {
    if(strcmp(command_list[i],command) == 0) {
      command_num = i;
      break;
    }
  }

  strncpy(charReq,request,len - strlen(command) - 1);

  /* Do actions based on commands */
  switch(command_num)
  {
    case 0: /* Led state command */
          printk(KERN_INFO "LED command received\n");
          if(strncmp(charReq,"on",2)==0){
            ledState = 1;
            lightState = true;
            gpio_set_value(LED_GPIO,true);

            /* setup your timer to call freqTimer_callback */
            setup_timer(&freqTimer, freqtimer_callback, 0);

            dutyOn = ledPeriod*ledDutyCycle/100;
            dutyOff = ledPeriod-dutyOn;
            ledTime = dutyOn;
            mod_timer(&freqTimer, jiffies + msecs_to_jiffies(ledTime));
          }else if(strncmp(charReq,"off",3)==0){
            ledState = 0;
            lightState = false;
            gpio_set_value(LED_GPIO,lightState);
            if((return_val = del_timer(&freqTimer))== 0)
            {
              printk(KERN_ALERT "Timer inactive\n");
            }
          }
          printk(KERN_INFO "Led state %d\n",ledState);
          break;

    case 1:/* Period modification command */
          printk(KERN_INFO "Period command received\n");
          if(ledState == 1){
            if((return_val = kstrtol(charReq,10,&ledPeriod))!=0){
              return return_val;
            }
            printk(KERN_INFO "Period to set - %ld\n",ledPeriod);

            lightState = 1;
            dutyOn = ledPeriod*ledDutyCycle/100;
            dutyOff = ledPeriod-dutyOn;
            ledTime = dutyOn;
            gpio_set_value(LED_GPIO,lightState);

            /* setup timer interval  */
            mod_timer(&freqTimer, jiffies + msecs_to_jiffies(dutyOn));
          }else{
            printk(KERN_INFO "LED OFF\n");
          }

          break;

    case 2: /* Duty cycle modification command */
          printk(KERN_INFO "Duty cycle command received\n");
          if(ledState == 1){
            if((return_val = kstrtol(charReq,10, &ledDutyCycle))!=0){
              return return_val;
            }
            printk(KERN_INFO "Duty Cycle to set - %ld\n",ledDutyCycle);

            lightState = 1;
            dutyOn = ledPeriod*ledDutyCycle/100;
            dutyOff = ledPeriod-dutyOn;
            ledTime = dutyOn;
            gpio_set_value(LED_GPIO,lightState);

            /* setup timer interval  */
            mod_timer(&freqTimer, jiffies + msecs_to_jiffies(dutyOn));
          }else{
            printk(KERN_INFO "LED OFF\n");
          }

          break;

    case 3: /* Read command */
          printk(KERN_INFO "Read command received");

          memset(responseBuff,(int)'\0',DATA_SIZE);

          if(strncmp(charReq,"all",3)==0){
            sprintf(responseBuff,"State - %d, Period - %ld ms, Duty cycle - %ld",ledState,ledPeriod,ledDutyCycle);
          }else if(strncmp(charReq,"state",5)==0){
            sprintf(responseBuff,"State - %d",ledState);
          }else if(strncmp(charReq,"period",6)==0){
            sprintf(responseBuff,"Period - %ld ms",ledPeriod);
          }else if(strncmp(charReq,"duty",4)==0){
            sprintf(responseBuff,"Duty Cycle - %ld",ledDutyCycle);
          }
          //printk(KERN_INFO "Response - %s\n",responseBuff);

          break;

    default:
          printk(KERN_ALERT "Invalid command\n");

          break;
  }

  kfree(dataBuff);
  memset(charReq,(int)'\0',REQUEST_SIZE);
  command = NULL;
  request = NULL;
  //memset(dataBuff,(int)'\0',DATA_SIZE);
  //printk(KERN_INFO "Writing to device\n");
  return len;
}


/*Indicate the Initializing function*/
module_init(ledCharDriver_init);

/*Indicate the Exit function*/
module_exit(ledCharDriver_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Virag Gada");
MODULE_DESCRIPTION("A simple char driver for BBG led");
