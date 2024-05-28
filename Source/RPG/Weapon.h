// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Weapon.generated.h"

UCLASS()
class RPG_API AWeapon : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWeapon();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

//변수들
public:
	//https://forums.unrealengine.com/t/setting-skeletal-mesh-through-c/31585
	//이거 보고 잘 생각 프로젝트를 기준으로 하는거라 경로는 상관 없음

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	class USkeletalMeshComponent* WeaponMesh;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	class UBoxComponent* WeaponCollision;

	class AMyCharacter* OwnerCharacter;

	//검출된 대상의 정보를 델리게이트로 전달 -> 캐릭터한테
};
