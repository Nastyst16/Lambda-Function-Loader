// SPDX-License-Identifier: BSD-3-Clause

#include <dlfcn.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#include "ipc.h"
#include "server.h"

#ifndef OUTPUT_TEMPLATE
#define OUTPUT_TEMPLATE "../checker/output/out-XXXXXX"
#endif

static int lib_prehooks(struct lib *lib)
{
	/* TODO: Implement lib_prehooks(). */
	return 0;
}

static int lib_load(struct lib *lib)
{
	/* TODO: Implement lib_load(). */
	return 0;
}

static int lib_execute(struct lib *lib)
{
	/* TODO: Implement lib_execute(). */
	return 0;
}
 
static int lib_close(struct lib *lib)
{
	/* TODO: Implement lib_close(). */
	return 0;
}

static int lib_posthooks(struct lib *lib)
{
	/* TODO: Implement lib_posthooks(). */
	return 0;
}

static int lib_run(struct lib *lib)
{
	int err;

	err = lib_prehooks(lib);
	if (err)
		return err;

	err = lib_load(lib);
	if (err)
		return err;

	err = lib_execute(lib);
	if (err)
		return err;

	err = lib_close(lib);
	if (err)
		return err;

	return lib_posthooks(lib);
}

static int parse_command(const char *buf, char *name, char *func, char *params)
{
	int ret;

	ret = sscanf(buf, "%s %s %s", name, func, params);
	if (ret < 0)
		return -1;

	return ret;
}

int main(void)
{
	/* TODO: Implement server connection. */
	int sock_fd;
	struct lib lib;

	sock_fd = create_socket();
	if (sock_fd < 0) {
		perror("create_socket");
		exit(EXIT_FAILURE);
	}

	sock_fd = bind_socket(sock_fd);
	if (sock_fd < 0) {
		perror("bind_socket");
		exit(EXIT_FAILURE);
	}

	sock_fd = listen_socket(sock_fd);
	if (sock_fd < 0) {
		perror("listen_socket");
		exit(EXIT_FAILURE);
	}

	while (1) {
		/* TODO - get message from client */
		
		accept_socket(sock_fd);
		
		char buf[BUFSIZE];
		recv_socket(sock_fd, buf, BUFSIZE);

		send_socket(sock_fd, buf, strlen(buf));



		/* TODO - parse message with parse_command and populate lib */
		/* TODO - handle request from client */
		sock_fd = lib_run(&lib);
	}

	close_socket(sock_fd);

	return 0;
}
