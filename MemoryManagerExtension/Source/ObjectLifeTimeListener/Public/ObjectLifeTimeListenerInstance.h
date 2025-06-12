// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <memory>

#include "CoreMinimal.h"
#include "UObject/Object.h"
DECLARE_LOG_CATEGORY_EXTERN(LogObjectListener, Log, Log);

static bool GStartObjectLifeTimeListener = false;
static FAutoConsoleVariableRef CVarStartObjectLifeTimeListener(
	TEXT("StartObjectLifeTimeListener"),
	GStartObjectLifeTimeListener,
	TEXT("Set bStart Of the Object Life Time Listener"),
	ECVF_Default
);

static bool GObjectLifeTimeListenerAutoMode = false;
static FAutoConsoleVariableRef CVarObjectLifeTimeListenerAutoMode(
	TEXT("AutoModeObjectLifeTimeListener"),
	GObjectLifeTimeListenerAutoMode,
	TEXT("Set Auto Mode Of Object Life Time Listener"),
	ECVF_Default
);

static bool GObjectLifeTimeListenerManualMode = false;
static FAutoConsoleVariableRef CVarGObjectLifeTimeListenerManualMode(
	TEXT("ManualModeObjectLifeTimeListener"),
	GObjectLifeTimeListenerManualMode,
	TEXT("Set Manual Mode Of Object Life Time Listener"),
	ECVF_Default
);

static bool GLogReference = false;
static FAutoConsoleVariableRef CVarGObjectLifeTimeLogReference(
	TEXT("ObjectLifeTimeListenerLogReference"),
	GLogReference,
	TEXT("b Log Reference"),
	ECVF_Default
);

class FObjectLifeTimeListenerInstance : public FUObjectArray::FUObjectCreateListener, public FUObjectArray::FUObjectDeleteListener
{
public:
	static FObjectLifeTimeListenerInstance* GetInstance();
	static void ReleaseInstance();
	static void SetStartAutoMode(bool bStart);
	FObjectLifeTimeListenerInstance();
	virtual ~FObjectLifeTimeListenerInstance() override;
	virtual void OnUObjectArrayShutdown() override;

private:
	virtual void NotifyUObjectCreated(const UObjectBase* Object, int32 Index) override;
	virtual void NotifyUObjectDeleted(const class UObjectBase* Object, int32 Index) override;

	FObjectLifeTimeListenerInstance(const FObjectLifeTimeListenerInstance&) = delete;
	FObjectLifeTimeListenerInstance& operator=(const FObjectLifeTimeListenerInstance&) = delete;
	void OnWorldPostCleanup(UWorld* World, bool bSessionEnded, bool bCleanupResources);
	void OnManualModeChanged(IConsoleVariable* InVar);

private: //Data
	FDelegateHandle WorldCleanupHandle;

	static std::unique_ptr<FObjectLifeTimeListenerInstance> Instance;
	TMap<int32, TWeakObjectPtr<const UObject>> ObjIndex2Obj;
	TMap<int32, TWeakObjectPtr<const UObject>> ObjIndex2ObjManualMode;
	void HandleLog(const TMap<int32, TWeakObjectPtr<const UObject>>& InMap, bool bLogReference = false);

	FCriticalSection CurCriticalSection;
};
