// Copyright CERN and copyright holders of ALICE O2. This software is
// distributed under the terms of the GNU General Public License v3 (GPL
// Version 3), copied verbatim in the file "COPYING".
//
// See http://alice-o2.web.cern.ch/license for full licensing information.
//
// In applying this license CERN does not waive the privileges and immunities
// granted to it by virtue of its status as an Intergovernmental Organization
// or submit itself to any jurisdiction.

/// \file BarInterfaceBase.cxx
/// \brief Implementation of the BarInterfaceBase class.
///
/// \author Pascal Boeschoten (pascal.boeschoten@cern.ch)
/// \author Kostas Alexopoulos (kostas.alexopoulos@cern.ch)

#include "BarInterfaceBase.h"
#include "Utilities/SmartPointer.h"

namespace AliceO2
{
namespace roc
{

BarInterfaceBase::BarInterfaceBase(const Parameters& parameters, std::unique_ptr<RocPciDevice> rocPciDevice)
  : mBarIndex(parameters.getChannelNumberRequired()),
    mRocPciDevice(std::move(rocPciDevice))
{
  Utilities::resetSmartPtr(mPdaBar, mRocPciDevice->getPciDevice(), mBarIndex);
  mPdaBar = std::move(mRocPciDevice->getBar(mBarIndex));
}

BarInterfaceBase::BarInterfaceBase(std::shared_ptr<Pda::PdaBar> bar)
{
  mPdaBar = std::move(bar);
}

BarInterfaceBase::~BarInterfaceBase()
{
}

uint32_t BarInterfaceBase::readRegister(int index)
{
  // TODO Access restriction
  return mPdaBar->readRegister(index);
}

void BarInterfaceBase::writeRegister(int index, uint32_t value)
{
  // TODO Access restriction
  mPdaBar->writeRegister(index, value);
}

void BarInterfaceBase::modifyRegister(int index, int position, int width, uint32_t value)
{
  mPdaBar->modifyRegister(index, position, width, value);
}

void BarInterfaceBase::log(std::string logMessage, InfoLogger::InfoLogger::Severity logLevel)
{
  mLogger << logLevel;
  mLogger << "[PCI ID: " << mRocPciDevice->getPciAddress().toString() << " | bar" << getIndex() << "] : " << logMessage << InfoLogger::InfoLogger::endm;
}

} // namespace roc
} // namespace AliceO2
