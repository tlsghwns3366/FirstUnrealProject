// Fill out your copyright notice in the Description page of Project Settings.


#include "MainPlayerController.h"
#include "PlayerCharacter.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"


AMainPlayerController::AMainPlayerController()
{
}

void AMainPlayerController::BeginPlay()
{

    // Add Input Mapping Context
    if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
    {
        Subsystem->AddMappingContext(DefaultMappingContext, 0);
    }
}

void AMainPlayerController::SetupInputComponent()
{
    Super::SetupInputComponent(); 
    if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(InputComponent))
    {
        // ** Moving ** //
        EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AMainPlayerController::RequestMove);
        // ** Looking ** //
        EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AMainPlayerController::RequestLook);
        //** Jumping **//
        EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Triggered, this, &AMainPlayerController::RequestJumpStart);
        EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &AMainPlayerController::RequestJumpStop);

        //** Sprinting **//
        EnhancedInputComponent->BindAction(SprintAction, ETriggerEvent::Triggered, this, &AMainPlayerController::SetIsRunTrue);
        EnhancedInputComponent->BindAction(SprintAction, ETriggerEvent::Completed, this, &AMainPlayerController::SetIsRunFalse);

        // ** INTERACT ** //
        EnhancedInputComponent->BindAction(InteractAction, ETriggerEvent::Triggered, this, &AMainPlayerController::RequestInteract);

		// ** ATTACK ** //
        EnhancedInputComponent->BindAction(AttackAction, ETriggerEvent::Triggered, this, &AMainPlayerController::RequestAttack);

        // ** ALT ** //
        EnhancedInputComponent->BindAction(AltAction, ETriggerEvent::Completed, this, &AMainPlayerController::SetShowMouse);
    }
}

void AMainPlayerController::RequestMove(const FInputActionValue& Value)
{
    APlayerCharacter* MainPlayer = Cast<APlayerCharacter>(GetCharacter());
    // input is a Vector2D
    FVector2D MovementVector = Value.Get<FVector2D>();

    if (MainPlayer != nullptr)
    {
        // find out which way is forward
        const FRotator Rotation = MainPlayer->GetControlRotation();
        const FRotator YawRotation(0, Rotation.Yaw, 0);

        // get forward vector
        const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);

        // get right vector
        const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

        // add movement
        MainPlayer->AddMovementInput(ForwardDirection, MovementVector.Y);
        MainPlayer->AddMovementInput(RightDirection, MovementVector.X);
    }
}

void AMainPlayerController::RequestLook(const FInputActionValue& Value)
{
    APlayerCharacter* MainPlayer = Cast<APlayerCharacter>(GetCharacter());

    // input is a Vector2D
    FVector2D LookAxisVector = Value.Get<FVector2D>();

    if (MainPlayer != nullptr)
    {
        // add yaw and pitch input to controller
        MainPlayer->AddControllerYawInput(-LookAxisVector.X);
        MainPlayer->AddControllerPitchInput(LookAxisVector.Y);
    }
}

void AMainPlayerController::RequestAttack()
{
    APlayerCharacter* MainPlayer = Cast<APlayerCharacter>(GetCharacter());
    if (MainPlayer)
    {
        MainPlayer->Attack();
    }
}

void AMainPlayerController::RequestInteract()
{
}


void AMainPlayerController::RequestJumpStart()
{
    if (GetCharacter())
    {
        GetCharacter()->Jump();
    }
}

void AMainPlayerController::RequestJumpStop()
{
    if (GetCharacter())
    {
        GetCharacter()->StopJumping();
    }
}
void AMainPlayerController::SetIsRunTrue()
{
    APlayerCharacter* MainPlayer = Cast<APlayerCharacter>(GetCharacter());
    if (MainPlayer)
    {
        MainPlayer->SetIsRunTrue();
    }
}

void AMainPlayerController::SetIsRunFalse()
{
    APlayerCharacter* MainPlayer = Cast<APlayerCharacter>(GetCharacter());
    if (MainPlayer)
    {
        MainPlayer->SetIsRunFalse();
    }
}

void AMainPlayerController::SetShowMouse()
{
    if (!MouseInput)
    {
        MouseInput = true;
        SetShowMouseCursor(true);
        SetInputMode(FInputModeGameAndUI());
    }
    else
    {
        MouseInput = false;
        SetShowMouseCursor(false);
        SetInputMode(FInputModeGameOnly());
    }
}
