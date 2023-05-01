﻿#pragma once

#include <Objects.hpp>
#include <NumTypes.hpp>
#include <ConPrinter.hpp>

#include <cstring>

#undef DeviceCapabilities

struct PciConfigHeader final
{
    u16 VendorID;
    u16 DeviceID;
    u16 Command;
    u16 Status;
    u32 RevisionID : 8;
    u32 ClassCode : 24;
    u8 CacheLineSize;
    u8 MasterLatencyTimer;
    u8 HeaderType;
    u8 BIST;
    u32 BAR0;
    u32 BAR1;
    u32 BAR2;
    u32 BAR3;
    u32 BAR4;
    u32 BAR5;
    u32 CardBusCISPointer;
    u16 SubsystemVendorID;
    u16 SubsystemID;
    u32 ExpansionROMBaseAddress;
    u32 CapPointer : 8;
    u32 Reserved0 : 24;
    u32 Reserved1;
    u8 InterruptLine;
    u8 InterruptPin;
    u8 MinGnt;
    u8 MaxLat;
};

static_assert(sizeof(PciConfigHeader) == 64, "PCI Config Header is not 64 bytes.");

struct PciCapabilityHeader final
{
    u8 CapabilityId;
    u8 NextCapabilityPointer;
};

static_assert(sizeof(PciCapabilityHeader) == 2, "PCI Capability Header is not 2 bytes.");

struct PciExtendedCapabilityHeader final
{
    u16 CapabilityId;
    u16 CapabilityVersion : 4;
    u16 NextCapabilityPointer : 12;
};

static_assert(sizeof(PciExtendedCapabilityHeader) == 4, "PCIe Extended Capability Header is not 2 bytes.");

struct PcieCapabilitiesRegister final
{
    u16 CapabilityVersion : 4;
    u16 DeviceType : 4;
    u16 SlotImplemented : 1;
    u16 InterruptMessageNumber : 5;
    u16 ReservedP : 2;
};

static_assert(sizeof(PcieCapabilitiesRegister) == 2, "PCIe Capabilities Register is not 2 bytes.");

struct DeviceCapabilitiesRegister final
{
    u32 MaxPayloadSizeSupported : 3;
    u32 PhantomFunctionsSupported : 2;
    u32 ExtendedTagFieldSupported : 1;
    u32 EndpointL0sAcceptableLatency : 3;
    u32 EndpointL1AcceptableLatency : 3;
    u32 Undefined : 3;
    u32 RoleBasedErrorReporting : 1;
    u32 ReservedP0 : 2;
    u32 CapturedSlotPowerLimitValue : 8;
    u32 CapturedSlotPowerLimitScale : 2;
    u32 ReservedP1 : 4;
};

static_assert(sizeof(DeviceCapabilitiesRegister) == 4, "Device Capabilities Register is not 4 bytes.");

union DeviceControlRegister final
{
    u16 Packed;
    struct
    {
        u16 CorrectableErrorReportingEnable : 1;
        u16 NonFatalErrorReportingEnable : 1;
        u16 FatalErrorReportingEnable : 1;
        u16 UnsupportedRequestReportingEnable : 1;
        u16 EnableRelaxedOrdering : 1;
        u16 MaxPayloadSize : 3;
        u16 ExtendedTagFieldEnable : 1;
        u16 PhantomFunctionsEnable : 1;
        u16 AuxPowerPmEnable : 1;
        u16 EnableSnoopNotRequired : 1;
        u16 MaxReadRequestSize : 3;
        u16 Reserved : 1;
    };
};

static_assert(sizeof(DeviceControlRegister) == 2, "Device Control Register is not 2 bytes.");

struct DeviceStatusRegister final
{
    u16 CorrectableErrorDetected : 1;
    u16 NonFatalErrorDetected : 1;
    u16 FatalErrorDetected : 1;
    u16 UnsupportedRequestDetected : 1;
    u16 AuxPowerDetected : 1;
    u16 TransactionsPending : 1;
    u16 ReservedZ : 10;
};

static_assert(sizeof(DeviceStatusRegister) == 2, "Device Status Register is not 2 bytes.");

