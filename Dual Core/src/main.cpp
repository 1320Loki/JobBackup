#include <Arduino.h>
//  https://www.youtube.com/watch?v=WQGAs9MwXno


int c1=0, c2=0;
TaskHandle_t task1_handle = NULL;

//  How to create a task 1
void task1 (void * parameters)  {

  for (;;)  {
    Serial.print("Task 1 contador: ");
    Serial.println(c1++);
    vTaskDelay(1000 / portTICK_PERIOD_MS);

    if (c1 > 3) { vTaskDelete(NULL);}
    
  }
}

//  creation of task 2
void task2 (void * parameters)  {

  for (;;)  {
    Serial.print("Task 2 contador: ");
    Serial.println(c2++);
    vTaskDelay(1000 / portTICK_PERIOD_MS);
  }
}


void setup() {

  Serial.begin(115200);

  xTaskCreate(
    task1,
    "Task 1",
    1000,
    NULL,
    1,
    &task1_handle
  );

  xTaskCreate(
    task2,          //  Function Name
    "Task 2",       //  Task Name
    1000,           //  Stack Size
    NULL,           //  Task Parameters
    1,              //  Task priority
    NULL            //  Task Handle
  );
}

void loop() {
  
  if (c1 > 3 && task1_handle != NULL)   {vTaskSuspend(task1_handle);  }
  if (c1 == 5 && task1_handle != NULL)  { vTaskResume(task1_handle);  }
  
}

void ImportantTask()  {

  vTaskSuspendAll();      //  Stops All tasks
  xTaskResumeAll();       //  Resumes All tasks
}