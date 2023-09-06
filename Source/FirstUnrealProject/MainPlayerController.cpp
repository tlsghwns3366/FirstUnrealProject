// Fill out your copyright notice in the Description page of Project Settings.


#include "MainPlayerController.h"
#include "PlayerCharacter.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Camera/CameraComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/KismetMathLibrary.h"
#include "EnemyCharacter.h"
#include "CharacterStateComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "PlayerAnimInstance.h"
#include "Math/UnrealMathUtility.h"

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
    if (IsValid(TargetActor))
    {
        const FRotator LookAtRotation = UKismetMathLibrary::FindLookAtRotation(MainPlayer->Camera->GetComponentLocation(), TargetActor->GetActorLocation());
        const FRotator InterpRotation = UKismetMathLibrary::RInterpTo(GetControlRotation(), LookAtRotation, DeltaTime,3.f);
        SetControlRotation(InterpRotation);
        if (TargetActor->MainStateComponent->IsDie)
        {
            TargetActor = nullptr;
            TargetLook = false;
        }
    }
    
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

        // ** ALT ** //
        EnhancedInputComponent->BindAction(AltAction, ETriggerEvent::Triggered, this, &AMainPlayerController::SetShowMouse);

        // ** TARGET ** //
        EnhancedInputComponent->BindAction(TargetAction, ETriggerEvent::Triggered, this, &AMainPlayerController::Target);

        // ** SpaceAction ** //
        EnhancedInputComponent->BindAction(SpaceAction, ETriggerEvent::Triggered, this, &AMainPlayerController::RequestSpace);
        // ** LeftCtrlAction ** //
        EnhancedInputComponent->BindAction(CtrlAction, ETriggerEvent::Triggered, this, &AMainPlayerController::RequestCrouch);


    }
}

void AMainPlayerController::RequestMove(const FInputActionValue& Value)
{
    // input is a Vector2D
    FVector2D MovementVector = Value.Get<FVector2D>();
    if (IsValid(MainPlayer))
    {
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

    float Sensitivity = 1.f;
    float InterpolatedYawInput = FMath::Lerp(0.f, LookAxisVector.X, Sensitivity);
    float InterpolatedPitchInput = FMath::Lerp(0.f, LookAxisVector.Y, Sensitivity);

    if (IsValid(MainPlayer))
    {
        // add yaw and pitch input to controller
        MainPlayer->AddControllerYawInput(-InterpolatedYawInput);
        MainPlayer->AddControllerPitchInput(InterpolatedPitchInput);
    }
}

void AMainPlayerController::RequestZoom(const FInputActionValue& Value)
{
    float StartArmLength = MainPlayer->SpringArm->TargetArmLength;   
    float InterpolatedInput = FMath::Lerp(StartArmLength, StartArmLength+Value.Get<float>()*30,1.f);
    if(InterpolatedInput <= 2000.f && InterpolatedInput >= 200)
    MainPlayer->SpringArm->TargetArmLength = InterpolatedInput;
}

void AMainPlayerController::RequestAttack()
{
    if (IsValid(MainPlayer))
    {
        MainPlayer->Attack();
    }
}

void AMainPlayerController::RequestInteract()
{
    if (IsValid(MainPlayer))
    {
        MainPlayer->Interaction();
    }
}


void AMainPlayerController::RequestJumpStart()
{
    if (IsValid(GetCharacter()))
    {
        MainPlayer->JumpStart();
    }
}

void AMainPlayerController::RequestJumpStop()
{
    if (IsValid(GetCharacter()))
    {
        MainPlayer->JumpEnd();
    }
}
void AMainPlayerController::SetIsRunTrue()
{
    if (IsValid(MainPlayer))
    {
        MainPlayer->SetIsRunTrue();
    }
}

void AMainPlayerController::SetIsRunFalse()
{
    if (IsValid(MainPlayer))
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
    if (!TargetLook)
    {
        FVector Start = MainPlayer->GetActorLocation();
        FVector End = Start + MainPlayer->Camera->GetForwardVector() * 1000.f;
        FHitResult HitResult;
        TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
        ObjectTypes.Add(EObjectTypeQuery::ObjectTypeQuery3);
        TArray<AActor*> ActorsToIgnore;
        ActorsToIgnore.Add(MainPlayer);
        bool Result = UKismetSystemLibrary::SphereTraceSingleForObjects(
            GetWorld(),
            Start,
            End,
            125.0f,
            ObjectTypes,
            false,
            ActorsToIgnore,
            EDrawDebugTrace::ForDuration,
            HitResult,
            true,
            FLinearColor::Red,
            FLinearColor::Green,
            5.0f
        );
        if (Result)
        {
            TargetActor = Cast<AEnemyCharacter>(HitResult.GetActor());
            TargetLook = true;
        }
        else
        {
            TargetActor = nullptr;
            TargetLook = false;
        }
    }
    else
    {
        TargetActor = nullptr;
        TargetLook = false;
    }
}

void AMainPlayerController::RequestSpace()
{
    if (IsValid(MainPlayer))
    {
        MainPlayer->DodgeAction();
    }
}

void AMainPlayerController::RequestCrouch()
{
    if (IsValid(MainPlayer))
    {
        MainPlayer->CrouchAction();
    }
}