struct LinkCapabilitiesRegister final
{
    u32 MaximumLinkSpeed : 4;
    u32 MaximumLinkWidth : 6;
    u32 ASPMSupport : 2;
    u32 L0sExitLatency : 3;
    u32 L1ExitLatency : 3;
    u32 ClockPowerManagement : 1;
    u32 SurpriseDownErrorReportingCapable : 1;
    u32 DataLinkLayerActiveReportingCapable : 1;
    u32 Reserved : 3;
    u32 PortNumber : 8;
};

static_assert(sizeof(LinkCapabilitiesRegister) == 4, "Link Capabilities Register is not 4 bytes.");

union LinkControlRegister final
{
    u16 Packed;
    struct
    {
        u16 ASPMControl : 2;
        u16 ReservedP0 : 1;
        u16 ReadCompletionBoundary : 1;
        u16 LinkDisable : 1;
        u16 RetrainLink : 1;
        u16 CommonClockConfiguration : 1;
        u16 ExtendedSynch : 1;
        u16 EnableClockPowerManagement : 1;
        u16 ReservedP1 : 7;
    };
};

static_assert(sizeof(LinkControlRegister) == 2, "Link Control Register is not 2 bytes.");

struct LinkStatusRegister final
{
    u16 LinkSpeed : 4;
    u16 NegotiatedLinkWidth : 6;
    u16 Undefined : 1;
    u16 LinkTraining : 1;
    u16 SlotClockConfiguration : 1;
    u16 DataLinkLayerActive : 1;
    u16 ReservedZ : 2;
};

static_assert(sizeof(LinkStatusRegister) == 2, "Link Status Register is not 2 bytes.");

struct PcieCapabilityStructure final
{
    PciCapabilityHeader Header;
    PcieCapabilitiesRegister CapabilitiesRegister;
    DeviceCapabilitiesRegister DeviceCapabilities;
    DeviceControlRegister DeviceControl;
    DeviceStatusRegister DeviceStatus;
    LinkCapabilitiesRegister LinkCapabilities;
    LinkControlRegister LinkControl;
    LinkStatusRegister LinkStatus;
};

static_assert(sizeof(PcieCapabilityStructure) == 0x14, "PCIe Capability Structure is not 20 bytes.");

union PowerManagementCapabilitiesRegister final
{
    u16 Packed;
    struct
    {
        u16 Version : 3;
        u16 PmeClock : 1;
        u16 Reserved : 1;
        u16 DeviceSpecificInitialization : 1;
        u16 AuxCurrent : 3;
        u16 D1Support : 1;
        u16 D2Support : 1;
        u16 PmeSupport : 5;
    };
};

static_assert(sizeof(PowerManagementCapabilitiesRegister) == 2, "Power Management Capability Register is not 2 bytes.");

union PowerManagementControlStatusRegister final
{
    u16 Packed;
    struct
    {
        u16 PowerState : 2;
        u16 Reserved0 : 1;
        u16 NoSoftReset : 1;
        u16 Reserved1 : 4;
        u16 PmeEnable : 1;
        u16 DataSelect : 4;
        u16 DataScale : 2;
        u16 PmeStatus : 1;
    };
};

static_assert(sizeof(PowerManagementControlStatusRegister) == 2, "Power Management Control/Status Register is not 2 bytes.");

struct PowerManagementCapabilityStructure final
{
    PciCapabilityHeader Header;
    PowerManagementCapabilitiesRegister PowerManagementCapabilities;
    PowerManagementControlStatusRegister PowerManagementControlStatusRegister;
    u8 BridgeExtensions;
    u8 Data;
};

static_assert(sizeof(PowerManagementCapabilityStructure) == 8, "PCI Power Management Capability Structure is not 8 bytes.");

struct AdvancedErrorReportingCapabilityStructure final
{
    PciExtendedCapabilityHeader Header;
    u32 UncorrectableErrorStatusRegister;
    u32 UncorrectableErrorMaskRegister;
    u32 UncorrectableErrorSeverityRegister;
    u32 CorrectableErrorStatusRegister;
    u32 CorrectableErrorMaskRegister;
    u32 AdvancedCapabilitiesAndControlRegister;
    u32 HeaderLogRegister[4];
};

static_assert(sizeof(AdvancedErrorReportingCapabilityStructure) == 0x2C, "Advanced Error Reporting Capability Structure is not 44 bytes.");

class PciController final
{
    DEFAULT_DESTRUCT(PciController);
    DELETE_CM(PciController);
public:
    static inline constexpr u16 COMMAND_REGISTER_MASK_BITS = 0x0446;

