#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"

static bool GStartMemoryCheckByStat = false;
static FAutoConsoleVariableRef CVarStartMemoryCheckByStat(
	TEXT("StartMemoryCheckByStat"),
	GStartMemoryCheckByStat,
	TEXT("Set bStart Of the MemoryCheckByStat"),
	ECVF_Default
);

static bool GStartIOSOOMDetector = false;
static FAutoConsoleVariableRef CVarStartIOSOOMDetector(
	TEXT("StartIOSOOMDetector"),
	GStartIOSOOMDetector,
	TEXT("Set bStart Of the MemoryCheckByStat"),
	ECVF_Default
);


class FMemoryModuleControlCenterModule : public IModuleInterface
{
public:
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;

private:
	void OnStartObjectLifeTimeListener(IConsoleVariable* InVariable);
	void OnStartMemoryCheckByStat(IConsoleVariable* InVariable);
	FDelegateHandle OnStartObjectLifeTimeListenerHandle;
	FDelegateHandle OnStartMemoryCheckByStatHandle;
};
