// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "EquipItemObject.h"
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
		float CriticalChance;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Info)
		float CriticalDamage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Info, meta = (ClampMin = 0.0, ClampMax = 100.0))
		float DodgeChance;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Info)
		float HpRegen;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Info)
		float StaminaRegen;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool IsDie = false;

	UPROPERTY(BlueprintAssignable)
		FOnHpUpdated OnhpUpdated;




	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Info)
		class UEquipItemObject* Helmat;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Info)
		class UEquipItemObject* TopArmor;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Info)
		class UEquipItemObject* BottomArmor;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Info)
		class UEquipItemObject* Boots;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Info)
		class UEquipItemObject* Gloves;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Info)
		class UEquipItemObject* Weapons_1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Info)
		class UEquipItemObject* Weapons_2;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Info)
		class UEquipItemObject* Ring_1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Info)
		class UEquipItemObject* Ring_2;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

public:
	virtual void SetLevel(int32 _Level);
public:
	virtual void SetHp(float NewHp);
	virtual int32 GetLevel() { return Level; }
	virtual int32 GetHp() { return Hp; }
	virtual float GetPhysicalDamage();
	virtual UEquipItemObject* GetEquip(UEquipItemObject* Item);
	virtual bool SetEquip(UEquipItemObject* Item);
};
