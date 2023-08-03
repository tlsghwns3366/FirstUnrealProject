// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "BTService_UpdateEnemy.generated.h"

/**
 * 
 */
UCLASS()
class FIRSTUNREALPROJECT_API UBTService_UpdateEnemy : public UBTService
{
	GENERATED_BODY()
public:
	UBTService_UpdateEnemy();
public:
	UPROPERTY(VisibleAnywhere)
	class AEnemyCharacter* CurrentPawn;
public:
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
};
