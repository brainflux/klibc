/*
 * ipconfig/packet.h
 */
int packet_open(void);
void packet_close(void);
int packet_send(struct netdev *dev, struct iovec *iov, int iov_len);
int packet_peek(int *ifindex);
int packet_recv(struct iovec *iov, int iov_len);