    static inline constexpr u32 BAR0_MASK_BITS = 0xFF000000;
    static inline constexpr u32 BAR1_MASK_BITS = 0x80000000;
    static inline constexpr u32 BAR2_MASK_BITS = 0xFFFFFFFF;
    static inline constexpr u32 BAR3_MASK_BITS = 0x00000000;
    static inline constexpr u32 BAR4_MASK_BITS = 0x00000000;
    static inline constexpr u32 BAR5_MASK_BITS = 0x00000000;

    static inline constexpr u32 BAR0_READ_ONLY_BITS = 0x00000000;
    static inline constexpr u32 BAR1_READ_ONLY_BITS = 0x0000000C;
    static inline constexpr u32 BAR2_READ_ONLY_BITS = 0x00000000;
    static inline constexpr u32 BAR3_READ_ONLY_BITS = 0x00000000;
    static inline constexpr u32 BAR4_READ_ONLY_BITS = 0x00000000;
    static inline constexpr u32 BAR5_READ_ONLY_BITS = 0x00000000;

    static inline constexpr u32 EXPANSION_ROM_BAR_MASK_BITS         = 0xFFFF8001;
    static inline constexpr u32 EXPANSION_ROM_BAR_ADDRESS_MASK_BITS = 0xFFFF8000;
    static inline constexpr u32 EXPANSION_ROM_BAR_READ_ONLY_BITS    = 0x00000000;
    static inline constexpr u32 EXPANSION_ROM_BAR_ENABLE_BIT        = 0x00000001;
    static inline constexpr u32 EXPANSION_ROM_SIZE = 32 * 1024;

    static inline constexpr u16 DEVICE_CONTROL_REGISTER_MASK_BITS      = 0x7CFF;
    static inline constexpr u16 DEVICE_CONTROL_REGISTER_READ_ONLY_BITS = 0x0000;

    static inline constexpr u16 LINK_CONTROL_REGISTER_MASK_BITS = 0x01C3;
    static inline constexpr u16 LINK_CONTROL_REGISTER_READ_ONLY_BITS = 0x0000;

    static inline constexpr u16 PM_CONTROL_REGISTER_MASK_BITS      = 0x0003;
    static inline constexpr u16 PM_CONTROL_REGISTER_READ_ONLY_BITS = 0x0000;

    static inline constexpr u16 COMMAND_REGISTER_MEMORY_SPACE_BIT = 0x0002;

    static inline constexpr u8 EXPANSION_ROM_BAR_ID = 0x7F;
public:
    PciController() noexcept
        : m_PciConfig{ 0 }
        , m_PciExtendedConfig{ 0 }
    {
        InitConfigHeader();
        InitPcieCapabilityStructure();
        InitPowerManagementCapabilityStructure();
        InitAdvancedErrorReportingCapabilityStructure();
    }

    [[nodiscard]] u32 ConfigRead(u16 address, u8 size) noexcept;

