/*
** Copyright (C) 2022-2023 Arseny Vakhrushev <arseny.vakhrushev@me.com>
**
** This firmware is free software: you can redistribute it and/or modify
** it under the terms of the GNU General Public License as published by
** the Free Software Foundation, either version 3 of the License, or
** (at your option) any later version.
**
** This firmware is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
** GNU General Public License for more details.
**
** You should have received a copy of the GNU General Public License
** along with this firmware. If not, see <http://www.gnu.org/licenses/>.
*/

#include "common.h"

__attribute__((__weak__))
void hsictl(int x) {
	int cr = RCC_CR;
	int tv = (cr & 0xf8) >> 3; // 5 bits
	RCC_CR = (cr & ~0xf8) | ((tv + x) & 0x1f) << 3;
}

char crc8(const char *buf, int len) {
	static const char tbl[] = {
		0x00, 0x07, 0x0e, 0x09, 0x1c, 0x1b, 0x12, 0x15, 0x38, 0x3f, 0x36, 0x31, 0x24, 0x23, 0x2a, 0x2d,
		0x70, 0x77, 0x7e, 0x79, 0x6c, 0x6b, 0x62, 0x65, 0x48, 0x4f, 0x46, 0x41, 0x54, 0x53, 0x5a, 0x5d,
		0xe0, 0xe7, 0xee, 0xe9, 0xfc, 0xfb, 0xf2, 0xf5, 0xd8, 0xdf, 0xd6, 0xd1, 0xc4, 0xc3, 0xca, 0xcd,
		0x90, 0x97, 0x9e, 0x99, 0x8c, 0x8b, 0x82, 0x85, 0xa8, 0xaf, 0xa6, 0xa1, 0xb4, 0xb3, 0xba, 0xbd,
		0xc7, 0xc0, 0xc9, 0xce, 0xdb, 0xdc, 0xd5, 0xd2, 0xff, 0xf8, 0xf1, 0xf6, 0xe3, 0xe4, 0xed, 0xea,
		0xb7, 0xb0, 0xb9, 0xbe, 0xab, 0xac, 0xa5, 0xa2, 0x8f, 0x88, 0x81, 0x86, 0x93, 0x94, 0x9d, 0x9a,
		0x27, 0x20, 0x29, 0x2e, 0x3b, 0x3c, 0x35, 0x32, 0x1f, 0x18, 0x11, 0x16, 0x03, 0x04, 0x0d, 0x0a,
		0x57, 0x50, 0x59, 0x5e, 0x4b, 0x4c, 0x45, 0x42, 0x6f, 0x68, 0x61, 0x66, 0x73, 0x74, 0x7d, 0x7a,
		0x89, 0x8e, 0x87, 0x80, 0x95, 0x92, 0x9b, 0x9c, 0xb1, 0xb6, 0xbf, 0xb8, 0xad, 0xaa, 0xa3, 0xa4,
		0xf9, 0xfe, 0xf7, 0xf0, 0xe5, 0xe2, 0xeb, 0xec, 0xc1, 0xc6, 0xcf, 0xc8, 0xdd, 0xda, 0xd3, 0xd4,
		0x69, 0x6e, 0x67, 0x60, 0x75, 0x72, 0x7b, 0x7c, 0x51, 0x56, 0x5f, 0x58, 0x4d, 0x4a, 0x43, 0x44,
		0x19, 0x1e, 0x17, 0x10, 0x05, 0x02, 0x0b, 0x0c, 0x21, 0x26, 0x2f, 0x28, 0x3d, 0x3a, 0x33, 0x34,
		0x4e, 0x49, 0x40, 0x47, 0x52, 0x55, 0x5c, 0x5b, 0x76, 0x71, 0x78, 0x7f, 0x6a, 0x6d, 0x64, 0x63,
		0x3e, 0x39, 0x30, 0x37, 0x22, 0x25, 0x2c, 0x2b, 0x06, 0x01, 0x08, 0x0f, 0x1a, 0x1d, 0x14, 0x13,
		0xae, 0xa9, 0xa0, 0xa7, 0xb2, 0xb5, 0xbc, 0xbb, 0x96, 0x91, 0x98, 0x9f, 0x8a, 0x8d, 0x84, 0x83,
		0xde, 0xd9, 0xd0, 0xd7, 0xc2, 0xc5, 0xcc, 0xcb, 0xe6, 0xe1, 0xe8, 0xef, 0xfa, 0xfd, 0xf4, 0xf3,
	};
	char crc = 0;
	while (len--) crc = tbl[crc ^ *buf++];
	return crc;
}

