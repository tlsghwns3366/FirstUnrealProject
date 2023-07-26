// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCharacter.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "PlayerAnimInstance.h"
#include "PlayerActorComponent.h"
#include "PlayerInventoryComponent.h"
#include "Kismet/GameplayStatics.h"
#include "EnemyCharacter.h"

// Sets default values
APlayerCharacter::APlayerCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	static ConstructorHelpers::FObjectFinder<USkeletalMesh> SkeletalMesh(TEXT("/Script/Engine.SkeletalMesh'/Game/ThirdPerson/Characters/Mannequins/Meshes/SKM_Quinn.SKM_Quinn'"));
	static ConstructorHelpers::FClassFinder<UAnimInstance> Anim(TEXT("/Script/Engine.AnimBlueprint'/Game/Animation/ABP_Player.ABP_Player_C'"));

	PlayerComponent = CreateDefaultSubobject<UPlayerActorComponent>(TEXT("PlayerComponent"));
	PlayerInventoryComponent = CreateDefaultSubobject<UPlayerInventoryComponent>(TEXT("PlayerInventoryComponent"));
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


	GetCharacterMovement()->MaxWalkSpeed = 500;
	
	if (Anim.Succeeded())
	{
		GetMesh()->SetAnimClass(Anim.Class);
	}
}

// Called when the game starts or when spawned
void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
	Animinstance = Cast<UPlayerAnimInstance>(GetMesh()->GetAnimInstance());
	
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

	PlayerInputComponent->BindAction(TEXT("Jump"), EInputEvent::IE_Pressed, this, &APlayerCharacter::Jump);


	PlayerInputComponent->BindAction(TEXT("PlayerRun"), EInputEvent::IE_Pressed, this, &APlayerCharacter::SetIsRunTrue);
	PlayerInputComponent->BindAction(TEXT("PlayerRun"), EInputEvent::IE_Released, this, &APlayerCharacter::SetIsRunFalse);

	PlayerInputComponent->BindAction(TEXT("LeftMouseClick"), EInputEvent::IE_Released, this, &APlayerCharacter::Attack);

	PlayerInputComponent->BindAction(TEXT("Interaction"), EInputEvent::IE_Pressed, this, &APlayerCharacter::Interaction);

}

void APlayerCharacter::Attack()
{
	if (IsValid(Animinstance))
	{
		//UE_LOG(LogTemp, Log, TEXT("Attack"));
		//AnimInstance->PlayMontage();
	}

}

void APlayerCharacter::Interaction()
{
	//UE_LOG(LogTemp, Log, TEXT("%f, %f, %f"), GetActorLocation().X, GetActorLocation().Y, GetActorLocation().Z);
	float Range = 50.f;
	FHitResult HitResult;
	FVector Center = GetActorLocation() + GetActorForwardVector() * Range + FVector(0.f, 0.f, -30.f);
	FVector Forward = Center + FVector(0.f,0.f,200.f);
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this);
	bool Result = GetWorld()->SweepSingleByChannel
	(OUT HitResult,
		Center,
		Forward,
		FQuat::Identity,
		ECollisionChannel::ECC_GameTraceChannel3,
		FCollisionShape::MakeSphere(Range),
		Params
	);
	if (Result)
	{
		DrawDebugLine(GetWorld(), Center, Forward, FColor::Green, true);
		AEnemyCharacter* Enemy = Cast<AEnemyCharacter>(HitResult.GetActor());
		if (Enemy)
		{
			Enemy->SetEnemyInventory();
		}
	}
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


void APlayerCharacter::SetIsRunTrue()
{
	Cast<UPlayerAnimInstance>(GetMesh()->GetAnimInstance())->SetIsRunTrue();
}

void APlayerCharacter::SetIsRunFalse()
{
	Cast<UPlayerAnimInstance>(GetMesh()->GetAnimInstance())->SetIsRunFalse();
}
