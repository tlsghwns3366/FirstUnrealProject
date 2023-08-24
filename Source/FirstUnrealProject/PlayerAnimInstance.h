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

public:
public:
	virtual void NativeInitializeAnimation() override;
	virtual void NativeBeginPlay() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override; 

public:
};
