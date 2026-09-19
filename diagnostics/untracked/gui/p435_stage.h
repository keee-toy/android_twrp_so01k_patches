#pragma once

enum So01kP435PreUiStage : unsigned {
	SO01K_P435_RECOVERY_MAIN_ENTERED  = 1,
	SO01K_P435_EARLY_SETUP_DONE       = 2,
	SO01K_P435_STARTUP_SETUP_DONE     = 3,
	SO01K_P435_FSTAB_PATH_DONE        = 4,
	SO01K_P435_BEFORE_PROCESS_FSTAB   = 5,
	SO01K_P435_PROCESS_FSTAB_ENTERED  = 6,
	SO01K_P435_PROCESS_FSTAB_RETURNED = 7,
	SO01K_P435_GUI_INIT_REACHED       = 8,
	SO01K_P435_GR_INIT_REACHED        = 9,
};

/* RAM-only, best-effort diagnostic publication. It never changes recovery
 * control flow and deliberately provides no retry or durability operation. */
void so01k_p435_publish_preui_stage(unsigned stage);
