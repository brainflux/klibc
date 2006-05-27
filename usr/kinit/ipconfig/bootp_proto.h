/*
 * bootp_proto.h
 */
int bootp_send_request(struct netdev *dev);
int bootp_recv_reply(struct netdev *dev);
int bootp_parse(struct netdev *dev, struct bootp_hdr *hdr, uint8_t * exts,
		int extlen);
int bootp_init_if(struct netdev *dev);
