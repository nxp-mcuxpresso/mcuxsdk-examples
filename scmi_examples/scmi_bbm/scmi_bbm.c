/*
 * Copyright 2026 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "app.h"
#include "board.h"
#include "fsl_debug_console.h"
#include "scmi.h"
#include "scmi_bbm.h"
#include "fsl_datetime.h"
#include "scmi_common.h"

/* Per-feature test switches.
 *
 * Every BBM feature group exercised by this example can be enabled or disabled
 * independently, because not all of them can be tested on every target. For
 * example an emulation platform such as Zebu has no physical ON/OFF button, so
 * DEMO_BBM_TEST_BUTTON has to be turned off there.
 *
 * A board or target overrides a switch either in its app.h or on the compiler
 * command line, for example -DDEMO_BBM_TEST_BUTTON=0.
 */
#ifndef DEMO_BBM_TEST_RTC
#define DEMO_BBM_TEST_RTC 1
#endif
#ifndef DEMO_BBM_TEST_BUTTON
#define DEMO_BBM_TEST_BUTTON 0
#endif
#ifndef DEMO_BBM_TEST_RTC_TIME
#define DEMO_BBM_TEST_RTC_TIME 1
#endif
#ifndef DEMO_BBM_TEST_RTC_ALARM
#define DEMO_BBM_TEST_RTC_ALARM 1
#endif
#ifndef DEMO_BBM_TEST_GPR
#define DEMO_BBM_TEST_GPR 0
#endif

/* The RTC time and alarm groups only make sense while the RTC group itself
 * runs, because they reuse its RTC instance.
 */
#if (DEMO_BBM_TEST_RTC == 0)
#undef DEMO_BBM_TEST_RTC_TIME
#define DEMO_BBM_TEST_RTC_TIME 0
#undef DEMO_BBM_TEST_RTC_ALARM
#define DEMO_BBM_TEST_RTC_ALARM 0
#endif

/* The alarm is programmed relative to the time this example writes, so the
 * alarm check needs the time group.
 */
#if (DEMO_BBM_TEST_RTC_TIME == 0) && (DEMO_BBM_TEST_RTC_ALARM != 0)
#undef DEMO_BBM_TEST_RTC_ALARM
#define DEMO_BBM_TEST_RTC_ALARM 0
#endif

/* SCMI agent-to-platform channel used by all BBM requests. The channel is
 * configured by SystemPlatformInit through SMT_ChannelConfig, so this example
 * only needs board init.
 */
#define DEMO_SCMI_CHANNEL  SCMI_A2P
/* RTC instance exercised by this example. */
#define DEMO_RTC_ID        0U
/* Persistent GPR index used for the read-back check. */
#define DEMO_GPR_INDEX     0U
/* Arbitrary pattern written to the GPR. */
#define DEMO_GPR_PATTERN   0x5A5AA5A5U
/*! Number of distinct dates the RTC time check writes */
#define DEMO_RTC_SET_COUNT  2U
/*! Seconds the read-back value may exceed the value written.
 *
 * The counter keeps running after the write, so the read-back is allowed to be
 * slightly ahead. The window is small enough that a stale or constant value
 * from a different date still fails the check.
 */
#define DEMO_RTC_READ_BACK_WINDOW_SECONDS 60U
/*! Alarm is programmed this many seconds after the time just written */
#define DEMO_RTC_ALARM_OFFSET_SECONDS 10U

static bool s_failed = false;

/*!
 * Every BBM message this protocol defines, queried one by one so the log
 * states which of them the platform actually implements.
 */
typedef struct
{
    /*! Message ID as defined by the BBM protocol */
    uint32_t messageId;
    /*! Name printed in the log */
    const char *name;
} demo_bbm_message_t;

static const demo_bbm_message_t s_bbmMessages[] = {
    {SCMI_MSG_BBM_GPR_SET, "GPR_SET"},
    {SCMI_MSG_BBM_GPR_GET, "GPR_GET"},
    {SCMI_MSG_BBM_RTC_ATTRIBUTES, "RTC_ATTRIBUTES"},
    {SCMI_MSG_BBM_RTC_TIME_SET, "RTC_TIME_SET"},
    {SCMI_MSG_BBM_RTC_TIME_GET, "RTC_TIME_GET"},
    {SCMI_MSG_BBM_RTC_ALARM_SET, "RTC_ALARM_SET"},
    {SCMI_MSG_BBM_BUTTON_GET, "BUTTON_GET"},
    {SCMI_MSG_BBM_RTC_NOTIFY, "RTC_NOTIFY"},
    {SCMI_MSG_BBM_BUTTON_NOTIFY, "BUTTON_NOTIFY"},
    {SCMI_MSG_BBM_RTC_STATE, "RTC_STATE"}
};

