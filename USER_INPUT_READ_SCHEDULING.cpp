#include <Arduino.h>

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
const char msg[] = "nsnsns task_1.";
int lol=500;

// Task handles
static TaskHandle_t task_1 = NULL;
static TaskHandle_t task_2 = NULL;

// ----------------------
// LOW priority task
// ----------------------
void startTask1(void *parameter) {


  while (1){
    Serial.println("enter the value for delay");
    
    while(!Serial.available()){
      vTaskDelay(10 / portTICK_PERIOD_MS);
    }
    
    lol=Serial.parseInt();
    Serial.println(lol);
    vTaskDelay(200 / portTICK_PERIOD_MS);
  }
}

// ----------------------
// HIGH priority task
// ----------------------
void startTask2(void *parameter) {

  while (1) {
 
   digitalWrite(LED_BUILTIN, HIGH);
  vTaskDelay(lol/ portTICK_PERIOD_MS);
  digitalWrite(LED_BUILTIN, LOW);
  vTaskDelay(lol / portTICK_PERIOD_MS);
    

    vTaskDelay(200 / portTICK_PERIOD_MS);
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