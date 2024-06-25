// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/DamageType.h"
#include "DamageInterface.h"
#include "ActorComponent/CoolTimeInterface.h"
#include "DamageTypeBase.generated.h"

/**
 * 
 */
UCLASS()
class FIRSTUNREALPROJECT_API UDamageTypeBase : public UDamageType, public IDamageInterface, public ICoolTimeInterface
{
	GENERATED_BODY()
public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Data")
	FString DamageName;
	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = "Data")
	FColor DamageColor;

	UPROPERTY(VisibleDefaultsOnly)
	class UDamageComponent* TargetDamageComponent;


	UPROPERTY(EditDefaultsOnly, Category = "Critical")
	bool bUseCritical = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Critical", Meta = (EditCondition = "bUseCritical"))
	FColor CriticalDamageColor;


	UPROPERTY(EditDefaultsOnly, Category = "Dot")
	bool bUseDamageOverTime = false;
	UPROPERTY(EditDefaultsOnly, Category = "Dot",Meta = (EditCondition = "bUseDamageOverTime"))
	float DamageOverTime = 1.0f;
	UPROPERTY(EditDefaultsOnly, Category = "Dot", Meta = (EditCondition = "bUseDamageOverTime"))
	float DotDamage = 1.0f;

	UPROPERTY(VisibleDefaultsOnly, Category = "Dot", Meta = (EditCondition = "bUseDamageOverTime"))
	float OneTimeDot= 1.0f;
	UPROPERTY(VisibleDefaultsOnly, Category = "Dot", Meta = (EditCondition = "bUseDamageOverTime"))
	float CurrentDamageOverTime = 1.0f;



public:
	virtual bool SetAttackType(class UDamageComponent* DamageComponent, float Damage, AActor* DamageCauser);

	virtual void StartCooldown_Implementation(float CooldownTime);
	virtual void EndCooldown_Implementation();
	virtual void OneTimeCooldown_Implementation();
};
