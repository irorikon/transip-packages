/*
 * @Author: iRorikon
 * @Date: 2024-04-18 22:58:14
 * @FilePath: \transip-packages\transip-info\src\transip-info.h
 */
#ifndef __TEANSIP_INFO_H
#define __TEANSIP_INFO_H

#include <linux/proc_fs.h>
#include <linux/sizes.h>
#include <linux/if.h>
#include <linux/types.h>
#include <linux/slab.h>

#define TEANSIP_INFO_DRV_NAME "transip-info_v1.0"
#define PROC_DIR "transip-hw-info"

#define MAC_LEN 6
#define SN_LEN 16
#define COUNTRY_LEN 2

struct transip_hw_info {
    struct proc_dir_entry *parent;
    u8 device_mac[18];
    u8 device_sn[SN_LEN + 1];
};

extern struct transip_hw_info transip_info;

struct proc_dir_entry *create_proc_node(const char *name, const char *value);
int proc_init_transip_info(void);
int proc_remove_transip_info(void);

void make_factory_data(struct device_node *np);

const char *lookup_country(int iso3166);

#endif