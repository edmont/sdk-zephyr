
#include <zephyr/kernel.h>
#include <zephyr/net/openthread.h>
#include <openthread/thread.h>
#include <openthread/radio_stats.h>

#include <zephyr/logging/log.h>
LOG_MODULE_REGISTER(csl_tst, LOG_LEVEL_INF);

#define POLL_PERIOD 1000

uint8_t conf_role = 0; /* 0 = Leader, 1 = SED, 2 = SSED */
uint8_t conf_accuracy;
uint8_t conf_uncert;

uint32_t result_total_time_ms;
uint32_t result_disabled_time_ms;
uint32_t result_sleep_time_ms;
uint32_t result_rx_time_ms;
uint32_t result_tx_time_ms;

static struct openthread_context *context;

void start_stats(void)
{
    otRadioTimeStatsReset(context->instance);
}

void gather_stats(void)
{
    const otRadioTimeStats *radioStats = otRadioTimeStatsGet(context->instance);

    result_total_time_ms = (radioStats->mDisabledTime + radioStats->mSleepTime + radioStats->mTxTime + radioStats->mRxTime) / 1000;
    result_disabled_time_ms = radioStats->mDisabledTime/1000;
    result_sleep_time_ms = radioStats->mSleepTime/1000;
    result_rx_time_ms = radioStats->mRxTime/1000;
    result_tx_time_ms = radioStats->mTxTime/1000;
}

int main(void)
{
    context = openthread_get_default_context();

    LOG_INF("conf_role=%u, conf_accuracy=%u, conf_uncert=%u", conf_role, conf_accuracy, conf_uncert);

    if (conf_role != 0)
    {
        otLinkModeConfig linkMode;

        linkMode.mRxOnWhenIdle = false;
        linkMode.mDeviceType = false;
        linkMode.mNetworkData = false;
        otThreadSetLinkMode(context->instance, linkMode);
    }

    if (conf_role == 1)
    {
        otLinkSetPollPeriod(context->instance, POLL_PERIOD);
    }

    if (conf_role == 2)
    {
        otLinkSetCslPeriod(context->instance, POLL_PERIOD*1000);
    }

	openthread_start(context);

    return 0;
}
