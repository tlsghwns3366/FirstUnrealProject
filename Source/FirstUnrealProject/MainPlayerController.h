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

	/** Player */
	UPROPERTY(VisibleAnywhere)
		class APlayerCharacter* MainPlayer;

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

	/** MouseWheelAxis Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
		UInputAction* MouseWheelAxisAction;

	/** Target Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
		UInputAction* TargetAction;

	/** LeftShift Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
		UInputAction* SpaceAction;

	/** LeftCtrl Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
		UInputAction* CtrlAction;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool TargetLook;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class AEnemyCharacter* TargetActor;

public:
	AMainPlayerController();
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
public:
		void SetupInputComponent() override;

		/** Called for movement input */
		void RequestMove(const FInputActionValue& Value);
		void RequestMoveSet(const FInputActionValue& Value);

		/** Called for looking input */
		void RequestLook(const FInputActionValue& Value);

		/** Called for looking input */
		void RequestZoom(const FInputActionValue& Value);

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

		/** Called for Alt input */
		void SetShowMouse();

		/** Called for Target input */
		void Target();

		/** Called for SpaceAction input */
		void RequestSpace();

		/** Called for attack input **/
		void RequestCrouch();

};