char crc8dvbs2(const char *buf, int len) {
	static const char tbl[] = {
		0x00, 0xd5, 0x7f, 0xaa, 0xfe, 0x2b, 0x81, 0x54, 0x29, 0xfc, 0x56, 0x83, 0xd7, 0x02, 0xa8, 0x7d,
		0x52, 0x87, 0x2d, 0xf8, 0xac, 0x79, 0xd3, 0x06, 0x7b, 0xae, 0x04, 0xd1, 0x85, 0x50, 0xfa, 0x2f,
		0xa4, 0x71, 0xdb, 0x0e, 0x5a, 0x8f, 0x25, 0xf0, 0x8d, 0x58, 0xf2, 0x27, 0x73, 0xa6, 0x0c, 0xd9,
		0xf6, 0x23, 0x89, 0x5c, 0x08, 0xdd, 0x77, 0xa2, 0xdf, 0x0a, 0xa0, 0x75, 0x21, 0xf4, 0x5e, 0x8b,
		0x9d, 0x48, 0xe2, 0x37, 0x63, 0xb6, 0x1c, 0xc9, 0xb4, 0x61, 0xcb, 0x1e, 0x4a, 0x9f, 0x35, 0xe0,
		0xcf, 0x1a, 0xb0, 0x65, 0x31, 0xe4, 0x4e, 0x9b, 0xe6, 0x33, 0x99, 0x4c, 0x18, 0xcd, 0x67, 0xb2,
		0x39, 0xec, 0x46, 0x93, 0xc7, 0x12, 0xb8, 0x6d, 0x10, 0xc5, 0x6f, 0xba, 0xee, 0x3b, 0x91, 0x44,
		0x6b, 0xbe, 0x14, 0xc1, 0x95, 0x40, 0xea, 0x3f, 0x42, 0x97, 0x3d, 0xe8, 0xbc, 0x69, 0xc3, 0x16,
		0xef, 0x3a, 0x90, 0x45, 0x11, 0xc4, 0x6e, 0xbb, 0xc6, 0x13, 0xb9, 0x6c, 0x38, 0xed, 0x47, 0x92,
		0xbd, 0x68, 0xc2, 0x17, 0x43, 0x96, 0x3c, 0xe9, 0x94, 0x41, 0xeb, 0x3e, 0x6a, 0xbf, 0x15, 0xc0,
		0x4b, 0x9e, 0x34, 0xe1, 0xb5, 0x60, 0xca, 0x1f, 0x62, 0xb7, 0x1d, 0xc8, 0x9c, 0x49, 0xe3, 0x36,
		0x19, 0xcc, 0x66, 0xb3, 0xe7, 0x32, 0x98, 0x4d, 0x30, 0xe5, 0x4f, 0x9a, 0xce, 0x1b, 0xb1, 0x64,
		0x72, 0xa7, 0x0d, 0xd8, 0x8c, 0x59, 0xf3, 0x26, 0x5b, 0x8e, 0x24, 0xf1, 0xa5, 0x70, 0xda, 0x0f,
		0x20, 0xf5, 0x5f, 0x8a, 0xde, 0x0b, 0xa1, 0x74, 0x09, 0xdc, 0x76, 0xa3, 0xf7, 0x22, 0x88, 0x5d,
		0xd6, 0x03, 0xa9, 0x7c, 0x28, 0xfd, 0x57, 0x82, 0xff, 0x2a, 0x80, 0x55, 0x01, 0xd4, 0x7e, 0xab,
		0x84, 0x51, 0xfb, 0x2e, 0x7a, 0xaf, 0x05, 0xd0, 0xad, 0x78, 0xd2, 0x07, 0x53, 0x86, 0x2c, 0xf9,
	};
	char crc = 0;
	while (len--) crc = tbl[crc ^ *buf++];
	return crc;
}

