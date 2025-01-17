/*
 * Copyright (C) 2012 - David Goulet <dgoulet@efficios.com>
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License, version 2 only, as
 * published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along with
 * this program; if not, write to the Free Software Foundation, Inc., 51
 * Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */

#ifndef CMD_H
#define CMD_H

#include "context.h"
#include "session.h"

struct notification_thread_handle;

/*
 * A callback (and associated user data) that should be run after a command
 * has been executed. No locks should be taken while executing this handler.
 *
 * The command's reply should not be sent until the handler has run and
 * completed successfully. On failure, the handler's return code should
 * be the only reply sent to the client.
 */
typedef enum lttng_error_code (*completion_handler_function)(void *);
struct cmd_completion_handler {
	completion_handler_function run;
	void *data;
};

/*
 * Init the command subsystem. Must be called before using any of the functions
 * above. This is called in the main() of the session daemon.
 */
void cmd_init(void);

/* Session commands */
enum lttng_error_code cmd_create_session(struct command_ctx *cmd_ctx, int sock,
		struct lttng_session_descriptor **return_descriptor);
int cmd_destroy_session(struct ltt_session *session,
		struct notification_thread_handle *notification_thread_handle);

/* Channel commands */
int cmd_disable_channel(struct ltt_session *session,
		enum lttng_domain_type domain, char *channel_name);
int cmd_enable_channel(struct ltt_session *session,
		struct lttng_domain *domain, struct lttng_channel *attr,
		int wpipe);
int cmd_track_pid(struct ltt_session *session, enum lttng_domain_type domain,
		int pid);
int cmd_untrack_pid(struct ltt_session *session, enum lttng_domain_type domain,
		int pid);

/* Event commands */
int cmd_disable_event(struct ltt_session *session,
		enum lttng_domain_type domain,
		char *channel_name,
		struct lttng_event *event);
int cmd_add_context(struct ltt_session *session, enum lttng_domain_type domain,
		char *channel_name, struct lttng_event_context *ctx, int kwpipe);
int cmd_set_filter(struct ltt_session *session, enum lttng_domain_type domain,
		char *channel_name, struct lttng_event *event,
		struct lttng_filter_bytecode *bytecode);
int cmd_enable_event(struct ltt_session *session, struct lttng_domain *domain,
		char *channel_name, struct lttng_event *event,
		char *filter_expression,
		struct lttng_filter_bytecode *filter,
		struct lttng_event_exclusion *exclusion,
		int wpipe);

/* Trace session action commands */
int cmd_start_trace(struct ltt_session *session);
int cmd_stop_trace(struct ltt_session *session);

/* Consumer commands */
int cmd_register_consumer(struct ltt_session *session,
		enum lttng_domain_type domain,
		const char *sock_path, struct consumer_data *cdata);
int cmd_set_consumer_uri(struct ltt_session *session, size_t nb_uri,
		struct lttng_uri *uris);
int cmd_setup_relayd(struct ltt_session *session);

/* Listing commands */
ssize_t cmd_list_domains(struct ltt_session *session,
		struct lttng_domain **domains);
ssize_t cmd_list_events(enum lttng_domain_type domain,
		struct ltt_session *session, char *channel_name,
		struct lttng_event **events, size_t *total_size);
ssize_t cmd_list_channels(enum lttng_domain_type domain,
		struct ltt_session *session, struct lttng_channel **channels);
ssize_t cmd_list_domains(struct ltt_session *session,
		struct lttng_domain **domains);
void cmd_list_lttng_sessions(struct lttng_session *sessions,
		size_t session_count, uid_t uid, gid_t gid);
ssize_t cmd_list_tracepoint_fields(enum lttng_domain_type domain,
		struct lttng_event_field **fields);
ssize_t cmd_list_tracepoints(enum lttng_domain_type domain,
		struct lttng_event **events);
ssize_t cmd_snapshot_list_outputs(struct ltt_session *session,
		struct lttng_snapshot_output **outputs);
ssize_t cmd_list_syscalls(struct lttng_event **events);
ssize_t cmd_list_tracker_pids(struct ltt_session *session,
		enum lttng_domain_type domain, int32_t **pids);

int cmd_data_pending(struct ltt_session *session);

/* Snapshot */
int cmd_snapshot_add_output(struct ltt_session *session,
		struct lttng_snapshot_output *output, uint32_t *id);
int cmd_snapshot_del_output(struct ltt_session *session,
		struct lttng_snapshot_output *output);
int cmd_snapshot_record(struct ltt_session *session,
		struct lttng_snapshot_output *output, int wait);

int cmd_set_session_shm_path(struct ltt_session *session,
		const char *shm_path);
int cmd_regenerate_metadata(struct ltt_session *session);
int cmd_regenerate_statedump(struct ltt_session *session);

int cmd_register_trigger(struct command_ctx *cmd_ctx, int sock,
		struct notification_thread_handle *notification_thread_handle);
int cmd_unregister_trigger(struct command_ctx *cmd_ctx, int sock,
		struct notification_thread_handle *notification_thread_handle);

int cmd_rotate_session(struct ltt_session *session,
		struct lttng_rotate_session_return *rotate_return);
int cmd_rotate_get_info(struct ltt_session *session,
		struct lttng_rotation_get_info_return *info_return,
		uint64_t rotate_id);
int cmd_rotation_set_schedule(struct ltt_session *session,
		bool activate, enum lttng_rotation_schedule_type schedule_type,
		uint64_t value,
		struct notification_thread_handle *notification_thread_handle);

const struct cmd_completion_handler *cmd_pop_completion_handler(void);

#endif /* CMD_H */
