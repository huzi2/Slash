// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Characters/BaseCharacter.h"
#include "Characters/CharacterTypes.h"
#include "Enemy.generated.h"

class UHealthBarComponent;
class AAIController;
class UPawnSensingComponent;

UCLASS()
class SLASH_API AEnemy : public ABaseCharacter
{
	GENERATED_BODY()

private:
	AEnemy();

public:
	virtual void GetHit_Implementation(const FVector& ImpactPoint) override;

private:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;
	virtual void Destroyed() override;

private:
	virtual void Die() override;

private:
	UFUNCTION()
	void PawnSee(APawn* SeenPawn);

private:
	void CheckCombatTarget();
	void CheckPatrolTarget();
	const bool InTargetRange(AActor* Target, double Radius) const;
	void MoveToTarget(AActor* Target);
	AActor* ChoosePatrolTarget() const;
	void PatrolTimerFinished();

protected:
	UPROPERTY(BlueprintReadOnly)
	EDeathPose DeathPose;

private:
	UPROPERTY(VisibleAnywhere)
	UHealthBarComponent* HealthBarWidget;

	UPROPERTY(VisibleAnywhere)
	UPawnSensingComponent* PawnSensing;

	UPROPERTY(EditAnywhere)
	TSubclassOf<AWeapon> WeaponClass;

	UPROPERTY()
	AActor* CombatTarget;
	
	UPROPERTY(EditAnywhere)
	double CombatRadius;

	UPROPERTY(EditAnywhere)
	double AttackRadius;

	UPROPERTY(EditAnywhere)
	double PatrolRadius;

	UPROPERTY()
	AAIController* EnemyController;

	UPROPERTY(EditInstanceOnly, Category = "AI Navigation")
	AActor* PatrolTarget;

	UPROPERTY(EditInstanceOnly, Category = "AI Navigation")
	TArray<AActor*> PatrolTargets;

	UPROPERTY(EditAnywhere, Category = "AI Navigation")
	float WaitMin;

	UPROPERTY(EditAnywhere, Category = "AI Navigation")
	float WaitMax;

private:
	FTimerHandle PatrolTimer;
	EEnemyState EnemyState;
};
