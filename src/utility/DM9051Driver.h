/*
  This file is part of the EthernetESP32 library for Arduino
  https://github.com/Networking-for-Arduino/EthernetESP32
  Copyright 2024 Juraj Andrassy

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
*/

#ifndef _DM9051_DRIVER_H_
#define _DM9051_DRIVER_H_

#include "EthDriver.h"

class DM9051Driver : public EthSpiDriver {
public:

  DM9051Driver(int8_t cs = SS, int8_t irq = -1, int8_t rst = -1)
      : EthSpiDriver(cs, irq, rst) {
  }

  virtual bool read(uint32_t cmd, uint32_t addr, void *data, uint32_t data_len);
  virtual bool write(uint32_t cmd, uint32_t addr, const void *data, uint32_t data_len);

protected:
  virtual esp_eth_mac_t* newMAC();
  virtual esp_eth_phy_t* newPHY();
};

class DM9051CallbackDriver : public DM9051Driver {
public:

  DM9051CallbackDriver(uint16_t poll_interval_ms = 60, uint32_t sw_reset_timeout_ms = 300)
      : DM9051Driver(-1, -1, -1), poll_interval(poll_interval_ms > 0 ? poll_interval_ms : 60), sw_reset_timeout(sw_reset_timeout_ms > 0 ? sw_reset_timeout_ms : 300) {
  }

  virtual bool read(uint32_t cmd, uint32_t addr, void *data, uint32_t data_len) override;
  virtual bool write(uint32_t cmd, uint32_t addr, const void *data, uint32_t data_len) override;
  void onRead(bool (*function)(uint32_t cmd, uint32_t addr, void* data, uint32_t data_len));
  void onWrite(bool (*function)(uint32_t cmd, uint32_t addr, const void* data, uint32_t data_len));

protected:
  esp_eth_mac_t* newMAC() override;
  bool (*user_onRead)(uint32_t cmd, uint32_t addr, void* data, uint32_t data_len);
  bool (*user_onWrite)(uint32_t cmd, uint32_t addr, const void* data, uint32_t data_len);

private:
  const uint16_t poll_interval;
  const uint32_t sw_reset_timeout;
};

#endif
