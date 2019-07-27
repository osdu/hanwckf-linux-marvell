// SPDX-License-Identifier: GPL-2.0
/*
 * Simple, generic PCI host controller driver targetting firmware-initialised
 * systems and virtual machines (e.g. the PCI emulation provided by kvmtool).
 *
 * Copyright (C) 2014 ARM Limited
 *
 * Author: Will Deacon <will.deacon@arm.com>
 */

#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/of_address.h>
#include <linux/of_pci.h>
#include <linux/pci-ecam.h>
#include <linux/platform_device.h>

static struct pci_ecam_ops gen_pci_cfg_cam_bus_ops = {
	.bus_shift	= 16,
	.pci_ops	= {
		.map_bus	= pci_ecam_map_bus,
		.read		= pci_generic_config_read,
		.write		= pci_generic_config_write,
	}
};

static const struct of_device_id gen_pci_of_match[] = {
	{ .compatible = "pci-host-cam-generic",
	  .data = &gen_pci_cfg_cam_bus_ops },

	{ .compatible = "pci-host-ecam-generic",
	  .data = &pci_generic_ecam_ops },

	{ },
};

static int gen_pci_probe(struct platform_device *pdev)
{
	const struct of_device_id *of_id;
	struct pci_ecam_ops *ops;

	of_id = of_match_node(gen_pci_of_match, pdev->dev.of_node);
	ops = (struct pci_ecam_ops *)of_id->data;

	return pci_host_common_probe(pdev, ops);
}

static struct platform_driver gen_pci_driver = {
	.driver = {
		.name = "pci-host-generic",
		.of_match_table = gen_pci_of_match,
		.suppress_bind_attrs = true,
	},
	.probe = gen_pci_probe,
};
builtin_platform_driver(gen_pci_driver);