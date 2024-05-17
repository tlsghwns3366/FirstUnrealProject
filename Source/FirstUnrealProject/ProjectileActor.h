// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ProjectileActor.generated.h"

UCLASS()
class FIRSTUNREALPROJECT_API AProjectileActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AProjectileActor();

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class USceneComponent* SceneComponent;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class AActor* OwnerActor;


	UPROPERTY(EditAnywhere, BlueprintReadWrite);
	class UStaticMeshComponent* StaticMeshComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UArrowComponent* StartPoint;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hit Box")
	class UBoxComponent* HitBox;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UProjectileMovementComponent* ProjectileComponent;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	float MaxSpeed = 3000.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	float MinSpeed = 1000.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	float Gravity;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	float DestroyTime = 5.f;

	FTimerHandle DestroyTimer;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void SetOwnerCharacter(AActor* CharacterActor);
	virtual void ActorDestroy();
};
