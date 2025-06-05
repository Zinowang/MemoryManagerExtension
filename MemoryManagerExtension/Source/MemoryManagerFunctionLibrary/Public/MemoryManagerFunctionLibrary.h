#pragma once
#include "CoreMinimal.h"
#include "MemoryManagerFunctionLibrary.generated.h"
class FMemoryManagerFunctionLibraryModule : public IModuleInterface
{
public:
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
};
UCLASS(BlueprintType)
class MEMORYMANAGERFUNCTIONLIBRARY_API UMemoryManagerFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
public:
	UFUNCTION(Blueprintable)
	static uint64 GetUsedPhysicalMemory();
	UFUNCTION(Blueprintable)
	static void ForceGC();
};