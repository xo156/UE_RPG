// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EquipableItem.h"
#include "Weapon.generated.h"

UCLASS()
class RPG_API AWeapon : public AEquipableItem
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

	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
						UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex,
						bool bFromSweep, const FHitResult& SweepResult);

	void ApplyDamageToActor(AActor* ActorToDamage);

	class USkeletalMeshComponent* GetWeaponMesh() const;
	class UBoxComponent* GetWeaponCollision() const;
	FTransform GetHandOffsetTransform(EEquipSlotType Slot) const;
	TArray<AActor*>& GetOverlapActors();
	class UAnimMontage* GetLightAttackMontage() const;
	class UAnimMontage* GetHeavyAttackMontage() const;

	//데미지용 델리게이트
	FTimerHandle DamageUPHandle;

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon", meta = (AllowPrivateAccess = "true"))
	class USkeletalMeshComponent* WeaponMesh;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon", meta = (AllowPrivateAccess = "true"))
	class UBoxComponent* WeaponCollision;

	// 오른손 오프셋
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Offset", meta = (AllowPrivateAccess = "true"))
	FVector RightHandLocationOffset = FVector::ZeroVector;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Offset", meta = (AllowPrivateAccess = "true"))
	FRotator RightHandRotationOffset = FRotator::ZeroRotator;

	// 왼손 오프셋
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Offset", meta = (AllowPrivateAccess = "true"))
	FVector LeftHandLocationOffset = FVector::ZeroVector;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Offset", meta = (AllowPrivateAccess = "true"))
	FRotator LeftHandRotationOffset = FRotator::ZeroRotator;

	//변수
	float AttackPower;
	float DefensePower;

	//공격
	class AMyCharacter* OwnerCharacter;
	TArray<AActor*> OverlapActors;

	//몽타주
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Montage", meta = (AllowPrivateAccess = "true"))
	class UAnimMontage* LightAttackMontage;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Montage", meta = (AllowPrivateAccess = "true"))
	class UAnimMontage* HeavyAttackMontage;
};
