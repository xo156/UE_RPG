// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MonsterProjectile.generated.h"

UCLASS()
class RPG_API AMonsterProjectile : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMonsterProjectile();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void GoToPlayer();
	void DestroyProjectile();

	UFUNCTION()
	void OnBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Projectile")
	class USphereComponent* SphereComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Projectile")
	UStaticMeshComponent* ProjectileMesh;

	FTimerHandle ProjectileTimerHandle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Projectile")
	float SpeedIncreaseRate = 0.1f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Projectile")
	float InitialSpeed = 600.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Projectile")
	float TargetSpeed = InitialSpeed * 3.f;
	
	float ProjectileSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Projectile")
	float ProjectileLifeTime = 5.f;

	float DamageAmount = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Projectile")
	TSubclassOf<class AMonster> MonsterActor;
	AController* InstigatorController;
};
