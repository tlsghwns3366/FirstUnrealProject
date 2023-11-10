// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCharacter.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "PlayerAnimInstance.h"
#include "CharacterStateComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Perception/AIPerceptionStimuliSourceComponent.h"
#include "ItemActor.h"
#include "ItemObject.h"
#include "Engine/DamageEvents.h"
#include "AttackSystemComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Engine/World.h"
#include "QuickSlotComponent.h"
#include "PlayerMessageComponent.h"

// Sets default values
APlayerCharacter::APlayerCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	static ConstructorHelpers::FObjectFinder<USkeletalMesh> SkeletalMesh(TEXT("/Script/Engine.SkeletalMesh'/Game/ThirdPerson/Characters/Mannequins/Meshes/SKM_Quinn.SKM_Quinn'"));
	static ConstructorHelpers::FClassFinder<UAnimInstance> AnimInstance(TEXT("/Script/Engine.AnimBlueprint'/Game/Animation/Player/ABP_Player.ABP_Player_C'"));

	Scene = CreateDefaultSubobject<USceneComponent>(TEXT("SceneComponent"));
	QuickSlotComponent = CreateDefaultSubobject<UQuickSlotComponent>(TEXT("QuickSlotComponent"));
	PlayerMessageComponent = CreateDefaultSubobject<UPlayerMessageComponent>(TEXT("PlayerMessageComponent"));
	AIPerceptionStimuliSourceComponent = CreateDefaultSubobject<UAIPerceptionStimuliSourceComponent>(TEXT("AIPerceptionStimuliSourceComponent"));
	if (SkeletalMesh.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(SkeletalMesh.Object);
		GetMesh()->SetRelativeLocationAndRotation(FVector(0.f, 0.f, -88.0f), FRotator(0.f, -90.f, 0.f));
	}


	Scene->SetupAttachment(RootComponent);
	GetMesh()->SetupAttachment(Scene);

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));

	SpringArm->SetupAttachment(GetCapsuleComponent());
	Camera->SetupAttachment(SpringArm);

	SpringArm->TargetArmLength = 600.f;
	SpringArm->SetRelativeRotation(FRotator(-35.f, 0.f, 0.f));
	SpringArm->SocketOffset = FVector(0.f, 50.f, 75.f);

	SpringArm->bUsePawnControlRotation = true;


	TraceDistance = SpringArm->TargetArmLength + 300;
	GetCharacterMovement()->MaxWalkSpeed = 500;
	
	if (AnimInstance.Succeeded())
	{
		GetMesh()->SetAnimClass(AnimInstance.Class);
	}
	GetMovementComponent()->GetNavAgentPropertiesRef().bCanCrouch = true;
}

// Called when the game starts or when spawned
void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
	Anim = Cast<UPlayerAnimInstance>(GetMesh()->GetAnimInstance());
}

// Called every frame
void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (IsRun)
		Running(DeltaTime);
	if(Anim->ForwardInput != 0 || Anim->SideInput != 0)
		SetActorRotation(FRotator(0.f,UKismetMathLibrary::RInterpTo(GetActorRotation(), GetController()->GetControlRotation(), DeltaTime,0.f).Yaw,0.f));
	ForwardTrace();
}

// Called to bind functionality to input
float APlayerCharacter::TakeDamage(float Damage, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	Super::TakeDamage(Damage, DamageEvent, EventInstigator, DamageCauser);
	return Damage;
}


void APlayerCharacter::Attack()
{
	IsAttacking = AttackSystemComponent->Attack();
}

bool APlayerCharacter::SetIsRunTrue()
{
	if (MainStateComponent->CurrentStamina < 10.f)
		return false;

	GetCharacterMovement()->MaxWalkSpeed = MainStateComponent->FinalState.RunSpeed;
	MainStateComponent->CurrentSpeed = MainStateComponent->FinalState.RunSpeed;
	IsRun = true;
	return true;
}

void APlayerCharacter::SetIsRunFalse()
{
	GetCharacterMovement()->MaxWalkSpeed = MainStateComponent->FinalState.WalkSpeed;
	MainStateComponent->CurrentSpeed = MainStateComponent->FinalState.WalkSpeed;
	IsRun = false;
}

void APlayerCharacter::Running(float DeltaTime)
{
	MainStateComponent->StaminaUseDelay = 1.5f;
	float CurrentStamina = MainStateComponent->CurrentStamina;
	CurrentStamina = UKismetMathLibrary::FInterpTo(CurrentStamina, CurrentStamina-1.f, DeltaTime,10.f);
	if (CurrentStamina < 0)
		SetIsRunFalse();
	MainStateComponent->CurrentStamina = CurrentStamina;
}

