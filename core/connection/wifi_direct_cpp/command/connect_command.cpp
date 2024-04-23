/*
 * Copyright (c) 2024 Huawei Device Co., Ltd.
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
#include "connect_command.h"

#include "conn_log.h"

#include "channel/auth_negotiate_channel.h"
#include "channel/proxy_negotiate_channel.h"
#include "data/link_manager.h"
#include "processor_selector_factory.h"
#include "utils/wifi_direct_utils.h"

namespace OHOS::SoftBus {
ConnectCommand::ConnectCommand(const WifiDirectConnectInfo &info, const WifiDirectConnectCallback &callback) :
    callback_(callback)
{
    info_.info_ = info;
    remoteDeviceId_ = WifiDirectUtils::NetworkIdToUuid(info_.info_.remoteNetworkId);
}

std::string ConnectCommand::GetRemoteDeviceId() const
{
    if (!remoteDeviceId_.empty()) {
        return remoteDeviceId_;
    }
    remoteDeviceId_ = WifiDirectUtils::NetworkIdToUuid(info_.info_.remoteNetworkId);
    return remoteDeviceId_;
}

std::shared_ptr<WifiDirectProcessor> ConnectCommand::GetProcessor()
{
    CONN_LOGI(CONN_WIFI_DIRECT, "enter");
    auto selector = ProcessorSelectorFactory::GetInstance().NewSelector();
    if (selector == nullptr) {
        CONN_LOGE(CONN_WIFI_DIRECT, "selector is null");
        return nullptr;
    }
    return (*selector)(info_.info_);
}

ConnectInfo ConnectCommand::GetConnectInfo() const
{
    return info_;
}

WifiDirectConnectCallback ConnectCommand::GetConnectCallback() const
{
    return callback_;
}

void ConnectCommand::OnSuccess(const WifiDirectLink &link) const
{
    CONN_LOGI(CONN_WIFI_DIRECT, "requestId=%{public}u linkId=%{public}d", info_.info_.requestId, link.linkId);
    callback_.onConnectSuccess(info_.info_.requestId, &link);
}

void ConnectCommand::OnFailure(WifiDirectErrorCode reason) const
{
    CONN_LOGI(CONN_WIFI_DIRECT, "requestId=%{public}u reason=%{public}d", info_.info_.requestId, reason);
    callback_.onConnectFailure(info_.info_.requestId, reason);
}

void ConnectCommand::PreferNegotiateChannel()
{
    auto innerLink = LinkManager::GetInstance().GetReuseLink(info_.info_.connectType, remoteDeviceId_);
    if (innerLink == nullptr || innerLink->GetNegotiateChannel() == nullptr) {
        if (info_.info_.negoChannel.type == NEGO_CHANNEL_AUTH) {
            CONN_LOGI(CONN_WIFI_DIRECT, "prefer input auth channel");
            info_.channel_ = std::make_shared<AuthNegotiateChannel>(info_.info_.negoChannel.handle.authHandle);
        }
        if (info_.info_.negoChannel.type == NEGO_CHANNEL_COC) {
            CONN_LOGI(CONN_WIFI_DIRECT, "prefer input proxy channel");
            info_.channel_ = std::make_shared<CoCProxyNegotiateChannel>(info_.info_.negoChannel.handle.channelId);
        }
        return;
    }

    CONN_LOGI(CONN_WIFI_DIRECT, "prefer inner channel");
    info_.channel_ = innerLink->GetNegotiateChannel();
}
} // namespace OHOS::SoftBus
