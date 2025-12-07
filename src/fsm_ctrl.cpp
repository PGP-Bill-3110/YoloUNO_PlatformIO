#include "fsm_ctrl.h"

void fsmCtrlTask(void *pvParameters) {
    while(1) {
        if(fsm_mode && !tinyml_mode) {
            fsm_state = FSM_CONFIG;
        } else if(!fsm_mode && tinyml_mode) {
            fsm_state = FSM_TINYML;
        } else {
            fsm_state = FSM_NORMAL; 
        }

        vTaskDelay(pdMS_TO_TICKS(50));
    }
}