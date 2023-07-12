// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCharacter.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "PlayerAnimInstance.h"

// Sets default values
APlayerCharacter::APlayerCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	static ConstructorHelpers::FObjectFinder<USkeletalMesh> SkeletalMesh(TEXT("/Script/Engine.SkeletalMesh'/Game/ThirdPerson/Characters/Mannequins/Meshes/SKM_Quinn.SKM_Quinn'"));

	if (SkeletalMesh.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(SkeletalMesh.Object);
		GetMesh()->SetRelativeLocationAndRotation(FVector(0.f, 0.f, -88.0f), FRotator(0.f, -90.f, 0.f));
	}


	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));

	SpringArm->SetupAttachment(GetCapsuleComponent());
	Camera->SetupAttachment(SpringArm);

	SpringArm->TargetArmLength = 600.f;
	SpringArm->SetRelativeRotation(FRotator(-35.f, 0.f, 0.f));
	SpringArm->SocketOffset = FVector(0.f, 50.f, 75.f);

	SpringArm->bUsePawnControlRotation = true;

	
	static ConstructorHelpers::FClassFinder<UAnimInstance> Anim(TEXT("/Script/Engine.AnimBlueprint'/Game/Anim/ABP_Player.ABP_Player_C'"));
	if (Anim.Succeeded())
	{
		GetMesh()->SetAnimClass(Anim.Class);
	}

}

// Called when the game starts or when spawned
void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis(TEXT("UpDown"), this, &APlayerCharacter::KeyUpDown);
	PlayerInputComponent->BindAxis(TEXT("LeftRight"), this, &APlayerCharacter::KeyLeftRight);
	PlayerInputComponent->BindAxis(TEXT("LookLeftRight"), this, &APlayerCharacter::LookLeftRight);
	PlayerInputComponent->BindAxis(TEXT("LookUpDown"), this, &APlayerCharacter::LookUpDown);

	PlayerInputComponent->BindAxis(TEXT("Run"), this, &APlayerCharacter::SetRun);

}

void APlayerCharacter::KeyUpDown(float value)
{
	AddMovementInput(GetActorForwardVector(), value);
}

void APlayerCharacter::KeyLeftRight(float value)
{
	AddMovementInput(GetActorRightVector(), value);
}

void APlayerCharacter::LookLeftRight(float value)
{
	AddControllerYawInput(value);
}

void APlayerCharacter::LookUpDown(float value)
{
	AddControllerPitchInput(value);
}

void APlayerCharacter::SetRun(float value)
{
	auto Anim = Cast<UPlayerAnimInstance>(GetMesh()->GetAnimInstance());
	Anim->SetIsRun(value);
}
