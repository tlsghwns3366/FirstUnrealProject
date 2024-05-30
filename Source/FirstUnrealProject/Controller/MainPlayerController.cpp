// Fill out your copyright notice in the Description page of Project Settings.


#include "MainPlayerController.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Camera/CameraComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/KismetMathLibrary.h"
#include "GameFramework/SpringArmComponent.h"
#include "Math/UnrealMathUtility.h"

#include "Character/EnemyCharacter.h"
#include "Character/CustomCharacter.h"
#include "Anim/PlayerAnimInstance.h"
#include "ActorComponent/CharacterStateComponent.h"
#include "Character/PlayerCharacter.h"

AMainPlayerController::AMainPlayerController()
{
}

void AMainPlayerController::BeginPlay()
{
	MainPlayer = Cast<APlayerCharacter>(GetCharacter());
	// Add Input Mapping Context
	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
	{
		Subsystem->AddMappingContext(DefaultMappingContext, 0);
	}
}

void AMainPlayerController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AMainPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(InputComponent))
	{
		// ** Moving ** //
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AMainPlayerController::RequestMove);
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Completed, this, &AMainPlayerController::RequestMoveSet);
		// ** Looking ** //
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AMainPlayerController::RequestLook);
		//** Jumping **//
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Triggered, this, &AMainPlayerController::RequestJumpStart);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &AMainPlayerController::RequestJumpStop);

		//** Sprinting **//
		EnhancedInputComponent->BindAction(SprintAction, ETriggerEvent::Triggered, this, &AMainPlayerController::SetIsRunTrue);
		EnhancedInputComponent->BindAction(SprintAction, ETriggerEvent::Completed, this, &AMainPlayerController::SetIsRunFalse);

		// ** MouseWheelAxis ** //
		EnhancedInputComponent->BindAction(MouseWheelAxisAction, ETriggerEvent::Triggered, this, &AMainPlayerController::RequestZoom);

		//-----------------------------------------------------------------------------------------------------------------------------------------//


		// ** INTERACT ** //
		EnhancedInputComponent->BindAction(InteractAction, ETriggerEvent::Triggered, this, &AMainPlayerController::RequestInteract);

		// ** ATTACK ** //
		EnhancedInputComponent->BindAction(AttackAction, ETriggerEvent::Triggered, this, &AMainPlayerController::RequestAttack);
		EnhancedInputComponent->BindAction(AttackAction, ETriggerEvent::Completed, this, &AMainPlayerController::RequestAttackRelease);

		// ** ALT ** //
		EnhancedInputComponent->BindAction(AltAction, ETriggerEvent::Triggered, this, &AMainPlayerController::SetShowMouse);

		// ** TARGET ** //
		EnhancedInputComponent->BindAction(TargetAction, ETriggerEvent::Triggered, this, &AMainPlayerController::Target);

		// ** SpaceAction ** //
		EnhancedInputComponent->BindAction(SpaceAction, ETriggerEvent::Triggered, this, &AMainPlayerController::RequestSpace);
		// ** LeftCtrlAction ** //
		EnhancedInputComponent->BindAction(CtrlAction, ETriggerEvent::Triggered, this, &AMainPlayerController::RequestCrouch);

		// ** SlotAction ** //
		EnhancedInputComponent->BindAction(SlotAction, ETriggerEvent::Triggered, this, &AMainPlayerController::RequestSlotAction);

		// ** Temp ** //
		EnhancedInputComponent->BindAction(TempAction, ETriggerEvent::Triggered, this, &AMainPlayerController::RequestTemp);

		// ** CameraZoom ** //
		EnhancedInputComponent->BindAction(ZoomAction, ETriggerEvent::Triggered, this, &AMainPlayerController::SetZoomTrue);
		EnhancedInputComponent->BindAction(ZoomAction, ETriggerEvent::Completed, this, &AMainPlayerController::SetZoomFalse);
	}
}

