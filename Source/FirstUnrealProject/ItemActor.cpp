// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemActor.h"
#include "ItemObject.h"
#include "InventoryComponent.h"
#include "Components/WidgetComponent.h"
#include "InteractionUserWidget.h"

// Sets default values
AItemActor::AItemActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	static ConstructorHelpers::FObjectFinder<UMaterialInstance> FoundMaterial(TEXT("/Script/Engine.MaterialInstanceConstant'/Game/SMT/HighLight/M_HighLightM_Inst.M_HighLightM_Inst'"));
	if (FoundMaterial.Succeeded())
	{
		OverlayMaterialInstance = UMaterialInstanceDynamic::Create(FoundMaterial.Object, nullptr); 
	}
	static ConstructorHelpers::FClassFinder<UInteractionUserWidget>BlueprintWidget(TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/UI/WBP_InteractionE.WBP_InteractionE_C'"));
	if (BlueprintWidget.Succeeded())
	{
		Widget = BlueprintWidget.Class;
	}
	InteractionWidget = CreateWidget<UInteractionUserWidget>(GetWorld(), Widget);
	WidgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("WidgetComponent"));
	WidgetComponent->SetWidget(InteractionWidget);
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
		if (InteractionWidget != nullptr)
		{
			InteractionWidget->SetWidget(Item);
		}
		OverlayMaterialInstance->SetVectorParameterValue(TEXT("Color"), GetItemColor());
		StaticMesh->SetOverlayMaterial(OverlayMaterialInstance);
	}
}

FLinearColor AItemActor::GetItemColor()
{
	FLinearColor Color;
	switch (Item->ItemRank)
	{
	case EItemRank::E_Rank_Normal:
		Color = FLinearColor(FColor(255, 255, 255, 255));
		break;
	case EItemRank::E_Rank_Magic:
		Color = FLinearColor(FColor(50, 205, 50, 255));
		break;
	case EItemRank::E_Rank_Rare:
		Color = FLinearColor(FColor(30, 144, 255, 255));
		break;
	case EItemRank::E_Rank_Epic:
		Color = FLinearColor(FColor(128, 00, 128, 255));
		break;
	case EItemRank::E_Rank_Unique:
		Color = FLinearColor(FColor(255, 215, 0, 255));
		break;
	case EItemRank::E_Rank_Legendary:
		Color = FLinearColor(FColor(255, 69, 0, 255));
		break;
	case EItemRank::E_Rank_Mythic:
		Color = FLinearColor(FColor(128, 0, 0, 255));
		break;
	}
	return Color;
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