int scale(int x, int a1, int a2, int b1, int b2) {
	if (x <= a1) return b1;
	if (x >= a2) return b2;
	return b1 + (x - a1) * (b2 - b1) / (a2 - a1);
}

int smooth(int *s, int x, int n) {
	int q;
	if (x < 0) return 0; // Sanity check
	if ((q = *s) < 0) *s = q = x << n; // Initialize state
	return (*s = x + q - (q >> n)) >> n;
}

int calcpid(PID *pid, int x, int y) {
	int p = x - y; // Proportional error
	int l = pid->Li; // Integral error limit
	int i = clamp(pid->i + p, -l, l); // Integral error
	int d = x - pid->x; // Derivative error
	pid->i = i;
	pid->x = x;
	return pid->Kp * p + pid->Ki * i + pid->Kd * d;
}

void checkcfg(void) {
#ifdef ANALOG
	cfg.arm = 0;
#else
#ifndef ANALOG_PIN
	if (IO_ANALOG) cfg.arm = 1; // Ensure low level on startup
	else
#endif
	cfg.arm = !!cfg.arm;
#endif
#ifdef PWM_ENABLE
	cfg.damp = 1;
#else
	cfg.damp = !!cfg.damp;
#endif
	cfg.revdir = !!cfg.revdir;
#ifdef SENSORED
	cfg.brushed = 0;
	cfg.timing = 0;
	cfg.sine_range = 0;
	cfg.sine_power = 0;
#else
	cfg.brushed = !!cfg.brushed;
	cfg.timing = clamp(cfg.timing, 1, 7);
	cfg.sine_range = cfg.damp && cfg.sine_range ? clamp(cfg.sine_range, 5, 25) : 0;
	cfg.sine_power = clamp(cfg.sine_power, 1, 15);
#endif
	cfg.freq_min = clamp(cfg.freq_min, 16, 48);
	cfg.freq_max = clamp(cfg.freq_max, cfg.freq_min, 96);
	cfg.duty_min = clamp(cfg.duty_min, 1, 100);
	cfg.duty_max = clamp(cfg.duty_max, cfg.duty_min, 100);
	cfg.duty_spup = clamp(cfg.duty_spup, 1, 50);
	cfg.duty_ramp = clamp(cfg.duty_ramp, 0, 100);
	cfg.duty_rate = clamp(cfg.duty_rate, 1, 100);
	cfg.duty_drag = clamp(cfg.duty_drag, 0, 100);
	cfg.throt_mode = IO_ANALOG ? 0 : clamp(cfg.throt_mode, 0, 2);
	cfg.throt_set = cfg.arm ? 0 : clamp(cfg.throt_set, 0, 100);
	cfg.throt_cal = !!cfg.throt_cal;
	cfg.throt_min = clamp(cfg.throt_min, 900, 1900);
	cfg.throt_max = clamp(cfg.throt_max, cfg.throt_min + 200, 2100);
	cfg.throt_mid = clamp(cfg.throt_mid, cfg.throt_min + 100, cfg.throt_max - 100);
#ifdef IO_PA2
	cfg.input_mode = clamp(cfg.input_mode, 0, 5);
	cfg.input_chid = cfg.input_mode >= 3 ? clamp(cfg.input_chid, 1, cfg.input_mode == 3 ? 14 : 16) : 0;
#else
#if defined IO_PA6 || defined ANALOG_PIN
	cfg.input_mode = clamp(cfg.input_mode, 0, 1);
#else
	cfg.input_mode = 0;
#endif
	cfg.input_chid = 0;
#endif
	cfg.telem_mode = clamp(cfg.telem_mode, 0, 4);
	cfg.telem_phid =
		cfg.telem_mode == 2 ? clamp(cfg.telem_phid, 1, 3):
		cfg.telem_mode == 3 ? clamp(cfg.telem_phid, 1, 28):
		cfg.input_mode == 4 ? clamp(cfg.telem_phid, 0, 4) : 0;
	cfg.telem_poles = clamp(cfg.telem_poles & ~1, 2, 100);
	cfg.prot_temp = cfg.prot_temp ? clamp(cfg.prot_temp, 60, 140) : 0;
#if SENS_CNT > 0
	cfg.prot_volt = cfg.prot_volt ? clamp(cfg.prot_volt, 28, 38) : 0;
	cfg.prot_cells = clamp(cfg.prot_cells, 0, 16);
#else
	cfg.prot_volt = 0;
	cfg.prot_cells = 0;
#endif
#if SENS_CNT < 2
	cfg.prot_curr = 0;
#endif
	cfg.volume = clamp(cfg.volume, 0, 100);
	cfg.beacon = clamp(cfg.beacon, 0, 100);
#if LED_CNT > 0
	cfg.led &= (1 << LED_CNT) - 1;
#else
	cfg.led = 0;
#endif
}

