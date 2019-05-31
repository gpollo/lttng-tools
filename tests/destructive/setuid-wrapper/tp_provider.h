/*
 * Copyright (c) - 2019 Gabriel-Andrew Pollo-Guilbert <gabriel.pollo-guilbert@efficios.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * as published by the Free Software Foundation; only version 2
 * of the License.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */

#undef TRACEPOINT_PROVIDER
#define TRACEPOINT_PROVIDER setuid_wrapper

#undef TRACEPOINT_INCLUDE
#define TRACEPOINT_INCLUDE "./tp_provider.h"

#if !defined(_TP_PROVIDER_H) || defined(TRACEPOINT_HEADER_MULTI_READ)
#define _TP_PROVIDER_H

#include <lttng/tracepoint.h>

TRACEPOINT_EVENT(
	setuid_wrapper,
	before,
	TP_ARGS(
		unsigned int, uid
	),
	TP_FIELDS(
		ctf_integer(unsigned int, uid, uid)
	)
)

TRACEPOINT_EVENT(
	setuid_wrapper,
	after,
	TP_ARGS(
		unsigned int, uid
	),
	TP_FIELDS(
		ctf_integer(unsigned int, uid, uid)
	)
)

#endif /* _TP_PROVIDER_H */

#include <lttng/tracepoint-event.h>
