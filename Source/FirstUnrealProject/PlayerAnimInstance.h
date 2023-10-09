// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CharacterAnimInstance.h"
#include "PlayerAnimInstance.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnAttckHit);
/**
 * 
 */
UCLASS()
class FIRSTUNREALPROJECT_API UPlayerAnimInstance : public UCharacterAnimInstance
{
	GENERATED_BODY()
public:

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Animation)
		float ForwardInput;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Animation)
		float SideInput;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Animation)
		bool IsCrouch;



public:
	virtual void NativeInitializeAnimation() override;
	virtual void NativeBeginPlay() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override; 

public:

	void SetLocomotionState();
};
