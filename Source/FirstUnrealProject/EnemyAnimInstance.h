// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "EnemyAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class FIRSTUNREALPROJECT_API UEnemyAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	
public:

	UPROPERTY(Category = Animation, EditAnywhere, BlueprintReadOnly)
		float	Speed;

	UPROPERTY(Category = Animation, EditAnywhere, BlueprintReadOnly)
		FVector Velocity;

	UPROPERTY(Category = Animation, EditAnywhere, BlueprintReadOnly)
		bool	IsMoving;
	UPROPERTY(Category = Animation, EditAnywhere, BlueprintReadOnly)
		bool	IsFalling;


	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		float LeftRight;

	UPROPERTY(Category = Animation, EditAnywhere, BlueprintReadOnly)
		bool	IsRest;
	UPROPERTY(Category = Animation, EditAnywhere, BlueprintReadOnly)
		bool	IsSleep;



	UPROPERTY(VisibleAnywhere)
		class AEnemyCharacter* Enemy;
	UPROPERTY(VisibleAnywhere)
		class	UCharacterMovementComponent* CharacterMovement;

public:
	virtual void NativeInitializeAnimation() override;
	virtual void NativeBeginPlay() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;
public:
	void SetIsRun(bool Run);
};