void APlayerCharacter::JumpStart()
{
	if (!GetCharacterMovement()->IsFalling() && !Anim->IsCrouch)
	{
		if (MainStateComponent->UseStamina(30.f))
		{
			Jump();
		}
	}
}

void APlayerCharacter::JumpEnd()
{
	StopJumping();
}

void APlayerCharacter::DodgeAction()
{
	if (IsValid(Anim))
	{
		if (Anim->IsFalling)
			return;
		if (Anim->ForwardInput == 0 && Anim->SideInput == 0)
			return;
		StopAttack.Broadcast();
		if (MainStateComponent->UseStamina(50.f))
		{
			float DeshSize = 1000.f;
			FRotator YawRotation(0, GetControlRotation().Yaw, 0);
			FVector Input = FVector(Anim->ForwardInput, Anim->SideInput, 0.f);
			if ((Anim->ForwardInput == 1 || Anim->ForwardInput == -1) && (Anim->SideInput == 1 || Anim->SideInput == -1))
				Input = Input * 0.5f;
			Input = YawRotation.RotateVector(Input);
			FVector LaunchVelocity = Input * DeshSize;
			LaunchVelocity.Z = 100.f;
			GetCharacterMovement()->AddImpulse(LaunchVelocity, true);
			//GetCharacterMovement()->Launch(LaunchVelocity);
		}
	}
}

void APlayerCharacter::CrouchAction()
{
	if (IsValid(Anim))
	{
		if (Anim->IsCrouch)
		{
			FHitResult HitResult;
			FVector Start = GetActorLocation();
			FVector End = (UKismetMathLibrary::GetUpVector(GetActorRotation()) * 200.f) + GetActorLocation();
			FCollisionQueryParams CollisionParams;
			CollisionParams.AddIgnoredActor(this); 
			bool Result = GetWorld()->LineTraceSingleByChannel(HitResult, Start, End, ECC_WorldStatic, CollisionParams);
			DrawDebugLine(GetWorld(), Start, End, FColor::Orange, false, 2.0f);
			if (!Result)
			{
				Anim->IsCrouch = false;
				GetCharacterMovement()->UnCrouch(true);
			}
		}
		else
		{
			Anim->IsCrouch = true;
			GetCharacterMovement()->Crouch(true);
		}
	}
}

void APlayerCharacter::ForwardTrace()
{
	FVector Loc;
	FRotator Rot;
	FHitResult HitResult;
	GetController()->GetPlayerViewPoint(Loc, Rot);

	FVector Start = Loc;
	FVector End = Start + (Rot.Vector() * TraceDistance);

	FCollisionQueryParams TraceParams;
	bool Result = GetWorld()->LineTraceSingleByChannel(HitResult, Start, End, ECC_GameTraceChannel2, TraceParams);
	//DrawDebugLine(GetWorld(), Start, End, FColor::Orange, false, 2.0f);

	if (Result)
	{
		AActor* Interactable = HitResult.GetActor();
		if (Interactable)
		{
			if (FocusedActor != Interactable)
			{
				if (FocusedActor)
				{
					EndFocus(FocusedActor);
				}
				StartFocus(Interactable);
				FocusedActor = Interactable;
			}
		}
		else
		{
			if (FocusedActor)
			{
				EndFocus(FocusedActor);
			}
			FocusedActor = nullptr;
		}
	}
	else
	{
		EndFocus(FocusedActor);
		FocusedActor = nullptr;
	}
}

void APlayerCharacter::StartFocus(AActor* TraceActor)
{
	IInteractionInterface* Interface = Cast<IInteractionInterface>(TraceActor);
	if (Interface)
	{
		Interface->Execute_StartFocus(TraceActor);
	}
}

void APlayerCharacter::EndFocus(AActor* TraceActor)
{
	IInteractionInterface* Interface = Cast<IInteractionInterface>(TraceActor);
	if (Interface)
	{
		Interface->Execute_EndFocus(TraceActor);
	}
}

void APlayerCharacter::Interaction()
{
	if (IsValid(FocusedActor))
	{
		if (AItemActor* Item = Cast< AItemActor>(FocusedActor))
		{
			Item->AddInventory(InventoryComponent);
		}
		else if(IInteractionInterface* Interface = Cast<IInteractionInterface>(FocusedActor))
		{
			Interface->Execute_OnInteract(FocusedActor,this);
		}
	}
}

void APlayerCharacter::SlotUse(float Num)
{
	QuickSlotComponent->UseSlot(Num);
}

void APlayerCharacter::SetTraceDistance(float Value)
{
	TraceDistance = 300 + Value;
}