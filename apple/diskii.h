#ifndef __DISKII_H
#define __DISKII_H

#ifdef TEENSYDUINO
#include <Arduino.h>
#else
#include <stdint.h>
#include <stdio.h>
#endif

#include "filemanager.h"
#include "applemmu.h"
#include "slot.h"

#include "RingBuffer.h"
#include "nibutil.h"

class DiskII : public Slot {
 public:
  DiskII(AppleMMU *mmu);
  virtual ~DiskII();

  virtual void Reset(); // used by BIOS cold-boot
  virtual uint8_t readSwitches(uint8_t s);
  virtual void writeSwitches(uint8_t s, uint8_t v);
  virtual void loadROM(uint8_t *toWhere);

  void insertDisk(int8_t driveNum, const char *filename, bool drawIt = true);
  void ejectDisk(int8_t driveNum);

  const char *DiskName(int8_t num);
  void flushTrack();

 private:
  void step(uint8_t phase);

  bool isWriteProtected();
  void setWriteMode(bool enable);
  void select(int8_t which); // 0 or 1 for drives 1 and 2, respectively
  uint8_t readOrWriteByte();

#ifndef TEENSYDUINO
  void convertDskToNib(const char *outFN);
#endif

 private:
  uint8_t curTrack;
  bool trackDirty; // does this track need flushing to disk?
  uint8_t writeLatch;
  RingBuffer *trackBuffer; // nibblized data
  uint8_t *rawTrackBuffer; // not nibblized data
  
  bool writeMode;
  bool writeProt;
  AppleMMU *mmu;

  int8_t disk[2];
  uint8_t indicatorIsOn[2];
  uint8_t diskType[2];

  int8_t selectedDisk;
};

#endif