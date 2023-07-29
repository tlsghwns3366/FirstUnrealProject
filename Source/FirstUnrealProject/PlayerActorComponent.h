// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CharacterStateComponent.h"
#include "PlayerActorComponent.generated.h"



DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnExpUpdated);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class FIRSTUNREALPROJECT_API UPlayerActorComponent : public UCharacterStateComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UPlayerActorComponent();
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Info)
		float Exp;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Info)
		float MaxExp;

	UPROPERTY(BlueprintAssignable)
		FOnExpUpdated OnExpUpdated;

protected:
	// Called when the game starts
	virtual void InitializeComponent() override;// BeginPlay전에 호출
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

		
};
