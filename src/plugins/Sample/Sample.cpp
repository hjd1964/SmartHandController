// Sample plugin

#include "Sample.h"
#include "../../Common.h"
#include "../../lib/tasks/OnTask.h"

void sampleWrapper() { sample.loop(); }

void Sample::init() {
  VLF("MSG: Plugins, starting: sample");

  // start a task that runs twice a second, run at priority level 7 so we can block
  // using tasks.yield(); fairly aggressively without significant impact on operation
  tasks.add(2000, 0, true, 7, sampleWrapper);
}

void Sample::loop() {
  Serial.println("Hello from the sample plugin!");
}

Sample sample;
