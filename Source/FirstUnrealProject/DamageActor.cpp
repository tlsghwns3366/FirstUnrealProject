// Fill out your copyright notice in the Description page of Project Settings.


#include "DamageActor.h"
#include "Components/WidgetComponent.h"
#include "DamageTextWidget.h"
// Sets default values
ADamageActor::ADamageActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	static ConstructorHelpers::FClassFinder<UDamageTextWidget>BlueprintWidget(TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/UI/WBP_DamageText.WBP_DamageText_C'"));
	if (BlueprintWidget.Succeeded())
	{
		Widget = BlueprintWidget.Class;
	}
	DamageWidget = CreateWidget<UDamageTextWidget>(GetWorld(), Widget);
	WidgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("WidgetComponent"));


	WidgetComponent->SetWidget(DamageWidget);
	WidgetComponent->SetWidgetSpace(EWidgetSpace::Screen);
	WidgetComponent->SetDrawSize(FVector2D(200.f, 50.f));

}

// Called when the game starts or when spawned
void ADamageActor::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ADamageActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ADamageActor::SetDamage(float Damage)
{
	if (IDamageInterface* DamageInterface = Cast<IDamageInterface>(DamageWidget))
	{
		DamageInterface->SetDamageValue(Damage);
	}
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, [&]() {
		Destroy();
		}, 3.f, false);
}

void ADamageActor::SetColor(FColor Color)
{
	DamageWidget->SetTextColor(Color);
}

void ADamageActor::SetRandomPosition()
{
	DamageWidget->SetRandomPosition();
}
