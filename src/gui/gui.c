#include "gui.h"
#include "gui_meun/gui_main/gui_main.h"

/* =========================
 * 全局GUI实例定义
 * ========================= */
gui_t gui =
    {
        /* =====================
         * menu
         * ===================== */
        .main =
            {
                .knob =
                    {
                        .ui = NULL,
                        .value = 0,
                        .active = false,
                    },
                .active = false,
            },

        /* =====================
         * heater
         * ===================== */
        .heater =
            {
                .heater =
                    {
                        .ui = NULL,
                        .value = false,
                        .active = false,
                    },
                .active = false,
            },

        /* =====================
         * water heater
         * ===================== */
        .water_heater =
            {
                .temperature =
                    {
                        .ui = NULL,
                        .temperature = 40,
                        .active = false,
                    },
                .active = false,
            },

        /* =====================
         * schedule（完整修复）
         * ===================== */
        .schedule =
            {
                .ui = NULL,
                .main = NULL,
                .active_index = 0,
                .active = false,

                .items =
                    {
                        [0 ... GUI_SCHEDULE_MAX - 1] =
                            {
                                .start_hour = 0,
                                .start_min = 0,
                                .end_hour = 0,
                                .end_min = 0,
                                .enabled = false,
                                .state = false,
                            }}},

        /* =====================
         * system（如果有）
         * ===================== */
        .system =
            {
                .buzzer =
                    {
                        .ui = NULL,
                        .value = false,
                        .active = false,
                    },
                .motor =
                    {
                        .ui = NULL,
                        .value = false,
                        .active = false,
                    },
                .display =
                    {
                        .ui = NULL,
                        .value = 30,
                        .active = false,
                    }
            }
    };

void gui_run(void)
{
    gui_main_page();
}
