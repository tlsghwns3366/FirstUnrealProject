// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/DamageType.h"
#include "DamageInterface.h"
#include "DamageType_FIre.generated.h"

/**
 * 
 */
UCLASS()
class FIRSTUNREALPROJECT_API UDamageType_FIre : public UDamageType, public IDamageInterface
{
	GENERATED_BODY()
public:
	virtual bool SetAttackType(class UDamageComponent* DamageComponent, float Damage) const;
};

