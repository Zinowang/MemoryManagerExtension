// Copyright Epic Games, Inc. All Rights Reserved.

#include "MemoryCheckByStat.h"

#include "MemoryManagerFunctionLibrary.h"

#define LOCTEXT_NAMESPACE "FMemoryCheckByStatModuleModule"
DEFINE_LOG_CATEGORY(LogMemoryCheckByStat);

void UMemoryCheckByStatManager::Tick(float DeltaTime)
{
	TickRecordMemoryWarning();
}

TStatId UMemoryCheckByStatManager::GetStatId() const
{
	return TStatId();
}

void UMemoryCheckByStatManager::TickRecordMemoryWarning()
{
	UWorld* World = GWorld->GetWorld();
	if (!World)
	{
		return;
	}
	uint64 CurPhysicalMemory = UMemoryManagerFunctionLibrary::GetUsedPhysicalMemory();
	const double CurrentTime = World->GetTimeSeconds();
	if (LastRecordTimeStamp == 0.f)
	{
		LastRecordMemoryBytes = CurPhysicalMemory;
		LastRecordTimeStamp = CurrentTime;
	}
	if (CurrentTime - LastRecordTimeStamp < RecordMemoryGap)
	{
		Frame2MemoryByteArray.Add({GFrameCounter, CurPhysicalMemory});
	}
	else
	{
		const int64 DeltaMemory = CurPhysicalMemory - LastRecordMemoryBytes;
		if (DeltaMemory > RecordMemoryWarningGapValue)
		{
			UE_LOG(LogMemoryCheckByStat, Warning, TEXT("[UMemoryCheckByStatManager] [%f]Seconds Added Memory : %lld MB"), RecordMemoryGap, DeltaMemory/1024/1024);
			int64 Cursor = INDEX_NONE;
			for (const auto& Pair : Frame2MemoryByteArray)
			{
				if (Cursor != Pair.Value)
				{
					Cursor = Pair.Value;
					UE_LOG(LogMemory, Log, TEXT("[UMemoryCheckByStatManager] FrameNumber :[%d] Cur Memory is %lld MB"), Pair.Key, Pair.Value/1024/1024);
				}
			}
		}
		LastRecordTimeStamp = CurrentTime;
		Frame2MemoryByteArray.Reset();
		LastRecordMemoryBytes = CurPhysicalMemory;
	}
}

void FMemoryCheckByStatModule::StartupModule()
{
	MemoryCheckByStatManager = TStrongObjectPtr(NewObject<UMemoryCheckByStatManager>());
}

void FMemoryCheckByStatModule::ShutdownModule()
{
	MemoryCheckByStatManager.Reset();
}

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FMemoryCheckByStatModule, MemoryCheckByStat)