/* Report one SCMI call. Returns true when the call succeeded.
 *
 * Resource ownership is a System Manager configuration decision, so a request
 * for a resource this logical machine does not own is answered with
 * SCMI_ERR_DENIED, and a protocol feature the platform does not implement is
 * answered with SCMI_ERR_NOT_SUPPORTED. Neither is a defect, so such a request
 * is reported as skipped and does not fail the example.
 */
static bool CheckStatus(const char *what, int32_t status)
{
    if ((status == SCMI_ERR_DENIED) || (status == SCMI_ERR_NOT_SUPPORTED))
    {
        PRINTF("  %s: SKIPPED, not granted to this agent (%s, %d)\r\n", what,
               SCMI_StatusToString(status), (int)status);
        return false;
    }

    if (status != SCMI_ERR_SUCCESS)
    {
        PRINTF("  %s: FAILED, %s (%d)\r\n", what, SCMI_StatusToString(status),
               (int)status);
        s_failed = true;
        return false;
    }

    return true;
}

#if DEMO_BBM_TEST_RTC_TIME || DEMO_BBM_TEST_RTC_ALARM
/*!
 * Print a calendar date and time in ISO 8601 style.
 */
static void PrintDateTime(const char *what, const datetime_t *dt)
{
    PRINTF("  %s: %04d-%02d-%02d %02d:%02d:%02d UTC\r\n", what, dt->year, dt->month,
           dt->day, dt->hour, dt->minute, dt->second);
}
#endif /* DEMO_BBM_TEST_RTC_TIME || DEMO_BBM_TEST_RTC_ALARM */

#if DEMO_BBM_TEST_RTC_TIME
/*!
 * Dates the RTC time check writes, in order.
 *
 * Two clearly different dates are used so the read-back proves the value
 * followed the write. A single date cannot distinguish a real write from an
 * implementation that returns a constant or a value left over from an earlier
 * run.
 */
static const datetime_t s_rtcSetTimes[DEMO_RTC_SET_COUNT] = {
    {2026U, 1U, 1U, 0U, 0U, 0U},   /* 2026-01-01 00:00:00 UTC */
    {2030U, 6U, 15U, 12U, 34U, 56U} /* 2030-06-15 12:34:56 UTC */
};
#endif /* DEMO_BBM_TEST_RTC_TIME */

