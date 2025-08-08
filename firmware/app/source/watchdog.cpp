#include "watchdog.h"

// Watchdog timer
static TimerHandle_t watchdog_timer = nullptr;

void WTDG_TimeoutCallback(TimerHandle_t xTimer)
{
  SendMessage((const uint8_t *)"watchdog reset", 14);
  // NVIC_SystemReset(); 
}

void WTDG_Kick()
{
  if (watchdog_timer != nullptr)
  {
    xTimerReset(watchdog_timer, 0); // Reset countdown
  }
}

void WTDG_Init(void)
{
  watchdog_timer = xTimerCreate(
      "WatchdogTimer",
      pdMS_TO_TICKS(5000), // 5 second timeout
      pdFALSE,            
      nullptr,
      WTDG_TimeoutCallback);

  xTimerStart(watchdog_timer, 0);
}
