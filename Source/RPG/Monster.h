// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Monster.generated.h"

USTRUCT(BlueprintType)
struct FMonsterStatus
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Status")
	float CurrentMonsterHP;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Status")
	float MaxMonsterHP;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Status")
	float Damage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Status")
	float DropMoney;

	float UseHP(float HPCost) {
		if (HPCost >= 0) {
			CurrentMonsterHP = FMath::Max(CurrentMonsterHP - HPCost, 0.0f);
		}
		UE_LOG(LogTemp, Warning, TEXT("CurrentMonsterHP: %f"), CurrentMonsterHP);
		return CurrentMonsterHP;
	}
};
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnEnemyDieEvent, float, DropMoney);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnMonsterUIUpdated, float, NewHP);

UCLASS()
class RPG_API AMonster : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AMonster();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void WidgetFaceToPlayer();

	void MonsterAttack();
	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, 
						 UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, 
						 bool bFromSweep, const FHitResult& SweepResult);
	virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

	void ApplyDamageToActor(AActor* ActorToDamage);

	class UBehaviorTree* GetBehaviorTree() const;
	class APatrolPath* GetPatrolPath() const;
	class UAnimMontage* GetMonsterAttackMontage();

	//델리게이트
	FOnEnemyDieEvent OnEventDieEvent;
	FOnMonsterUIUpdated OnMonsterUIUpdated;

	//구조체
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Status")
	FMonsterStatus MonsterStatus;
	void ConsumeHPForAction(float HPCost);
	bool bHasEnoughHP(float HPCost) const;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI", meta = (AllowPrivateAccess = "true"))
	class UBehaviorTree* BehaviorTree;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI", meta = (AllowPrivateAccess = "true"))
	class APatrolPath* PatrolPath;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Collision", meta = (AllowPrivateAccess = "true"))
	class UCapsuleComponent* MonsterAttackCollisionComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Montage", meta = (AllowPrivateAccess = "true"))
	class UAnimMontage* MonsterAttackMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Montage", meta = (AllowPrivateAccess = "true"))
	class UAnimMontage* MonsterHitMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Montage", meta = (AllowPrivateAccess = "true"))
	class UAnimMontage* MonsterDieMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<class UMonsterWidget> MonsterWidgetClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI", meta = (AllowPrivateAccess = "true"))
	class UWidgetComponent* MonsterWidgetComponent;

};
