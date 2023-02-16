#include <Arduino.h>
//  https://www.youtube.com/watch?v=WQGAs9MwXno
int c1=0, c2=0;

void task1 (void * parameters)  {

  for (;;)  {
    Serial.print("Task 1 contador: ");
    Serial.println(c1++);
    vTaskDelay(1000 / portTICK_PERIOD_MS);
  }
}

void task2 (void * parameters)  {

  for (;;)  {
    Serial.print("Task 1 contador: ");
    Serial.println(c1++);
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
    NULL
  );

  xTaskCreate(
    task2,
    "Task 2",
    1000,
    NULL,
    1,
    NULL
  );
}

void loop() {
  
}