// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "DialogueTable.h"
#include "DialogueComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class RPG_API UDialogueComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UDialogueComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void CreateDialogueWidget();

	//void LoadDialogues(UDataTable* InDialogueTable);

	void NextDialogue();

	void SetDialogueCache(UDataTable* InDialogueTable);

	class UDialogueWidget* GetDialogueWidgetInstance();

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dialogue", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<UUserWidget> DialogueWidgetClass;
	class UDialogueWidget* DialogueWidgetInstance;

	UDataTable* NowDialogueTable;

	TMap<int32, FDialogueTable*> DialogueCache;
	int32 CurrentIndex = 0;
};