    void ConfigWrite(const u16 address, const u32 size, const u32 value) noexcept
    {
        switch(address)
        {
            case 4:
                if(size != 2)
                {
                    break;
                }
                // Mask the command to only the RW bits.
                m_ConfigHeader.Command = static_cast<u16>(value & COMMAND_REGISTER_MASK_BITS);
                break;
            case 6:
                if(size != 2)
                {
                    break;
                }
                // Mask the status to only the RW bits.
                m_ConfigHeader.Status = static_cast<u16>(value & 0xFB00);
                break;
            case 0xC:
                if(size != 1)
                {
                    break;
                }
                m_ConfigHeader.CacheLineSize = static_cast<u8>(value);
                break;
            case 0x10:
                if(size != 4)
                {
                    break;
                }
                m_ConfigHeader.BAR0 = (value & BAR0_MASK_BITS) | BAR0_READ_ONLY_BITS;
                break;
            case 0x14:
                if(size != 4)
                {
                    break;
                }
                m_ConfigHeader.BAR1 = (value & BAR1_MASK_BITS) | BAR1_READ_ONLY_BITS;
                break;
            case 0x18:
                if(size != 4)
                {
                    break;
                }
                m_ConfigHeader.BAR2 = (value & BAR2_MASK_BITS) | BAR2_READ_ONLY_BITS;
                break;
            case 0x1C:
                if(size != 4)
                {
                    break;
                }
                m_ConfigHeader.BAR3 = (value & BAR3_MASK_BITS) | BAR3_READ_ONLY_BITS;
                break;
            case 0x20:
                if(size != 4)
                {
                    break;
                }
                m_ConfigHeader.BAR4 = (value & BAR4_MASK_BITS) | BAR4_READ_ONLY_BITS;
                break;
            case 0x24:
                if(size != 4)
                {
                    break;
                }
                m_ConfigHeader.BAR5 = (value & BAR5_MASK_BITS) | BAR5_READ_ONLY_BITS;
                break;
            case 0x30:
                if(size != 4)
                {
                    break;
                }
                m_ConfigHeader.ExpansionROMBaseAddress = (value & EXPANSION_ROM_BAR_MASK_BITS) | EXPANSION_ROM_BAR_READ_ONLY_BITS;
                break;
            case 0x3C:
                if(size != 1)
                {
                    break;
                }
                m_ConfigHeader.InterruptLine = static_cast<u8>(value);
                break;
            case offsetof(PciController, m_PcieCapability) + offsetof(PcieCapabilityStructure, DeviceControl):
                if(size != 2)
                {
                    break;
                }
                m_PcieCapability.DeviceControl.Packed = (value & DEVICE_CONTROL_REGISTER_MASK_BITS) | DEVICE_CONTROL_REGISTER_READ_ONLY_BITS;
                break;
            case offsetof(PciController, m_PcieCapability) + offsetof(PcieCapabilityStructure, LinkControl):
                if(size != 2)
                {
                    break;
                }
                m_PcieCapability.LinkControl.Packed = (value & LINK_CONTROL_REGISTER_MASK_BITS) | LINK_CONTROL_REGISTER_READ_ONLY_BITS;
                break;
            case offsetof(PciController, m_PowerManagementCapability) + offsetof(PowerManagementCapabilityStructure, PowerManagementControlStatusRegister):
                if(size != 2)
                {
                    break;
                }
                m_PowerManagementCapability.PowerManagementControlStatusRegister.Packed = (value & PM_CONTROL_REGISTER_MASK_BITS) | PM_CONTROL_REGISTER_READ_ONLY_BITS;
                break;
            default: break;
        }
    }

    [[nodiscard]] u8 GetBARFromAddress(const u64 address) noexcept
    {
        if(address < 0xFFFFFFFF)
        {
            if(address >= (m_ConfigHeader.BAR0 & BAR0_MASK_BITS) && address < ((m_ConfigHeader.BAR0 & BAR0_MASK_BITS) + 16 * 1024 * 1024))
            {
                return 0;
            }

            if(address >= (m_ConfigHeader.ExpansionROMBaseAddress & EXPANSION_ROM_BAR_ADDRESS_MASK_BITS) && address < ((m_ConfigHeader.ExpansionROMBaseAddress & EXPANSION_ROM_BAR_ADDRESS_MASK_BITS) + EXPANSION_ROM_SIZE))
            {
                return EXPANSION_ROM_BAR_ID;
            }
        }

        const u64 bar1 = (static_cast<u64>(m_ConfigHeader.BAR2 & BAR2_MASK_BITS) << 32) | (m_ConfigHeader.BAR1 & BAR1_MASK_BITS);

        if(address >= bar1 && address < bar1 + (2ull * 1024ull * 1024ull * 1024ull))
        {
            return 1;
        }

        return 0xFF;
    }

    [[nodiscard]] u64 GetBAROffset(const u64 address, const u8 bar) noexcept
    {
        if(bar == 0)
        {
            return address - (m_ConfigHeader.BAR0 & BAR0_MASK_BITS);
        }

        if(bar == 1)
        {
            const u64 bar1 = (static_cast<u64>(m_ConfigHeader.BAR2 & BAR2_MASK_BITS) << 32) | (m_ConfigHeader.BAR1 & BAR1_MASK_BITS);

            return address - bar1;
        }

        if(bar == EXPANSION_ROM_BAR_ID)
        {
            return address - (m_ConfigHeader.ExpansionROMBaseAddress & EXPANSION_ROM_BAR_ADDRESS_MASK_BITS);
        }

        return address;
    }

