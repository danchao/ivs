/****************************************************************
 *
 *        Copyright 2013, Big Switch Networks, Inc.
 *
 * Licensed under the Eclipse Public License, Version 1.0 (the
 * "License"); you may not use this file except in compliance
 * with the License. You may obtain a copy of the License at
 *
 *        http://www.eclipse.org/legal/epl-v10.html
 *
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND,
 * either express or implied. See the License for the specific
 * language governing permissions and limitations under the
 * License.
 *
 ****************************************************************/

/*
 * ivs - Types and functions shared across IVS modules
 */

#ifndef IVS_H
#define IVS_H

#define VLAN_CFI_BIT (1<<12)
#define VLAN_TCI(vid, pcp) ( (((pcp) & 0x7) << 13) | ((vid) & 0xfff) )
#define VLAN_VID(tci) ((tci) & 0xfff)
#define VLAN_PCP(tci) ((tci) >> 13)

/* Same as VLAN_TCI above except the vid includes the CFI bit */
#define VLAN_TCI_WITH_CFI(vid, pcp) ( (((pcp) & 0x7) << 13) | ((vid) & 0x1fff) )

#define IP_DSCP_MASK 0xfc
#define IP_ECN_MASK 0x03
#define IPV6_FLABEL_MASK 0x000fffff

#define ARRAY_SIZE(a)  (sizeof(a) / sizeof((a)[0]))

#define ALIGN8(x) (((x) + 7) & ~7)

/*
 * Derived from a flow's actions/instructions.
 */
struct ind_ovs_flow_effects {
    struct xbuf apply_actions;
    struct xbuf write_actions;
    uint64_t metadata;
    uint64_t metadata_mask;
    uint32_t meter_id;
    uint8_t next_table_id;
    unsigned clear_actions : 1;
};

struct ind_ovs_flow_stats {
    uint64_t packets;
    uint64_t bytes;
};

/*
 * Canonical Flow Representation
 * Compressed version of the OpenFlow match fields for use in matching.
 * Does not contain the non-OpenFlow fields of the flow key.
 * Only contains OF 1.0 fields for now.
 * Wildcarded fields must be zeroed in the flow entry's CFR.
 * sizeof(struct ind_ovs_cfr) must be a multiple of 8.
 * All fields are in network byte order except in_port.
 */

struct ind_ovs_cfr {
    uint8_t dl_dst[6];          /* Ethernet destination address. */
    uint8_t dl_src[6];          /* Ethernet source address. */
    uint16_t in_port;           /* Input switch port. */
    uint16_t dl_type;           /* Ethernet frame type. */
    uint16_t dl_vlan;           /* VLAN id and priority, same as wire format
                                   plus CFI bit set if tag present. */
    uint8_t nw_tos;             /* IPv4 DSCP. */
    uint8_t nw_proto;           /* IP protocol. */
    uint32_t nw_src;            /* IP source address. */
    uint32_t nw_dst;            /* IP destination address. */
    uint16_t tp_src;            /* TCP/UDP source port. */
    uint16_t tp_dst;            /* TCP/UDP destination port. */
    uint32_t ipv6_src[4];       /* IPv6 source address. */
    uint32_t ipv6_dst[4];       /* IPv6 destination address. */
} __attribute__ ((aligned (8)));

AIM_STATIC_ASSERT(CFR_SIZE, sizeof(struct ind_ovs_cfr) == 8*8);

#endif
