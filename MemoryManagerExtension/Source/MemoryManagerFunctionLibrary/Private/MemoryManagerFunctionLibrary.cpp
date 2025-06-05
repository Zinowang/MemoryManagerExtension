#include "MemoryManagerFunctionLibrary.h"

#define LOCTEXT_NAMESPACE "FMemoryManagerFunctionLibraryModule"

void FMemoryManagerFunctionLibraryModule::StartupModule()
{
    
}

void FMemoryManagerFunctionLibraryModule::ShutdownModule()
{
    
}

uint64 UMemoryManagerFunctionLibrary::GetUsedPhysicalMemory()
{
	return FPlatformMemory::GetStats().UsedPhysical;
}

void UMemoryManagerFunctionLibrary::ForceGC()
{
	GEngine->ForceGarbageCollection(true);
}

#undef LOCTEXT_NAMESPACE
    
IMPLEMENT_MODULE(FMemoryManagerFunctionLibraryModule, MemoryManagerFunctionLibrary)