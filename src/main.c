#include "esp_event.h"
#include "freertos/timers.h"


ESP_EVENT_DEFINE_BASE(MY_CUSTOM_EVENT_BASE);


char * message_at_event;

static StaticTimer_t xTimerBuffer;



void my_events_handler(void* event_handler_arg, esp_event_base_t event_base, int32_t event_id, void* event_data)
{
    printf("%s", message_at_event);
}

void timerCallback( TimerHandle_t xExpiredTimer )
{
    esp_event_post(MY_CUSTOM_EVENT_BASE, 0, NULL, 0, pdMS_TO_TICKS(0));
}

void app_main()
{
    esp_event_handler_instance_t event_instance;
    TimerHandle_t timer_instance;
    esp_err_t err;
    printf("Start\n");

    ESP_ERROR_CHECK(esp_event_loop_create_default());

    message_at_event = "correct event message\n";

    // subscribe to event
    printf("Subscribe to event ");
    err = esp_event_handler_instance_register(MY_CUSTOM_EVENT_BASE, ESP_EVENT_ANY_ID, my_events_handler, NULL, &event_instance);
    if (err == ESP_OK)
    {
        printf("OK\n");
    }
    else
    {
        printf("fail (%#08X) !!!\n", err);
    }

    timer_instance = xTimerCreateStatic( "timer", pdMS_TO_TICKS(1000), pdTRUE, NULL, timerCallback, &xTimerBuffer );
    printf("Timer creation ");
    if (timer_instance != NULL &&
        xTimerStart( timer_instance, 0 ) == pdPASS)
    {
        printf("OK\n");
    }
    else
    {
        printf("fail!!!\n");
    }

    vTaskDelay(pdMS_TO_TICKS(10000));

    // unsubscribe
    printf("Unregister event ");
    if (esp_event_handler_instance_unregister(MY_CUSTOM_EVENT_BASE, ESP_EVENT_ANY_ID, event_instance) == ESP_OK)
    {
        printf("OK\n");
    }
    else
    {
        printf("fail!!!\n");
    }
    message_at_event = "you should never see this text at event!!!\n";
}

