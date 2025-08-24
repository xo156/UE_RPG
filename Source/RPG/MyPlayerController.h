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
	UI UMETA(DisplayName = "UI"),
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

	//빙의하는 타이밍에(캐릭터에 컨트롤러가 들어갈 때)
	virtual void OnPossess(APawn* InPawn) override;

	//인풋
	virtual void SetupInputComponent() override;

	//IMC 전환
	void ChangeInputMappingContext(EIMCState NewIMC);

public:
	//Normal 액션
	void TryMove(const FInputActionValue& Value);
	void TryRunStart();
	void TryRunEnd();
	void TryRoll();
	void TryJump();
	void TryLook(const FInputActionValue& Value);
	void TryLightAttackStart(const FInputActionValue& Value);
	void TryHeavyAttakStart(const FInputActionValue& Value);
	void TryGuard();
	void TryLockOnTarget();
	void TryInteract();
	void TryOpenInventory();
	void TryUseQuickSlot();
	void TrySelectQuickSlot();
	void TryOpenMainMenu();
	void TryOpenEquip();

	//인벤토리 액션
	void TryConfirm();
	void TryNavigate(const FInputActionValue& Value);
	void TryCloseInventory();

private:
	TObjectPtr<class UEnhancedInputLocalPlayerSubsystem> InputSubsystem;

	//Normal 인풋 액션
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
	class UInputAction* EquipAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
	class UInputAction* CloseAction;

	//UI 인풋 액션
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
	class UInputMappingContext* InventoryMappingContext;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
	class UInputAction* ConfirmAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
	class UInputAction* NavigateAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
	class UInputAction* CloseInventoryAction;

	class AMyCharacter* MyCharacter;
	EIMCState CurrentIMC;
};
