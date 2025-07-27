// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "InputActionValue.h"
#include "MyPlayerController.generated.h"

/**
 * 
 */

UENUM(BlueprintType)
enum class EIMCState : uint8
{
	Normal UMETA(DisplayName = "Normal"),
	Inventory UMETA(DisplayName = "Inventory"),
	Dialogue UMETA(DisplayName = "Dialogue"),
};

UCLASS()
class RPG_API AMyPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	AMyPlayerController();

	//getter
	class AMyCharacter* GetCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	//�����ϴ� Ÿ�ֿ̹�(ĳ���Ϳ� ��Ʈ�ѷ��� �� ��)
	virtual void OnPossess(APawn* InPawn) override;

	//��ǲ
	virtual void SetupInputComponent() override;

	//IMC ��ȯ
	void ChangeInputMappingContext(EIMCState NewIMC);

	//Normal �׼�
	void TryMove(const FInputActionValue& Value);
	void TryRunStart();
	void TryRunEnd();
	void TryRoll();
	void TryJump();
	void TryLook(const FInputActionValue& Value);
	void TryLightAttackStart(const FInputActionValue& Value);
	void TryHeavyAttakStart(const FInputActionValue& Value);
	void TryLockOnTarget();
	void TryRootItem();
	void TryOpenInventory();
	void TryQuickSlot();
	void TryClose();

public:
	//�κ��丮 �׼�
	void TryConfirm();
	void TryNavigate();
	void TryCloseInventory();

private:
	TObjectPtr<class UEnhancedInputLocalPlayerSubsystem> InputSubsystem;

	//Normal ��ǲ �׼�
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
	class UInputMappingContext* NormalMappingContext;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
	class UInputAction* MoveAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
	class UInputAction* RunAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
	class UInputAction* JumpAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
	class UInputAction* LightAttackAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
	class UInputAction* HeavyAttackAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
	class UInputAction* LookAction;
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

	//Inventory ��ǲ �׼�
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
	class UInputMappingContext* InventoryMappingContext;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
	class UInputAction* ConfirmAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
	class UInputAction* NavigateAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
	class UInputAction* CloseInventoryAction;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI", meta = (AllowPrivateAccess = "true"))
	class UInventoryTooltip* CurrentTooltip;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI", meta = (AllowPrivateAccess = "true"))
	class UInventoryItemAction* InventoryItemAction;

	class AMyCharacter* MyCharacter;
	EIMCState CurrentIMC;
};
