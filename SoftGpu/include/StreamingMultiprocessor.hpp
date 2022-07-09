#pragma once

#include "RegisterFile.hpp"
#include "LoadStore.hpp"
#include "DispatchUnit.hpp"
#include "Core.hpp"
#include "DebugManager.hpp"

class Processor;

class StreamingMultiprocessor final
{
    DEFAULT_DESTRUCT(StreamingMultiprocessor);
    DELETE_CM(StreamingMultiprocessor);
public:
    StreamingMultiprocessor(Processor* const processor, const u32 smIndex) noexcept
        : m_Processor(processor)
        , m_RegisterFile{ }
        , m_LdSt { { this, 0 }, { this, 1 }, { this, 2 }, { this, 3 } }
        , m_FpCores { { this, 0 }, { this, 1 }, { this, 2 }, { this, 3 }, { this, 4 }, { this, 5 }, { this, 6 }, { this, 7 } }
        , m_IntFpCores { { this, 0 }, { this, 1 }, { this, 2 }, { this, 3 }, { this, 4 }, { this, 5 }, { this, 6 }, { this, 7 } }
        , m_DispatchUnits { { this, 0 }, { this, 1 } }
        , m_SMIndex(smIndex)
    { }

    void Clock() noexcept
    {
        if(GlobalDebug.IsAttached())
        {
            m_RegisterFile.ReportRegisters(m_SMIndex);
            m_DispatchUnits[0].ReportBaseRegisters(m_SMIndex);
            m_DispatchUnits[1].ReportBaseRegisters(m_SMIndex);
        }

        m_LdSt[0].Clock();
        m_LdSt[1].Clock();
        m_LdSt[2].Clock();
        m_LdSt[3].Clock();

        for(u32 subClockIndex = 0; subClockIndex <= 5; ++subClockIndex)
        {
            for(u32 coreIndex = 0; coreIndex < 8; ++coreIndex)
            {
                m_FpCores[coreIndex].Clock(subClockIndex);
                m_IntFpCores[coreIndex].Clock(subClockIndex);
            }
        }

        m_DispatchUnits[0].ResetCycle();
        m_DispatchUnits[1].ResetCycle();

        for(u32 i = 0; i < 6; ++i)
        {
            m_DispatchUnits[0].Clock();
            m_DispatchUnits[1].Clock();
        }
    }

    void TestLoadProgram(const u32 dispatchPort, const u8 replicationMask, const u64 program)
    {
        const u32 baseRegisters[4] = { (dispatchPort * 4 + 0) * 256, (dispatchPort * 4 + 1) * 256, (dispatchPort * 4 + 2) * 256, (dispatchPort * 4 + 3) * 256 };
        m_DispatchUnits[dispatchPort].LoadIP(replicationMask, baseRegisters, program);
    }

    void TestLoadRegister(const u32 dispatchPort, const u32 replicationIndex, const u8 registerIndex, const u32 registerValue)
    {
        m_RegisterFile.SetRegister((dispatchPort * 4 + replicationIndex) * 256 + registerIndex, registerValue);
    }

    [[nodiscard]] u32 Read(u64 address) noexcept;
    void Write(u64 address, u32 value) noexcept;
    void Prefetch(u64 address) noexcept;

    [[nodiscard]] u32 GetRegister(const u32 targetRegister) const noexcept
    {
        return m_RegisterFile.GetRegister(targetRegister);
    }

    void SetRegister(const u32 targetRegister, const u32 value) noexcept
    {
        m_RegisterFile.SetRegister(targetRegister, value);
    }

    void ReportFpCoreReady(const u32 unitIndex) noexcept
    {
        m_DispatchUnits[0].ReportUnitReady(unitIndex + FP_AVAIL_OFFSET);
        m_DispatchUnits[1].ReportUnitReady(unitIndex + FP_AVAIL_OFFSET);
    }

    void ReportIntFpCoreReady(const u32 unitIndex) noexcept
    {
        m_DispatchUnits[0].ReportUnitReady(unitIndex + INT_FP_AVAIL_OFFSET);
        m_DispatchUnits[1].ReportUnitReady(unitIndex + INT_FP_AVAIL_OFFSET);
    }

    void ReportLdStReady(const u32 unitIndex) noexcept
    {
        m_DispatchUnits[0].ReportUnitReady(unitIndex + LDST_AVAIL_OFFSET);
        m_DispatchUnits[1].ReportUnitReady(unitIndex + LDST_AVAIL_OFFSET);
    }

    [[nodiscard]] bool CanReadRegister(const u32 registerIndex) noexcept
    {
        return m_RegisterFile.CanReadRegister(registerIndex);
    }

    [[nodiscard]] bool CanWriteRegister(const u32 registerIndex) noexcept
    {
        return m_RegisterFile.CanWriteRegister(registerIndex);
    }

    void ReleaseRegisterContestation(const u32 registerIndex) noexcept
    {
        m_RegisterFile.ReleaseRegisterContestation(registerIndex);
    }

    void LockRegisterRead(const u32 registerIndex) noexcept
    {
        m_RegisterFile.LockRegisterRead(registerIndex);
    }

    void LockRegisterWrite(const u32 registerIndex) noexcept
    {
        m_RegisterFile.LockRegisterWrite(registerIndex);
    }

    // [[deprecated]] void ReleaseRegisterContestation(const u32 dispatchPort, const u32 replicationIndex, const u32 registerIndex)
    // {
    //     m_DispatchUnits[dispatchPort].ReleaseRegisterContestation(registerIndex, replicationIndex);
    // }

    void DispatchLdSt(const u32 ldStIndex, const LoadStoreInstruction instructionInfo) noexcept
    {
        m_DispatchUnits[0].ReportUnitBusy(ldStIndex + LDST_AVAIL_OFFSET);
        m_DispatchUnits[1].ReportUnitBusy(ldStIndex + LDST_AVAIL_OFFSET);

        m_LdSt[ldStIndex].Execute(instructionInfo);
    }

    void DispatchFpu(const u32 fpIndex, const FpuInstruction instructionInfo) noexcept
    {
        if(fpIndex < 8)
        {
            m_DispatchUnits[0].ReportUnitBusy(fpIndex + FP_AVAIL_OFFSET);
            m_DispatchUnits[1].ReportUnitBusy(fpIndex + FP_AVAIL_OFFSET);
            m_FpCores[fpIndex].InitiateInstruction(instructionInfo);
        }
        else
        {
            m_DispatchUnits[0].ReportUnitBusy(fpIndex - 8 + INT_FP_AVAIL_OFFSET);
            m_DispatchUnits[1].ReportUnitBusy(fpIndex - 8 + INT_FP_AVAIL_OFFSET);
            m_IntFpCores[fpIndex - 8].InitiateInstructionFP(instructionInfo);
        }
    }

    void FlushCache() noexcept;
private:
    Processor* m_Processor;
    RegisterFile m_RegisterFile;
    LoadStore m_LdSt[4];
    FpCore m_FpCores[8];
    IntFpCore m_IntFpCores[8];
    DispatchUnit m_DispatchUnits[2];
    u32 m_SMIndex;
};
