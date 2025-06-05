// Fill out your copyright notice in the Description page of Project Settings.


#include "ObjectLifeTimeListenerInstance.h"
DEFINE_LOG_CATEGORY(LogObjectListener);

std::unique_ptr<FObjectLifeTimeListenerInstance> FObjectLifeTimeListenerInstance::Instance = nullptr;

FObjectLifeTimeListenerInstance* FObjectLifeTimeListenerInstance::GetInstance()
{
	Instance = std::make_unique<FObjectLifeTimeListenerInstance>();
	return Instance.get();
}

void FObjectLifeTimeListenerInstance::ReleaseInstance()
{
	Instance.reset();
}

FObjectLifeTimeListenerInstance::FObjectLifeTimeListenerInstance()
{
	GUObjectArray.AddUObjectCreateListener(this);
	GUObjectArray.AddUObjectDeleteListener(this);

	WorldCleanupHandle = FWorldDelegates::OnPostWorldCleanup.AddRaw(this, &FObjectLifeTimeListenerInstance::OnWorldPostCleanup);
	CVarGObjectLifeTimeListenerManualMode->OnChangedDelegate().AddRaw(this, &FObjectLifeTimeListenerInstance::OnManualModeChanged);
}

FObjectLifeTimeListenerInstance::~FObjectLifeTimeListenerInstance()
{
	FWorldDelegates::OnPostWorldCleanup.Remove(WorldCleanupHandle);
	ObjIndex2Obj.Reset();
}

void FObjectLifeTimeListenerInstance::OnUObjectArrayShutdown()
{
	GUObjectArray.RemoveUObjectCreateListener(this);
	GUObjectArray.RemoveUObjectDeleteListener(this);
}

void FObjectLifeTimeListenerInstance::OnWorldPostCleanup(UWorld* World, bool bSessionEnded, bool bCleanupResources)
{
	if (CVarObjectLifeTimeListenerAutoMode->GetBool())
	{
		HandleLog(ObjIndex2Obj);
		ObjIndex2Obj.Reset();
	}
}

void FObjectLifeTimeListenerInstance::OnManualModeChanged(IConsoleVariable* InVar)
{
	if (!InVar->GetBool())
	{
		HandleLog(ObjIndex2ObjManualMode);
	}
	ObjIndex2ObjManualMode.Empty();
}

void FObjectLifeTimeListenerInstance::HandleLog(const TMap<int32, TWeakObjectPtr<const UObject>>& InMap, bool bLogReference)
{
	for (const auto& Element : InMap)
	{
		if (Element.Value.IsValid())
		{
			UE_LOG(LogObjectListener, Log, TEXT("FObjectLifeTimeListenerInstance::HandleLog Obj Name is %s"), *Element.Value->GetFName().ToString());
			if (bLogReference || CVarGObjectLifeTimeLogReference->GetBool())
			{
				GEngine->Exec(GWorld, *FString::Printf(TEXT("OBJ REFS name=%s"), *Element.Value->GetFName().ToString()));
			}
		}
	}
}

void FObjectLifeTimeListenerInstance::SetStartAutoMode(bool bStart)
{
	CVarObjectLifeTimeListenerAutoMode->Set(bStart);
}

void FObjectLifeTimeListenerInstance::NotifyUObjectCreated(const UObjectBase* Object, int32 Index)
{
	if (!CVarStartObjectLifeTimeListener->GetBool())
	{
		return;
	}
	FScopeLock Lock(&CurCriticalSection);
	if (CVarObjectLifeTimeListenerAutoMode->GetBool())
	{
		ObjIndex2Obj.Emplace(Index, MakeWeakObjectPtr(Cast<UObject>(Object)));
	}
	if (CVarGObjectLifeTimeListenerManualMode->GetBool())
	{
		ObjIndex2ObjManualMode.Emplace(Index, MakeWeakObjectPtr(Cast<UObject>(Object)));
	}
}

void FObjectLifeTimeListenerInstance::NotifyUObjectDeleted(const class UObjectBase* Object, int32 Index)
{
	FScopeLock Lock(&CurCriticalSection);
	if (CVarObjectLifeTimeListenerAutoMode->GetBool())
	{
		ObjIndex2Obj.Remove(Index);
	}
	if (CVarGObjectLifeTimeListenerManualMode->GetBool())
	{
		ObjIndex2ObjManualMode.Remove(Index);
	}
}
