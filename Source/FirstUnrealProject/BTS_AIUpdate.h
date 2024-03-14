// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "BTS_AIUpdate.generated.h"

/**
 * 
 */
UCLASS()
class FIRSTUNREALPROJECT_API UBTS_AIUpdate : public UBTService
{
	GENERATED_BODY()

public:
	UBTS_AIUpdate();

public:
	UPROPERTY(VisibleAnywhere)
		class AEnemyCharacter* Character;

public:
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
};
