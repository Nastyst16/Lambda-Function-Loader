// SPDX-License-Identifier: BSD-3-Clause

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>

#include "ipc.h"


int create_socket(void) {
    int fd;

    if ((fd = socket(AF_UNIX, SOCK_STREAM, 0)) == -1) {
        perror("Error creating socket");
        return -1;
    }

    return fd;
}

int bind_socket(int fd) {

    struct sockaddr_un socket_address;
    socket_address.sun_family = AF_UNIX;
    strncpy(socket_address.sun_path, SOCKET_NAME, sizeof(socket_address.sun_path) - 1);

	// binding the socket
    if (bind(fd, (struct sockaddr*)&socket_address, sizeof(socket_address)) == -1) {
        perror("Error binding socket");
        close(fd);
        return -1;
    }

	return fd;
}


int listen_socket(int fd) {
	if (listen(fd, MAX_CLIENTS) == -1) {
		perror("Error listening on socket");
		close(fd);
		return -1;
	}

	return fd;
}

int accept_socket(int fd) {
    int client_socket;
    struct sockaddr_un client_addr;
    socklen_t client_len;

    // Accept a connection
    client_len = sizeof(client_addr);
    if ((client_socket = accept(fd, (struct sockaddr*)&client_addr, &client_len)) == -1) {
        perror("accept");
        return -1;
    }

    printf("Client connected.\n");

    return client_socket;
}




 
int connect_socket(int fd)
{
	/* TODO: Implement connect_socket(). */
	struct sockaddr_un addr;
	memset(&addr, 0, sizeof(struct sockaddr_un));
	addr.sun_family = AF_UNIX;
	strncpy(addr.sun_path, SOCKET_NAME, sizeof(addr.sun_path) - 1);
	if (connect(fd, (struct sockaddr *)&addr, sizeof(struct sockaddr_un)) == -1) {
		perror("connect");
		return -1;
	}

	// todorest
	return fd;
}

ssize_t send_socket(int fd, const char *buf, size_t len)
{
    ssize_t bytes_sent = send(fd, buf, len, 0);
    if (bytes_sent == -1) {
        perror("send");
        return -1;
    }
    return bytes_sent;
}

ssize_t recv_socket(int fd, char *buf, size_t len)
{
    ssize_t bytes_received = recv(fd, buf, len, 0);
    if (bytes_received == -1) {
        perror("recv");
        return -1;
    }
    return bytes_received;
}

void close_socket(int fd)
{
    if (close(fd) == -1) {
        perror("close");
    }
    // Optionally, you can remove the socket file after closing the socket
    remove(SOCKET_NAME);
}