int main(void)
{
    int32_t status;
    uint32_t version    = 0U;
    uint32_t attributes = 0U;
    uint32_t msg;
#if DEMO_BBM_TEST_RTC
    uint32_t state = 0U;
    uint8_t name[SCMI_BBM_MAX_NAME + 1U];
    scmi_rtc_time_t rtcTime;
    const uint32_t *rtcWords = (const uint32_t *)&rtcTime;
#endif
#if DEMO_BBM_TEST_RTC_TIME
    datetime_t readTime;
    uint32_t setSeconds = 0U;
    uint32_t readSeconds;
    uint32_t pass;
#endif
#if DEMO_BBM_TEST_RTC_ALARM
    datetime_t alarmTime;
#endif
#if DEMO_BBM_TEST_BUTTON
    uint32_t button = 0U;
#endif
#if DEMO_BBM_TEST_GPR
    uint32_t readBack = 0U;
#endif

    BOARD_InitHardware();

    PRINTF("\r\nSCMI BBM protocol example\r\n");
    PRINTF("Exercises the BBM protocol of the System Manager so that BBNSM\r\n");
    PRINTF("features can be verified without booting Linux.\r\n");
    /* Say which target each group is valid on, so a reader of the log knows
     * whether a result means anything on the target it came from. "any" runs
     * on emulation and on silicon alike, while "silicon" needs a target that
     * runs at real speed or has the physical part.
     */
    PRINTF("Enabled tests, with the target each one is valid on:\r\n");
    PRINTF("  protocol queries : yes (any)\r\n");
    PRINTF("  RTC              : %s (any)\r\n",
           (DEMO_BBM_TEST_RTC != 0) ? "yes" : "no");
    PRINTF("  RTC time         : %s (any)\r\n",
           (DEMO_BBM_TEST_RTC_TIME != 0) ? "yes" : "no");
    PRINTF("  RTC alarm        : %s (any to program, silicon to fire)\r\n",
           (DEMO_BBM_TEST_RTC_ALARM != 0) ? "yes" : "no");
    PRINTF("  button           : %s (silicon)\r\n",
           (DEMO_BBM_TEST_BUTTON != 0) ? "yes" : "no");
    PRINTF("  GPR              : %s (any)\r\n\r\n",
           (DEMO_BBM_TEST_GPR != 0) ? "yes" : "no");

    status = SCMI_BbmProtocolVersion(DEMO_SCMI_CHANNEL, &version);
    if (CheckStatus("protocol version", status))
    {
        PRINTF("  protocol version: 0x%08X\r\n", version);
    }

    status = SCMI_BbmProtocolAttributes(DEMO_SCMI_CHANNEL, &attributes);
    if (CheckStatus("protocol attributes", status))
    {
        PRINTF("  RTC count: %u, GPR count: %u\r\n",
               (unsigned int)SCMI_BBM_PROTO_ATTR_NUM_RTC(attributes),
               (unsigned int)SCMI_BBM_PROTO_ATTR_NUM_GPR(attributes));
    }

    /* Ask about each message in turn. A message the platform does not
     * implement answers with SCMI_ERR_NOT_FOUND, which explains up front why a
     * later request using it would not work, rather than leaving that to be
     * discovered when the request itself fails.
     */
    PRINTF("  implemented messages:\r\n");
    for (msg = 0U; msg < (sizeof(s_bbmMessages) / sizeof(s_bbmMessages[0]));
         msg++)
    {
        attributes = 0U;
        status = SCMI_BbmProtocolMessageAttributes(
            DEMO_SCMI_CHANNEL, s_bbmMessages[msg].messageId, &attributes);
        if (status == SCMI_ERR_SUCCESS)
        {
            PRINTF("    %-16s (0x%02X): yes, attributes 0x%08X\r\n",
                   s_bbmMessages[msg].name,
                   (unsigned int)s_bbmMessages[msg].messageId, attributes);
        }
        else if (status == SCMI_ERR_NOT_FOUND)
        {
            PRINTF("    %-16s (0x%02X): no\r\n", s_bbmMessages[msg].name,
                   (unsigned int)s_bbmMessages[msg].messageId);
        }
        else
        {
            /* Any other code means the query itself went wrong. */
            (void)CheckStatus("protocol message attributes", status);
            break;
        }
    }

    /* Negotiate the version that was just read back. Asking for the version
     * the platform reported must succeed, so this both exercises the message
     * and confirms the two agree on which version is in use.
     */
    status = SCMI_BbmNegotiateProtocolVersion(DEMO_SCMI_CHANNEL, version);
    if (CheckStatus("negotiate protocol version", status))
    {
        PRINTF("  negotiated protocol version: 0x%08X\r\n", version);
    }

#if DEMO_BBM_TEST_RTC
    (void)memset(name, 0, sizeof(name));
    status = SCMI_BbmRtcAttributes(DEMO_SCMI_CHANNEL, DEMO_RTC_ID,
                                   &attributes, name);
    if (CheckStatus("RTC attributes", status))
    {
        PRINTF("  RTC %u name: %s\r\n", DEMO_RTC_ID, (char *)name);
        PRINTF("  seconds width: %u, tick width: %u, ticks per second: %u\r\n",
               (unsigned int)SCMI_BBM_RTC_ATTR_SEC_WIDTH(attributes),
               (unsigned int)SCMI_BBM_RTC_ATTR_TICK_WIDTH(attributes),
               (unsigned int)SCMI_BBM_RTC_ATTR_TICKS(attributes));
    }

    (void)memset(&rtcTime, 0, sizeof(rtcTime));
    status = SCMI_BbmRtcTimeGet(DEMO_SCMI_CHANNEL, DEMO_RTC_ID,
                                SCMI_BBM_RTC_FLAGS_TICKS(0U), &rtcTime);
    if (CheckStatus("RTC time get", status))
    {
        PRINTF("  RTC time (seconds): 0x%08X%08X\r\n", rtcWords[1],
               rtcWords[0]);
    }

#endif /* DEMO_BBM_TEST_RTC */

#if DEMO_BBM_TEST_RTC_TIME
    /* Write each date in turn and read it back. A read-back that follows the
     * value just written proves the whole path from this agent through the
     * System Manager into the RTC register. It does not prove the counter is
     * advancing at the right rate, which needs a target that runs at real
     * speed.
     */
    PRINTF("  RTC time write path: writing %u distinct dates\r\n",
           (unsigned int)DEMO_RTC_SET_COUNT);

    for (pass = 0U; pass < DEMO_RTC_SET_COUNT; pass++)
    {
        setSeconds    = DATETIME_ToSeconds(&s_rtcSetTimes[pass]);
        rtcTime.lower = setSeconds;
        rtcTime.upper = 0U;

        status = SCMI_BbmRtcTimeSet(DEMO_SCMI_CHANNEL, DEMO_RTC_ID,
                                    SCMI_BBM_RTC_FLAGS_TICKS(0U), rtcTime);
        if (!CheckStatus("SCMI_BbmRtcTimeSet", status))
        {
            break;
        }

        PrintDateTime("RTC time set to", &s_rtcSetTimes[pass]);

        status = SCMI_BbmRtcTimeGet(DEMO_SCMI_CHANNEL, DEMO_RTC_ID,
                                    SCMI_BBM_RTC_FLAGS_TICKS(0U), &rtcTime);
        if (!CheckStatus("SCMI_BbmRtcTimeGet after set", status))
        {
            break;
        }

        readSeconds = rtcTime.lower;
        DATETIME_FromSeconds(readSeconds, &readTime);
        PrintDateTime("RTC time read back", &readTime);

        if ((readSeconds < setSeconds) ||
            ((readSeconds - setSeconds) > DEMO_RTC_READ_BACK_WINDOW_SECONDS))
        {
            PRINTF("  RTC time read back does not follow the value written, "
                   "wrote %u read %u\r\n",
                   (unsigned int)setSeconds, (unsigned int)readSeconds);
            s_failed = true;
            break;
        }

        PRINTF("  RTC time write path: PASS for date %u\r\n",
               (unsigned int)(pass + 1U));
    }

    PRINTF("  RTC time write path checked; time advance is not verified "
           "here\r\n");

#endif /* DEMO_BBM_TEST_RTC_TIME */

#if DEMO_BBM_TEST_RTC_ALARM
    /* Program an RTC alarm a few seconds in the future and enable it. */
    rtcTime.lower = setSeconds + DEMO_RTC_ALARM_OFFSET_SECONDS;
    rtcTime.upper = 0U;
    status = SCMI_BbmRtcAlarmSet(DEMO_SCMI_CHANNEL, DEMO_RTC_ID,
                                 SCMI_BBM_ALARM_FLAGS_ENABLE(1U), rtcTime);
    if (CheckStatus("SCMI_BbmRtcAlarmSet", status))
    {
        DATETIME_FromSeconds(rtcTime.lower, &alarmTime);
        PrintDateTime("RTC alarm enabled at", &alarmTime);
    }

    /* Request RTC alarm notifications so the alarm can be observed. */
    status = SCMI_BbmRtcNotify(DEMO_SCMI_CHANNEL, DEMO_RTC_ID,
                               SCMI_BBM_NOTIFY_RTC_ALARM(1U));
    if (CheckStatus("SCMI_BbmRtcNotify", status))
    {
        PRINTF("  RTC alarm notification enabled\r\n");
    }

#endif /* DEMO_BBM_TEST_RTC_ALARM */

#if DEMO_BBM_TEST_RTC
    status = SCMI_BbmRtcState(DEMO_SCMI_CHANNEL, DEMO_RTC_ID, &state);
    if (CheckStatus("RTC state", status))
    {
        PRINTF("  RTC battery low: %u, RTC was reset: %u\r\n",
               (unsigned int)SCMI_BBM_RTC_STATE_BATT_LOW(state),
               (unsigned int)SCMI_BBM_RTC_STATE_RESET(state));
    }
#endif /* DEMO_BBM_TEST_RTC */

#if DEMO_BBM_TEST_GPR
    status = SCMI_BbmGprSet(DEMO_SCMI_CHANNEL, DEMO_GPR_INDEX,
                            DEMO_GPR_PATTERN);
    if (CheckStatus("GPR set", status))
    {
        status = SCMI_BbmGprGet(DEMO_SCMI_CHANNEL, DEMO_GPR_INDEX, &readBack);
        if (CheckStatus("GPR get", status))
        {
            if (readBack == DEMO_GPR_PATTERN)
            {
                PRINTF("  GPR %u round trip: PASS (0x%08X)\r\n",
                       DEMO_GPR_INDEX, readBack);
            }
            else
            {
                PRINTF("  GPR %u round trip: FAILED, wrote 0x%08X read "
                       "0x%08X\r\n",
                       DEMO_GPR_INDEX, DEMO_GPR_PATTERN, readBack);
                s_failed = true;
            }
        }
    }
#endif /* DEMO_BBM_TEST_GPR */

#if DEMO_BBM_TEST_BUTTON
    status = SCMI_BbmButtonGet(DEMO_SCMI_CHANNEL, &button);
    if (CheckStatus("button get", status))
    {
        PRINTF("  ON/OFF button state: %u\r\n", (unsigned int)button);
    }
#endif /* DEMO_BBM_TEST_BUTTON */

    (void)status;

    if (s_failed)
    {
        PRINTF("\r\nSCMI BBM example result: FAILED\r\n");
    }
    else
    {
        PRINTF("\r\nSCMI BBM example result: PASS\r\n");
    }

    while (1)
    {
    }
}
