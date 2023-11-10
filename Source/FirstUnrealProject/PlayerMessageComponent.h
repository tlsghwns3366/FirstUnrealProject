// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "NpcMessageComponent.h"
#include "PlayerMessageComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FMessageUpdated);

USTRUCT(BlueprintType)
struct FNpcMessage
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		FName NpcName;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		FString NpcMessage;
};
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class FIRSTUNREALPROJECT_API UPlayerMessageComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UPlayerMessageComponent();

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TArray<struct FNpcMessage> NpcMessageArray;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TArray<FString> ClearQuest;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TArray<FString> CurrentActiveQuest;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TArray<struct FQuestData> CurrentQuest;

	UPROPERTY(BlueprintAssignable)
		FMessageUpdated MessageUpdated;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

public:
	UFUNCTION(BlueprintCallable)
	bool AddMessage(struct FNpcMessage Message);
	void RemoveMessage();

	bool AddQuest(FString String);
	bool CompleteQuest(FString String);

	bool FindQuest(FString String);

};
