/*
 * Copyright (C) 2018 The LineageOS Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#define LOG_TAG "xiaomi_readmac"
#define LOG_NDEBUG 0

#include <cutils/log.h>
#include <private/android_filesystem_config.h>

#include <dlfcn.h>
#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>

#define _SCNx8 "%" SCNx8

#ifndef S_IRUGO
#define S_IRUGO (S_IRUSR|S_IRGRP|S_IROTH)
#endif

#define MAC_ADDR_SIZE 6
#define MAC_ADDR_STORAGE_LEN 17
#define WLAN_MAC_BIN "/persist/wlan_mac.bin"
#define BD_MAC_BIN "/data/misc/bluetooth/bdaddr"

#define LIB_QMINVAPI "libqminvapi.so"
#define LIB_QMI_CCI "libqmi_cci.so"

#define QMI_CLIENT_RETRIES_MAX 3

#define DEBUG 0
#define DBG(fmt, ...) \
        do { if (DEBUG) fprintf(stderr, "%s:%d: " fmt, \
                                __func__, __LINE__, ##__VA_ARGS__); } while (0)

const uint8_t xiaomi_oui_list[][3] =
{
	{ 0x9C, 0x99, 0xA0 },
	{ 0x18, 0x59, 0x36 },
	{ 0x98, 0xFA, 0xE3 },
	{ 0x64, 0x09, 0x80 },
	{ 0x8C, 0xBE, 0xBE },
	{ 0xF8, 0xA4, 0x5F },
	{ 0xC4, 0x0B, 0xCB },
	{ 0xEC, 0xD0, 0x9F },
	{ 0xE4, 0x46, 0xDA },
	{ 0xF4, 0xF5, 0xDB },
	{ 0x28, 0xE3, 0x1F },
	{ 0x0C, 0x1D, 0xAF },
	{ 0x14, 0xF6, 0x5A },
	{ 0x74, 0x23, 0x44 },
	{ 0xF0, 0xB4, 0x29 },
	{ 0xD4, 0x97, 0x0B },
	{ 0x64, 0xCC, 0x2E },
	{ 0xB0, 0xE2, 0x35 },
	{ 0x38, 0xA4, 0xED },
	{ 0xF4, 0x8B, 0x32 },
	{ 0x3C, 0xBD, 0x3E },
	{ 0x4C, 0x49, 0xE3 },
	{ 0x00, 0x9E, 0xC8 },
	{ 0xAC, 0xF7, 0xF3 },
	{ 0x10, 0x2A, 0xB3 },
	{ 0x58, 0x44, 0x98 },
	{ 0xA0, 0x86, 0xC6 },
	{ 0x7C, 0x1D, 0xD9 },
	{ 0x28, 0x6C, 0x07 },
	{ 0xAC, 0xC1, 0xEE },
	{ 0x78, 0x02, 0xF8 },
	{ 0x50, 0x8F, 0x4C },
	{ 0x68, 0xDF, 0xDD },
	{ 0xC4, 0x6A, 0xB7 },
	{ 0xFC, 0x64, 0xBA },
	{ 0x20, 0x82, 0xC0 },
	{ 0x34, 0x80, 0xB3 },
	{ 0x74, 0x51, 0xBA },
	{ 0x64, 0xB4, 0x73 },
	{ 0x34, 0xCE, 0x00 },
	{ 0x00, 0xEC, 0x0A },
	{ 0x78, 0x11, 0xDC },
	{ 0x50, 0x64, 0x2B },
};
const size_t xiaomi_oui_list_size = sizeof(xiaomi_oui_list) / 3;

static inline int is_xiaomi_oui(char *buf) {
	size_t i;

	for (i = 0; i < xiaomi_oui_list_size; i++) {
		if(buf[0] == xiaomi_oui_list[i][0] && buf[1] == xiaomi_oui_list[i][1] && buf[2] == xiaomi_oui_list[i][2])
			return 1;
	}

	return 0;
}

static int check_mac_file(const char *file, int binary) {
	FILE* fp;
	char content[binary ? MAC_ADDR_SIZE : MAC_ADDR_STORAGE_LEN+1];
	char addr[ binary ? 1 : MAC_ADDR_SIZE ];
	int rc;

	fp = fopen(file, "rb");
	if (fp == NULL)
		return 1;

	fread(content, 1, (binary ? MAC_ADDR_SIZE : MAC_ADDR_STORAGE_LEN), fp);
	fclose(fp);

	if (!binary) {
		content[MAC_ADDR_STORAGE_LEN] = '\0';
		sscanf(content, _SCNx8 ":" _SCNx8 ":" _SCNx8 ":" _SCNx8 ":" _SCNx8 ":" _SCNx8,
			&addr[0], &addr[1], &addr[2], &addr[3], &addr[4], &addr[5]);
		rc = is_xiaomi_oui(addr);
	} else {
		rc = is_xiaomi_oui(content);
	}

	if (rc)
		return 0;

	fprintf(stderr, "invalid mac addr in %s\n", file);
	return 1;
}

static int write_mac_file(const char *file, uint8_t addr[MAC_ADDR_SIZE], int binary)
{
	FILE* fp;
	char buf[binary ? 1 : MAC_ADDR_STORAGE_LEN+1];

	fp = fopen(file, "wb");
	if (fp == NULL)
		return 1;

	if (binary)
		fwrite((void *)addr, MAC_ADDR_SIZE, 1, fp);
	else {
		snprintf(buf, MAC_ADDR_STORAGE_LEN+1, "%02X:%02X:%02X:%02X:%02X:%02X",
			addr[0], addr[1], addr[2], addr[3], addr[4], addr[5]);
		fwrite((void *)buf, MAC_ADDR_STORAGE_LEN, 1, fp);
	}
	fclose(fp);

	return 0;
}

struct dummy_t {
	uint8_t data[128];
};

typedef struct {
	uint32_t index;
	uint32_t offset;
} qminvapi_req_t;

typedef struct {
	uint8_t buf[136];
} qminvapi_resp_t;

typedef int32_t (*qmi_client_notifier_init_t)();
typedef int32_t (*qmi_client_get_service_list_t)();
typedef int32_t (*qmi_client_init_t)();
typedef int32_t (*qmi_client_release_t)();
typedef int32_t (*qmi_client_send_msg_sync_t)();

int read_qminv(uint8_t *wlan, uint8_t *bd)
{

#define DLOPEN(handle, file) void * handle = dlopen(file, RTLD_NOW)
#define DLCLOSE(handle) do { if (handle) dlclose(handle); } while (0)

#define DLSYM(handle, fn) fn##_t fn = (fn##_t)dlsym(handle, #fn)
#define DLERR(x) do { if (!x) { fprintf(stderr, "%s", dlerror()); ret = -1; goto dlfcn_err; } } while (0)

	int ret = 0;
	int32_t rc32 = 0;

	int retries = QMI_CLIENT_RETRIES_MAX;

	void *clnt = NULL, *notifier = NULL;
	struct dummy_t params;
	struct dummy_t info[2];

	uint32_t num_entries, num_services;

	qminvapi_req_t req;
	qminvapi_resp_t resp;


	DLOPEN(libqminvapi_handle, LIB_QMINVAPI);
	DLOPEN(libqmi_cci_handle, LIB_QMI_CCI);

	DLERR(libqminvapi_handle);
	DLERR(libqmi_cci_handle);


	DLSYM(libqmi_cci_handle, qmi_client_notifier_init);
	DLERR(qmi_client_notifier_init);

	DLSYM(libqmi_cci_handle, qmi_client_get_service_list);
	DLERR(qmi_client_get_service_list);

	DLSYM(libqmi_cci_handle, qmi_client_init);
	DLERR(qmi_client_init);

	DLSYM(libqmi_cci_handle, qmi_client_release);
	DLERR(qmi_client_init);

	DLSYM(libqmi_cci_handle, qmi_client_send_msg_sync);
	DLERR(qmi_client_init);

	void *xiaomi_qmi_nv_qmi_idl_service_object_v01 = dlsym(libqminvapi_handle, "xiaomi_qmi_nv_qmi_idl_service_object_v01");
	DLERR(xiaomi_qmi_nv_qmi_idl_service_object_v01);

	DBG("[xiaomi_qmi_nv_qmi_idl_service_object_v01 == %p]\n", xiaomi_qmi_nv_qmi_idl_service_object_v01);

	rc32 = qmi_client_notifier_init(xiaomi_qmi_nv_qmi_idl_service_object_v01, &params, &notifier);

	while(retries-- > 0) {
		rc32 = qmi_client_get_service_list(xiaomi_qmi_nv_qmi_idl_service_object_v01, NULL, NULL, &num_services);
		DBG("XIAOMI_QMI_NV: qmi_client_get_service_list() returned %d num_services = %d\n", rc32, num_services);

		if (!rc32) {
			break;
		}

		// TODO: decipher the pthread_cond_wait instead of sleep
		sleep(1);
	}
	num_entries = num_services;

	rc32 = qmi_client_get_service_list(xiaomi_qmi_nv_qmi_idl_service_object_v01, info, &num_entries, &num_services);
	DBG("XIAOMI_QMI_NV: qmi_client_get_service_list() returned %d num_entries = %d num_services = %d\n", rc32, num_entries, num_services);

	rc32 = qmi_client_init(info, xiaomi_qmi_nv_qmi_idl_service_object_v01, NULL, NULL, NULL, &clnt);
	if (rc32) {
		DBG("XIAOMI_QMI_NV: Client init failed %d\n", rc32);
		ret = 1;
		goto dlfcn_err;
	}

	if (wlan != NULL) {
		req.index = 4678;
		req.offset = 0;
		memset((void *)&resp, 0, sizeof(resp));
		rc32 = qmi_client_send_msg_sync(clnt, 1, &req, sizeof(req), &resp, sizeof(resp), 200);
		if (rc32) {
			DBG("XIAOMI_QMI_NV: send_msg_sync error: %d\n", rc32);
			ret = 1;
			goto dlfcn_err;
		}

		for (int i = 0; i < 6; i++)
			wlan[i] = resp.buf[8+i];
	}

	if (bd != NULL) {
		req.index = 447;
		req.offset = 0;
		memset((void *)&resp, 0, sizeof(resp));
		rc32 = qmi_client_send_msg_sync(clnt, 1, &req, sizeof(req), &resp, sizeof(resp), 200);
		if (rc32) {
			DBG("XIAOMI_QMI_NV: send_msg_sync error: %d\n", rc32);
			ret = 1;
			goto dlfcn_err;
		}

		for (int i = 0; i < 6; i++)
			bd[i] = resp.buf[13-i];
	}

dlfcn_err:
	if (clnt) {
		rc32 = qmi_client_release(clnt);
		if (rc32) DBG("Client release returned: %d\n", rc32);
	}

	if (notifier) {
		rc32 = qmi_client_release(notifier);
		if (rc32) DBG("Notifier release returned: %d\n", rc32);
	}

	DLCLOSE(libqminvapi_handle);
	DLCLOSE(libqmi_cci_handle);

	return ret;
}

int main()
{
	uint8_t wlan_addr[] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
	uint8_t bd_addr[] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };

	int rc, i = 0;

	if ( (check_mac_file(WLAN_MAC_BIN, 1) == 0) && (check_mac_file(BD_MAC_BIN, 0) == 0) ) {
		fprintf(stderr, "mac addr files already exist! returning.\n");

		// fix permissions anyway
		chmod(WLAN_MAC_BIN, S_IRUGO | S_IWUSR);

		chown(BD_MAC_BIN, AID_BLUETOOTH, AID_BLUETOOTH);
		chmod(BD_MAC_BIN, S_IRUSR | S_IRGRP);

		return 0;
	}

	rc = read_qminv(wlan_addr, bd_addr);

	if (rc) {
		i = 1;
	} else {
		if (!is_xiaomi_oui((char *)wlan_addr) || !is_xiaomi_oui((char *)bd_addr))
			i = 1;
	}

	if (i) {
		fprintf(stderr, "generating randomized MAC address\n");
		ALOGI("generating randomized MAC address\n");

		// We don't need strong randomness, and if the NV is corrupted
		// any hardware values are suspect, so just seed it with the
		// current time
		srand(time(NULL));

		i = rand() % xiaomi_oui_list_size;
		memcpy(wlan_addr, xiaomi_oui_list[i], 3);
		memcpy(bd_addr, xiaomi_oui_list[i], 3);

		for (i = 3; i < MAC_ADDR_SIZE; i++) {
			wlan_addr[i] = rand() % 255;
			bd_addr[i] = rand() % 255;
		}
	}

	rc = write_mac_file(WLAN_MAC_BIN, wlan_addr, 1);
	if (rc) {
		fprintf(stderr, "writing %s failed!\n", WLAN_MAC_BIN);
	} else {
		chmod(WLAN_MAC_BIN, S_IRUGO | S_IWUSR);
	}

	rc = write_mac_file(BD_MAC_BIN, bd_addr, 0);
	if (rc) {
		fprintf(stderr, "writing %s failed!\n", BD_MAC_BIN);
	} else {
		chown(BD_MAC_BIN, AID_BLUETOOTH, AID_BLUETOOTH);
		chmod(BD_MAC_BIN, S_IRUSR | S_IRGRP);
	}

	printf("wlan=%02x%02x%02x%02x%02x%02x\n", wlan_addr[0], wlan_addr[1],
			wlan_addr[2], wlan_addr[3], wlan_addr[4], wlan_addr[5]);
	printf("bd=%02x%02x%02x%02x%02x%02x\n", bd_addr[0], bd_addr[1],
			bd_addr[2], bd_addr[3], bd_addr[4], bd_addr[5]);

	ALOGI("wlan=%02x%02x%02x%02x%02x%02x\n", wlan_addr[0], wlan_addr[1],
			wlan_addr[2], wlan_addr[3], wlan_addr[4], wlan_addr[5]);
	ALOGI("bd=%02x%02x%02x%02x%02x%02x\n", bd_addr[0], bd_addr[1],
			bd_addr[2], bd_addr[3], bd_addr[4], bd_addr[5]);

	return 0;
}
