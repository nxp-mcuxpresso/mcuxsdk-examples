/**@file ncp_host_command_system.h
 *
 *  Copyright 2008-2023 NXP
 *
 *  SPDX-License-Identifier: BSD-3-Clause
 *
 */

#ifndef __NCP_HOST_COMMAND_SYSTEM_H_
#define __NCP_HOST_COMMAND_SYSTEM_H_

/**
 * This API is used to set a configuration parameter value in flash.
 *
 * \param[in] argc    Argument count, the number of strings pointed to by argv, \n
 *                    argc should be 4.
 * \param[in] argv    Argument vector, \n
 *                    argv[0]: ncp-set \n
 *                    argv[1]: string value of module_name (Required) \n
 *                             This parameter takes a valid module name. \n
 *                    argv[2]: string value of variable_name (Required) \n
 *                             This parameter takes a valid parameter name under the module name specified. \n
 *                    argv[3]: string value of value (Required) \n
 *                             This parameter takes a valid value for the module name and parameter name specified. \n
 *
 * \return NCP_SUCCESS if success.
 * \return -NCP_FAIL if failure.
 */
int ncp_set_command(int argc, char **argv);

/**
 * This API can be used to process set configuration response.
 *
 * \param[in] res    A pointer to \ref MCU_NCPCmd_DS_COMMAND response.
 *
 * \return NCP_SUCCESS if success.
 * \return -NCP_FAIL if failure.
 */
int ncp_process_set_cfg_response(uint8_t *res);

/**
 * This API is used to get a configuration parameter value in flash.
 *
 * \param[in] argc    Argument count, the number of strings pointed to by argv, \n
 *                    argc should be 3.
 * \param[in] argv    Argument vector, \n
 *                    argv[0]: ncp-get \n
 *                    argv[1]: string value of module_name (Required) \n
 *                             This parameter takes a valid module name. \n
 *                    argv[2]: string value of variable_name (Required) \n
 *                             This parameter takes a valid parameter name under the module name specified. \n
 *
 * \return NCP_SUCCESS if success.
 * \return -NCP_FAIL if failure.
 */
int ncp_get_command(int argc, char **argv);

/**
 * This API can be used to process get configuration response.
 *
 * \param[in] res    A pointer to \ref MCU_NCPCmd_DS_COMMAND response.
 *
 * \return NCP_SUCCESS if success.
 * \return -NCP_FAIL if failure.
 */
int ncp_process_get_cfg_response(uint8_t *res);

/**
 * This API is used to configure how NCP host wakes up NCP device.
 * Also, user can configure to subscribe/unsubscribe sleep status event of NCP device.
 *
 * \note If this command is never specified by user, the default wake mode is GPIO with subscribe event.
 *
 * \param[in] argc    Argument count, the number of strings pointed to by argv,
 *                    argc should be 4.
 * \param[in] argv    Argument vector.\n
 *                    argv[0]: ncp-wake-cfg\n
 *                    argv[1]: String of wake mode (Required)\n
 *                             INTF -- host wakes up NCP device with interface interrupt\n
 *                             GPIO -- host wakes up NCP device with GPIO interrupt\n
 *                             WIFI-NB -- For FRDMRW612 v2 board as NCP device only\n
 *                                        Host can't wake up NCP device with this mode\n
 *                                        NCP device can wake up by itself with WIFI or NB\n
 *                    argv[2]: String value of subscribe events (Required)\n
 *                             0 -- Unsubscribe NCP device sleep status event\n
 *                             1 -- ubscribe NCP device sleep status event\n
 *                    argv[3]: String value of wake duration (Required)\n
 *                             Unit is second. Default value is 5.
 *
 * \note NCP device will keep full power within argv[3] duration after waking
 * up, before entering low power for next round.
 *
 * \return NCP_SUCCESS if the call was successful.
 * \return -NCP_FAIL if failed.
 */
int ncp_wake_cfg_command(int argc, char **argv);

