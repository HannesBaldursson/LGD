/*
 * Copyright (c) 2022 Huawei Device Co., Ltd.
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <thread>
#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <securec.h>

#include "lnn_lane_common.h"
#include "lnn_lane_deps_mock.h"
#include "lnn_trans_lane_deps_mock.h"
#include "lnn_lane_interface.h"
#include "lnn_lane_score_virtual.c"
#include "lnn_trans_lane.h"
#include "message_handler.h"
#include "softbus_adapter_mem.h"
#include "softbus_error_code.h"

namespace OHOS {
using namespace testing::ext;
using namespace testing;

constexpr int32_t CHANNEL_ID = 5;
constexpr int32_t INTERVAL = 2;
constexpr uint32_t LIST_SIZE = 10;

class LNNTransLaneMockTest : public testing::Test {
public:
    static void SetUpTestCase();
    static void TearDownTestCase();
    void SetUp();
    void TearDown();
};

void LNNTransLaneMockTest::SetUpTestCase()
{
    GTEST_LOG_(INFO) << "LNNTransLaneMockTest start";
    LnnInitLaneLooper();
}

void LNNTransLaneMockTest::TearDownTestCase()
{
    LnnDeinitLaneLooper();
    GTEST_LOG_(INFO) << "LNNTransLaneMockTest end";
}

void LNNTransLaneMockTest::SetUp()
{
}

void LNNTransLaneMockTest::TearDown()
{
}

/*
* @tc.name: LNN_TRANS_LANE_001
* @tc.desc: Init
* @tc.type: FUNC
* @tc.require:
*/
HWTEST_F(LNNTransLaneMockTest, LNN_TRANS_LANE_001, TestSize.Level1)
{
    LaneDepsInterfaceMock mock;
    EXPECT_CALL(mock, StartBaseClient).WillRepeatedly(Return(SOFTBUS_OK));
    LaneInterface *transObj = TransLaneGetInstance();
    EXPECT_TRUE(transObj != nullptr);
    transObj->Init(nullptr);

    uint32_t laneReqId = 1;
    int32_t ret = transObj->AllocLane(laneReqId, nullptr, nullptr);
    EXPECT_TRUE(ret != SOFTBUS_OK);

    LaneRequestOption request;
    request.type = LANE_TYPE_BUTT;
    ret = transObj->AllocLane(laneReqId, (const LaneRequestOption *)&request, nullptr);
    EXPECT_TRUE(ret != SOFTBUS_OK);

    transObj->Deinit();
}

/*
* @tc.name: LNN_TRANS_LANE_002
* @tc.desc: Callback process
* @tc.type: FUNC
* @tc.require:
*/
HWTEST_F(LNNTransLaneMockTest, LNN_TRANS_LANE_002, TestSize.Level1)
{
    LaneDepsInterfaceMock mock;
    EXPECT_CALL(mock, StartBaseClient).WillRepeatedly(Return(SOFTBUS_OK));
    TransLaneDepsInterfaceMock laneMock;
    LaneInterface *transObj = TransLaneGetInstance();
    EXPECT_TRUE(transObj != nullptr);
    transObj->Init(nullptr);
    uint32_t laneReqId = 1;
    LaneRequestOption request;
    request.type = LANE_TYPE_TRANS;
    EXPECT_CALL(laneMock, SelectLane).WillOnce(Return(SOFTBUS_OK));
    int32_t ret = transObj->AllocLane(laneReqId, (const LaneRequestOption *)&request, nullptr);
    EXPECT_TRUE(ret != SOFTBUS_OK);
    std::this_thread::sleep_for(std::chrono::milliseconds(200)); // delay 200ms for looper completion.
    transObj->Deinit();
}

/*
* @tc.name: LNN_TRANS_LANE_003
* @tc.desc: Callback process
* @tc.type: FUNC
* @tc.require:
*/
HWTEST_F(LNNTransLaneMockTest, LNN_TRANS_LANE_003, TestSize.Level1)
{
    LaneDepsInterfaceMock mock;
    EXPECT_CALL(mock, StartBaseClient).WillRepeatedly(Return(SOFTBUS_OK));
    TransLaneDepsInterfaceMock laneMock;
    LaneInterface *transObj = TransLaneGetInstance();
    EXPECT_TRUE(transObj != nullptr);
    transObj->Init(nullptr);
    uint32_t laneReqId = 1;
    LaneRequestOption request;
    request.type = LANE_TYPE_TRANS;
    EXPECT_CALL(laneMock, SelectExpectLaneByParameter).WillOnce(Return(SOFTBUS_ERR));
    EXPECT_CALL(laneMock, SelectExpectLanesByQos).WillOnce(Return(SOFTBUS_OK));
    int32_t ret = transObj->allocLaneByQos(laneReqId, (const LaneRequestOption *)&request, nullptr);
    EXPECT_TRUE(ret != SOFTBUS_OK);
    std::this_thread::sleep_for(std::chrono::milliseconds(200)); // delay 200ms for looper completion.
    transObj->Deinit();
}

/*
* @tc.name: LNN_LANE_SCORE_VIRTUAL_001
* @tc.desc: lnn_lane_score_virtual.c
* @tc.type: FUNC
* @tc.require:
*/
HWTEST_F(LNNTransLaneMockTest, LNN_LANE_SCORE_VIRTUAL_001, TestSize.Level1)
{
    uint32_t listSize = LIST_SIZE;
    int32_t ret = LnnInitScore();
    EXPECT_TRUE(ret == SOFTBUS_OK);
    LnnDeinitScore();
    ret = LnnGetWlanLinkedInfo(nullptr);
    EXPECT_TRUE(ret == SOFTBUS_ERR);
    ret = LnnGetCurrChannelScore(CHANNEL_ID);
    EXPECT_TRUE(ret == VIRTUAL_DEFAULT_SCORE);
    ret = LnnStartScoring(INTERVAL);
    EXPECT_TRUE(ret == SOFTBUS_OK);
    ret = LnnStopScoring();
    EXPECT_TRUE(ret == SOFTBUS_OK);
    ret = LnnGetAllChannelScore(nullptr, &listSize);
    EXPECT_TRUE(ret == SOFTBUS_OK);
}
} // namespace OHOS