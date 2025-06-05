#include "ObjectLifeTimeListener.h"

#include "ObjectLifeTimeListenerInstance.h"

#define LOCTEXT_NAMESPACE "FObjectLifeTimeListenerModule"
void FObjectLifeTimeListenerModule::StartupModule()
{
	FObjectLifeTimeListenerInstance::GetInstance();   
}

void FObjectLifeTimeListenerModule::ShutdownModule()
{
	FObjectLifeTimeListenerInstance::GetInstance()->ReleaseInstance();   
}

#undef LOCTEXT_NAMESPACE
IMPLEMENT_MODULE(FObjectLifeTimeListenerModule, ObjectLifeTimeListener)