    [[nodiscard]] u16 CommandRegister() const noexcept { return m_ConfigHeader.Command; }
    [[nodiscard]] bool ExpansionRomEnable() const noexcept { return m_ConfigHeader.ExpansionROMBaseAddress & EXPANSION_ROM_BAR_ENABLE_BIT; }
private:
    void InitConfigHeader() noexcept
    {
        m_ConfigHeader.VendorID = 0xFFFD;
        m_ConfigHeader.DeviceID = 0x0001;
        m_ConfigHeader.Command = 0x0000;
        m_ConfigHeader.Status = 0x0010;
        m_ConfigHeader.RevisionID = 0x01;
        m_ConfigHeader.ClassCode = 0x030001;
        m_ConfigHeader.CacheLineSize = 0x0;
        m_ConfigHeader.MasterLatencyTimer = 0x0;
        m_ConfigHeader.HeaderType = 0x00;
        m_ConfigHeader.BIST = 0x00;
        // Memory, 32 bit, Not Prefetchable.
        m_ConfigHeader.BAR0 = 0x00000000;
        // Memory, 64 bit, Prefetchable.
        m_ConfigHeader.BAR1 = 0x0000000C;
        // Part of BAR1
        m_ConfigHeader.BAR2 = 0x00000000;
        // Unused
        m_ConfigHeader.BAR3 = 0x00000000;
        // Unused
        m_ConfigHeader.BAR4 = 0x00000000;
        // Unused
        m_ConfigHeader.BAR5 = 0x00000000;
        m_ConfigHeader.CardBusCISPointer = 0x0;
        m_ConfigHeader.SubsystemVendorID = 0x0;
        m_ConfigHeader.SubsystemID = 0x0;
        // 32KiB, Not Enabled
        m_ConfigHeader.ExpansionROMBaseAddress = 0x00000000;
        // m_ConfigHeader.CapPointer = 0;
        m_ConfigHeader.CapPointer = offsetof(PciController, m_PcieCapability);
        m_ConfigHeader.Reserved0 = 0x0;
        m_ConfigHeader.Reserved1 = 0x0;
        m_ConfigHeader.InterruptLine = 0x0;
        m_ConfigHeader.InterruptPin = 0x0;
        m_ConfigHeader.MinGnt = 0x00;
        m_ConfigHeader.MaxLat = 0x00;
    }

