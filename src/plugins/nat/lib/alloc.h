/*
 * Copyright (c) 2020 Cisco and/or its affiliates.
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at:
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
/**
 * @file
 * @brief NAT port/address allocation lib
 */

#ifndef included_nat_lib_alloc_h__
#define included_nat_lib_alloc_h__

#include <vnet/ip/ip.h>

#define foreach_nat_error \
  _(VALUE_EXIST, -1, "Value already exists") \
  _(NO_SUCH_ENTRY, -2, "No such entry") \
  _(UNKNOWN_PROTOCOL, -3, "Unknown protocol") \
  _(OUT_OF_TRANSLATIONS, -4, "Out of translations")

#define foreach_nat_protocol \
  _(UDP, 0, udp, "udp")       \
  _(TCP, 1, tcp, "tcp")       \
  _(ICMP, 2, icmp, "icmp")

typedef enum
{
#define _(N, i, s) NAT_ERROR_##N = i,
  foreach_nat_error
#undef _
} nat_error_t;

typedef enum
{
#define _(N, i, n, s) NAT_PROTOCOL_##N = i,
  foreach_nat_protocol
#undef _
} nat_protocol_t;

typedef struct nat_ip4_pool_addr_s nat_ip4_pool_addr_t;
typedef struct nat_ip4_addr_port_s nat_ip4_addr_port_t;
typedef struct nat_ip4_pool_s nat_ip4_pool_t;

typedef void (nat_add_del_ip4_pool_addr_cb_t) (ip4_address_t addr,
					       u8 is_add, void *opaque);

typedef int (nat_alloc_ip4_addr_and_port_cb_t) (nat_ip4_pool_t * pool,
						u32 fib_index,
						u32 thread_index,
						u32 nat_thread_index,
						u16 port_per_thread,
						u16 protocol,
						nat_ip4_addr_port_t * out);

struct nat_ip4_pool_addr_s
{
  ip4_address_t addr;
  u32 fib_index;
/* *INDENT-OFF* */
#define _(N, i, n, s) \
  u16 busy_##n##_ports; \
  u16 * busy_##n##_ports_per_thread; \
  uword * busy_##n##_port_bitmap;
  foreach_nat_protocol
#undef _
/* *INDENT-ON* */
};

struct nat_ip4_addr_port_s
{
  ip4_address_t addr;
  u16 port;
};

struct nat_ip4_pool_s
{
  nat_add_del_ip4_pool_addr_cb_t *add_del_pool_addr_cb;
  nat_alloc_ip4_addr_and_port_cb_t *alloc_addr_and_port_cb;
  nat_ip4_pool_addr_t *pool_addr;
  u32 random_seed;
};

int
nat_add_del_ip4_pool_addr (nat_ip4_pool_t * pool,
			   ip4_address_t addr, u8 is_add);

int
nat_add_del_ip4_pool_addrs (nat_ip4_pool_t * pool,
			    ip4_address_t addr,
			    u32 count, u8 is_add, void *opaque);

int
nat_alloc_ip4_addr_and_port_cb_default (nat_ip4_pool_t * pool,
					u32 fib_index,
					u32 thread_index,
					u32 nat_thread_index,
					u16 port_per_thread,
					u16 protocol,
					nat_ip4_addr_port_t * out);

int
nat_alloc_ip4_addr_and_port (nat_ip4_pool_t * pool,
			     u32 fib_index,
			     u32 thread_index,
			     u32 nat_thread_index,
			     u16 port_per_thread,
			     u16 protocol, nat_ip4_addr_port_t * out);

int
nat_free_ip4_addr_and_port (nat_ip4_pool_t * pool,
			    u32 thread_index,
			    u16 protocol, nat_ip4_addr_port_t * in);

#endif /* included_nat_lib_alloc_h__ */

/*
 * fd.io coding-style-patch-verification: ON
 *
 * Local Variables:
 * eval: (c-set-style "gnu")
 * End:
 */