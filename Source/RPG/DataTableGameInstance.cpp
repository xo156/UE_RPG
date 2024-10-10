// Fill out your copyright notice in the Description page of Project Settings.


#include "DataTableGameInstance.h"
#include "Engine/DataTable.h"

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
