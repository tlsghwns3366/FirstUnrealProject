// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "EnemyCharacter.generated.h"

UCLASS()
class FIRSTUNREALPROJECT_API AEnemyCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AEnemyCharacter();
public:

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		class UEnemyStateActorComponent* EnemyStateComponent;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		class UEnemyInventoryComponent* EnemyInventoryComponent;
public:

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
public:

	void Attack();
	void SetIsRun(bool Run);

public:
	UFUNCTION(BlueprintImplementableEvent)
		void SetEnemyInventory();
	

};
