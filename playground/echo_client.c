#include <errno.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#define MAXLINE 1024
#define RIO_BUFSIZE 8192
typedef struct {
	int rio_fd; // Descriptor for this internal buf
	int rio_cnt; // Unread bytes in internal buf
	char *rio_bufptr; // Next unread byte in internal buf
	char rio_buf[RIO_BUFSIZE]; // Internal buffer
} rio_t;

static ssize_t rio_read(rio_t *rp, char *usrbuf, size_t n)
{
    int cnt;

    while (rp->rio_cnt <= 0) {  /* Refill if buf is empty */
		rp->rio_cnt = read(rp->rio_fd, rp->rio_buf, sizeof(rp->rio_buf));
		if (rp->rio_cnt < 0) {
		    if (errno != EINTR) /* Interrupted by sig handler return */
				return -1;
		}
		else if (rp->rio_cnt == 0)  /* EOF */
		    return 0;
		else 
		    rp->rio_bufptr = rp->rio_buf; /* Reset buffer ptr */
    }

    /* Copy min(n, rp->rio_cnt) bytes from internal buf to user buf */
    cnt = n;          
    if (rp->rio_cnt < n)   
		cnt = rp->rio_cnt;
    memcpy(usrbuf, rp->rio_bufptr, cnt);
    rp->rio_bufptr += cnt;
    rp->rio_cnt -= cnt;
    return cnt;
}

void rio_readinitb(rio_t *rp, int fd) {
	rp->rio_fd = fd;
	rp->rio_cnt = 0;
	rp->rio_bufptr = rp->rio_buf;
}

int open_clientfd(char *hostname, char *port) {
	int clientfd;
	struct addrinfo hints, *listp, *p;

	memset(&hints, 0, sizeof(struct addrinfo));
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_NUMERICSERV;
	hints.ai_flags |= AI_ADDRCONFIG;
	getaddrinfo(hostname, port, &hints, &listp);

	for (p = listp; p; p = p->ai_next) {
		if ((clientfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) < 0)
			continue;
		if (connect(clientfd, p->ai_addr, p->ai_addrlen) != -1)
			break;
		close(clientfd);
	}

	freeaddrinfo(listp);
	if (!p)
		return -1;
	else
		return clientfd;
}

ssize_t rio_writen(int fd, void *usrbuf, size_t n) 
{
    size_t nleft = n;
    ssize_t nwritten;
    char *bufp = usrbuf;

    while (nleft > 0) {
		if ((nwritten = write(fd, bufp, nleft)) <= 0) {
			if (errno == EINTR)  /* Interrupted by sig handler return */
				nwritten = 0;    /* and call write() again */
			else
				return -1;       /* errno set by write() */
		}
		nleft -= nwritten;
		bufp += nwritten;
    }
    return n;
}

ssize_t rio_readlineb(rio_t *rp, void *usrbuf, size_t maxlen)
{
    int n, rc;
    char c, *bufp = usrbuf;

    for (n = 1; n < maxlen; n++) {
        if ((rc = rio_read(rp, &c, 1)) == 1) {
			*bufp++ = c;
	    	if (c == '\n') {
        	    n++;
     			break;
        	}
		} else if (rc == 0) {
		    if (n == 1)
				return 0; /* EOF, no data read */
		    else
				break;    /* EOF, some data was read */
		} else
		    return -1;	  /* Error */
    }
    *bufp = 0;
    return n-1;
}

int main(int argc, char *argv[]) {
	int clientfd;
	char *host, *port, buf[MAXLINE];
	rio_t rio;

	if (argc != 3) {
		fprintf(stderr, "usage: %s <host> <port>\n", argv[0]);
		exit(0);
	}
	host = argv[1];
	port = argv[2];

	clientfd = open_clientfd(host, port);
	rio_readinitb(&rio, clientfd);

	while (fgets(buf, MAXLINE, stdin) != NULL) {
		rio_writen(clientfd, buf, strlen(buf));
		rio_readlineb(&rio, buf, MAXLINE);
		fputs(buf, stdout);
	}
	close(clientfd);
	exit(0);
}