int savecfg(void) {
	if (ertm) return 0;
	__disable_irq();
	FLASH_KEYR = FLASH_KEYR_KEY1;
	FLASH_KEYR = FLASH_KEYR_KEY2;
	FLASH_SR = -1; // Clear errors
	FLASH_CR = FLASH_CR_PER;
#ifdef STM32G0
	FLASH_CR = FLASH_CR_PER | FLASH_CR_STRT | ((uint32_t)(_cfg - _boot) >> 11) << FLASH_CR_PNB_SHIFT; // Erase page
#else
	FLASH_AR = (uint32_t)_cfg;
	FLASH_CR = FLASH_CR_PER | FLASH_CR_STRT; // Erase page
#endif
	while (FLASH_SR & FLASH_SR_BSY);
	FLASH_CR = FLASH_CR_PG;
#ifdef STM32G0
#define T uint32_t
#else
#define T uint16_t
#endif
	T *dst = (T *)_cfg;
	T *src = (T *)_cfg_start;
	T *end = (T *)_cfg_end;
#undef T
	while (src < end) { // Write data
		*dst++ = *src++;
#ifdef STM32G0
		*dst++ = *src++;
#endif
		while (FLASH_SR & FLASH_SR_BSY);
	}
	FLASH_CR = FLASH_CR_LOCK;
	__enable_irq();
#ifdef STM32G0
	if (FLASH_SR & (FLASH_SR_PROGERR | FLASH_SR_WRPERR)) return 0;
#else
	if (FLASH_SR & (FLASH_SR_PGERR | FLASH_SR_WRPRTERR)) return 0;
#endif
	return !memcmp(_cfg, _cfg_start, _cfg_end - _cfg_start);
}

int resetcfg(void) {
	__disable_irq();
	memcpy(&cfg, &cfgdata, sizeof cfgdata);
	__enable_irq();
	return savecfg();
}

