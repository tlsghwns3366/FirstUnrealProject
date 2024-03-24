// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "Perception/AIPerceptionTypes.h"
#include "EnemyAIController.generated.h"

/**
 * 
 */
UCLASS()
class FIRSTUNREALPROJECT_API AEnemyAIController : public AAIController
{
	GENERATED_BODY()
public:
	AEnemyAIController();
public:
	UPROPERTY(EditAnywhere)
		class UBehaviorTree* BehaviorTree;
	UPROPERTY(EditAnywhere)
		class UBlackboardData* BlackboardData;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class UAIPerceptionComponent* AIPerceptionComponent;
	UPROPERTY(EditAnywhere)
		class UAISenseConfig_Sight* SightConfig;
	UPROPERTY(EditAnywhere)
		class UAISenseConfig_Hearing* HearingConfig;
	UPROPERTY(EditAnywhere)
		class UAISenseConfig_Damage* DamageConfig;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FVector SenseLocation;


public:

	virtual void BeginPlay() override;


	virtual void OnPossess(APawn* InPawn) override;
	virtual void OnUnPossess() override;

	void SetPerceptionSystem();

	UFUNCTION(BlueprintCallable)
		void SetCharacterState();
	UFUNCTION(BlueprintCallable)
		void SetAggroSensitivity(float Value);

	UFUNCTION()
		void SenseDetected(AActor* Actor, FAIStimulus const Stimulus);

	void SightDetected(AActor* Actor, FAIStimulus const Stimulus);
	void HearingDetected(AActor* Actor, FAIStimulus const Stimulus);
	void DamageDetected(AActor* Actor, FAIStimulus const Stimulus);
};
