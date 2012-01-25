#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/string.h>
#include <linux/socket.h>
#include <linux/net.h>
#include <linux/in.h>
#include <net/sock.h>
#include <asm/processor.h>
#include <asm/uaccess.h>

int sock_close(struct socket* sk)
{
	int ret;

	ret = sk->ops->release(sk);

	if (sk)
		sock_release(sk);

	return ret;
}

ssize_t sock_recv(struct socket *sk, void *buffer, size_t length, int flags)
{
	struct msghdr msg;
	struct iovec iov;
	int ret;
#ifndef KSOCKET_ADDR_SAFE
	mm_segment_t old_fs;
#endif


	iov.iov_base = (void *)buffer;
	iov.iov_len = (__kernel_size_t)length;

	msg.msg_name = NULL;
	msg.msg_namelen = 0;
	msg.msg_iov = &iov;
	msg.msg_iovlen = 1;
	msg.msg_control = NULL;
	msg.msg_controllen = 0;

	/*
	 * msg.msg_iov->iov_base is declared as follows:
	 * void __user *iov_base;
	 * which means there is an user space pointer in 'msg'
	 * use set_fs(KERNEL_DS) to make the pointer safe to kernel space
	 */
#ifndef KSOCKET_ADDR_SAFE
	old_fs = get_fs();
	set_fs(KERNEL_DS);
#endif
	ret = sock_recvmsg(sk, &msg, length, flags);
#ifndef KSOCKET_ADDR_SAFE
	set_fs(old_fs);
#endif
	if (ret < 0)
		goto out_krecv;
	//ret = msg.msg_iov.iov_len;//?
	
out_krecv:
	return ret;

}

ssize_t sock_send(struct socket *sk, const void *buffer, size_t length, int flags)
{
	struct msghdr msg;
	struct iovec iov;
	int len;
#ifndef KSOCKET_ADDR_SAFE
	mm_segment_t old_fs;
#endif

	iov.iov_base = (void *)buffer;
	iov.iov_len = (__kernel_size_t)length;

	msg.msg_name = NULL;
	msg.msg_namelen = 0;
	msg.msg_iov = &iov;
	msg.msg_iovlen = 1;
	msg.msg_control = NULL;
	msg.msg_controllen = 0;

	msg.msg_flags = flags;

#ifndef KSOCKET_ADDR_SAFE
	old_fs = get_fs();
	set_fs(KERNEL_DS);
#endif
	len = sock_sendmsg(sk, &msg, length);//?
#ifndef KSOCKET_ADDR_SAFE
	set_fs(old_fs);
#endif
	
	return len;//len ?
}


struct socket* sock_accept(struct socket *sk, struct sockaddr *address, int *address_len)
{
	struct socket *new_sk = NULL;
	int ret;

	ret = sock_create(sk->sk->sk_family, sk->type, sk->sk->sk_protocol, &new_sk);
	if (ret < 0)
		return NULL;
	if (!new_sk)
		return NULL;
	
	new_sk->type = sk->type;
	new_sk->ops = sk->ops;
	
	ret = sk->ops->accept(sk, new_sk, 0 /*sk->file->f_flags*/);
	if (ret < 0)
		goto error_sock_accept;
	
	if (address)
	{
		ret = new_sk->ops->getname(new_sk, address, address_len, 2);
		if (ret < 0)
			goto error_sock_accept;
	}
	
	return new_sk;

error_sock_accept:
	sock_release(new_sk);
	return NULL;
}

