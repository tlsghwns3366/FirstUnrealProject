// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CustomCharacter.h"
#include "InteractionInterface.h"
#include "NpcCharacter.generated.h"

/**
 * 
 */
UCLASS()
class FIRSTUNREALPROJECT_API ANpcCharacter : public ACustomCharacter, public IInteractionInterface
{
	GENERATED_BODY()
public:
	ANpcCharacter();
public:

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		class UWidgetComponent* WidgetComponent;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		class UWidgetComponent* NpcWidgetComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class UWidgetComponent* InteractionWidgetComponent;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSubclassOf<class UUserWidget> Widget;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class UInteractionUserWidget* InteractionWidget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class UNpcMessageComponent* NpcMessageComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class APlayerCharacter* Player;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		FName NpcId;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		int32 Index;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		int32 QuestNum;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		bool IsTalk;

	FTimerHandle TimerHandle;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	virtual float TakeDamage(float Damage, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

	void NpcTalk();
	void QuestTalk();


	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Interaction")
		void OnInteract(AActor* Caller);
	virtual void OnInteract_Implementation(AActor* Caller);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Interaction")
		void StartFocus();
	virtual void StartFocus_Implementation();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Interaction")
		void EndFocus();
	virtual void EndFocus_Implementation();
};
