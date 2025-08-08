// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MainMenuWidget.generated.h"

/**
 * 
 */
UCLASS()
class RPG_API UMainMenuWidget : public UUserWidget
{
	GENERATED_BODY()
	

public:
	virtual void NativeConstruct() override;

	void OpenInventoryWidget();
	void OpenEquipWidget();
	void OpenSettingWidget();
	void OpenQuitGameWidget();

protected:
	//인벤토리
	UPROPERTY(meta = (BindWidget))
	class UButton* InventoryButton;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* InventoryText;

	//장비
	UPROPERTY(meta = (BindWidget))
	class UButton* EquipButton;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* EquipText;

	//설정
	UPROPERTY(meta = (BindWidget))
	class UButton* SettingButton;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* SettingText;

	//종료
	UPROPERTY(meta = (BindWidget))
	class UButton* QuitGameButton;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* QuitGameText;
};
