// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "InputAction.h"
#include "InputActionValue.h"
#include "InputMappingContext.h"
#include "MainPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class FIRSTUNREALPROJECT_API AMainPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	/** MappingContext */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
		UInputMappingContext* DefaultMappingContext;
		
	/** Move Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
		UInputAction* MoveAction;

	/** Look Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
		UInputAction* LookAction;

	/** Sprint Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
		UInputAction* SprintAction;

	/** Sprint Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
		UInputAction* JumpAction;

	/** Interact Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
		UInputAction* InteractAction;

	/** Interact Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
		UInputAction* AttackAction;

	/** Alt Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
		UInputAction* AltAction;

		bool MouseInput;
public:
	AMainPlayerController();
	virtual void BeginPlay() override;
public:
		void SetupInputComponent() override;

		/** Called for movement input */
		void RequestMove(const FInputActionValue& Value);

		/** Called for looking input */
		void RequestLook(const FInputActionValue& Value);

		/** Called for attack input **/
		void RequestAttack();

		/** Called for interact input **/
		void RequestInteract();

		/** Called for jump input **/
		void RequestJumpStart();
		void RequestJumpStop();

		/** Called for sprint input */
		void SetIsRunTrue();
		void SetIsRunFalse();

		void SetShowMouse();

};
