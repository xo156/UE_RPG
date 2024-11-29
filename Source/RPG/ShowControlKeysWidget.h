// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ShowControlKeysWidget.generated.h"

/**
 * 
 */
UCLASS()
class RPG_API UShowControlKeysWidget : public UUserWidget
{
	GENERATED_BODY()


protected:
	virtual void NativeConstruct() override;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* DialogueTextAttack;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* DialogueTextGuard;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* DialogueTextMove;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* DialogueTextRun;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* DialogueTextRoll;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* DialogueTextJump;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* DialogueTextClose;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* DialogueTextRootItem;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* DialogueTextLockOn;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* DialogueTextLook;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* DialogueTextTalkNPC;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* DialogueTextInventory;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* DialogueTextQuickSlot;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* DialogueTextShowControlKeys;
};
