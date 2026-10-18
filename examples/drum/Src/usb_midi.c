/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2019 Ha Thach (tinyusb.org)
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "bsp/board_api.h"
#include "pico/multicore.h"
#include "tusb.h"

/* This MIDI example send sequence of note (on/off) repeatedly. To test on PC,
 * you need to install synth software and midi connection management software.
 * On
 * - Linux (Ubuntu): install qsynth, qjackctl. Then connect TinyUSB output port
 * to FLUID Synth input port
 * - Windows: install MIDI-OX
 * - MacOS: SimpleSynth
 */

//--------------------------------------------------------------------+
// MACRO CONSTANT TYPEDEF PROTYPES
//--------------------------------------------------------------------+

//--------------------------------------------------------------------+
// Device callbacks
//--------------------------------------------------------------------+

// Invoked when device is mounted
void tud_mount_cb(void) {}

// Invoked when device is unmounted
void tud_umount_cb(void) {}

// Invoked when usb bus is suspended
// remote_wakeup_en : if host allow us  to perform remote wakeup
// Within 7ms, device must draw an average of current less than 2.5 mA from bus
void tud_suspend_cb(bool remote_wakeup_en) { (void)remote_wakeup_en; }

// Invoked when usb bus is resumed
void tud_resume_cb(void) {}

//--------------------------------------------------------------------+
// MIDI Task
//--------------------------------------------------------------------+

#define MAX_SEND 8
void midi_task(void) {

  uint8_t const cable_num = 0; // MIDI jack associated with USB endpoint

  // The MIDI interface always creates input and output port/jack descriptors
  // regardless of these being used or not. Therefore incoming traffic should be
  // read (possibly just discarded) to avoid the sender blocking in IO
  while (tud_midi_available()) {
    uint8_t packet[4];
    tud_midi_packet_read(packet);
  }

  static const uint8_t CHANNEL_NOTES[16] = {
      38, // snare,
      42, // Hithat-closed
      36, // base
      49, // crash1
      50, // tom1
      47, // tom2
      45, // tom3
      51, // ride
      57, // crash2
  };

  int max_retry = 0;
  while (multicore_fifo_rvalid() && (max_retry++ < MAX_SEND)) {
    uint32_t pop = multicore_fifo_pop_blocking();
    // first byte: index of channel, second byte: velocity (0~127)

    uint8_t *data = (uint8_t *)(&pop);
    uint8_t const channel = data[0];
    uint8_t const velocity = data[1];
    uint8_t notes[3] = {velocity ? 0x99 : 0x89, CHANNEL_NOTES[channel],
                        velocity};

    tud_midi_stream_write(cable_num, notes, 3);
  }
}
