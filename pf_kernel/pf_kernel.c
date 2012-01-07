#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/string.h>
#include <linux/socket.h>
#include <linux/net.h>
#include <linux/in.h>
#include <net/sock.h>
#include <asm/processor.h>
#include <asm/uaccess.h>
#include<linux/fs.h>
#include<linux/ip.h>
#include<linux/tcp.h>
#include<linux/udp.h>
#include<linux/icmp.h>
#include<linux/pf_ring.h>
#include<linux/unistd.h>

#define DOMAIN PF_RING
#define TYPE SOCK_RAW
#define PROTOCOL htons(ETH_P_ALL)


//extern and global declarations

FlowSlotInfo *slots_info;
extern struct pf_ring_socket *pkap_pfr;
struct socket *capture_sockfd;
char *slots;
u_int16_t slot_header_len;



int capture_thread(void *arg)
{
	int ret,caplen=128;
	struct sockaddr sa;
	u_int memSlotsLen;
	char device_name[]="eth0";
	//socklen_t len = sizeof(slot_header_len);
	//packet_direction direction=0;
	char dummy;
	
	printk("in function capture_thread");	
	ret = sock_create(DOMAIN, TYPE , PROTOCOL , &capture_sockfd); 
	if (ret <0)
	{
		printk("socket creation failed\n");
		return -1;
	}
	else
	{
		printk("socket created successfully\n");
	}

	//setsockopt    :::::::
	//capture_sockfd->ops->release(capture_sockfd);
	
	//if (SOL_SOCKET==0)
		//ret = sock_setsockopt(capture_sockfd, SOL_SOCKET, SO_RING_BUCKET_LEN, (void *)&caplen, sizeof(caplen));
	//else
		ret = capture_sockfd->ops->setsockopt(capture_sockfd, 0, SO_RING_BUCKET_LEN,&caplen, sizeof(caplen));

	//t = capture_sockfd->ops->setsockopt(capture_sockfd, 0, SO_RING_BUCKET_LEN, &caplen, sizeof(caplen));	
	if (ret != 0)
	{
		printk("setsockopt failed with return code =%d \n",ret);
		return -1;
	}
	else
	{
		printk("setsockopt success\n");
	}	
		
	//bind ::::::::

	memset(&sa, 0, sizeof(sa));
	sa.sa_family = PF_RING;
  	snprintf(sa.sa_data, sizeof(sa.sa_data), "%s", "wlan0");

  	ret = capture_sockfd->ops->bind(capture_sockfd, (struct sockaddr *)&sa, sizeof(sa));
  	if(ret != 0) {
		printk(KERN_ALERT "failed to bind....\n");
		sock_release(capture_sockfd);
		return -1;
	}
	else
	{
		printk(KERN_ALERT "bind successful\n");	
	}
/*
	//mmap ::::::::::
	//printk(KERN_ALERT "pkap_pfr=%u",pkap_pfr);
	ret = capture_sockfd->ops->setsockopt(capture_sockfd, 0,SO_SET_PKAP_RING, 1, sizeof(int));

	if (ret != 0)
	{
		printk("setsockopt SO_SET_PKAP_RING failed with return code =%d \n",ret);
		return -1;
	}
	else
	{
		printk("SO_SET_PKAP_RING setsockopt success \n");
	}	
	//mmap done

	slots_info = (FlowSlotInfo *)(pkap_pfr->ring_memory);

	if(slots_info->version != RING_FLOWSLOT_VERSION)
	 {
	printk("Wrong RING version: "
	   "kernel is %i, libpfring was compiled with %i\n",
	   slots_info->version, RING_FLOWSLOT_VERSION);
	sock_release(capture_sockfd);
	return -1;
	}
	else
	{
		printk("Slot info success");
	}	

	memSlotsLen =slots_info->tot_mem;
	//munmap :::

	ret = capture_sockfd->ops->setsockopt(capture_sockfd, 0,SO_SET_PKAP_RING, 0, sizeof(int));

	if (ret != 0)
	{
		printk("setsockopt disabling SO_SET_PKAP_RING failed with return code =%d \n",ret);
		return -1;
	}
	else
	{
		printk("disabling SO_SET_PKAP_RING setsockopt success \n");
	}	
	
	//mmap again :::::
	
	ret = capture_sockfd->ops->setsockopt(capture_sockfd, 0,SO_SET_PKAP_RING, 1, sizeof(int));

	if (ret != 0)
	{
		printk("setsockopt SO_SET_PKAP_RING failed with return code =%d \n",ret);
		return -1;
	}
	else
	{
		printk("SO_SET_PKAP_RING setsockopt success \n");
	}
	
	slots_info = (FlowSlotInfo *)buffer;
	slots = (char *)(buffer+sizeof(FlowSlotInfo));
	//mmap done

	//getsockopt call: last para in declaration int __user *optlen

	ret = capture_sockfd->ops->getsockopt(capture_sockfd, 0, SO_GET_PKT_HEADER_LEN, &slot_header_len, &len);
	if(ret!=0)
	{
		printk("error in getting socket header len");
		sock_release(capture_sockfd);
		
		//munmap :::

	ret = capture_sockfd->ops->setsockopt(capture_sockfd, 0,SO_SET_PKAP_RING, 0, sizeof(int));

	if (ret != 0)
	{
		printk("setsockopt disabling SO_SET_PKAP_RING failed with return code =%d \n",ret);
		return -1;
	}
	else
	{
		printk("disabling SO_SET_PKAP_RING setsockopt success \n");
	}	
		return -1;
	}
	//getsockopt done

	
	//getting mac address of bounded device
	if(1)
	{
	 	socklen_t len1 = 6;
		u_char mac_address[6];
		char buf[32];
		if(capture_sockfd->ops->getsockopt(capture_sockfd, 0, SO_GET_BOUND_DEVICE_ADDRESS, mac_address, &len1) != 0)
    			printk("Impossible to know the device address\n");
  		else
    			printk("Capturing from %s [%s]\n", device_name, etheraddr_string(mac_address, buf));

	  	
	}
	
	//setsockopt ::::	
	capture_sockfd->ops->setsockopt(capture_sockfd, 0, SO_SET_PACKET_DIRECTION, &direction, sizeof(direction));

	//setsockopt done

	dummy = 0;
  	
	//setscokopt ::::activate ring

	ret=capture_sockfd->ops->setsockopt(capture_sockfd, 0, SO_ACTIVATE_RING, &dummy, sizeof(dummy));
	
	if(ret!=0)
	{
		printk("Error in enableing ring\n");
		
	//do munmap::::

		ret = capture_sockfd->ops->setsockopt(capture_sockfd, 0,SO_SET_PKAP_RING, 0, sizeof(int));

	if (ret != 0)
	{
		printk("setsockopt disabling SO_SET_PKAP_RING failed with return code =%d \n",ret);
		return -1;
	}
	else
	{
		printk("disabling SO_SET_PKAP_RING setsockopt success \n");
	}	
		
		sock_release(capture_sockfd);
		return -1;
	}
	// ring activation done

	while(1) {
	    ret = pfring_recv(sock_raw, &actual_buffer, 0, &hdr, wait_for_packet);
	    if(ret < 0)
	      break;
	    else if(ret > 0)
	      looper(&hdr, actual_buffer,  (u_char*)NULL);
	    else {
	      sleep(1);
	    }
	  }

*/
	sock_release(capture_sockfd);
	return 0;
}



static int ker_pf_ring_init(void)
{
	printk(KERN_ALERT "Initializing PF_RING kernel module");
	kernel_thread(capture_thread, NULL, 0);
	return 0;
}



static void ker_pf_ring_exit(void)
{
	printk(KERN_ALERT "Exiting PF_RING kernel module");
}



module_init(ker_pf_ring_init);
module_exit(ker_pf_ring_exit);
MODULE_LICENSE("Dual BSD/GPL");
