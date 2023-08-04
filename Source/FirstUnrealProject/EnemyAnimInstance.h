// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "EnemyAnimInstance.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnAttckHit);
/**
 * 
 */
UCLASS()
class FIRSTUNREALPROJECT_API UEnemyAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
public:

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Animation)
		float	Speed;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Animation)
		FVector Velocity;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Animation)
		bool	IsMoving;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Animation)
		bool	IsFalling;


	UPROPERTY(EditAnywhere, BlueprintReadOnly,Category = Animation)
		float LeftRight;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animation)
		bool	IsRest;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Blackboard)
		bool	IsNight;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Blackboard)
		bool	FindTarget;


	UPROPERTY(VisibleAnywhere)
		class AEnemyCharacter* Enemy;
	UPROPERTY(VisibleAnywhere)
		class	UCharacterMovementComponent* CharacterMovement;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class AEnemyAIController* EnemyController;

public:
	FOnAttckHit OnAttackHit;
public:
	virtual void NativeInitializeAnimation() override;
	virtual void NativeBeginPlay() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;
public:
	void SetRest(bool Rest) { IsRest = Rest; };
	bool GetRest() { return IsRest; };
public:	
	UFUNCTION()
		void AnimNotify_Hit();
};
