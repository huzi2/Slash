// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/AttributeComponent.h"

UAttributeComponent::UAttributeComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UAttributeComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UAttributeComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UAttributeComponent::ReceiveDamage(const float Damage)
{
	Health = FMath::Clamp(Health - Damage, 0.f, MaxHealth);
}

const float UAttributeComponent::GetHealthPercent() const
{
	return Health / MaxHealth;
}

const bool UAttributeComponent::IsAlive() const
{
	return Health > 0.f;
}
