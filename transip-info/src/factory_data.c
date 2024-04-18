#include <linux/module.h>
#include <linux/module.h>
#include <linux/sizes.h>
#include <linux/of_address.h>
#include <linux/of_platform.h>
#include <linux/etherdevice.h>
#include <linux/mtd/mtd.h>
#include <linux/blkdev.h>
#include <linux/version.h>
#include <linux/pagemap.h>

#include "transip-info.h"

static int block_part_read(const char *part, unsigned int from,
                  void *val, size_t bytes)
{
    pgoff_t index = from >> PAGE_SHIFT;
    int offset = from & (PAGE_SIZE - 1);
    const fmode_t mode = FMODE_READ;
    struct block_device *bdev;
    struct page *page;
    char *buf = val;
    int cpylen;

#if LINUX_VERSION_CODE < KERNEL_VERSION(6, 5, 0)
    bdev = blkdev_get_by_path(part, mode, NULL);
#else
    bdev = blkdev_get_by_path(part, mode, NULL, NULL);
#endif
    if (IS_ERR(bdev))
        return -1;

    while (bytes) {
        if ((offset + bytes) > PAGE_SIZE)
            cpylen = PAGE_SIZE - offset;
        else
            cpylen = bytes;

        page = read_mapping_page(bdev->bd_inode->i_mapping, index, NULL);
        if (IS_ERR(page)) {
            blkdev_put(bdev, mode);
            return PTR_ERR(page);
        }

        memcpy(buf, page_address(page) + offset, cpylen);
        put_page(page);

        buf += cpylen;
        bytes -= cpylen;  // Fix: Correctly update remaining bytes to read

        // Fix: Update index and offset for next iteration
        if (bytes) {
            offset = 0;
            index++;
        }
    }

#if LINUX_VERSION_CODE < KERNEL_VERSION(6, 5, 0)
    blkdev_put(bdev, mode);
#else
    blkdev_put(bdev, NULL);
#endif

    return 0;
}

#ifdef CONFIG_MTD
static int parse_mtd_value(struct device_node *np, const char *prop,
                           void *dest, int len)
{
    const char *part, *offset_str;
    struct mtd_info *mtd;
    unsigned long offset = 0;  // 使用unsigned long以匹配simple_strtoul的返回值
    size_t retlen;
    int ret;

    ret = of_property_read_string_index(np, prop, 0, &part);
    if (ret)
        return -1;

    ret = of_property_read_string_index(np, prop, 1, &offset_str);
    if (!ret) {
        offset = simple_strtoul(offset_str, NULL, 0);
    }

    if (!strncmp(part, "/dev/mmc", 8))
        return block_part_read(part, offset, dest, len);

    mtd = get_mtd_device_nm(part);
    if (IS_ERR(mtd))
        return -1;

    mtd_read(mtd, offset, len, &retlen, dest);
    if (retlen != (size_t)len) {
        // 添加错误处理逻辑，例如打印警告或返回错误代码
        printk(KERN_WARNING "Failed to read the requested length from MTD device.\n");
        put_mtd_device(mtd);
        return -EIO;  // 或其他适当的错误代码
    }

    put_mtd_device(mtd);

    return 0;
}
#endif

static void make_device_country_code(struct device_node *np)
{
#ifdef CONFIG_MTD
    __be16 country_code = 0;

    if (parse_mtd_value(np, "country_code", &country_code, COUNTRY_LEN))
        return;

    create_proc_node("country_code", lookup_country(be16_to_cpu(country_code)));
#endif
}

static void make_device_sn(struct device_node *np)
{
#ifdef CONFIG_MTD
    if (parse_mtd_value(np, "device_sn", transip_info.device_sn, SN_LEN))
        return;

    if (strlen(transip_info.device_sn))
        create_proc_node("device_sn", transip_info.device_sn);
#endif
}

static void make_device_mac(struct device_node *np)
{
#ifdef CONFIG_MTD
    u8 mac[ETH_ALEN];

    if (parse_mtd_value(np, "device_mac", mac, MAC_LEN))
        return;

    if (is_valid_ether_addr(mac)) {
        snprintf(transip_info.device_mac, sizeof(transip_info.device_mac), "%pM", mac);
        create_proc_node("device_mac", transip_info.device_mac);
    }
#endif
}

void make_factory_data(struct device_node *np)
{
    make_device_mac(np);
    make_device_sn(np);
    make_device_country_code(np);
}