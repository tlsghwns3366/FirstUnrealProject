// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTaskNodeRandomPosition.generated.h"

/**
 * 
 */
UCLASS()
class FIRSTUNREALPROJECT_API UBTTaskNodeRandomPosition : public UBTTaskNode
{
	GENERATED_BODY()
public:
	UBTTaskNodeRandomPosition();
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
		float SearchRadius = 500.f;


public:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	
};
