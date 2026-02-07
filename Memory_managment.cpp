#include <Arduino.h>
#include <stdlib.h> // atoi
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#define LED_BUILTIN 2

// Use only core 1 for demo purposes
#if CONFIG_FREERTOS_UNICORE
static const BaseType_t app_cpu = 0;
#else
static const BaseType_t app_cpu = 1;
#endif

// Some string to print

int const str_len=20;

int idx=0,lmao=0;
char lol[str_len]={0};
int *p=NULL;
// Task handles
static TaskHandle_t task_1 = NULL;
static TaskHandle_t task_2 = NULL;

// ----------------------
// HIGH priority task
// ----------------------
void startTask1(void *parameter) {

 Serial.println("sentence plz!");
  while (1){
    while (Serial.available()){
    char c=Serial.read();
    if(c=='\n'){
    Serial.println("memory before malloc alloc:");
     Serial.println(xPortGetFreeHeapSize());
     p=(int*)pvPortMalloc(sizeof(int));
     //for pue character like 'A','B' or c dont use atoi we have to first change pointer type to char
     //then manually copy charcters fron lol and pass on to *(p+i)
     
    *(p)=atoi(lol);
     
    xTaskNotify(task_2,0,eNoAction);
    memset(lol,0,str_len);
    idx=0;
    }
    
    else if(idx<str_len-1){ 
      lol[idx++]=c;
    }
    
    
  }vTaskDelay(200 / portTICK_PERIOD_MS);}
}

// ----------------------
// LOW priority task
// ----------------------
void startTask2(void *parameter) {
  
  while (1) {
   ulTaskNotifyTake(pdTRUE, portMAX_DELAY);
  //do nothing
  
  Serial.println("notification from B!");
  Serial.println("memory after malloc alloc:");
  Serial.println(xPortGetFreeHeapSize());
  vPortFree(p);
  }

  
  
}

// ----------------------

void setup() {
  pinMode(LED_BUILTIN,OUTPUT);

  Serial.begin(115200);
  vTaskDelay(1000 / portTICK_PERIOD_MS);

  Serial.println();
  Serial.println("--- FreeRTOS Task Demo ---");

  // Print which core + priority setup() is running on
  Serial.print("Setup and loop task running on core ");
  Serial.print(xPortGetCoreID());
  Serial.print(" with priority ");
  Serial.println(uxTaskPriorityGet(NULL));

  // Create low priority task
  xTaskCreatePinnedToCore(
    startTask1,
    "Task 1",
    1024,
    NULL,
    2,
    &task_1,
    app_cpu
  );

  // Create high priority task
  xTaskCreatePinnedToCore(
    startTask2,
    "Task 2",
    1024,
    NULL,
    1,
    &task_2,
    app_cpu
  );
}

void loop() {
vTaskDelay(1000/portTICK_PERIOD_MS);
  
  // Suspend the higher priority task for some intervals
  //for (int i = 0; i < 3; i++) {

    //vTaskSuspend(task_2);
    //vTaskDelay(2000 / portTICK_PERIOD_MS);

    //vTaskResume(task_2);
    //vTaskDelay(2000 / portTICK_PERIOD_MS);
  //}

  // Delete the lower priority task
  //if (task_1 != NULL) {

    //vTaskDelete(task_1);
    //task_1 = NULL;
  //}
} 