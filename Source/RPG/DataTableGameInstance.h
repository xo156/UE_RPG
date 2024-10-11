// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "DataTableGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class RPG_API UDataTableGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	class UDataTable* GetItemTable();
	class UDataTable* GetDropItemTable();
	TSubclassOf<class UCameraShakeBase> GetCameraShake();
	TSubclassOf<class UCameraShakeBase> GetBossCameraShake();

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ItemData", meta = (AllowPrivateAccess = "true"))
	class UDataTable* ItemTable;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ItemData", meta = (AllowPrivateAccess = "true"))
	class UDataTable* DropItemTable;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CameraShake", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<class UCameraShakeBase> CameraShake;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CameraShake", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<class UCameraShakeBase> BossCameraShake;
};