    void InitPcieCapabilityStructure() noexcept
    {
        // PCI Express Capability ID
        m_PcieCapability.Header.CapabilityId = 0x10;
        m_PcieCapability.Header.NextCapabilityPointer = offsetof(PciController, m_PowerManagementCapability);
        m_PcieCapability.CapabilitiesRegister.CapabilityVersion = 0x01;
        // Legacy PCI Express Endpoint device, this is what my 3070 Ti reports, and as what makes the most sense based on its description.
        m_PcieCapability.CapabilitiesRegister.DeviceType = 0b0001;
        m_PcieCapability.CapabilitiesRegister.SlotImplemented = 0b0;
        m_PcieCapability.CapabilitiesRegister.InterruptMessageNumber = 0b00000;
        // 256 Bytes
        m_PcieCapability.DeviceCapabilities.MaxPayloadSizeSupported = 0b001;
        m_PcieCapability.DeviceCapabilities.PhantomFunctionsSupported = 0b00;
        m_PcieCapability.DeviceCapabilities.ExtendedTagFieldSupported = 0b1;
        // No limit
        m_PcieCapability.DeviceCapabilities.EndpointL0sAcceptableLatency = 0b111;
        // Maximum of 64 us
        m_PcieCapability.DeviceCapabilities.EndpointL1AcceptableLatency = 0b110;
        m_PcieCapability.DeviceCapabilities.Undefined = 0b000;
        m_PcieCapability.DeviceCapabilities.RoleBasedErrorReporting = 0b1;
        m_PcieCapability.DeviceCapabilities.ReservedP0 = 0b00;
        m_PcieCapability.DeviceCapabilities.CapturedSlotPowerLimitValue = 0x00;
        // 1.0x
        m_PcieCapability.DeviceCapabilities.CapturedSlotPowerLimitScale = 0b00;
        m_PcieCapability.DeviceCapabilities.ReservedP1 = 0x0;

        m_PcieCapability.DeviceControl.CorrectableErrorReportingEnable = 0b0;
        m_PcieCapability.DeviceControl.NonFatalErrorReportingEnable = 0b0;
        m_PcieCapability.DeviceControl.UnsupportedRequestReportingEnable = 0b0;
        m_PcieCapability.DeviceControl.EnableRelaxedOrdering = 0b1;
        m_PcieCapability.DeviceControl.MaxPayloadSize = 0b000;
        m_PcieCapability.DeviceControl.ExtendedTagFieldEnable = 0b0;
        m_PcieCapability.DeviceControl.PhantomFunctionsEnable = 0b0;
        m_PcieCapability.DeviceControl.AuxPowerPmEnable = 0b0;
        m_PcieCapability.DeviceControl.EnableSnoopNotRequired = 0b1;
        // 512 bytes. This is the defined default.
        m_PcieCapability.DeviceControl.MaxReadRequestSize = 0b010;
        m_PcieCapability.DeviceControl.Reserved = 0b0;

        m_PcieCapability.DeviceStatus.CorrectableErrorDetected = 0b0;
        m_PcieCapability.DeviceStatus.NonFatalErrorDetected = 0b0;
        m_PcieCapability.DeviceStatus.FatalErrorDetected = 0b0;
        m_PcieCapability.DeviceStatus.UnsupportedRequestDetected = 0b0;
        m_PcieCapability.DeviceStatus.TransactionsPending = 0b0;

        m_PcieCapability.LinkCapabilities.MaximumLinkSpeed = 0b0001;
        m_PcieCapability.LinkCapabilities.MaximumLinkWidth = 0b001000;
        m_PcieCapability.LinkCapabilities.ASPMSupport = 0b01;
        m_PcieCapability.LinkCapabilities.L0sExitLatency = 0b100;
        m_PcieCapability.LinkCapabilities.L1ExitLatency = 0b010;
        m_PcieCapability.LinkCapabilities.ClockPowerManagement = 0b1;
        m_PcieCapability.LinkCapabilities.SurpriseDownErrorReportingCapable = 0b0;
        m_PcieCapability.LinkCapabilities.DataLinkLayerActiveReportingCapable = 0b0;
        m_PcieCapability.LinkCapabilities.Reserved = 0x0;
        m_PcieCapability.LinkCapabilities.PortNumber = 0x00;

        m_PcieCapability.LinkControl.ASPMControl = 0b00;
        m_PcieCapability.LinkControl.ReservedP0 = 0b0;
        m_PcieCapability.LinkControl.ReadCompletionBoundary = 0b0;
        m_PcieCapability.LinkControl.LinkDisable = 0b0;
        m_PcieCapability.LinkControl.RetrainLink = 0b0;
        m_PcieCapability.LinkControl.CommonClockConfiguration = 0b0;
        m_PcieCapability.LinkControl.ExtendedSynch = 0b0;
        m_PcieCapability.LinkControl.EnableClockPowerManagement = 0b0;
        m_PcieCapability.LinkControl.ReservedP1 = 0b0000000;

        m_PcieCapability.LinkStatus.LinkSpeed = 0b0001;
        m_PcieCapability.LinkStatus.NegotiatedLinkWidth = 0b001000;
        m_PcieCapability.LinkStatus.Undefined = 0b0;
        m_PcieCapability.LinkStatus.LinkTraining = 0b0;
        m_PcieCapability.LinkStatus.SlotClockConfiguration = 0b0;
        m_PcieCapability.LinkStatus.DataLinkLayerActive = 0b0;
        m_PcieCapability.LinkStatus.ReservedZ = 0x0;
    }

    void InitPowerManagementCapabilityStructure() noexcept
    {
        // PCI Power Management Capability ID
        m_PowerManagementCapability.Header.CapabilityId = 0x01;
        m_PowerManagementCapability.Header.NextCapabilityPointer = 0x00;
        // The defined default version in PCI Bus Power Management Interface Specification Rev 1.2
        m_PowerManagementCapability.PowerManagementCapabilities.Version = 0b011;
        // PCI Express Base 1.1 requires this to be hardwired to 0
        m_PowerManagementCapability.PowerManagementCapabilities.PmeClock = 0b0;
        m_PowerManagementCapability.PowerManagementCapabilities.Reserved = 0b0;
        m_PowerManagementCapability.PowerManagementCapabilities.DeviceSpecificInitialization = 0b0;
        m_PowerManagementCapability.PowerManagementCapabilities.AuxCurrent = 0b000;
        m_PowerManagementCapability.PowerManagementCapabilities.D1Support = 0b0;
        m_PowerManagementCapability.PowerManagementCapabilities.D2Support = 0b0;
        m_PowerManagementCapability.PowerManagementCapabilities.PmeSupport = 0b00000;

        m_PowerManagementCapability.PowerManagementControlStatusRegister.PowerState = 0b00;
        m_PowerManagementCapability.PowerManagementControlStatusRegister.Reserved0 = 0b0;
        m_PowerManagementCapability.PowerManagementControlStatusRegister.NoSoftReset = 0b0;
        m_PowerManagementCapability.PowerManagementControlStatusRegister.Reserved1 = 0x0;
        m_PowerManagementCapability.PowerManagementControlStatusRegister.PmeEnable = 0b0;
        m_PowerManagementCapability.PowerManagementControlStatusRegister.DataSelect = 0x0;
        m_PowerManagementCapability.PowerManagementControlStatusRegister.DataScale = 0b00;
        m_PowerManagementCapability.PowerManagementControlStatusRegister.PmeStatus = 0b0;
    }

