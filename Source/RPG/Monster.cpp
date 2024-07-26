// Fill out your copyright notice in the Description page of Project Settings.


#include "Monster.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "MonsterWidget.h"
#include "MyCharacter.h"
#include "AIController.h"
#include "Components/WidgetComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/DamageEvents.h"

// Sets default values
AMonster::AMonster()
{	
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.f);
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = true;
	bUseControllerRotationRoll = false;

	GetCharacterMovement()->MaxWalkSpeed = 500.f;
	GetCapsuleComponent()->SetCollisionProfileName(TEXT("Enemy"));
	GetMesh()->SetCollisionProfileName(TEXT("NoCollision"));

	//위젯 컴포넌트 생성 및 초기화
	MonsterWidgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("MonsterWidgetComponent"));
	MonsterWidgetComponent->SetupAttachment(GetMesh(), FName("HealthWidgetSocket")); // 헤드 소켓에 붙이기
	MonsterWidgetComponent->SetRelativeLocation(GetActorLocation());
	MonsterWidgetComponent->SetWidgetClass(MonsterWidgetClass); 

	//몬스터 공격 콜리전 검출
	MonsterAttackCollisionComponent = CreateDefaultSubobject<UCapsuleComponent>(TEXT("AttackCollisionComponent"));
	MonsterAttackCollisionComponent->SetupAttachment(GetMesh(), FName("AttackCollision"));
	MonsterAttackCollisionComponent->SetCollisionProfileName(TEXT("Enemy"));
	MonsterAttackCollisionComponent->OnComponentBeginOverlap.AddDynamic(this, &AMonster::OnOverlapBegin);
	
	//구조체
	MonsterStatus.MaxMonsterHP = 100.0f;
	MonsterStatus.CurrentMonsterHP = MonsterStatus.MaxMonsterHP;
	MonsterStatus.Damage = 10.f;
	MonsterStatus.DropMoney = 0.f;
}

void AMonster::ConsumeHPForAction(float HPCost)
{
	MonsterStatus.UseHP(HPCost);
	OnMonsterUIUpdated.Broadcast(MonsterStatus.CurrentMonsterHP);
	UE_LOG(LogTemp, Log, TEXT("Monster HP Updated: %f"), MonsterStatus.CurrentMonsterHP);
}

bool AMonster::bHasEnoughHP(float HPCost) const
{
	return MonsterStatus.CurrentMonsterHP > HPCost;
}

// Called when the game starts or when spawned
void AMonster::BeginPlay()
{
	Super::BeginPlay();

	if (MonsterWidgetComponent) {
		if (auto* HealthWidget = Cast<UMonsterWidget>(MonsterWidgetComponent->GetUserWidgetObject())) {
			//HealthWidget->UpdateHP(MonsterStatus.CurrentMonsterHP, MonsterStatus.MaxMonsterHP);
			UE_LOG(LogTemp, Log, TEXT("BeginPlay: HealthWidget Initialized with HP: %f / %f"), MonsterStatus.CurrentMonsterHP, MonsterStatus.MaxMonsterHP);
		
			HealthWidget->SetOwnerMonster(this);
		}
		else {
			UE_LOG(LogTemp, Warning, TEXT("BeginPlay: HealthWidget is nullptr"));
		}
	}
	else {
		UE_LOG(LogTemp, Warning, TEXT("BeginPlay: WidgetComponent is nullptr"));
	}
}

// Called every frame
void AMonster::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	WidgetFaceToPlayer();

}

// Called to bind functionality to input
void AMonster::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void AMonster::WidgetFaceToPlayer()
{
	if (MonsterWidgetComponent) {
		//현재 카메라 위치를 가져오기
		if (auto* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0)) {
			FVector CameraLocation;
			FRotator CameraRotation;
			PlayerController->GetPlayerViewPoint(CameraLocation, CameraRotation);

			// 위젯이 항상 카메라를 향하도록 회전 설정
			FVector DirectionToCamera = (CameraLocation - MonsterWidgetComponent->GetComponentLocation()).GetSafeNormal();
			FRotator LookAtRotation = DirectionToCamera.Rotation();
			MonsterWidgetComponent->SetWorldRotation(LookAtRotation);
		}
	}
}

void AMonster::MonsterAttack()
{
	if (UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance()) {
		if (MonsterAttackMontage) {
			GetCharacterMovement()->DisableMovement();
			AnimInstance->Montage_Play(MonsterAttackMontage);
			FOnMontageEnded MontageEndedDelegate;
			MontageEndedDelegate.BindUObject(this, &AMonster::OnAttackMontageEnd);
			AnimInstance->Montage_SetEndDelegate(MontageEndedDelegate, MonsterAttackMontage);
			GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Yellow, TEXT("MonsterAttackMontage"));
		}
	}
}

void AMonster::OnAttackMontageEnd(UAnimMontage* Montage, bool bInterrupted)
{
	if (Montage == MonsterAttackMontage) {
		GetCharacterMovement()->SetMovementMode(MOVE_Walking);
	}
}

void AMonster::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor && OtherActor != this && OtherActor->IsA(AMyCharacter::StaticClass())) {
		if (!OverlapActors.Contains(OtherActor)) {
			OverlapActors.Add(OtherActor);
			ApplyDamageToActor(OtherActor);
			UE_LOG(LogTemp, Log, TEXT("Monster attacked Player!"));
		}
	}
}

float AMonster::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	if (EventInstigator == nullptr)
		return -1.f;
	if (DamageCauser == nullptr)
		return -1.f;

	if (bHasEnoughHP(DamageAmount)) {
		//체력이 충분해서 데미지를 입을때
		ConsumeHPForAction(DamageAmount);
		UE_LOG(LogTemp, Log, TEXT("Monster Damaged, CurrentHP: %f"), MonsterStatus.CurrentMonsterHP);
	}
	else {
		//체력이 없어서 죽을때
		ConsumeHPForAction(DamageAmount);
		UE_LOG(LogTemp, Log, TEXT("Monster Die"));
	}

	return DamageAmount;
}

void AMonster::ApplyDamageToActor(AActor* ActorToDamage)
{
	float Damage = MonsterStatus.Damage;
	FDamageEvent DamageEvent;
	ActorToDamage->TakeDamage(Damage, DamageEvent, GetInstigatorController(), this);
}

UBehaviorTree* AMonster::GetBehaviorTree() const
{
	return BehaviorTree;
}

APatrolPath* AMonster::GetPatrolPath() const
{
	return PatrolPath;	
}

UAnimMontage* AMonster::GetMonsterAttackMontage() const
{
	return MonsterAttackMontage;
}

UCapsuleComponent* AMonster::GetAttackCollision() const
{
	return MonsterAttackCollisionComponent;
}

TArray<AActor*>& AMonster::GetOverlapActors()
{
	return OverlapActors;
}
