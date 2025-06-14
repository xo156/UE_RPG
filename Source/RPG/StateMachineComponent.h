// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "StateMachineComponent.generated.h"


UENUM(BlueprintType)
enum class EPlayerState : uint8
{
	PlayerIdle UMETA(DisplayName = "PlayerIdle"),
	PlayerWalk UMETA(DisplayName = "PlayerWalk"),
	PlayerRun UMETA(DisplayName = "PlayerRun"),
	PlayerJump UMETA(DisplayName = "PlayerJump"),
	PlayerAttack UMETA(DisplayName = "PlayerAttack"),
	PlayerHit UMETA(DisplayName = "PlayerHit"),
	PlayerDead UMETA(DisplayName = "PlayerDead"),
};

UENUM(BlueprintType)
enum class EMonsterState : uint8
{
	MonsterIdle UMETA(DisplayName = "MonsterIdle"),
	MonsterPatrol UMETA(DisplayName = "MonsterPatrol"),
	MonsterChase UMETA(DisplayName = "MonsterChase"),
	MonsterAttack UMETA(DisplayName = "MonsterAttack"),
	MonsterHit UMETA(DisplayName = "MonsterHit"),
	MonsterDead UMETA(DisplayName = "MonsterDead"),
};

UENUM(BlueprintType)
enum class ENPCState : uint8
{
	NPCIdle UMETA(DisplayName = "NPCIdle"),
	NPCWalk UMETA(DisplayName = "NPCWalk"),
	NPCRun UMETA(DisplayName = "NPCRun"),
	NPCJump UMETA(DisplayName = "NPCJump"),
	NPCAttack UMETA(DisplayName = "NPCAttack"),
	NPCHit UMETA(DisplayName = "NPCHit"),
	NPCDead UMETA(DisplayName = "NPCDead"),
	NPCDialogue UMETA(DisplayName = "NPCDialouge"),
	NPCShop UMETA(DisplayName = "NPCShop"),
};

UENUM(BlueprintType)
enum class EAnimalState : uint8
{
	AnimalIdle UMETA(DisplayName = "AnimalIdle"),
	AnimalWalk UMETA(DisplayName = "AnimalWalk"),
	AnimalRun UMETA(DisplayName = "AnimalRun"),
	AnimalJump UMETA(DisplayName = "AnimalJump"),
	AnimalAttack UMETA(DisplayName = "AnimalAttack"),
	AnimalHit UMETA(DisplayName = "AnimalHit"),
	AnimalDead UMETA(DisplayName = "AnimalDead"),
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class RPG_API UStateMachineComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UStateMachineComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	//개별 상태 함수
	EPlayerState GetPlayerState() const { return CurrentPlayerState; }
	void SetPlayerState(EPlayerState NewState);
	EMonsterState GetMonsterState() const { return CurrentMonsterState; }
	void SetMonsterState(EMonsterState NewState);
	ENPCState GetNPCState() const { return CurrentNPCState; }
	void SetNPCState(ENPCState NewState);
	EAnimalState GetAnimalState() const { return CurrentAnimalState; }
	void SetAnimalState(EAnimalState NewState);

private:
	EPlayerState CurrentPlayerState = EPlayerState::PlayerIdle;
	EMonsterState CurrentMonsterState = EMonsterState::MonsterIdle;
	ENPCState CurrentNPCState = ENPCState::NPCIdle;
	EAnimalState CurrentAnimalState = EAnimalState::AnimalIdle;


};
