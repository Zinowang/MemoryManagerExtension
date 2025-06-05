#pragma once
#include "MemoryCheckByStat.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(LogMemoryCheckByStat, Log, Log);

UCLASS(Transient, Config=Game, DefaultConfig)
class UMemoryCheckByStatManager : public UObject, public FTickableGameObject

{
	GENERATED_BODY()

public:
	UPROPERTY(Config, Transient)
	float RecordMemoryGap;
	UPROPERTY(Config, Transient)
	int64 RecordMemoryWarningGapValue;

	virtual void Tick(float DeltaTime) override;
	virtual TStatId GetStatId() const override;
	virtual ETickableTickType GetTickableTickType() const override { return ETickableTickType::Always; }
	virtual bool IsTickableInEditor() const override { return true; }

private:
	void TickRecordMemoryWarning();
	int64 LastRecordMemoryBytes = 0L;
	double LastRecordTimeStamp = 0.f;
	TArray<TPair<int32, int64>> Frame2MemoryByteArray;
};


class FMemoryCheckByStatModule : public IModuleInterface
{
public:
	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
	TStrongObjectPtr<UMemoryCheckByStatManager> MemoryCheckByStatManager;
};
