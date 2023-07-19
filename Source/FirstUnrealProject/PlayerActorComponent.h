// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PlayerActorComponent.generated.h"




UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class FIRSTUNREALPROJECT_API UPlayerActorComponent : public UActorComponent
{
	GENERATED_BODY()

private:
	UPROPERTY(EditAnywhere, Category = Info, Meta = (AllowPrivateAccess = true))
		int32 Level;
	UPROPERTY(EditAnywhere, Category = Info, Meta = (AllowPrivateAccess = true))
		float Hp;
	UPROPERTY(EditAnywhere, Category = Info, Meta = (AllowPrivateAccess = true))
		float MaxHp;
	UPROPERTY(EditAnywhere, Category = Info, Meta = (AllowPrivateAccess = true))
		float Speed;
	UPROPERTY(EditAnywhere, Category = Info, Meta = (AllowPrivateAccess = true))
		float exp;


	UPROPERTY(EditAnywhere, Category = Info, Meta = (AllowPrivateAccess = true))
		float AttackDamage;
	UPROPERTY(EditAnywhere, Category = Info, Meta = (AllowPrivateAccess = true))
		float Shild;

	UPROPERTY(EditAnywhere, Category = Info, Meta = (AllowPrivateAccess = true))
		int32 CriticalChance;
	UPROPERTY(EditAnywhere, Category = Info, Meta = (AllowPrivateAccess = true))
		int32 CriticalDamage;

public:	
	// Sets default values for this component's properties
	UPlayerActorComponent();

protected:
	// Called when the game starts
	virtual void InitializeComponent() override;// BeginPlay전에 호출
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
public:
	void SetLevel(int32 _Level);
	void OnDamaged(float DamageAmount);
public:
	int32 GetLevel() { return Level; }
	int32 GetHp() { return Hp; }
		
};
