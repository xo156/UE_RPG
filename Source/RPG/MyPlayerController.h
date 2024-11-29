// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "InputActionValue.h"
#include "MyPlayerController.generated.h"

/**
 * 
 */
class AMyCharacter;

UCLASS()
class RPG_API AMyPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	AMyPlayerController();

	//getter
	AMyCharacter* GetCharacter();

	//UI
	void ShowTooltipAtMousePosition(class UInventoryTooltip* TooltipWidget);
	void HideTooltip();
	void ShotItemActionMousePosition(class UInventoryItemAction* ItemActionWidget);
	void HideItemAction();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	//빙의하는 타이밍에(캐릭터에 컨트롤러가 들어갈 때)
	virtual void OnPossess(APawn* InPawn) override;

	//인풋
	virtual void SetupInputComponent() override;

	//액션
	void Move(const FInputActionValue& Value);
	void RunStart();
	void RunEnd();
	void Jump();
	void Look(const FInputActionValue& Value);
	void AttackStart(const FInputActionValue& Value);
	void GuardUp();
	void GuardDown();
	void Roll();
	void LockOnTarget();
	void RootItem();
	void OpenInventory();
	void QuickSlot();
	void TalkNPC();
	void Close();
	void TEST();

private:
	//인풋 액션
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
	class UInputMappingContext* DefaultMappingContext;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
	class UInputAction* MoveAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
	class UInputAction* RunAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
	class UInputAction* JumpAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
	class UInputAction* AttackAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
	class UInputAction* LookAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
	class UInputAction* GuardAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
	class UInputAction* RollAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
	class UInputAction* LockOnAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
	class UInputAction* RootItemAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
	class UInputAction* InventoryAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
	class UInputAction* QuickSlotAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
	class UInputAction* CloseAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
	class UInputAction* TESTSTATUSAction;

	class AMyCharacter* MyCharacter;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI", meta = (AllowPrivateAccess = "true"))
	class UInventoryTooltip* CurrentTooltip;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI", meta = (AllowPrivateAccess = "true"))
	class UInventoryItemAction* InventoryItemAction;

};
