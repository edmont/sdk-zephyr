/*
 * Copyright (c) 2023 Nordic Semiconductor ASA
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include "bs_types.h"
#include "bs_tracing.h"
#include "bs_utils.h"
#include "time_machine.h"
#include "bstests.h"

#include <zephyr/kernel.h>
#include <openthread/radio_stats.h>

#define START_TIME 15 /* Seconds */
#define MEASURE_TIME 100 /* Seconds */

static bool stats_started = false;

extern enum bst_result_t bst_result;

extern uint8_t conf_role;
extern uint8_t conf_accuracy;
extern uint8_t conf_uncert;

extern uint32_t result_total_time_ms;
extern uint32_t result_disabled_time_ms;
extern uint32_t result_sleep_time_ms;
extern uint32_t result_rx_time_ms;
extern uint32_t result_tx_time_ms;

#define FAIL(...)					\
	do {						\
		bst_result = Failed;			\
		bs_trace_error_time_line(__VA_ARGS__);	\
	} while (0)

#define PASS(...)					\
	do {						\
		bst_result = Passed;			\
		bs_trace_info_time(1, __VA_ARGS__);	\
	} while (0)

static void test_csl_init(void)
{
	bs_trace_info_time(1, "role: %u\n", conf_role);

	if (conf_role == 0)
	{
		PASS("Parent test passed.\n");
	}
	else
	{
		bs_trace_info_time(1, "next event: %u s\n", START_TIME);
		bst_ticker_set_next_tick_absolute(START_TIME*1e6);
		bst_result = In_progress;
	}
}

static void test_csl_tick(bs_time_t HW_device_time)
{
	extern void start_stats(void);
	extern void gather_stats(void);

	if (!stats_started)
	{

		start_stats();
		stats_started = true;
		bs_trace_info_time(1, "next event: %u s\n", MEASURE_TIME);
		bst_ticker_set_next_tick_delta(MEASURE_TIME*1e6);
	}
	else
	{
		gather_stats();
		bs_trace_info_time(1, "Total time: %lu ms\n", result_total_time_ms);
		bs_trace_info_time(1, "Disabled time: %lu ms\n", result_disabled_time_ms);
		bs_trace_info_time(1, "Sleep time: %lu ms\n", result_sleep_time_ms);
		bs_trace_info_time(1, "Rx time: %lu ms\n", result_rx_time_ms);
		bs_trace_info_time(1, "Tx time: %lu ms\n", result_tx_time_ms);

		PASS("Child test passed.\n");
		bs_trace_exit("Done, disconnecting from simulation\n");
	}
}

void test_args(int argc, char **argv)
{
	conf_role = atol(argv[0]);

	if (argc == 3)
	{
		conf_accuracy = atol(argv[1]);
		conf_uncert = atol(argv[2]);
	}
}

static const struct bst_test_instance test_def[] = {
	{
		.test_id = "parent",
		.test_descr = "CSL Transmitter role",
		.test_post_init_f = test_csl_init,
		.test_args_f = test_args,
	},
	{
		.test_id = "child",
		.test_descr = "SED/SSED role",
		.test_post_init_f = test_csl_init,
		.test_tick_f = test_csl_tick,
		.test_args_f = test_args,
	},
	BSTEST_END_MARKER,
};

struct bst_test_list *test_csl_install(struct bst_test_list *tests)
{
	return bst_add_tests(tests, test_def);
}
