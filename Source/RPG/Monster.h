// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "DropRate.h"
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
	TArray<FDropRate> DropRates;

	float UseHP(float HPCost) {
		if (HPCost >= 0) {
			CurrentMonsterHP = FMath::Max(CurrentMonsterHP - HPCost, 0.0f);
		}
		else {
			CurrentMonsterHP = FMath::Min(CurrentMonsterHP - HPCost, MaxMonsterHP);
		}
		UE_LOG(LogTemp, Warning, TEXT("CurrentMonsterHP: %f"), CurrentMonsterHP);
		return CurrentMonsterHP;
	}
};
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

	void MonsterAttackStart();
	virtual void MonsterAttackExecute();
	void MonsterAttackEnd();
	void OnAttackMontageEnded(UAnimMontage* Montage, bool bInterrupted);
	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, 
						 UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, 
						 bool bFromSweep, const FHitResult& SweepResult);
	virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;
	void DroppedItem();
	void DestroyMonster();
	void OnDieMontageEnded(UAnimMontage* NowPlayMontage, bool bInterrupted);
	void ApplyDamageToActor(AActor* ActorToDamage);

	//getter
	class UBehaviorTree* GetBehaviorTree() const;
	class APatrolPath* GetPatrolPath() const;
	class UAnimMontage* GetMonsterAttackMontage() const;
	TArray<AActor*>& GetOverlapActors();
	class UWidgetComponent* GetMonsterWidgetComponent() const;
	TSubclassOf<class UMonsterWidget> GetMonsterWidgetClass() const;
	UCapsuleComponent* GetAttackCollisionComponent(FName AttackCollisionFName) const;

	//setter
	void SetMonsterAttackCollision(class UCapsuleComponent* AttackCollision);

	//델리게이트
	FOnMonsterUIUpdated OnMonsterUIUpdated;

	//구조체
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Status")
	FMonsterStatus MonsterStatus;
	virtual void ConsumeHPForAction(float HPCost);
	bool bHasEnoughHP(float HPCost) const;

	//상태
	bool bIsMonsterAttack = false;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	//AI
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI", meta = (AllowPrivateAccess = "true"))
	class UBehaviorTree* BehaviorTree;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI", meta = (AllowPrivateAccess = "true"))
	class APatrolPath* PatrolPath;

	//공격
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack", meta = (AllowPrivateAccess = "true"))
	class UCapsuleComponent* MonsterAttackCollisionComponent0;
	TArray<class UCapsuleComponent*> MonsterAttackCollisionComponents;
	TArray<AActor*> OverlapActors; //몬스터가 때릴 때 콜리전 검출하기
	float Damage = 0.f;

	//몽타주
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Montage", meta = (AllowPrivateAccess = "true"))
	class UAnimMontage* MonsterAttackMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Montage", meta = (AllowPrivateAccess = "true"))
	class UAnimMontage* MonsterDieMontage;

	//위젯
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<class UMonsterWidget> MonsterWidgetClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI", meta = (AllowPrivateAccess = "true"))
	class UWidgetComponent* MonsterWidgetComponent;

	//아이템 드랍
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item", meta = (AllowPrivateAccess="true"))
	TSubclassOf<class ADropItem> DropItemClass;
	UDataTable* ItemDropTable;

	//흔들림
	TSubclassOf<class UCameraShakeBase> CameraShake;
};
