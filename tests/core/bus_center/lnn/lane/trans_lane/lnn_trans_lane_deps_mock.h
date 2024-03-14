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

#ifndef LNN_TRANS_LANE_DEPS_MOCK_H
#define LNN_TRANS_LANE_DEPS_MOCK_H

#include <gmock/gmock.h>

#include "lnn_lane_common.h"
#include "lnn_lane_link.h"
#include "lnn_lane_model.h"
#include "lnn_lane_select.h"
#include "lnn_lane_reliability.h"

namespace OHOS {
class TransLaneDepsInterface {
public:
    TransLaneDepsInterface() {};
    virtual ~TransLaneDepsInterface() {};

    virtual int32_t SelectLane(const char *networkId, const LaneSelectParam *request,
    LanePreferredLinkList *recommendList, uint32_t *listNum);
    virtual int32_t SelectExpectLanesByQos(const char *networkId, const LaneSelectParam *request,
    LanePreferredLinkList *recommendList);
    virtual int32_t BuildLink(const LinkRequest *reqInfo, uint32_t reqId, const LaneLinkCb *cb) = 0;
    virtual void DestroyLink(const char *networkId, uint32_t reqId, LaneLinkType type, int32_t pid) = 0;
    virtual uint32_t GenerateLaneProfileId(const LaneGenerateParam *param) = 0;
    virtual void UnbindLaneReqIdFromProfile(uint32_t laneReqId, uint32_t profileId) = 0;
    virtual int32_t BindLaneIdToProfile(uint64_t laneId, LaneProfile *profile) = 0;
    virtual int32_t AddLaneResourceItem(const LaneResource *resourceItem) = 0;
    virtual int32_t DelLaneResourceItem(const LaneResource *resourceItem) = 0;
    virtual int32_t AddLinkInfoItem(const LaneLinkInfo *linkInfoItem) = 0;
    virtual int32_t DelLinkInfoItem(uint32_t laneReqId) = 0;
    virtual int32_t FindLaneLinkInfoByLaneReqId(uint32_t laneReqId, LaneLinkInfo *linkInfoitem) = 0;
    virtual int32_t ConvertToLaneResource(const LaneLinkInfo *linkInfo, LaneResource *laneResourceInfo) = 0;
    virtual int32_t DelLaneResourceItemWithDelay(LaneResource *resourceItem, uint32_t laneReqId,
        bool *isDelayDestroy) = 0;
    virtual void FreeLaneReqId(uint32_t laneReqId) = 0;
    virtual void HandleLaneReliabilityTime(void) = 0;
    virtual int32_t SelectExpectLaneByParameter(LanePreferredLinkList *setRecommendLinkList) = 0;
};

class TransLaneDepsInterfaceMock : public TransLaneDepsInterface {
public:
    TransLaneDepsInterfaceMock();
    ~TransLaneDepsInterfaceMock() override;
    MOCK_METHOD4(SelectLane, int32_t (const char*, const LaneSelectParam *, LanePreferredLinkList *, uint32_t *));
    MOCK_METHOD3(SelectExpectLanesByQos, int32_t (const char*, const LaneSelectParam *, LanePreferredLinkList *));
    MOCK_METHOD3(BuildLink, int32_t (const LinkRequest *, uint32_t, const LaneLinkCb *));
    MOCK_METHOD4(DestroyLink, void (const char *, uint32_t, LaneLinkType, int32_t));
    MOCK_METHOD1(GenerateLaneProfileId, uint32_t (const LaneGenerateParam *));
    MOCK_METHOD2(UnbindLaneReqIdFromProfile, void (uint32_t, uint32_t));
    MOCK_METHOD2(BindLaneIdToProfile, int32_t (uint64_t, LaneProfile *));
    MOCK_METHOD1(AddLaneResourceItem, int32_t (const LaneResource *resourceItem));
    MOCK_METHOD1(DelLaneResourceItem, int32_t (const LaneResource *resourceItem));
    MOCK_METHOD1(AddLinkInfoItem, int32_t (const LaneLinkInfo *linkInfoItem));
    MOCK_METHOD1(DelLinkInfoItem, int32_t (uint32_t laneReqId));
    MOCK_METHOD2(FindLaneLinkInfoByLaneReqId, int32_t (uint32_t laneReqId, LaneLinkInfo *linkInfoitem));
    MOCK_METHOD2(ConvertToLaneResource, int32_t (const LaneLinkInfo *linkInfo, LaneResource *laneResourceInfo));
    MOCK_METHOD3(DelLaneResourceItemWithDelay, int32_t (LaneResource *resourceItem, uint32_t laneReqId,
        bool *isDelayDestroy));
    MOCK_METHOD1(FreeLaneReqId, void (uint32_t laneReqId));
    MOCK_METHOD0(HandleLaneReliabilityTime, void ());
    MOCK_METHOD1(SelectExpectLaneByParameter, int32_t (LanePreferredLinkList *));
};
} // namespace OHOS
#endif // LNN_TRANS_LANE_DEPS_MOCK_H