void AMainPlayerController::RequestMove(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (MainPlayer)
	{
		if (MainPlayer->IsAttacking)
			return;
		// find out which way is forward
		const FRotator Rotation = MainPlayer->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		// get right vector
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		MainPlayer->Anim->ForwardInput = MovementVector.Y;
		MainPlayer->Anim->SideInput = MovementVector.X;

		// add movement
		MainPlayer->AddMovementInput(ForwardDirection, MovementVector.Y);
		MainPlayer->AddMovementInput(RightDirection, MovementVector.X);
	}
}

void AMainPlayerController::RequestMoveSet(const FInputActionValue& Value)
{
	MainPlayer->Anim->ForwardInput = 0.f;
	MainPlayer->Anim->SideInput = 0.f;
}

void AMainPlayerController::RequestLook(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (MainPlayer)
	{
		// add yaw and pitch input to controller
		MainPlayer->AddControllerYawInput(-LookAxisVector.X);
		MainPlayer->AddControllerPitchInput(LookAxisVector.Y);
	}
}

void AMainPlayerController::RequestZoom(const FInputActionValue& Value)
{
	if (!MainPlayer->IsZoom)
	{
		float StartArmLength = MainPlayer->CameraDistance;
		float InterpolatedInput = FMath::Lerp(StartArmLength, StartArmLength + Value.Get<float>() * 30.f, 1.f);
		MainPlayer->SetTraceDistance(InterpolatedInput);
		if (InterpolatedInput <= 2000.f && InterpolatedInput >= 200.f)
			MainPlayer->CameraDistance = InterpolatedInput;
	}
}

void AMainPlayerController::RequestAttack()
{
	if (MainPlayer)
	{
		MainPlayer->AttackCharage();
	}
}

void AMainPlayerController::RequestAttackRelease()
{
	if (MainPlayer)
	{
		MainPlayer->Attack();
	}
}

void AMainPlayerController::RequestInteract()
{
	if (MainPlayer)
	{
		MainPlayer->Interaction();
	}
}


void AMainPlayerController::RequestJumpStart()
{
	if (MainPlayer)
	{
		MainPlayer->JumpStart();
	}
}

void AMainPlayerController::RequestJumpStop()
{
	if (MainPlayer)
	{
		MainPlayer->JumpEnd();
	}
}
void AMainPlayerController::SetIsRunTrue()
{
	if (MainPlayer)
	{
		MainPlayer->SetIsRunTrue();
	}
}

void AMainPlayerController::SetIsRunFalse()
{
	if (MainPlayer)
	{
		MainPlayer->SetIsRunFalse();
	}
}

void AMainPlayerController::SetShowMouse()
{
	if (this->GetMouseCursor() == EMouseCursor::None)
	{
		SetShowMouseCursor(true);
		SetInputMode(FInputModeGameAndUI());
	}
	else
	{
		SetShowMouseCursor(false);
		SetInputMode(FInputModeGameOnly());
	}
}

void AMainPlayerController::Target()
{
	if (MainPlayer)
	{
		MainPlayer->SetTargetActor();
	}
}

void AMainPlayerController::RequestSpace()
{
	if (MainPlayer)
	{
		MainPlayer->DodgeAction();
	}
}

void AMainPlayerController::RequestCrouch()
{
	if (MainPlayer)
	{
		MainPlayer->CrouchAction();
	}
}
void AMainPlayerController::RequestSlotAction(const FInputActionValue& Value)
{
	float Num = Value.Get<float>();
	if (MainPlayer)
	{
		MainPlayer->SlotUse(Num);
	}
}

void AMainPlayerController::RequestTemp()
{
	if (MainPlayer)
	{
		MainPlayer->TempAction();
	}
}

void AMainPlayerController::SetZoomTrue()
{
	if (MainPlayer)
	{
		MainPlayer->ZoomAction(true);
	}
}

void AMainPlayerController::SetZoomFalse()
{
	if (MainPlayer)
	{
		MainPlayer->ZoomAction(false);
	}
}
