// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BTTask_SmoothRotation.generated.h"

/**
 * 
 */
UCLASS(config = Game)
class FIRSTUNREALPROJECT_API UBTTask_SmoothRotation : public UBTTask_BlackboardBase
{
	GENERATED_BODY()
public:
	UBTTask_SmoothRotation();
protected:

	UPROPERTY(config, Category = Node, EditAnywhere, meta = (ClampMin = "0.0"))
	float Precision = 10.f;
private:
	float PrecisionDot;

public:

	FRotator StartRotator;
public:
	virtual void PostInitProperties() override;
	virtual void PostLoad() override;

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
};
