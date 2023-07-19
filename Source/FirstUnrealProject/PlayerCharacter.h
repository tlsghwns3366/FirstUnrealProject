// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "PlayerCharacter.generated.h"

UCLASS()
class FIRSTUNREALPROJECT_API APlayerCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	APlayerCharacter();
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		class USpringArmComponent* SpringArm;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		class UCameraComponent* Camera;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		class UPlayerAnimInstance* Animinstance;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		class UPlayerActorComponent* PlayerComponent;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		class UInventoryComponent* InventoryComponent;

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

	void KeyUpDown(float value);
	void KeyLeftRight(float value);
	void LookLeftRight(float value);
	void LookUpDown(float value);

	void SetIsRun();
};
