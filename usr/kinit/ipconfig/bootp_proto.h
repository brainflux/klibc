/*
 * bootp_proto.h
 */
int bootp_send_request(struct netdev *dev);
int bootp_recv_reply(struct netdev *dev);
int bootp_parse(struct netdev *dev, struct bootp_hdr *hdr, __u8 * exts,
		int extlen);
int bootp_init_if(struct netdev *dev);