/**
 * This API can be used to process wake cfg response.
 *
 * \param[in] res    A pointer to \ref MCU_NCPCmd_DS_COMMAND response.
 *
 * \return NCP_SUCCESS.
 */
int ncp_process_wake_cfg_response(uint8_t *res);

/**
 * This API is used to enable/disable low power of NCP device and
 * configure method to enter low power mode.
 *
 * \param[in] argc    Argument count, the number of strings pointed to by argv,
 *                    argc should range from 3 to 4.
 * \param[in] argv    Argument vector.\n
 *                    argv[0]: ncp-mcu-sleep\n
 *                    argv[1]: String value of enable/disable low power (Required)\n
 *                             0 -- disable NCP device low power\n
 *                             1 -- enable NCP device low power\n
 *                    argv[2]: String of mode to enter low power (Required)\n
 *                             pm -- NCP device enters low power automatically and periodically with Power Manager\n
 *                             manual -- NCP device enters low power manually\n
 *                    argv[3]: String value of RTC timer (Required if argv[2] is pm)\n
 *                             Unit is second.
 *
 * \note If no other wakeup source wakes up NCP device, the RTC timer will wakeup device when times out.
 * \note If argv[2] is manual, should use API \ref wlan_suspend_command() to request NCP device enter low power.
 * \note If argv[2] is pm, the low power mode of NCP device is depends on
 * value of argv[1] of API \ref ncp_wake_cfg_command():\n
 * INTF -- PM2, GPIO -- PM3(RDRW612) or PM2(FRDMRW612), WIFI-NB -- PM3(FRDMRW612).
 *
 * \return NCP_SUCCESS if the call was successful.
 * \return -NCP_FAIL if failed.
 */
int ncp_mcu_sleep_command(int argc, char **argv);

/**
 * This API can be used to process mcu sleep response.
 *
 * \param[in] res    A pointer to \ref MCU_NCPCmd_DS_COMMAND response.
 *
 * \return NCP_SUCCESS.
 */
int ncp_process_mcu_sleep_response(uint8_t *res);

/**
 * This API is used to configure if NCP device should wakeup NCP host or not.\n
 * If enabled, NCP device will toggle GPIO to wakeup NCP host if NCP device is woken up by Wi-Fi.
 *
 * \param[in] argc    Argument count, the number of strings pointed to by argv,
 *                    argc should be 2.
 * \param[in] argv    Argument vector.\n
 *                    argv[0]: ncp-wakeup-host\n
 *                    argv[1]: String value of enable/disable wakeup host (Required)\n
 *                             0 -- enable\n
 *                             1 -- disable\n
 *                             Default is 0.
 *
 * \return NCP_SUCCESS if the call was successful.
 * \return -NCP_FAIL if failed.
 */
int ncp_wakeup_host_command(int argc, char **argv);

/**
 * This API can be used to process wakeup host response.
 *
 * \param[in] res    A pointer to \ref MCU_NCPCmd_DS_COMMAND response.
 *
 * \return NCP_SUCCESS.
 */
int ncp_process_wakeup_host_response(uint8_t *res);

/**
 * This API is used to get all current power related configurations.
 *
 * \param[in] argc    Argument count, the number of strings pointed to by argv.
 * \param[in] argv    Argument vector.
 *                    argv should be NULL.
 *
 * \return NCP_SUCCESS.
 */
int ncp_get_mcu_sleep_conf_command(int argc, char **argv);

/**
 * This API is used to configure NCP device enter or exit PM2 by USB remote wakeup.
 *
 * \note This API should be used with USB interface only.
 *
 * \param[in] argc    Argument count, the number of strings pointed to by argv,
 *                    argc should be 2
 * \param[in] argv    Argument vector.\n
 *                    argv[0]: ncp-usb-pm-cfg\n
 *                    argv[1]: String value of enter/exit PM2\n
 *                             1 -- Enter PM2\n
 *                             2 -- Exit PM2\n
 *
 * \return NCP_SUCCESS.
 */
int usb_pm_cfg(int argc, char **argv);
#endif /*__NCP_HOST_COMMAND_SYSTEM_H_*/
