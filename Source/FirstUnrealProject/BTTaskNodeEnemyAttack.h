// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTaskNodeEnemyAttack.generated.h"

/**
 * 
 */
UCLASS()
class FIRSTUNREALPROJECT_API UBTTaskNodeEnemyAttack : public UBTTaskNode
{
	GENERATED_BODY()
public:
	UBTTaskNodeEnemyAttack();
public:

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

};
