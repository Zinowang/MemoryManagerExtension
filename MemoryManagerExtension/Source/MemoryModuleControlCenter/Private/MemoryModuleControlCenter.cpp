#include "MemoryModuleControlCenter.h"

#include "ObjectLifeTimeListenerInstance.h"

#define LOCTEXT_NAMESPACE "FMemoryModuleControlCenterModule"

void FMemoryModuleControlCenterModule::StartupModule()
{
	if (CVarStartObjectLifeTimeListener->GetBool())
	{
		FModuleManager::Get().LoadModule(TEXT("ObjectLifeTimeListener"));
	}
	if (CVarStartMemoryCheckByStat->GetBool())
	{
		FModuleManager::Get().LoadModule(TEXT("MemoryCheckByStat"));
	}
#if PLATFORM_IOS
	if (CVarStartIOSOOMDetector)
	{
		FModuleManager::Get().LoadModule(TEXT("IOSOOMDetector"));
	}
#endif
	OnStartObjectLifeTimeListenerHandle = CVarStartObjectLifeTimeListener->OnChangedDelegate().AddRaw(this, &FMemoryModuleControlCenterModule::OnStartObjectLifeTimeListener);
	OnStartMemoryCheckByStatHandle = CVarStartMemoryCheckByStat->OnChangedDelegate().AddRaw(this, &FMemoryModuleControlCenterModule::OnStartMemoryCheckByStat);
}

void FMemoryModuleControlCenterModule::ShutdownModule()
{
	bool bIsModuleLoaded = FModuleManager::Get().IsModuleLoaded(TEXT("ObjectLifeTimeListener"));
	if (bIsModuleLoaded)
	{
		FModuleManager::Get().UnloadModule(TEXT("ObjectLifeTimeListener"));
	}
	bIsModuleLoaded = FModuleManager::Get().IsModuleLoaded(TEXT("MemoryCheckByStat"));
	if (bIsModuleLoaded)
	{
		FModuleManager::Get().UnloadModule(TEXT("MemoryCheckByStat"));
	}
	CVarStartObjectLifeTimeListener->OnChangedDelegate().Remove(OnStartObjectLifeTimeListenerHandle);
	CVarStartMemoryCheckByStat->OnChangedDelegate().Remove(OnStartMemoryCheckByStatHandle);

#if PLATFORM_IOS
	bIsModuleLoaded = FModuleManager::Get().IsModuleLoaded(TEXT("MemoryOOMDetector"));
	if (bIsModuleLoaded)
	{
		FModuleManager::Get().UnloadModule(TEXT("MemoryOOMDetector"));
	}
#endif
}

void FMemoryModuleControlCenterModule::OnStartObjectLifeTimeListener(IConsoleVariable* InVariable)
{
	if (InVariable->GetBool())
	{
		FModuleManager::Get().LoadModule(TEXT("ObjectLifeTimeListener"));
	}
	else
	{
		FModuleManager::Get().UnloadModule(TEXT("ObjectLifeTimeListener"));
	}
}

void FMemoryModuleControlCenterModule::OnStartMemoryCheckByStat(IConsoleVariable* InVariable)
{
	if (InVariable->GetBool())
	{
		FModuleManager::Get().LoadModule(TEXT("MemoryCheckByStat"));
	}
	else
	{
		FModuleManager::Get().UnloadModule(TEXT("MemoryCheckByStat"));
	}
}

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FMemoryModuleControlCenterModule, MemoryModuleControlCenter)
