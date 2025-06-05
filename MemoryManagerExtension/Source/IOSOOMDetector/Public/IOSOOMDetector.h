#pragma once

#include "CoreMinimal.h"
#include "Tickable.h"
#include "Modules/ModuleManager.h"

#if PLATFORM_IOS
static int32 AutoStartRecordMemoryThreshold = 3584;	//MB
static FAutoConsoleVariableRef CVar_AutoStartRecordMemoryThreshold(
	TEXT("AutoStartRecordMemoryThreshold"),
	AutoStartRecordMemoryThreshold,
	TEXT("Record Address Stack Cur Memory Threshold")
);

DECLARE_DELEGATE(FOnRecordFinished);
#endif
class FIOSOOMDetectorModule : public IModuleInterface
#if PLATFORM_IOS
	, public FTickableGameObject
#endif

{
public:
#if PLATFORM_IOS
	static FOnRecordFinished& GetOnRecordFinished()
	{
		static FOnRecordFinished OnRecordFinished;
		return OnRecordFinished;
	}
	virtual void Tick(float DeltaTime) override;
	virtual TStatId GetStatId() const override;
	virtual ETickableTickType GetTickableTickType() const override { return ETickableTickType::Always; }
	virtual bool IsTickableInEditor() const override { return false; }
#endif

	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;

private:
	void OnConsoleVariableChanged(IConsoleVariable* InVariable);
	static void HandleStartMemoryTraceBack(bool bStart);
};
