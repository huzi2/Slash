// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/BaseCharacter.h"
#include "Items/Weapons/Weapon.h"
#include "Components/BoxComponent.h"
#include "Components/AttributeComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Components/CapsuleComponent.h"

ABaseCharacter::ABaseCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	Attributes = CreateDefaultSubobject<UAttributeComponent>(TEXT("Attributes"));
}

void ABaseCharacter::BeginPlay()
{
	Super::BeginPlay();
}

void ABaseCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ABaseCharacter::AttackEnd()
{
}

const bool ABaseCharacter::CanAttack() const
{
	return false;
}

void ABaseCharacter::Attack()
{
}

void ABaseCharacter::Die()
{
}

void ABaseCharacter::HandleDamage(const float DamageAmount)
{
	if (Attributes)
	{
		Attributes->ReceiveDamage(DamageAmount);
	}
}

const int32 ABaseCharacter::PlayAttackMontage()
{
	return PlayRandomMontageSection(AttackMontage, AttackMontageSections);
}

const int32 ABaseCharacter::PlayDeathMontage()
{
	return PlayRandomMontageSection(DeathMontage, DeathMontageSections);
}

void ABaseCharacter::SetWeaponCollisionEnabled(const ECollisionEnabled::Type CollisionEnabled)
{
	if (EquippedWeapon && EquippedWeapon->GetWeaponBox())
	{
		EquippedWeapon->GetWeaponBox()->SetCollisionEnabled(CollisionEnabled);
		EquippedWeapon->IgnoreActors.Empty();
	}
}

const bool ABaseCharacter::IsAlive() const
{
	return Attributes && Attributes->IsAlive();
}

void ABaseCharacter::PlayHitReactMontage(const FName& SectionName)
{
	if (!HitReactMontage) return;

	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance)
	{
		AnimInstance->Montage_Play(HitReactMontage);
		AnimInstance->Montage_JumpToSection(SectionName, HitReactMontage);
	}
}

void ABaseCharacter::DirectionalHitReact(const FVector& ImpactPoint)
{
	// ������ ������ ��������� Ȯ��. ���溤�Ϳ� ����->�������� ���ͳ��� ����
	const FVector Forward = GetActorForwardVector();
	const FVector ImpactLowered(ImpactPoint.X, ImpactPoint.Y, GetActorLocation().Z);
	const FVector ToHit = (ImpactLowered - GetActorLocation()).GetSafeNormal();

	// �������� Acos�ϸ� ������ ���´�(|A|*|B| = cos(theta))
	const double CosTheta = FVector::DotProduct(Forward, ToHit);
	double Theta = FMath::Acos(CosTheta);
	Theta = FMath::RadiansToDegrees(Theta);

	// ������ �� ����δ� ����� ���ͼ� ������ �˾Ƶ� �������� ���������� �˼��� ����. �׷��� ������
	// �𸮾������� ������ �޼չ�Ģ�� ������ ������ ������ ������ ������ ������.
	// �׷��� ���� ��ġ(Z��)�� ������ �Ʒ�(0���� ����)���� Ȯ���ؼ� �������� ���������� Ȯ��
	// �޼����� ������ ���溤��, ������ ��Ʈ����Ʈ�� �����ϸ� ������ �������϶� ������ ���ΰ���
	const FVector CrossProduct = FVector::CrossProduct(Forward, ToHit);
	// ����(Z��)�� �Ʒ�(0���� ����)�̹Ƿ� ������ ����
	if (CrossProduct.Z < 0.0)
	{
		Theta *= -1.f;
	}

	// -135 ~ 135��
	FName Section(TEXT("FromBack"));

	if (Theta >= -45.f && Theta < 45.f)
	{
		Section = TEXT("FromFront");
	}
	else if (Theta >= -135.f && Theta < -45.f)
	{
		Section = TEXT("FromLeft");
	}
	else if (Theta >= 45.f && Theta < 135.f)
	{
		Section = TEXT("FromRight");
	}

	PlayHitReactMontage(Section);
}

void ABaseCharacter::PlayHitSound(const FVector& ImpactPoint)
{
	if (HitSound)
	{
		UGameplayStatics::PlaySoundAtLocation(this, HitSound, ImpactPoint);
	}
}

void ABaseCharacter::SpawnHitParticles(const FVector& ImpactPoint)
{
	if (HitParticles && GetWorld())
	{
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), HitParticles, ImpactPoint);
	}
}

void ABaseCharacter::PlayMontageSection(UAnimMontage* Montage, const FName& SectionName)
{
	if (!Montage) return;

	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance)
	{
		AnimInstance->Montage_Play(Montage);
		AnimInstance->Montage_JumpToSection(SectionName, Montage);
	}
}

const int32 ABaseCharacter::PlayRandomMontageSection(UAnimMontage* Montage, const TArray<FName>& SectionNames)
{
	if (!Montage) return -1;
	if (SectionNames.IsEmpty()) return -1;

	const int32 MaxSectionIndex = SectionNames.Num() - 1;
	const int32 Selection = FMath::RandRange(0, MaxSectionIndex);
	PlayMontageSection(Montage, SectionNames[Selection]);
	return Selection;
}

void ABaseCharacter::DisableCapsule()
{
	if (GetCapsuleComponent())
	{
		GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
}
