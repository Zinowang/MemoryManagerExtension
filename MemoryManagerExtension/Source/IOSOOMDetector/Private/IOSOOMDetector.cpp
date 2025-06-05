// Copyright Epic Games, Inc. All Rights Reserved.

#include "IOSOOMDetector.h"
#if PLATFORM_IOS
#include "HAL/IConsoleManager.h"
#endif
#if PLATFORM_IOS
#include "ThirdParty/Include/SeriaOOMDetector.h"
#endif

#define LOCTEXT_NAMESPACE "FIOSOOMDetectorModule"
#if PLATFORM_IOS
struct AddressItem
{
	int32 Depth = 0;
	vm_address_t* Address = nullptr;
};
static TArray<AddressItem> AddressInfos;

typedef void(malloc_record_t)(vm_address_t str1, int depth, unsigned long size, vm_address_t** load_address_stack);
extern malloc_record_t *malloc_record;
vm_address_t load_address;


static int32 GAllowMemoryTraceBack_Enable = 0;
static FAutoConsoleVariableRef CVar_StartIOSMemoryTraceBack(
    TEXT("StartIOSMemoryTraceBack.Enable"),
    GAllowMemoryTraceBack_Enable,
    TEXT("If > 0, Enable Malloc Hook To Record Malloc Load Address And Address")
);

static int32 GAllowMemoryTraceBackNum = 1000;
static FAutoConsoleVariableRef CVar_AllowMemoryTraceBackNum(
    TEXT("AllowMemoryTraceBackNum"),
    GAllowMemoryTraceBackNum,
    TEXT("Record Address Stack Num")
);

static int32 GLowestRecordSize = 0;
static FAutoConsoleVariableRef CVar_LowestRecordSize(
    TEXT("LowestRecordSize"),
    GLowestRecordSize,
    TEXT("Lowest Record Size Byte")
);

int32 BacktraceNum = 0;
void MallocRecordFunc(vm_address_t str1, int depth, unsigned long size, vm_address_t** load_address_stack)
{
    if(size <= GLowestRecordSize)
    {
        return;
    }
    if(BacktraceNum >= GAllowMemoryTraceBackNum)
    {
        [SeriaOOMDetector EndMallocLogger];
        malloc_record = nullptr;
        UE_LOG(LogMemory, Log, TEXT("[MallocRecordFunc] Load Address:[0x%lx]"), load_address);
        for(AddressItem & Item : AddressInfos)
        {
            if(Item.Address)
            {
                UE_LOG(LogMemory, Log, TEXT("[MallocRecordFunc] Depth:[%d] Address:[0x%lx]"), Item.Depth, Item.Address);
            }
        }
    	FSeriaIOSOOMDetectorModule::GetOnRecordFinished().Execute();
        return;
    }
    malloc_record = nullptr;
    if(str1 != load_address)
    {
        load_address = str1;
    }
    for(int i = 0; i<depth; i++)
	{
        if(load_address_stack[i])
        {
        	AddressItem Item;
        	Item.Depth = i;
        	Item.Address = load_address_stack[i];
            AddressInfos.Emplace(Item);
        }
	}
    BacktraceNum++;
    malloc_record = (malloc_record_t*)MallocRecordFunc;
}

static FString TraceBackVariableName = "StartIOSMemoryTraceBack.Enable";

void FIOSOOMDetectorModule::Tick(float DeltaTime)
{
	uint64 CurPhysicalMemory = UMemoryManagerFunctionLibrary::GetUsedPhysicalMemory();
	const uint64 Threshold = AutoStartRecordMemoryThreshold * 1024 * 1024;
	if (CurPhysicalMemory > Threshold)
	{

		if(USeriaLuaInterface::GetConsoleVariableInt(TraceBackVariableName) == 0)
		{
			USeriaCMDInterface::Exec(TEXT("StartIOSMemoryTraceBack.Enable 1"));
			return;
		}
	}
}
TStatId UMemoryCheckByStatManager::GetStatId() const
{
	return TStatId();
}
#endif


void FIOSOOMDetectorModule::StartupModule()
{
#if PLATFORM_IOS
    CVar_StartIOSMemoryTraceBack->OnChangedDelegate().AddRaw(this, &FSeriaIOSOOMDetectorModule::OnConsoleVariableChanged);
#endif
}

void FIOSOOMDetectorModule::ShutdownModule()
{
#if PLATFORM_IOS
    HandleStartMemoryTraceBack(false);
    CVar_StartIOSMemoryTraceBack->OnChangedDelegate().Clear();
    malloc_record = nullptr;
#endif
}

void FIOSOOMDetectorModule::OnConsoleVariableChanged(IConsoleVariable* InVariable)
{
#if PLATFORM_IOS
	if (!InVariable)
	{
		return;
	}
	bool bStart = InVariable->GetInt() > 0;
	if (bStart)
	{
        BacktraceNum = 0;
		malloc_record = (malloc_record_t*)MallocRecordFunc;
	}
	else
	{
		malloc_record = nullptr;
	}
    HandleStartMemoryTraceBack(bStart);
#endif
}

void FIOSOOMDetectorModule::HandleStartMemoryTraceBack(bool bStart)
{
#if PLATFORM_IOS
    if (bStart)
    {
        [SeriaOOMDetector StartMallocLogger];
    }
    else
    {
        [SeriaOOMDetector EndMallocLogger];
    }
#endif
}
#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FIOSOOMDetectorModule, IOSOOMDetector)