int playmusic(const char *str, int vol) {
	static const uint16_t arr[] = {30575, 28859, 27240, 25713, 24268, 22906, 21621, 20407, 19261, 18181, 17160, 16196, 15287};
	static char flag;
	char *end;
	int tmp = strtol(str, &end, 10); // Tempo
	if (str == end) tmp = 125; // 120BPM by default
	else {
		if (tmp < 10 || tmp > 999) return 0; // Sanity check
		tmp = 15000 / tmp;
		str = end;
	}
	if (!vol || ertm || flag) return 0;
	flag = 1;
	vol <<= 1;
#ifdef PWM_ENABLE
	TIM1_CCMR1 = TIM_CCMR1_OC1M_FORCE_LOW | TIM_CCMR1_OC2PE | TIM_CCMR1_OC2M_PWM1;
	TIM1_CCMR2 = TIM_CCMR2_OC3M_FORCE_LOW;
	int er = TIM_CCER_CC1NE | TIM_CCER_CC2E | TIM_CCER_CC3NE;
#else
	TIM1_CCMR1 = TIM_CCMR1_OC1M_FORCE_HIGH | TIM_CCMR1_OC2PE | TIM_CCMR1_OC2M_PWM1;
	TIM1_CCMR2 = TIM_CCMR2_OC3M_FORCE_HIGH;
	int er = TIM_CCER_CC1NE | TIM_CCER_CC2E | TIM_CCER_CC2NE | TIM_CCER_CC3NE;
#endif
#ifdef INVERTED_HIGH
	er |= TIM_CCER_CC1P | TIM_CCER_CC2P | TIM_CCER_CC3P;
#endif
#ifdef PWM_ENABLE
	er |= TIM_CCER_CC1NP | TIM_CCER_CC2NP | TIM_CCER_CC3NP;
#endif
	TIM1_CCER = er;
	TIM1_PSC = CLK_MHZ / 8 - 1; // 8MHz
	for (int a, b, c = 0; (a = *str++);) {
		if (a >= 'a' && a <= 'g') a -= 'c', b = 0; // Low note
		else if (a >= 'A' && a <= 'G') a -= 'C', b = 1; // High note
		else if (a == '_') goto update; // Pause
		else {
			if (a == '+' && !c++) continue; // Octave up
			if (a == '-' && c--) continue; // Octave down
			break; // Invalid specifier
		}
		a = (a + 7) % 7 << 1;
		if (a > 4) --a;
		if (*str == '#') ++a, ++str;
		TIM1_ARR = arr[a] >> (b + c); // Frequency
		TIM1_CCR2 = vol; // Volume
	update:
		TIM1_EGR = TIM_EGR_UG | TIM_EGR_COMG;
		a = strtol(str, &end, 10); // Duration
		if (str == end) a = 1;
		else {
			if (a < 1 || a > 99) break; // Sanity check
			str = end;
		}
		uint32_t t = tickms + tmp * a;
		while (t != tickms) TIM14_EGR = TIM_EGR_UG; // Reset arming timeout
		TIM1_CCR2 = 0; // Preload silence
	}
#ifdef PWM_ENABLE
	TIM1_CCMR1 = TIM_CCMR1_OC1M_FORCE_HIGH | TIM_CCMR1_OC2M_FORCE_HIGH;
	TIM1_CCMR2 = TIM_CCMR2_OC3M_FORCE_HIGH;
#else
	TIM1_CCMR1 = TIM_CCMR1_OC1M_FORCE_LOW | TIM_CCMR1_OC2M_FORCE_LOW;
	TIM1_CCMR2 = TIM_CCMR2_OC3M_FORCE_LOW;
#endif
	er = TIM_CCER_CC1NE | TIM_CCER_CC2NE | TIM_CCER_CC3NE;
#ifdef INVERTED_HIGH
	er |= TIM_CCER_CC1P | TIM_CCER_CC2P | TIM_CCER_CC3P;
#endif
#ifdef PWM_ENABLE
	er |= TIM_CCER_CC1NP | TIM_CCER_CC2NP | TIM_CCER_CC3NP;
#endif
	TIM1_CCER = er;
	TIM1_PSC = 0;
	TIM1_ARR = CLK_KHZ / 24 - 1;
	TIM1_EGR = TIM_EGR_UG | TIM_EGR_COMG;
	flag = 0;
	return !str[-1];
}
