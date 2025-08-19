/*
 * COPYRIGHT (C) 2023 MITSUBISHI ELECTRIC CORPORATION
 * ALL RIGHTS RESERVED
 */

#include "ETimerType.h"

#include <gtest/gtest.h>

namespace com {
namespace mitsubishielectric {
namespace ahu {
namespace vehiclepwrmgr {
namespace impl {

TEST(ETimerTypeTest, toString)
{
    ASSERT_EQ(toString(ETimerType::None), "None");
    ASSERT_EQ(toString(ETimerType::WDTRTimer), "WDTRTimer");
    ASSERT_EQ(toString(ETimerType::ADTRTimer), "ADTRTimer");
}

}  // namespace impl
}  // namespace vehiclepwrmgr
}  // namespace ahu
}  // namespace mitsubishielectric
}  // namespace com
