// Fill out your copyright notice in the Description page of Project Settings.


#include "NpcCharacter.h"
#include "Components/WidgetComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "InteractionUserWidget.h"

ANpcCharacter::ANpcCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	static ConstructorHelpers::FObjectFinder< USkeletalMesh> SkeletalMesh(TEXT("/Script/Engine.SkeletalMesh'/Game/ThirdPerson/Characters/Mannequins/Meshes/SKM_Quinn.SKM_Quinn'"));
	if (SkeletalMesh.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(SkeletalMesh.Object);
		GetMesh()->SetRelativeLocationAndRotation(FVector(0.f, 0.f, -88.0f), FRotator(0.f, -90.f, 0.f));
	}
	static ConstructorHelpers::FClassFinder<UInteractionUserWidget>BlueprintWidget(TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/UI/WBP_InteractionE.WBP_InteractionE_C'"));
	if (BlueprintWidget.Succeeded())
	{
		Widget = BlueprintWidget.Class;
	}
	InteractionWidget = CreateWidget<UInteractionUserWidget>(GetWorld(), Widget);

	WidgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("WidgetComponent"));
	WidgetComponent->SetupAttachment(GetMesh());
	WidgetComponent->SetWidgetSpace(EWidgetSpace::Screen);

	NpcWidgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("NpcWidgetComponent"));
	NpcWidgetComponent->SetupAttachment(GetMesh());
	NpcWidgetComponent->SetWidgetSpace(EWidgetSpace::World);
	NpcWidgetComponent->SetRelativeLocation(FVector(0.f, 0.f, 200.f));

	InteractionWidgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("InteractionWidgetComponent"));
	InteractionWidgetComponent->SetWidget(InteractionWidget);
	InteractionWidgetComponent->SetWidgetSpace(EWidgetSpace::Screen);
	InteractionWidgetComponent->SetupAttachment(GetMesh());
	InteractionWidgetComponent->SetVisibility(false);
}

void ANpcCharacter::BeginPlay()
{
	Super::BeginPlay();
}

void ANpcCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	FVector WidgetLocation = NpcWidgetComponent->GetComponentLocation();
	FVector TargetLocation = UGameplayStatics::GetPlayerCameraManager(GetWorld(), 0)->GetCameraLocation();
	NpcWidgetComponent->SetWorldRotation(UKismetMathLibrary::FindLookAtRotation(WidgetLocation, TargetLocation));
}

float ANpcCharacter::TakeDamage(float Damage, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	Super::TakeDamage(Damage, DamageEvent, EventInstigator, DamageCauser);
	return Damage;
}

void ANpcCharacter::OnInteract_Implementation(AActor* Caller)
{

}

void ANpcCharacter::StartFocus_Implementation()
{
	InteractionWidgetComponent->SetVisibility(true);
}

void ANpcCharacter::EndFocus_Implementation()
{
	InteractionWidgetComponent->SetVisibility(false);
}
