#ifndef _IPT_CONNLIMIT_H
#define _IPT_CONNLIMIT_H

struct ipt_connlimit_data;

struct ipt_connlimit_info {
	union {
		u_int32_t v4_mask;
		u_int32_t v6_mask[4];
	};
	unsigned int limit, inverse;

	/* this needs to be at the end */
	struct ipt_connlimit_data *data __attribute__((aligned(8)));
};

#endif /* _IPT_CONNLIMIT_H */
