// Fill out your copyright notice in the Description page of Project Settings.


#include "DataTableGameInstance.h"
#include "Engine/DataTable.h"
#include "Camera/CameraShakeBase.h"

UDataTable* UDataTableGameInstance::GetItemTable()
{
	if (ItemTable)
		return ItemTable;
	return nullptr;
}

UDataTable* UDataTableGameInstance::GetDropItemTable()
{
	if (DropItemTable)
		return DropItemTable;
	return nullptr;
}

TSubclassOf<class UCameraShakeBase> UDataTableGameInstance::GetCameraShake()
{
	if (CameraShake)
		return CameraShake;
	return nullptr;
}

TSubclassOf<class UCameraShakeBase> UDataTableGameInstance::GetBossCameraShake()
{
	if (BossCameraShake)
		return BossCameraShake;
	return nullptr;
}
