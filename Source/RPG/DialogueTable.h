// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "DialogueTable.generated.h"

/**
 * 
 */
USTRUCT(BlueprintType)
struct RPG_API FDialogueTable : public FTableRowBase
{
    GENERATED_USTRUCT_BODY()

public:
    //��ȭ ID
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 TableID;

    //��ȭ �޽���
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString TableContent;
};
