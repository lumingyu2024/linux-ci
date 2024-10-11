/* SPDX-License-Identifier: GPL-2.0 */
#ifndef __LINUX_IRQENTRYCOMMON_H
#define __LINUX_IRQENTRYCOMMON_H

#ifndef irqentry_state
/**
 * struct irqentry_state - Opaque object for exception state storage
 * @exit_rcu: Used exclusively in the irqentry_*() calls; signals whether the
 *            exit path has to invoke ct_irq_exit().
 * @lockdep: Used exclusively in the irqentry_nmi_*() calls; ensures that
 *           lockdep state is restored correctly on exit from nmi.
 *
 * This opaque object is filled in by the irqentry_*_enter() functions and
 * must be passed back into the corresponding irqentry_*_exit() functions
 * when the exception is complete.
 *
 * Callers of irqentry_*_[enter|exit]() must consider this structure opaque
 * and all members private.  Descriptions of the members are provided to aid in
 * the maintenance of the irqentry_*() functions.
 */
typedef struct irqentry_state {
	union {
		bool	exit_rcu;
		bool	lockdep;
	};
} irqentry_state_t;
#endif
#endif