    void InitAdvancedErrorReportingCapabilityStructure() noexcept
    {
        // The defined ID for the Advanced Error Reporting Capability in the PCI Express Base 1.1 spec.
        m_AdvancedErrorReportingCapability.Header.CapabilityId = 0x0001;
        m_AdvancedErrorReportingCapability.Header.CapabilityVersion = 0x1;
        m_AdvancedErrorReportingCapability.Header.NextCapabilityPointer = 0x0;

        m_AdvancedErrorReportingCapability.UncorrectableErrorStatusRegister = 0x00000000;
        m_AdvancedErrorReportingCapability.UncorrectableErrorMaskRegister = 0x00000000;
        m_AdvancedErrorReportingCapability.UncorrectableErrorSeverityRegister = 0x00062030;
        m_AdvancedErrorReportingCapability.CorrectableErrorStatusRegister = 0x00000000;
        m_AdvancedErrorReportingCapability.CorrectableErrorMaskRegister = 0x00002000;
        m_AdvancedErrorReportingCapability.AdvancedCapabilitiesAndControlRegister = 0x00000000;
        m_AdvancedErrorReportingCapability.HeaderLogRegister[0] = 0;
        m_AdvancedErrorReportingCapability.HeaderLogRegister[1] = 0;
        m_AdvancedErrorReportingCapability.HeaderLogRegister[2] = 0;
        m_AdvancedErrorReportingCapability.HeaderLogRegister[3] = 0;
    }

private:
    PciConfigHeader m_ConfigHeader;
    PcieCapabilityStructure m_PcieCapability;
    PowerManagementCapabilityStructure m_PowerManagementCapability;
    u8 m_PciConfig[256 - sizeof(m_ConfigHeader) - sizeof(m_PcieCapability) - sizeof(m_PowerManagementCapability)];
    AdvancedErrorReportingCapabilityStructure m_AdvancedErrorReportingCapability;
    u8 m_PciExtendedConfig[4096 - 256 - sizeof(m_AdvancedErrorReportingCapability)];

    friend class PciConfigOffsets;
};

/**
 * \brief A class of offsets for the all of the PCI headers.
 *
 * This is a class so that it can be a friend of the PciController.
 */
class PciConfigOffsets final
{
    DELETE_CONSTRUCT(PciConfigOffsets);
    DELETE_DESTRUCT(PciConfigOffsets);
    DELETE_CM(PciConfigOffsets);
public:
    static inline constexpr u16 ConfigHeaderBegin = 0;
    static inline constexpr u16 ConfigHeaderEnd = ConfigHeaderBegin + sizeof(PciController::m_ConfigHeader);

    static inline constexpr u16 PciCapabilityOffsetBegin = offsetof(PciController, m_PcieCapability);
    static inline constexpr u16 PciCapabilityOffsetEnd = PciCapabilityOffsetBegin + sizeof(PciController::m_PcieCapability);

    static inline constexpr u16 PowerManagementCapabilityOffsetBegin = offsetof(PciController, m_PowerManagementCapability);
    static inline constexpr u16 PowerManagementCapabilityOffsetEnd = PowerManagementCapabilityOffsetBegin + sizeof(PciController::m_PowerManagementCapability);

    static inline constexpr u16 PciConfigOffsetBegin = offsetof(PciController, m_PciConfig);
    static inline constexpr u16 PciConfigOffsetEnd = PciConfigOffsetBegin + sizeof(PciController::m_PciConfig);

