// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CharacterStateComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnHpUpdated);


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class FIRSTUNREALPROJECT_API UCharacterStateComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UCharacterStateComponent();
public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Info)
		int32 Level;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Info)
		float Hp;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Info)
		float MaxHp;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Info)
		float Speed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Info)
		float Stamina;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Info)
		float StaminaMax;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Info)
		float AttackDamage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Info)
		float Shild;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Info, meta = (ClampMin = 0.0, ClampMax = 100.0))
		int32 CriticalChance;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Info)
		int32 CriticalDamage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Info, meta = (ClampMin = 0.0, ClampMax = 100.0))
		int32 DodgeChance;

	UPROPERTY(BlueprintAssignable)
		FOnHpUpdated OnhpUpdated;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

public:
	virtual void SetLevel(int32 _Level);
	virtual void OnDamaged(float DamageAmount);
public:
	virtual void SetHp(float NewHp);
	virtual int32 GetLevel() { return Level; }
	virtual int32 GetHp() { return Hp; }
};
