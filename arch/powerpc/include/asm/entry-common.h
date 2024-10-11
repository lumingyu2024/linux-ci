/* SPDX-License-Identifier: GPL-2.0 */
#ifndef ARCH_POWERPC_ENTRY_COMMON_H
#define ARCH_POWERPC_ENTRY_COMMON_H

#include <linux/user-return-notifier.h>
#include <asm/switch_to.h>

static inline void arch_exit_to_user_mode_prepare(struct pt_regs *regs,
						  unsigned long ti_work)
{
	if (ti_work & _TIF_USER_RETURN_NOTIFY)
		fire_user_return_notifiers();
}

#define arch_exit_to_user_mode_prepare arch_exit_to_user_mode_prepare

static inline void arch_exit_to_user_mode_work(struct pt_regs *regs,
						unsigned long ti_work)
{

	if (IS_ENABLED(CONFIG_PPC_BOOK3S_64) && IS_ENABLED(CONFIG_PPC_FPU)) {
		if (IS_ENABLED(CONFIG_PPC_TRANSACTIONAL_MEM) &&
				unlikely((ti_work & _TIF_RESTORE_TM))) {
			restore_tm_state(regs);
		} else {
			unsigned long mathflags = MSR_FP;

			if (cpu_has_feature(CPU_FTR_VSX))
				mathflags |= MSR_VEC | MSR_VSX;
			else if (cpu_has_feature(CPU_FTR_ALTIVEC))
				mathflags |= MSR_VEC;

			/*
			 * If userspace MSR has all available FP bits set,
			 * then they are live and no need to restore. If not,
			 * it means the regs were given up and restore_math
			 * may decide to restore them (to avoid taking an FP
			 * fault).
			 */
			if ((regs->msr & mathflags) != mathflags)
				restore_math(regs);
		}
	}
}

#define arch_exit_to_user_mode_work arch_exit_to_user_mode_work

#endif
