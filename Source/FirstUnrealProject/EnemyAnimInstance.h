// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CharacterAnimInstance.h"
#include "MainGameState.h"
#include "EnemyAnimInstance.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnAttckHit);
/**
 * 
 */
UCLASS()
class FIRSTUNREALPROJECT_API UEnemyAnimInstance : public UCharacterAnimInstance
{
	GENERATED_BODY()
public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animation)
		bool	IsRest;
	UPROPERTY(EditAnywhere, BlueprintReadOnly,Category = Animation)
		float LeftRight;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animation)
		bool	IsDie;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Blackboard)
		bool	IsNight;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Blackboard)
		bool	FindTarget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class AEnemyAIController* EnemyController;
public:
	virtual void NativeInitializeAnimation() override;
	virtual void NativeBeginPlay() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;
public:
	void SetRest(bool Rest) { IsRest = Rest; };
	bool GetRest() { return IsRest; };
};
