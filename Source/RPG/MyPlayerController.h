// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "InputActionValue.h"
#include "MyPlayerController.generated.h"

/**
 * 
 */

UCLASS()
class RPG_API AMyPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	AMyPlayerController();

	//getter
	class AMyCharacter* GetCharacter();
	class UResourceComponent* GetResourceComponent();
	class UStateMachineComponent* GetStateMachineComponent();

	//UI
	void ShowTooltipAtMousePosition(class UInventoryTooltip* TooltipWidget);
	void HideTooltip();
	void ShotItemActionMousePosition(class UInventoryItemAction* ItemActionWidget);
	void HideItemAction();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	//�����ϴ� Ÿ�ֿ̹�(ĳ���Ϳ� ��Ʈ�ѷ��� �� ��)
	virtual void OnPossess(APawn* InPawn) override;

	//��ǲ
	virtual void SetupInputComponent() override;

	//�׼�
	void TryMove(const FInputActionValue& Value);
	void TryRunStart();
	void TryRunEnd();
	void TryJump();
	void TryLook(const FInputActionValue& Value);
	void TryAttackStart(const FInputActionValue& Value);
	void TryGuardUp();
	void TryGuardDown();
	void TryRoll();
	void TryLockOnTarget();
	void TryRootItem();
	void TryOpenInventory();
	void TryQuickSlot();
	void TryInteract();
	void TryClose();

private:
	//��ǲ �׼�
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
	class UInputMappingContext* DefaultMappingContext;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
	class UInputAction* MoveAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
	class UInputAction* RunAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
	class UInputAction* JumpAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
	class UInputAction* LightAttackAction;
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
	class UInputAction* InteractAction;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI", meta = (AllowPrivateAccess = "true"))
	class UInventoryTooltip* CurrentTooltip;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI", meta = (AllowPrivateAccess = "true"))
	class UInventoryItemAction* InventoryItemAction;

	class AMyCharacter* MyCharacter;
};
