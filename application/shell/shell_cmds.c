#include <string.h>

#include "nr_micro_shell.h"
#include "FreeRTOS.h"
#include "task.h"
#include "stm32f4xx_hal.h"
#include "wifi.h"

static void help_cmd(char argc, char *argv)
{
   shell_printf("help cmd\r\n");
}

static void memshow_cmd(char argc, char *argv)
{
   shell_printf("free:%d\r\n", xPortGetFreeHeapSize());
}

static char *prvWriteNameToBuffer( char *pcBuffer, const char *pcTaskName )
{
    long x;

    /* Start by copying the entire string. */
    strcpy( pcBuffer, pcTaskName );

    /* Pad the end of the string with spaces to ensure columns line up when
    printed out. */
    for( x = strlen( pcBuffer ); x < ( configMAX_TASK_NAME_LEN - 1 ); x++ )
    {
        pcBuffer[ x ] = ' ';
    }

    /* Terminate. */
    pcBuffer[ x ] = 0x00;

    /* Return the new end of string. */
    return &( pcBuffer[ x ] );
}

static void tasklist_cmd(char argc, char *argv)
{
#define tskBLOCKED_CHAR ('B')
#define tskREADY_CHAR ('R')
#define tskDELETED_CHAR ('D')
#define tskSUSPENDED_CHAR ('S')
#define tskRUNNING_CHAR ('E')
#define PRIORITY_TO_NATIVE_PRIORITY(priority) (uint8_t)(configMAX_PRIORITIES-1 - priority)
   TaskStatus_t *pxTaskStatusArray;
   unsigned portBASE_TYPE uxCurrentNumberOfTasks = uxTaskGetNumberOfTasks();
   volatile UBaseType_t uxArraySize, x;
   char cStatus;
   char pcTaskStatusStr[48];
   char *pcTaskStatusStrTmp;

   /* Take a snapshot of the number of tasks in case it changes while this
   function is executing. */
   uxArraySize = uxCurrentNumberOfTasks;

   /* Allocate an array index for each task.  NOTE!  if
    configSUPPORT_DYNAMIC_ALLOCATION is set to 0 then pvPortMalloc() will
    equate to NULL. */
   pxTaskStatusArray = pvPortMalloc(uxCurrentNumberOfTasks * sizeof(TaskStatus_t));

   shell_printf("%-12s Status     Prio    Stack   TCB\r\n", "Name");
   shell_printf("-------------------------------------------\r\n");


   if (pxTaskStatusArray != NULL)
   {
      /* Generate the (binary) data. */
      uxArraySize = uxTaskGetSystemState(pxTaskStatusArray, uxArraySize, NULL);

      /* Create a human readable table from the binary data. */
      for (x = 0; x < uxArraySize; x++)
      {
         switch (pxTaskStatusArray[x].eCurrentState)
         {
         case eReady:
            cStatus = tskREADY_CHAR;
            break;

         case eBlocked:
            cStatus = tskBLOCKED_CHAR;
            break;

         case eSuspended:
            cStatus = tskSUSPENDED_CHAR;
            break;

         case eDeleted:
            cStatus = tskDELETED_CHAR;
            break;
         case eRunning:
            cStatus = tskRUNNING_CHAR;
            break;

         default: /* Should not get here, but it is included
          to prevent static checking errors. */
            cStatus ='I';
            break;
         }

         /* Write the task name to the string, padding with spaces so it
          can be printed in tabular form more easily. */
         pcTaskStatusStrTmp = pcTaskStatusStr;
         pcTaskStatusStrTmp = prvWriteNameToBuffer(pcTaskStatusStrTmp, pxTaskStatusArray[x].pcTaskName);


         /* Write the rest of the string. */
         sprintf(pcTaskStatusStrTmp, "\t%c\t%u\t%u\t%u\r\n", cStatus,
                 /*PRIORITY_TO_NATIVE_PRIORITY((unsigned int)pxTaskStatusArray[x].uxCurrentPriority)*/
                 (unsigned int)pxTaskStatusArray[x].uxCurrentPriority,
                 (unsigned int)pxTaskStatusArray[x].usStackHighWaterMark,
                 (unsigned int)pxTaskStatusArray[x].xTaskNumber);

         shell_printf("%s", pcTaskStatusStr);
   
      }


      vPortFree(pxTaskStatusArray);
   }
   else
   {
      mtCOVERAGE_TEST_MARKER();
   }
   
}

static void uptime_cmd(char argc, char *argv)
{
   TickType_t ticks;
   uint32_t ms;

   ticks = xTaskGetTickCount();

   ms = ticks * (1000 / configTICK_RATE_HZ);

   shell_printf("uptime ms:%lu\r\n", ms);
}

static void reboot_cmd(char argc, char *argv)
{
   HAL_NVIC_SystemReset();
}


void fault_test_by_div0(void) {
    volatile int * SCB_CCR = (volatile int *) 0xE000ED14; // SCB->CCR
    int x, y, z;

    *SCB_CCR |= (1 << 4); /* bit4: DIV_0_TRP. */

    x = 10;
    y = 0;
    z = x / y;
    printf("z:%d\n", z);
}

static void test_cmd(char argc, char *argv)
{
   fault_test_by_div0();
}

static void wifi_status_cmd(char argc, char *argv)
{
   printf("call mcu_get_wifi_connect_status\r\n");
   mcu_get_wifi_connect_status();
}

static void wifi_reset_cmd(char argc, char *argv)
{
   printf("call mcu_reset_wifi\r\n");
   mcu_reset_wifi();
   
}


const static_cmd_st static_cmd[] =
{
   {"help", help_cmd, NULL},
   {"memshow", memshow_cmd, NULL},
   {"tasklist", tasklist_cmd, NULL},
   {"uptime", uptime_cmd, NULL},
   {"reboot", reboot_cmd, NULL},
   {"test", test_cmd, NULL},
   {"wifistate", wifi_status_cmd, NULL},
   {"wifireset", wifi_reset_cmd, NULL},
   {"\0",NULL}
};
