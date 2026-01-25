// SPDX-License-Identifier: GPL-2.0-only
/*
 * Copyright (c) Qualcomm Technologies, Inc. and/or its subsidiaries.
 */
#include <linux/clk-provider.h>
#include <linux/mod_devicetable.h>
#include <linux/module.h>
#include <linux/platform_device.h>
#include <linux/pm_runtime.h>
#include <linux/regmap.h>

#include <dt-bindings/clock/qcom,sm8750-gpucc.h>

#include "clk-alpha-pll.h"
#include "clk-branch.h"
#include "clk-rcg.h"
#include "clk-regmap.h"
#include "clk-regmap-divider.h"
#include "clk-regmap-mux.h"
#include "gdsc.h"
#include "reset.h"

static struct gdsc gx_cc_gx_gdsc = {
	.gdscr = 0x4024,
	.en_rest_wait_val = 0x2,
	.en_few_wait_val = 0x2,
	.clk_dis_wait_val = 0xf,
	.pd = {
		.name = "gx_cc_gx_gdsc",
		.power_on = gdsc_gx_do_nothing_enable,
	},
	.pwrsts = PWRSTS_OFF_ON,
	.flags = POLL_CFG_GDSCR | RETAIN_FF_ENABLE,
};

static struct gdsc *gx_cc_gdscs[] = {
	[GX_CC_GX_GDSC] = &gx_cc_gx_gdsc,
};

static const struct regmap_config gx_cc_regmap_config = {
	.reg_bits = 32,
	.reg_stride = 4,
	.val_bits = 32,
	.max_register = 0x6000,
	.fast_io = true,
};

static const struct qcom_cc_desc gx_cc_sm8750_desc = {
	.config = &gx_cc_regmap_config,
	.gdscs = gx_cc_gdscs,
	.num_gdscs = ARRAY_SIZE(gx_cc_gdscs),
	.use_rpm = true,
};

static const struct of_device_id gx_cc_sm8750_match_table[] = {
	{ .compatible = "qcom,sm8750-gxcc" },
	{ }
};
MODULE_DEVICE_TABLE(of, gx_cc_sm8750_match_table);

static int gx_cc_sm8750_probe(struct platform_device *pdev)
{
	return qcom_cc_probe(pdev, &gx_cc_sm8750_desc);
}

static struct platform_driver gx_cc_sm8750_driver = {
	.probe = gx_cc_sm8750_probe,
	.driver = {
		.name = "gx_cc-sm8750",
		.of_match_table = gx_cc_sm8750_match_table,
	},
};
module_platform_driver(gx_cc_sm8750_driver);

MODULE_DESCRIPTION("QTI GX_CC SM8750 Driver");
MODULE_LICENSE("GPL");
