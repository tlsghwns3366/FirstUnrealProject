// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "CharacterAnimInstance.generated.h"



DECLARE_MULTICAST_DELEGATE(FOnAttckHit);
/**
 * 
 */


UENUM(BlueprintType)
enum class ECharacterState : uint8
{
	E_Idle UMETA(DisplayName = "Idle"),
	E_Walk UMETA(DisplayName = "Walk"),
	E_Jog UMETA(DisplayName = "Jog")
};

UCLASS()
class FIRSTUNREALPROJECT_API UCharacterAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Animation)
		float	Speed;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Animation)
		FVector Velocity;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Animation)
		FVector InputVector;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Animation)
		bool	IsMoving;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Animation)
		bool	IsFalling;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Animation)
		bool	IsRun;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Animation)
		bool	IsAttack;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animation)
		ECharacterState CharacterLocoState = ECharacterState::E_Idle;


	UPROPERTY(VisibleAnywhere)
		class ACustomCharacter* Character;
	UPROPERTY(VisibleAnywhere)
		class	UCharacterMovementComponent* CharacterMovement;
	UPROPERTY(VisibleAnywhere)
		class USkeletalMeshComponent* SkeletalMeshComponent;
public:
	FOnAttckHit OnAttackHit;
public:
	virtual void NativeInitializeAnimation() override;
	virtual void NativeBeginPlay() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

	UFUNCTION()
		void AnimNotify_Hit();

	bool IsMovementWithinThresholds(float MinCurrentSpeed, float MinMaxSpeed, float MinInputAcceleration);
};
