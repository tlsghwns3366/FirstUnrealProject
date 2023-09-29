// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemActor.h"
#include "ItemObject.h"
#include "InventoryComponent.h"
#include "Components/WidgetComponent.h"

// Sets default values
AItemActor::AItemActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	static ConstructorHelpers::FObjectFinder<UMaterialInstance> FoundMaterial(TEXT("/Script/Engine.MaterialInstanceConstant'/Game/SMT/HighLight/M_HighLightM_Inst.M_HighLightM_Inst'"));
	if (FoundMaterial.Succeeded())
	{
		StaticMesh->SetOverlayMaterial(FoundMaterial.Object);
	}
	static ConstructorHelpers::FClassFinder<UUserWidget>BlueprintWidget(TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/UI/WBP_PickUpIcon.WBP_PickUpIcon_C'"));
	if (BlueprintWidget.Succeeded())
	{
		Widget = BlueprintWidget.Class;
	}
	PickUpWidget = CreateWidget<UUserWidget>(GetWorld(), Widget);
	WidgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("WidgetComponent"));
	WidgetComponent->SetWidget(PickUpWidget);
	WidgetComponent->SetWidgetSpace(EWidgetSpace::Screen);
	WidgetComponent->SetupAttachment(StaticMesh);
	WidgetComponent->SetVisibility(false);
}

// Called when the game starts or when spawned
void AItemActor::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AItemActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

bool AItemActor::AddInventory(UInventoryComponent* Inventory)
{
	if (Inventory->AddItem(Item))
	{
		Destroy();
		return true;
	}
	else
		return false;
}

void AItemActor::Iteminitialization(UItemObject* _Item)
{
	if (_Item != nullptr)
	{
		Item = _Item;
		StaticMesh->SetStaticMesh(Item->StaticMesh);
		StaticMesh->SetSimulatePhysics(true);
		StaticMesh->SetCollisionObjectType(ECollisionChannel::ECC_GameTraceChannel1);
	}
}

void AItemActor::OnInteract_Implementation(AActor* Caller)
{
}

void AItemActor::StartFocus_Implementation()
{
	WidgetComponent->SetVisibility(true);
}


void AItemActor::EndFocus_Implementation()
{
	WidgetComponent->SetVisibility(false);
}
