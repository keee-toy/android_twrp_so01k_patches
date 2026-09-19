#pragma once

enum So01kP434PreUiStage : unsigned {
	SO01K_P434_PROCESS_FSTAB_ENTERED = 1,
	SO01K_P434_SUPER_STAGE_COMPLETED = 2,
	SO01K_P434_PARTITION_CORE_COMPLETED = 3,
	SO01K_P434_PROCESS_FSTAB_RETURNED = 4,
	SO01K_P434_GUI_INIT_REACHED = 5,
	SO01K_P434_GR_INIT_REACHED = 6,
};

/* RAM-only, best-effort diagnostic publication. It never changes recovery
 * control flow and deliberately provides no retry or durability operation. */
void so01k_p434_publish_preui_stage(unsigned stage);
