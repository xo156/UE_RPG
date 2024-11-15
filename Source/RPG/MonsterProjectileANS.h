// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "MonsterProjectileANS.generated.h"

/**
 * 
 */
UCLASS()
class RPG_API UMonsterProjectileANS : public UAnimNotifyState
{
	GENERATED_BODY()

public:
	virtual void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration) override;
	virtual void NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;

	void SpawnActorAt(USkeletalMeshComponent* MeshComp);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Projectile")
	TSubclassOf<class AMonsterProjectile> AMonsterProjectileClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Projectile")
	int32 MaxSpawnCount = 3;
	int32 SpawnCount = 0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Projectile")
	float SpawnInterval = 0.5;
	FTimerHandle SpawnTimerHandle;
};
