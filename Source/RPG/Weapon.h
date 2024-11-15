// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ItemBase.h"
#include "Weapon.generated.h"

UCLASS()
class RPG_API AWeapon : public AItemBase
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
	
	void SetOwnerCharacter(class AMyCharacter* NewOwnerCharacter);

	virtual void Use() override;

	class USkeletalMeshComponent* GetWeaponMesh() const;
	class UBoxComponent* GetWeaponCollision() const;
	TArray<AActor*>& GetOverlapActors();

	FTimerHandle DamageUPHandle;

	bool bGuardSuccess;

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon", meta = (AllowPrivateAccess = "true"))
	class USkeletalMeshComponent* WeaponMesh;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon", meta = (AllowPrivateAccess = "true"))
	class UBoxComponent* WeaponCollision;

	class AMyCharacter* OwnerCharacter;
	TArray<AActor*> OverlapActors;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<class UWeaponBaseComponent> ThisWeaponComponent;
};
