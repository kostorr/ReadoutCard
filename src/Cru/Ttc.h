/// \file Ttc.h
/// \brief Definition of the Ttc class
///
/// \author Kostas Alexopoulos (kostas.alexopoulos@cern.ch)

#ifndef ALICEO2_READOUTCARD_CRU_TTC_H_
#define ALICEO2_READOUTCARD_CRU_TTC_H_

#include "Common.h"
#include "ReadoutCard/BarInterface.h"
#include "ReadoutCard/InterprocessLock.h"

namespace AliceO2
{
namespace roc
{

class Ttc
{
  using OnuStatus = Cru::OnuStatus;
  using LinkStatus = Cru::LinkStatus;

 public:
  Ttc(std::shared_ptr<BarInterface> bar, int serial = -1);

  void calibrateTtc();
  void setClock(uint32_t clock);
  void resetFpll();
  bool configurePonTx(uint32_t onuAddress);
  void selectDownstreamData(uint32_t downstreamData);
  uint32_t getPllClock();
  uint32_t getDownstreamData();
  uint32_t getHbTriggerLtuCount();
  uint32_t getPhyTriggerLtuCount();
  std::pair<uint32_t, uint32_t> getEoxSoxLtuCount();

  void resetCtpEmulator(bool doReset);
  void setEmulatorTriggerMode(Cru::TriggerMode mode);
  void doManualPhyTrigger();
  void setEmulatorContinuousMode();
  void setEmulatorIdleMode();
  void setEmulatorStandaloneFlowControl(bool allow = false);
  void setEmulatorBCMAX(uint32_t bcmax);
  void setEmulatorHBMAX(uint32_t hbmax);
  void setEmulatorPrescaler(uint32_t hbkeep, uint32_t hbdrop);
  void setEmulatorPHYSDIV(uint32_t physdiv);
  void setEmulatorCALDIV(uint32_t caldiv);
  void setEmulatorHCDIV(uint32_t hcdiv);
  void setEmulatorORBITINIT(uint32_t orbitInit);
  void setFixedBCTrigger(std::vector<uint32_t> FBCTVector);

  OnuStatus onuStatus();

 private:
  void configurePlls(uint32_t clock);
  void setRefGen(int frequency = 240);
  LinkStatus getOnuStickyBit();
  uint32_t getPonQuality();
  int getPonQualityStatus();

  std::shared_ptr<BarInterface> mBar;
  std::unique_ptr<Interprocess::Lock> mI2cLock;

  int mSerial;
  static constexpr uint32_t MAX_BCID = 3564 - 1;
};
} // namespace roc
} // namespace AliceO2

#endif // ALICEO2_READOUTCARD_CRU_TTC_H_
