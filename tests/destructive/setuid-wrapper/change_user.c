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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>

#define TRACEPOINT_DEFINE
#include "tp_provider.h"

#define CMD_SETUID    "setuid"
#define CMD_SETEUID   "seteuid"
#define CMD_SETREUID  "setreuid"
#define CMD_SETRESUID "setresuid"
#define CMD_MAX_LENGTH sizeof(CMD_SETRESUID)

static int call_setuid(int argc, char** argv)
{
	uid_t uid;

	if (argc != 1) {
		fprintf(stderr, "Usage: setuid UID\n");
		exit(1);
	}

	if(sscanf(argv[0], "%u", &uid) == 0) {
		fprintf(stderr, "sscanf: failed to parse UID=%s\n", argv[0]);
		exit(1);
	}

	return setuid(uid);
}

static int call_seteuid(int argc, char** argv)
{
	uid_t uid;

	if (argc != 1) {
		fprintf(stderr, "Usage: seteuid UID\n");
		exit(1);
	}

	if (sscanf(argv[0], "%u", &uid) == 0) {
		fprintf(stderr, "sscanf: failed to parse UID=%s\n", argv[0]);
		exit(1);
	}

	return seteuid(uid);
}

static int call_setreuid(int argc, char** argv)
{
	uid_t ruid, euid;

	if (argc != 2) {
		fprintf(stderr, "Usage: seteuid RUID EUID\n");
		exit(1);
	}

	if (sscanf(argv[0], "%u", &ruid) == 0) {
		fprintf(stderr, "sscanf: failed to parse RUID=%s\n", argv[0]);
		exit(1);
	}

	if (sscanf(argv[1], "%u", &euid) == 0) {
		fprintf(stderr, "sscanf: failed to parse EUID=%s\n", argv[1]);
		exit(1);
	}

	return setreuid(ruid, euid);
}

static int call_setresuid(int argc, char** argv)
{
	uid_t ruid, euid, suid;

	if (argc != 3) {
		fprintf(stderr, "Usage: setresuid RUID EUID SUID\n");
		exit(1);
	}

	if (sscanf(argv[0], "%u", &ruid) == 0) {
		fprintf(stderr, "sscanf: failed to parse RUID=%s\n", argv[0]);
		exit(1);
	}

	if (sscanf(argv[1], "%u", &euid) == 0) {
		fprintf(stderr, "sscanf: failed to parse EUID=%s\n", argv[1]);
		exit(1);
	}

	if (sscanf(argv[2], "%u", &suid) == 0) {
		fprintf(stderr, "sscanf: failed to parse SUID=%s\n", argv[2]);
		exit(1);
	}

	return setresuid(ruid, euid, suid);
}

int main(int argc, char** argv)
{
	char* call_func;
	char** call_params;
	int call_params_count, ret;

	if (argc <= 1) {
		fprintf(stderr, "Usage: %s CALL [OPTIONS]\n", argv[0]);
		exit(1);
	}

	if (argc > 5) {
		fprintf(stderr, "Error: too much parameter received\n");
		exit(1);
	}

	call_func = argv[1];
	call_params = &argv[2];
	call_params_count = argc - 2;

	tracepoint(setuid_wrapper, before, getuid());

	/*
	 * Under Linux, we could check that the program has the CAP_SETUID
	 * capability, but the interface isn't standard and may change in the
	 * future. If the program doesn't have the capability, the call will
	 * simply fail with EPERM.
	 */
	if (strncmp(CMD_SETUID, call_func, CMD_MAX_LENGTH) == 0) {
		ret = call_setuid(call_params_count, call_params);
	} else if (strncmp(CMD_SETEUID, call_func, CMD_MAX_LENGTH) == 0) {
		ret = call_seteuid(call_params_count, call_params);
	} else if (strncmp(CMD_SETREUID, call_func, CMD_MAX_LENGTH) == 0) {
		ret = call_setreuid(call_params_count, call_params);
	} else if (strncmp(CMD_SETRESUID, call_func, CMD_MAX_LENGTH) == 0) {
		ret = call_setresuid(call_params_count, call_params);
	} else {
		fprintf(stderr, "%s: %s is not a valid call\n", argv[0], argv[1]);
		exit(1);
	}

	/* All of these last calls return -1 and set errno on error */
	if (ret < 0) {
		fprintf(stderr, "%s failed: %s\n", argv[1], strerror(errno));
		exit(1);
	}

	tracepoint(setuid_wrapper, after, getuid());

	return 0;
}
