// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item/Weapon.h"
#include "Weapon_Arrow.generated.h"

/**
 * 
 */
UCLASS()
class FIRSTUNREALPROJECT_API AWeapon_Arrow : public AWeapon
{
	GENERATED_BODY()
public:

	AWeapon_Arrow();

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	AWeapon* OwnerWeapon;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class USplineComponent* ProjectilePath;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<class USplineMeshComponent*> ProjectilePathMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UStaticMesh* SplineMesh;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UMaterialInterface* SplineMaterial;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	TSubclassOf<class AProjectileActor> ProjectileBlueprint;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	float MaxSpeed = 3000.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	float MinSpeed = 1000.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	FRotator ArrowRotator;

public:

	virtual void Tick(float DeltaTime) override;
	void SetOwnerWeapon(AWeapon* Weapon);
	void SetArrowPosition();
	void ShowProjectilePath(float Value);
	void DeleteSpline();

};
