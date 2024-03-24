// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "Weapon.h"	
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
	E_Jog UMETA(DisplayName = "Jog"),
	E_Crouch UMETA(DisplayName = "Crouch")
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
		float	PlayRate;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Animation)
		float	DeltaTimeX;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animation)
		float WalkStartAngle;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animation)
		float JogStartAngle;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animation)
		FRotator StartRotation;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animation)
		FRotator PrimaryTargetRotation;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animation)
		FRotator SecondaryTargetRotation;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Animation)
		bool	IsMoving;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Animation)
		bool	IsFalling;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Animation)
		bool	IsRun;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Animation)
		bool	IsAttack;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Animation)
		bool IsCrouch;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animation)
		bool	PlayWalkStart;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animation)
		bool	PlayJogStart;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animation)
		bool DoInputVectorRotation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animation)
		ECharacterState CharacterLocoState = ECharacterState::E_Idle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		EWeaponEnum WeaponEnum = EWeaponEnum::E_Weapon_None;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
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
	virtual void NativePostEvaluateAnimation() override;

	UFUNCTION()
		void AnimNotify_Hit();


	void SetLocomotionState();
	bool IsMovementWithinThresholds(float MinCurrentSpeed, float MinMaxSpeed, float MinInputAcceleration);


	UFUNCTION(BlueprintCallable)
		void ResetTargetRotation();

	UFUNCTION(BlueprintCallable)
		void ResetTransitions();

	UFUNCTION(BlueprintPure)
		FRotator GetTargetRotation();

	UFUNCTION(BlueprintCallable)
		void UpdateOnWalkEntry();

	UFUNCTION(BlueprintCallable)
		void UpdateOnJogEntry();

	UFUNCTION(BlueprintCallable)
		void UpdateOnCrouchLoop();

	UFUNCTION(BlueprintCallable)
		void UpdateLocomotionValue();



};
