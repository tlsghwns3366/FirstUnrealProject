// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Weapon.generated.h"

UENUM(BlueprintType)
enum class EWeaponEnum : uint8
{
	E_Weapon_None UMETA(DisplayName = "Weapon_None"),
	E_Weapon_OneHandedWeapon UMETA(DisplayName = "Weapon_OneHandedWeapon"),
	E_Weapon_TwoHandedWeapon UMETA(DisplayName = "Weapon_TwoHandedWeapon"),
	E_Weapon_RangedWeapon UMETA(DisplayName = "Weapon_RangedWeapon"),
	E_Weapon_MagicWeapon UMETA(DisplayName = "Weapon_MagicWeapon"),
	E_Weapon_MiscellaneousWeapon UMETA(DisplayName = "Weapon_MiscellaneousWeapon"),
};

UCLASS()
class FIRSTUNREALPROJECT_API AWeapon : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWeapon();
public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		EWeaponEnum WeaponEnum = EWeaponEnum::E_Weapon_None;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class USceneComponent* SceneComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mesh");
		class UStaticMeshComponent* StaticMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hit Box")
		class UArrowComponent* StartPoint;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hit Box")
		class UArrowComponent* EndPoint;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Instanced, Category = "Info")
		class UEquipItemObject* EquipItem;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
		float StaminaCost;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool IsEquip = true;;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintImplementableEvent)
	void WeaponAction();

	UFUNCTION(BlueprintNativeEvent)
	void WeaponInitialize(UEquipItemObject* Item);
	virtual void WeaponInitialize_Implementation(UEquipItemObject* Item);

};