    static inline constexpr u16 AdvancedErrorReportingCapabilityOffsetBegin = offsetof(PciController, m_AdvancedErrorReportingCapability);
    static inline constexpr u16 AdvancedErrorReportingCapabilityOffsetEnd = AdvancedErrorReportingCapabilityOffsetBegin + sizeof(PciController::m_AdvancedErrorReportingCapability);

    static inline constexpr u16 PciExtendedConfigOffsetBegin = offsetof(PciController, m_PciExtendedConfig);
    static inline constexpr u16 PciExtendedConfigOffsetEnd = PciExtendedConfigOffsetBegin + sizeof(PciController::m_PciExtendedConfig);
};

inline u32 PciController::ConfigRead(const u16 address, const u8 size) noexcept
{
    if(size != 1 && size != 2 && size != 4)
    {
        return 0;
    }

    if(address < PciConfigOffsets::ConfigHeaderEnd)
    {
        if(address > (PciConfigOffsets::ConfigHeaderEnd - 4) && size == 4)
        {
            return 0;
        }

        if(address > (PciConfigOffsets::ConfigHeaderEnd - 2) && size == 2)
        {
            return 0;
        }

        u32 ret;
        (void) ::std::memcpy(&ret, reinterpret_cast<u8*>(&m_ConfigHeader) + address, size);
        return ret;
    }

    if(address < PciConfigOffsets::PciCapabilityOffsetEnd)
    {
        if(address > (PciConfigOffsets::PciCapabilityOffsetEnd - 4) && size == 4)
        {
            return 0;
        }

        if(address > (PciConfigOffsets::PciCapabilityOffsetEnd - 2) && size == 2)
        {
            return 0;
        }

        u32 ret;
        (void) ::std::memcpy(&ret, reinterpret_cast<u8*>(&m_PcieCapability) + (address - PciConfigOffsets::PciCapabilityOffsetBegin), size);
        return ret;
    }

    if(address < PciConfigOffsets::PowerManagementCapabilityOffsetEnd)
    {
        if(address > (PciConfigOffsets::PowerManagementCapabilityOffsetEnd - 4) && size == 4)
        {
            return 0;
        }

        if(address > (PciConfigOffsets::PowerManagementCapabilityOffsetEnd - 2) && size == 2)
        {
            return 0;
        }

        u32 ret;
        (void) ::std::memcpy(&ret, reinterpret_cast<u8*>(&m_PowerManagementCapability) + (address - PciConfigOffsets::PowerManagementCapabilityOffsetBegin), size);
        return ret;
    }

    if(address < PciConfigOffsets::PciConfigOffsetEnd)
    {
        if(address > (PciConfigOffsets::PciConfigOffsetEnd - 4) && size == 4)
        {
            return 0;
        }

        if(address > (PciConfigOffsets::PciConfigOffsetEnd - 2) && size == 2)
        {
            return 0;
        }

        u32 ret;
        (void) ::std::memcpy(&ret, &m_PciConfig[address - PciConfigOffsets::PciConfigOffsetBegin], size);
        return ret;
    }

    if(address < PciConfigOffsets::AdvancedErrorReportingCapabilityOffsetEnd)
    {
        if(address > (PciConfigOffsets::AdvancedErrorReportingCapabilityOffsetEnd - 4) && size == 4)
        {
            return 0;
        }

        if(address > (PciConfigOffsets::AdvancedErrorReportingCapabilityOffsetEnd - 2) && size == 2)
        {
            return 0;
        }

        u32 ret;
        (void) ::std::memcpy(&ret, reinterpret_cast<u8*>(&m_AdvancedErrorReportingCapability) + (address - PciConfigOffsets::AdvancedErrorReportingCapabilityOffsetBegin), size);
        return ret;
    }

    if(address < PciConfigOffsets::PciExtendedConfigOffsetEnd)
    {
        if(address > (PciConfigOffsets::PciExtendedConfigOffsetEnd - 4) && size == 4)
        {
            return 0;
        }

        if(address > (PciConfigOffsets::PciExtendedConfigOffsetEnd - 2) && size == 2)
        {
            return 0;
        }

        u32 ret;
        (void) ::std::memcpy(&ret, &m_PciExtendedConfig[address - PciConfigOffsets::PciExtendedConfigOffsetBegin], size);
        return ret;
    }

    return 0;
}
