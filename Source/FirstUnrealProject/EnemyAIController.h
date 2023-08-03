// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "EnemyAIController.generated.h"

/**
 * 
 */
UCLASS()
class FIRSTUNREALPROJECT_API AEnemyAIController : public AAIController
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere)
		class UBehaviorTree* BehaviorTree;
	UPROPERTY(EditAnywhere)
		class UBlackboardData* BlackboardData;

	UPROPERTY(VisibleAnywhere)
		class UAIPerceptionComponent* AIPerceptionComponent;
public:
	AEnemyAIController();
public:
	virtual void OnPossess(APawn* InPawn) override;
	virtual void OnUnPossess() override;
};
