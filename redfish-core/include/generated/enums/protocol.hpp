// SPDX-License-Identifier: Apache-2.0
// SPDX-FileCopyrightText: Copyright OpenBMC Authors
#pragma once
#include <nlohmann/json.hpp>

namespace protocol
{
// clang-format off

enum class Protocol{
    Invalid,
    PCIe,
    AHCI,
    UHCI,
    SAS,
    SATA,
    USB,
    NVMe,
    FC,
    iSCSI,
    FCoE,
    FCP,
    FICON,
    NVMeOverFabrics,
    SMB,
    NFSv3,
    NFSv4,
    HTTP,
    HTTPS,
    FTP,
    SFTP,
    iWARP,
    RoCE,
    RoCEv2,
    I2C,
    TCP,
    UDP,
    TFTP,
    GenZ,
    MultiProtocol,
    InfiniBand,
    Ethernet,
    NVLink,
    OEM,
    DisplayPort,
    HDMI,
    VGA,
    DVI,
    CXL,
    UPI,
    QPI,
    eMMC,
    UET,
};

NLOHMANN_JSON_SERIALIZE_ENUM(Protocol, {
    {Protocol::Invalid, "Invalid"},
    {Protocol::PCIe, "PCIe"},
    {Protocol::AHCI, "AHCI"},
    {Protocol::UHCI, "UHCI"},
    {Protocol::SAS, "SAS"},
    {Protocol::SATA, "SATA"},
    {Protocol::USB, "USB"},
    {Protocol::NVMe, "NVMe"},
    {Protocol::FC, "FC"},
    {Protocol::iSCSI, "iSCSI"},
    {Protocol::FCoE, "FCoE"},
    {Protocol::FCP, "FCP"},
    {Protocol::FICON, "FICON"},
    {Protocol::NVMeOverFabrics, "NVMeOverFabrics"},
    {Protocol::SMB, "SMB"},
    {Protocol::NFSv3, "NFSv3"},
    {Protocol::NFSv4, "NFSv4"},
    {Protocol::HTTP, "HTTP"},
    {Protocol::HTTPS, "HTTPS"},
    {Protocol::FTP, "FTP"},
    {Protocol::SFTP, "SFTP"},
    {Protocol::iWARP, "iWARP"},
    {Protocol::RoCE, "RoCE"},
    {Protocol::RoCEv2, "RoCEv2"},
    {Protocol::I2C, "I2C"},
    {Protocol::TCP, "TCP"},
    {Protocol::UDP, "UDP"},
    {Protocol::TFTP, "TFTP"},
    {Protocol::GenZ, "GenZ"},
    {Protocol::MultiProtocol, "MultiProtocol"},
    {Protocol::InfiniBand, "InfiniBand"},
    {Protocol::Ethernet, "Ethernet"},
    {Protocol::NVLink, "NVLink"},
    {Protocol::OEM, "OEM"},
    {Protocol::DisplayPort, "DisplayPort"},
    {Protocol::HDMI, "HDMI"},
    {Protocol::VGA, "VGA"},
    {Protocol::DVI, "DVI"},
    {Protocol::CXL, "CXL"},
    {Protocol::UPI, "UPI"},
    {Protocol::QPI, "QPI"},
    {Protocol::eMMC, "eMMC"},
    {Protocol::UET, "UET"},
});

}
// clang-format on
