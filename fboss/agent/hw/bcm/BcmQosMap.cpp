/*
 *  Copyright (c) 2004-present, Facebook, Inc.
 *  All rights reserved.
 *
 *  This source code is licensed under the BSD-style license found in the
 *  LICENSE file in the root directory of this source tree. An additional grant
 *  of patent rights can be found in the PATENTS file in the same directory.
 *
 */
#include "fboss/agent/hw/bcm/BcmQosMap.h"
#include "fboss/agent/FbossError.h"
#include "fboss/agent/hw/bcm/BcmQosPolicy.h"

#include "fboss/agent/hw/bcm/BcmPortQueueManager.h"
#include "fboss/agent/hw/bcm/BcmSwitch.h"
#include "fboss/agent/state/QosPolicy.h"

namespace {
constexpr auto kDefaultQueue = 0;
}

namespace facebook {
namespace fboss {

bool BcmQosMap::ruleExists(const QosRule& qosRule) const {
  return entries_.find(qosRule) != entries_.end();
}

size_t BcmQosMap::size() const {
  return entries_.size();
}

int BcmQosMap::getFlags() const {
  return flags_;
}

int BcmQosMap::getUnit() const {
  return hw_->getUnit();
}

int BcmQosMap::getHandle() const {
  return handle_;
}

bool BcmQosMap::rulesMatch(const std::set<QosRule>& qosRules) const {
  int qosRuleWithNonDefaultQueue = 0;
  for (const auto& qosRule : qosRules) {
    // Skip rules with the default queue
    if (qosRule.queueId == kDefaultQueue) {
      continue;
    }
    if (!ruleExists(qosRule)) {
      return false;
    }
    ++qosRuleWithNonDefaultQueue;
  }
  return qosRuleWithNonDefaultQueue == size();
}

} // namespace fboss
} // namespace facebook
