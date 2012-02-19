int sock_close(struct socket* sk);
ssize_t sock_recv(struct socket *sk, void *buffer, size_t length, int flags);
ssize_t sock_send(struct socket *sk, const void *buffer, size_t length, int flags);
struct socket* sock_accept(struct socket *sk, struct sockaddr *address, int *address_len);
