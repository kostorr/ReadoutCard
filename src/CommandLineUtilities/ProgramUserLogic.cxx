// Copyright CERN and copyright holders of ALICE O2. This software is
// distributed under the terms of the GNU General Public License v3 (GPL
// Version 3), copied verbatim in the file "COPYING".
//
// See http://alice-o2.web.cern.ch/license for full licensing information.
//
// In applying this license CERN does not waive the privileges and immunities
// granted to it by virtue of its status as an Intergovernmental Organization
// or submit itself to any jurisdiction.

/// \file ProgramUserLogic.cxx
/// \brief Tool to control and report on the dummy(!) User Logic
///
/// \author Kostas Alexopoulos (kostas.alexopoulos@cern.ch)

#include <iostream>
#include "Cru/CruBar.h"
#include "ReadoutCard/ChannelFactory.h"
#include "CommandLineUtilities/Options.h"
#include "CommandLineUtilities/Program.h"

using namespace AliceO2::roc::CommandLineUtilities;
using namespace AliceO2::roc;
namespace po = boost::program_options;

class ProgramUserLogic : public Program
{
 public:
  virtual Description getDescription()
  {
    return { "User Logic", "Control the dummy User Logic",
             "roc-ul --id 0042:0 --event-size=128 \n"
             "roc-ul --id 0042:0 --random-event-size \n"
             "roc-ul --id 0042:0 --status \n" };
  }

  virtual void addOptions(boost::program_options::options_description& options)
  {
    Options::addOptionCardId(options);
    options.add_options()("random-event-size",
                          po::bool_switch(&mOptions.randomEventSize),
                          "Toggle random event size");
    options.add_options()("event-size",
                          po::value<uint32_t>(&mOptions.eventSize)->default_value(100),
                          "Set the event size (in GBT words = 128bits)");
    options.add_options()("status",
                          po::bool_switch(&mOptions.status),
                          "Print UL status only");
  }

  virtual void run(const boost::program_options::variables_map& map)
  {

    auto cardId = Options::getOptionCardId(map);
    auto card = RocPciDevice(cardId).getCardDescriptor();
    auto cardType = card.cardType;
    if (cardType != CardType::type::Cru) {
      std::cerr << "Unsupported card type, only CRU supported." << std::endl;
      return;
    }

    Parameters params = Parameters::makeParameters(cardId, 2);
    auto bar = ChannelFactory().getBar(params);
    auto cruBar2 = std::dynamic_pointer_cast<CruBar>(bar);
    if (mOptions.status) {
      Cru::UserLogicInfo ulInfo = cruBar2->reportUserLogic();
      std::cout << "==========================" << std::endl;
      std::cout << "Event size: " << ulInfo.eventSize << " GBT words" << std::endl;
      std::cout << "Event size: " << (ulInfo.eventSize * 128) / 1024.0 << "Kb" << std::endl;
      std::cout << "Event size: " << (ulInfo.eventSize * 128) / (1024.0 * 8) << "KB" << std::endl;
      std::cout << "Randomized: " << std::boolalpha << ulInfo.random << std::endl;
      std::cout << "==========================" << std::endl;
    } else {
      cruBar2->controlUserLogic(mOptions.eventSize, mOptions.randomEventSize);
    }
  }

 private:
  struct OptionsStruct {
    uint32_t eventSize;
    bool randomEventSize;
    bool status;
  } mOptions;
};

int main(int argc, char** argv)
{
  return ProgramUserLogic().execute(argc, argv);
}