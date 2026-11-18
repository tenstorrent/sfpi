	.attribute arch, "rv64i2p0_xttcache"

	tt.cache.cflush.d.l1 x0
	tt.cache.cflush.d.l1 x31
	tt.cache.cdiscard.d.l1 x0
	tt.cache.cdiscard.d.l1 x31
