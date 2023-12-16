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

static struct lib *lib_create()
{
	struct lib *lib = malloc(sizeof(struct lib));

	lib->outputfile = malloc(sizeof(char) * BUFSIZE);
	lib->libname = malloc(sizeof(char) * BUFSIZE);
	lib->funcname = malloc(sizeof(char) * BUFSIZE);
	lib->filename = malloc(sizeof(char) * BUFSIZE);
	
	return lib;
}

static int lib_prehooks(struct lib *lib) {

    if (lib == NULL || lib->libname == NULL || lib->funcname == NULL || lib->filename == NULL) {
		perror("lib_prehooks");
        return -1;
    }

    return 0;
}

static int lib_load(struct lib *lib) {
    lib->handle = dlopen(lib->libname, RTLD_NOW);

    if (lib->handle == NULL) {
        perror(dlerror());
		return -1;
    }

    return 0;
}

static int lib_execute(struct lib *lib) {

    int (*libraryFunction)();

    *(void **)(&libraryFunction) = dlsym(lib->handle, lib->funcname);

    if (libraryFunction == NULL) {
        perror(dlerror());
        return -1;
    }

    int result = libraryFunction();

    return result;
}
 
static int lib_close(struct lib *lib) {

    if (dlclose(lib->handle) != 0) {
        perror(dlerror());
        return -1;
    }

    lib->handle = NULL;

    return 0; // Success
}

static int lib_posthooks(struct lib *lib)
{
	/* TODO: Implement lib_posthooks(). */
	return 0;
}

static int find_function(struct lib *lib) {
	
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
	struct lib *lib;

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


	int client_fd;
	char buf[BUFSIZE];

	lib = lib_create();

	while (1) {

		/* TODO - get message from client */
		client_fd = accept_socket(sock_fd);
		
		memset(buf, 0, BUFSIZE);
		recv_socket(client_fd, buf, BUFSIZE);

		/* TODO - parse message with parse_command and populate lib */
		parse_command(buf, lib->libname, lib->funcname, lib->filename);

		/* TODO - handle request from client */
		sock_fd = lib_run(&lib);
		
		send_socket(client_fd, buf, strlen(buf));
	}

	close_socket(sock_fd);

	return 0;
}
