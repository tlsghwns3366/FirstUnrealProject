// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"

#include "Item/Weapon.h"	
#include "CharacterAnimInstance.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnAttckHit);
DECLARE_MULTICAST_DELEGATE(FAttackStartTrace);
DECLARE_MULTICAST_DELEGATE(FOnDrawArrow);
DECLARE_MULTICAST_DELEGATE(FZoomOut);
/**
 * 
 */


UENUM(BlueprintType)
enum class ECharacterState : uint8
{
	E_Idle UMETA(DisplayName = "Idle"),
	E_Walk UMETA(DisplayName = "Walk"),
	E_Jog UMETA(DisplayName = "Jog"),
	E_Zoom UMETA(DisplayName = "Zoom")
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
		float ForwardInput;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Animation)
		float SideInput;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Animation)
		float	PlayRate;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Animation)
		float	DeltaTimeX;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animation)
		float WalkStartAngle;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animation)
		float JogStartAngle;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animation)
		float DeltaValue;

		UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Animation)
		float BowPower;

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
		bool	IsCrouch;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Animation)
		bool	IsZoom;
		UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Animation)
		bool	IsPull;


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
	FOnDrawArrow OnDrawArrow;
	FAttackStartTrace AttackStartTrace;

public:
	virtual void NativeInitializeAnimation() override;
	virtual void NativeBeginPlay() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;
	virtual void NativePostEvaluateAnimation() override;

	UFUNCTION()
		void AnimNotify_Hit();
	UFUNCTION()
		void AnimNotify_AttackStart();
	UFUNCTION()
		void AnimNotify_DrawArrow();
	UFUNCTION()
		void AnimNotify_Pull();
	UFUNCTION()
		void AnimNotify_PullOut();


	virtual void SetLocomotionState();
	virtual bool IsMovementWithinThresholds(float MinCurrentSpeed, float MinMaxSpeed, float MinInputAcceleration);


	void SetBowPower(float Value);

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
		void UpdateOnIdleEntry();

		UFUNCTION(BlueprintCallable)
		void UpdateOnZoomLoop();

	UFUNCTION(BlueprintCallable)
		void UpdateLocomotionValue();


	UFUNCTION(BlueprintCallable)
		void UpdateDeltaValue();

};
